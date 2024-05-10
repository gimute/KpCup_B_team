

///////////////////////////////////////
// 定数
///////////////////////////////////////
static const int NUM_DIRECTIONAL_LIGHT = 4; // ディレクションライトの数
static const int MAX_POINT_LIGHT = 32; // ポイントライトの最大数
static const int MAX_SPOT_LIGHT = 32; // スポットライトの最大数

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float3 normal : NORMAL; //法線
    
    float3 tangent : TANGENT; //接ベクトル
    float4 biNormal : BINORMAL; //従ベクトル
    
    float2 uv : TEXCOORD0; //UV座標。
    SSkinVSIn skinVert; //スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL; //法線
    
    float3 tangent : TANGENT; //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
    
    float2 uv : TEXCOORD0; //uv座標。
    float3 worldPos : TEXCOORD1; //ワールド座標系でのポジション
    float3 normalInView : TEXCOORD2; //カメラ空間の法線
    float distToEye : TEXCOORD3; //視点との距離
};


// ディレクションライト
struct DirectionalLight
{
    float3 direction; //ライトの方向
    int castShadow; //影をキャストするか？
    float3 color; //ライトのカラー
};

//ポイントライト
struct PointLight
{
    float3 position; //座標
    int isUse; //使用中フラグ
    float3 color; //ライトのカラー
    float3 attn; //減衰パラメータ
                        //xに影響範囲,yに影響率に累乗するパラメータ
};

struct SpotLight
{
    float3 position; //座標
    int isUse; //使用中フラグ
    float3 color; //ライトのカラー
    float range; //影響範囲
    float3 direction; //射出方向
    float angle; //射出角度
    float3 pow; //影響率に乗算するパラメータ
    					//xが距離による影響率に乗算するパラメータ
						//yが角度による影響率に乗算するパラメータ
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer LightCb : register(b1)
{
    DirectionalLight directionalLights[NUM_DIRECTIONAL_LIGHT]; //ディレクションライト。
    PointLight pointLights[MAX_POINT_LIGHT]; //ポイントライト。
    SpotLight spotLights[MAX_SPOT_LIGHT]; //スポットライト。
    float3 eyePos; //視点の位置
    int numPointLight; //使用中ポイントライトの数
    float3 ambientLight; //環境光
    int numSpotLight; //使用中スポットライトの数   
}

///////////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////////
// モデルテクスチャ
Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_normalMap : register(t1); //法線マップ
Texture2D<float4> g_speclarMap : register(t2); //スペキュラマップ

StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。

///////////////////////////////////////////
// サンプラーステート
///////////////////////////////////////////
sampler g_sampler : register(s0);

static const int pattern[4][4] =
{
    { 0, 32, 8, 40 },
    { 48, 16, 56, 24 },
    { 12, 44, 4, 36 },
    { 60, 28, 52, 20 },
};

///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float specPow);
float3 CalcLigFromDirectionLight(SPSIn psIn, DirectionalLight dirlig, float specPow);
float3 CalcLigFromPointLight(SPSIn psIn, PointLight ptlig, float specPow);
float3 CalcLigFromSpotLight(SPSIn psIn, SpotLight spLig, float specPow);
float3 CalcLigFromLimLight(float3 lightDirection, float3 lightColor, float3 normal, float3 normalInView);
//float3 CalcLigFormhemiLight(SPSIn psIn);  //現状使わない

//スキン行列を計算する。
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// 頂点シェーダーのコア関数。
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    psIn.pos = mul(m, vsIn.pos); // モデルの頂点をワールド座標系に変換
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    ///////////
    float4 posInCameraSpace = psIn.pos;
    ///////////
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    psIn.normal = mul(m, vsIn.normal); // 法線を回転させる
	
    psIn.uv = vsIn.uv;

    //カメラ空間の法線
    psIn.normalInView = mul(mView, psIn.normal);
    
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    
    // カメラからの距離を計算する
    psIn.distToEye = length(posInCameraSpace);
    
    return psIn;
}

// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

// スキンありメッシュの頂点シェーダーのエントリー関数。
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    int x = (int) fmod(psIn.pos.x, 4.0f);
    int y = (int) fmod(psIn.pos.y, 4.0f);

    int dither = pattern[y][x];
    
    float clipRange = 100.0f;

    float eyeToClipRange = max(0.0f, psIn.distToEye - clipRange);
    
    float clipRate = 1.0f - min(1.0f, eyeToClipRange / 100.0f);
    
    clip(dither - 64 * clipRate);
    
    
    
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv);
    localNormal = (localNormal - 0.5) * 2.0f;
    
    psIn.normal = psIn.tangent * localNormal.x
                + psIn.biNormal * localNormal.y
                + psIn.normal * localNormal.z;
    
    psIn.normal = normalize(psIn.normal);
    
    float specPow = g_speclarMap.Sample(g_sampler, psIn.uv);
    
    
    float3 finalLig;
    
    finalLig.x = 0.0f;
    finalLig.y = 0.0f;
    finalLig.z = 0.0f;
    
    for (int dirligNo = 0; dirligNo < NUM_DIRECTIONAL_LIGHT; dirligNo++)
    {
        finalLig += CalcLigFromDirectionLight(psIn, directionalLights[dirligNo], specPow);
    }
    
    
    //使用されているポイントライトがあるか確認
    if (numPointLight <= 0 == false)
    {
        //処理した使用中ライトの数をカウントする変数
        int countPt = 0;
        
         //ポイントライトの配列を回して使用中のライトを探す
        for (int ptLigNo = 0; ptLigNo < MAX_POINT_LIGHT; ptLigNo++)
        {
            if (pointLights[ptLigNo].isUse)
            {
            //ライトの計算処理
                finalLig += CalcLigFromPointLight(psIn, pointLights[ptLigNo], specPow);
            
            //処理した使用中ライトの数を加算
                countPt++;
            //処理した数が使用中ライトの数以上になったらfor文を抜ける
                if (countPt >= numPointLight)
                {
                    break;
                }
            }
        }
    }
    
    //使用されているポイントライトがあるか確認
    if (numSpotLight <= 0 == false)
    {
        //処理した使用中ライトの数をカウントする変数
        int countSp = 0;
        
         //ポイントライトの配列を回して使用中のライトを探す
        for (int spLigNo = 0; spLigNo < MAX_SPOT_LIGHT; spLigNo++)
        {
            if (spotLights[spLigNo].isUse)
            {
            //ライトの計算処理
                finalLig += CalcLigFromSpotLight(psIn, spotLights[spLigNo], specPow);
            
            //処理した使用中ライトの数を加算
                countSp++;
            //処理した数が使用中ライトの数以上になったらfor文を抜ける
                if (countSp >= numPointLight)
                {
                    break;
                }
            }
        }
    }
   
    
    
    finalLig += ambientLight;
    
   
    
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= finalLig;
    
    return finalColor;
}


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

