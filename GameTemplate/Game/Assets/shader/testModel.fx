//ライトの管理機能を追加していないので
//ライトがディレクションライトと環境光のみのシェーダー
//モーション対応

///////////////////////////////////////////
// 構造体
///////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos      : POSITION; //モデルの頂点座標。
    float3 normal   : NORMAL;   //法線
    
    float3 tangent  : TANGENT;  //接ベクトル
    float4 biNormal : BINORMAL; //従ベクトル
    
    float2 uv       : TEXCOORD0;//UV座標。
    
    SSkinVSIn skinVert; //スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos      : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float3 normal   : NORMAL;   //法線
    
    float3 tangent  : TANGENT;  //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
    
    //float3 normalInView : TEXCOORD2; //カメラ空間の法線
    
    float2 uv       : TEXCOORD0; //uv座標。
    float3 worldPos : TEXCOORD1; //ワールド空間座標
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
    float3 dirLigDirection; // ディレクションライトの方向
    float3 dirLigColor;     // ディレクションライトの色
    
    float3 ambientLig;      //アンビエントライト
    
    float3 eyePos;          //視点の位置
};

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


///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float specPow);
float3 CalcLigFromDirectionLight(SPSIn psIn, float specPow);

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



//モデル用の頂点シェーダーのエントリーポイント
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
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    psIn.normal = mul(m, vsIn.normal); // 法線を回転させる
    
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    
    psIn.uv = vsIn.uv;

    //psIn.normalInView = mul(mView, psIn.normal);
    
    return psIn;
}

/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

/// スキンありメッシュの頂点シェーダーのエントリー関数。
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

//モデル用のピクセルシェーダーのエントリーポイント
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv);
    localNormal = (localNormal - 0.5) * 2.0f;
    
    psIn.normal = psIn.tangent * localNormal.x
                + psIn.biNormal * localNormal.y
                + psIn.normal * localNormal.z;
    
    psIn.normal = normalize(psIn.normal);
    
    //スペキュラマップからスペキュラの反射の強さをサンプリング
    float specPow = g_speclarMap.Sample(g_sampler, psIn.uv);
    
    
    //ディレクションライトによるライティングを計算
    float3 directionLig = CalcLigFromDirectionLight(psIn, specPow);
    

    //最終的な光を求める
    float3 finalLig = directionLig + ambientLig;
    
    
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
float3 CalcLigFromDirectionLight(SPSIn psIn, float specPow)
{
    //ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(dirLigDirection, dirLigColor, psIn.normal);
    
    //ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(dirLigDirection, dirLigColor, psIn.worldPos, psIn.normal,specPow);
    
    //ディレクションライトの最終的な反射光を返す
    return diffDirection + specDirection;
}
