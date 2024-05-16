/*!
 * @brief	画像を表示するためのシェーダー
 */

cbuffer cb : register(b0){
    float4x4 mvp;		//MVP行列
    float4 mulColor;	//乗算カラー
};

//追加したアルファ値のレジスタ
cbuffer testCb : register(b1)
{
    float alphaAdd;
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0); // カラーテクスチャ
Texture2D<float4> signalAddTexture : register(t20);

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
	
    float4 signalAddTex = signalAddTexture.Sample(Sampler, In.uv);
			
    float4 finalColor = colorTex;
    finalColor.xyz *= mulColor;
	
	if(signalAddTex.r >= 0.5)
    {
        finalColor.g += 1.0f;
        finalColor.a = alphaAdd;
    }
	
	return finalColor;
}