// Lambert拡散反射光の計算
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    //ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;
    
    //内積の結果が0より小さい時は0にする
    t = max(0.0f, t);
    
    //拡散反射光を計算する
    return lightColor * t;
}

//Phonng鏡面反射光の計算
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float specPow)
{
    //反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);
    
    //光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);
    
    //鏡面反射の強さを求める
    float t = dot(refVec, toEye);
    
    //内積の結果が0より小さい時は0にする
    t = max(0.0f, t);
    
    //鏡面反射の強さを絞る
    t = pow(t, 5.0f);
    
    //鏡面反射光を求める
    float3 specularLig = lightColor * t;
    
    return specularLig * specPow;
}

//ディレクションライトによる反射光を計算
float3 CalcLigFromDirectionLight(SPSIn psIn, DirectionalLight dirLig, float specPow)
{
    //ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(dirLig.direction, dirLig.color, psIn.normal);
    
    //ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(dirLig.direction, dirLig.color, psIn.worldPos, psIn.normal, specPow);
    
    //ディレクションライトの最終的な反射光を返す
    return diffDirection + specDirection;
}

//ポイントライトによる反射光を計算
float3 CalcLigFromPointLight(SPSIn psIn, PointLight ptLig, float specPow)
{
    float3 ligDir = psIn.worldPos - ptLig.position;
    
    ligDir = normalize(ligDir);
    
    //拡散反射光を計算
    float3 diffPoint = CalcLambertDiffuse(
    ligDir,
    ptLig.color,
    psIn.normal
    );
    
    //鏡面反射光を計算
    float3 specPoint = CalcPhongSpecular(
    ligDir,
    ptLig.color,
    psIn.worldPos,
    psIn.normal,
    specPow
    );
    
    
    //距離による影響率を計算する
    //ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - ptLig.position);
    
    //影響率を距離によって変化させる
    float affect = 1.0f - 1.0f / ptLig.attn.x * distance;
    
    //影響率がマイナスにならないようにする
    affect = max(0.0f, affect);
    
    //乗算して影響率の変化を指数関数的にする
    affect = pow(affect, ptLig.attn.y);

    return (diffPoint + specPoint) * affect;
    
}

float3 CalcLigFromSpotLight(SPSIn psIn, SpotLight spLig, float specPow)
{
    //サーフェイスに入射するスポットライトの光の向きを求める
    float3 ligDir = psIn.worldPos - spLig.position;
    
    //正規化する
    ligDir = normalize(ligDir);
    
    //拡散反射光を計算する
    float3 diffLight = CalcLambertDiffuse(
    ligDir, //ライトの方向
    spLig.color, //ライトのカラー
    psIn.normal     //サーフェスの法線
    );

    //鏡面反射光を計算する
    float3 specLight = CalcPhongSpecular(
    ligDir, //ライトの方向
    spLig.color, //ライトのカラー
    psIn.worldPos, //サーフェスのワールド座標
    psIn.normal, //サーフェスの法線
    specPow //スペキュラの強さ
    );

    //距離による影響率を計算する
    //スポットライトとの距離を計算する
    float3 distance = length(psIn.worldPos - spLig.position);
    
    //影響率を距離に応じて小さくする
    float affect = 1.0f - 1.0f / spLig.range * distance;
    
    //影響率がマイナスにならないように
    affect = max(0.0f, affect);
    
    //影響の仕方を指数関数的にする
    affect = pow(affect, spLig.pow.x);
    
    //影響率を乗算して反映する
    diffLight *= affect;
    specLight *= affect;
    
    //角度による影響率を求める
    //入射光と射出方向の角度を求める
    float angle = dot(ligDir, spLig.direction);
    angle = abs(acos(angle));
    
    //角度に比例して小さくなっていく影響率を計算する
    affect = 1.0f - 1.0f / spLig.angle * angle;
    
    //影響率がマイナスにならないようにする
    affect = max(0.0f, affect);
    
    //影響の仕方を指数関数的にする
    affect = pow(affect, spLig.pow.y);
    
    //影響率を乗算して反映する
    diffLight *= affect;
    specLight *= affect;
    
    return diffLight + specLight;
}

