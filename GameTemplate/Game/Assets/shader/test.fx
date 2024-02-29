/*!
 * @brief	シンプルなモデルシェーダー。
 */



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
    float3 dirLigDirection; // ライトの方向
    float3 dirLigColor;     // ライトの色
    
    float3 ptLigPosition;   // ポイントライトの位置
    float3 ptLigColor;      // ポイントライトのカラー
    float ptLigRange;       // ポイントライトの影響範囲
    
    float3 spLigPosition;   // スポットライトの位置   
    float3 spLigColor;      // スポットライトのカラー
    float spLigRange;       // スポットライトの射出範囲
    float3 spLigDirection;  // スポットライトの射出方向
    float spLigAngle;       // スポットライトの射出角度
    
    float3 eyePos;          // 視点の位置
    float specPow;          // スペキュラの絞り
    
    float3 groundColor;     // 照り返しのライト
    float3 skyColor;        // 天球ライト
    float3 groundNormal;    // 地面の法線
};

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
    float3 biNormal : BINORMAL; //従ベクトル
    
    float2 uv : TEXCOORD0; //UV座標。
    SSkinVSIn skinVert; //スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL; //法線
    
    float3 tangent : TANGENT;   //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
    
    float2 uv : TEXCOORD0; //uv座標。
    float3 worldPos : TEXCOORD1;    //ワールド空間座標
    float3 normalInView : TEXCOORD2;    //カメラ空間の法線
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_normalMap : register(t1);   //法線マップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。
sampler g_sampler : register(s0); //サンプラステート。

////////////////////////////////////////////////
// 関数宣言。
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromDirectionLight(SPSIn psIn);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromSpotLight(SPSIn psIn);
float3 CalcLigFromLimLight(float3 lightDirection, float3 lightColor, float3 normal, float3 normalInView);
float3 CalcLigFormhemiLight(SPSIn psIn);

/// <summary>
//スキン行列を計算する。
/// </summary>
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

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
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
    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));
	
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    
    psIn.uv = vsIn.uv;

    //カメラ空間の法線
    psIn.normalInView = mul(mView, psIn.normal);
    
    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{   
    //法線マップで法線の向きを変える
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv);
    
    localNormal = (localNormal - 0.5) * 2.0f;
    
    psIn.normal = psIn.tangent * localNormal.x
                + psIn.biNormal * localNormal.y
                + psIn.normal * localNormal.z;
    
    psIn.normal = normalize(psIn.normal);
    
    
    //ディレクションライトによるライティングを計算
    float3 directionLig = CalcLigFromDirectionLight(psIn);
    //ポイントライトによるライティングを計算
    float3 pointLig = CalcLigFromPointLight(psIn);
    //スポットライトによるライティングを計算
    float3 spotLig = CalcLigFromSpotLight(psIn);
    //半球ライトによるライティングを計算
    float3 hemiLig = CalcLigFormhemiLight(psIn);
    
    
    
    
    //最終的な光を求める
    float3 finalLig = directionLig + hemiLig;+ pointLig + spotLig;
    
    //float3 finalLig = directionLig;
    //float3 finalLig = pointLig;
    //float3 finalLig = spotLig;
    //float3 finalLig = spotLig + ambientLight;
    
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= finalLig;
    
    return finalColor;
}



////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

// Lambert拡散反射光を計算する
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    //ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;
    
    //内積の結果が0より小さい時は0にする
    t = max(0.0f, t);
    
    //拡散反射光を計算する
    return lightColor * t;
}


//Phonng鏡面反射光を計算する
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
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
    return lightColor * t;
}

//ディレクションライトによる反射光を計算
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
    //ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(dirLigDirection, dirLigColor, psIn.normal);
    
    //ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(dirLigDirection, dirLigColor, psIn.worldPos, psIn.normal);
    
    //ディレクションライトによるリムライトを計算する
    float3 limLig = CalcLigFromLimLight(dirLigDirection,dirLigColor,psIn.normal,psIn.normalInView);
    
    //ディレクションライトの最終的な反射光を返す
    return diffDirection + specDirection + limLig;
}

