/*!
 * @brief	画像を表示するためのシェーダー
 */

cbuffer cb : register(b0){
    float4x4 mvp;		//MVP行列
    float4 mulColor;	//乗算カラー
};

//追加したアルファ値のレジスタ
cbuffer alphaCb : register(b1)
{
    float alphaAdd;
    float fontAlpha;
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};


Texture2D<float4> colorTexture : register(t0);// カラーテクスチャ
Texture2D<float4> loadMeianADDTexture : register(t20); // Now Loading...の明暗テクスチャ

sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	
	return psIn;
}
//ピクセルシェーダー
float4 PSMain( PSInput In ) : SV_Target0
{
    float4 colorTex = colorTexture.Sample(Sampler, In.uv);
	
    float4 loadMeianADDTex = loadMeianADDTexture.Sample(Sampler, In.uv);
    
    float4 finalColor = colorTex;
    
    finalColor.xyz *= mulColor;
	
	if(loadMeianADDTex.r >0.5)
    {
        finalColor.rgb = fontAlpha;
    }
    
    finalColor.a = alphaAdd;
	
    return finalColor;
}