//ポイントライトによる反射光を計算
float3 CalcLigFromPointLight(SPSIn psIn)
{
    //サーフェイスに入射している、ポイントライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - ptLigPosition;
    
    //正規化する
    ligDir = normalize(ligDir);
    
    //Lambert拡散反射光を計算する
    float3 diffPoint = CalcLambertDiffuse(
        ligDir,     //ライトの方向
        ptLigColor, //ライトのカラー
        psIn.normal //サーフェイスの法線
    );
    
    //Phong鏡面反射光を計算する
    float3 specPoint = CalcPhongSpecular(
        ligDir,         //ライトの方向
        ptLigColor,     //ライトのカラー
        psIn.worldPos,  //サーフェイスのワールド座標
        psIn.normal     //サーフェイスの法線
    );

    //リムライトを計算する
    float3 limLig = CalcLigFromLimLight(
    ligDir,             //ライトの方向
    ptLigColor,         //ライトのカラー
    psIn.normal,        //サーフェイスの法線
    psIn.normalInView   //カメラ空間のサーフェイスの法線
    );
    
    
    //距離による影響率を計算する
    //ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - ptLigPosition);
    
    //影響率を距離によって小さくする
    float affect = 1.0f - 1.0f / ptLigRange * distance;
    
    //影響力がマイナスにならないようにする
    if(affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    affect = pow(affect, 3.0f);
    
    diffPoint *= affect;
    specPoint *= affect;
    limLig *= affect;
    
    
    return diffPoint + specPoint + limLig;
}

float3 CalcLigFromSpotLight(SPSIn psIn)
{
    //サーフェイスに入射するスポットライトの光の向きを求める
    float3 ligDir = psIn.worldPos - spLigPosition;
    
    //正規化する
    ligDir = normalize(ligDir);
    
    //Lambert拡散反射光を計算する
    float3 diffSpotLight = CalcLambertDiffuse(
    ligDir,         //ライトの方向
    spLigColor,     //ライトのカラー
    psIn.normal     //サーフェイスの法線
    );

    //Phong鏡面反射光を計算する
    float3 specSpotLight = CalcPhongSpecular(
    ligDir,         //ライトの方向
    spLigColor,     //ライトのカラー
    psIn.worldPos,  //サーフェイスのワールド座標
    psIn.normal     //サーフェイスの法線
    );
    
    //リムライトを計算する
    float3 limLig = CalcLigFromLimLight(
    ligDir,             //ライトの方向
    spLigColor,         //ライトのカラー
    psIn.normal,        //サーフェイスの法線
    psIn.normalInView   //カメラ空間のサーフェイスの法線
    );
    
    //距離による影響率を計算する
    //スポットライトとの距離を計算する
    float3 distance = length(psIn.worldPos - spLigPosition);
    
    //影響率を距離に応じて小さくする
    float affect = 1.0f - 1.0f / spLigRange * distance;
    
    //影響率がマイナスにならないようにする
    if(affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    //影響の仕方を指数関数的にする
    affect = pow(affect, 3.0f);
    
    //影響率を乗算して反射光を弱める
    diffSpotLight *= affect;
    specSpotLight *= affect;
    limLig *= affect;
    
    //角度による影響率を求める
    //入射光と射出方向の角度を求める
    float angle = dot(ligDir, spLigDirection);
    angle = abs(acos(angle));
    
    //角度に比例して小さくなっていく影響率を計算する
    affect = 1.0f - 1.0f / spLigAngle * angle;
    
    //影響率がマイナスにならないようにする
    if(affect < 0.0f)
    {
        affect = 0.0f;
    }
    
    affect = pow(affect, 0.5f);
    
    //影響率を乗算して反射光を弱める
    diffSpotLight *= affect;
    specSpotLight *= affect;
    limLig *= affect;
    
    //最終的な反射光を返す
    return diffSpotLight + specSpotLight + limLig;
}

float3 CalcLigFromLimLight(float3 lightDirection, float3 lightColor, float3 normal, float3 normalInView)
{
    //float power1 = 1.0f - max(0.0f, dot(lightDirection, normal));
   
    //float power2 = 1.0f - max(0.0f, normalInView.z * -1.0f);
    
    float power1 = 1.0f - abs(dot(lightDirection, normal));
   
    float power2 = 1.0f - abs(normalInView.z * -1.0f);
   
    
    float limPower = power1 * power2;
    
    limPower = pow(limPower, 1.3f);
    
    return limPower * lightColor;
}

float3 CalcLigFormhemiLight(SPSIn psIn)
{
    //サーフェイスの法線と地面の法線との内積を計算する
    float t = dot(psIn.normal, groundColor);
    
    //内積の結果を0～1の範囲に変換する
    t = (t + 1.0f) / 2.0f;
    
    //地面色と天球色を補完率tで線形補完する
    float3 hemiLight = lerp(groundColor, skyColor, t);
    
    return hemiLight;
}
