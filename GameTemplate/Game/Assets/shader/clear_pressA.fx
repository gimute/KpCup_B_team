/*!
 * @brief	画像を表示するためのシェーダー
 */

cbuffer cb : register(b0){
    float4x4 mvp;		//MVP行列
    float4 mulColor;	//乗算カラー
};
struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

cbuffer alphaCb : register(b1)
{
    float alphaAdd;
}

Texture2D<float4> colorTexture : register(t0); // カラーテクスチャ
Texture2D<float4> testTexture : register(t20); // ユーザーが用意したテクスチャ
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	
	return psIn;
}
float4 PSMain( PSInput In ) : SV_Target0
{
    float4 colorTex = colorTexture.Sample(Sampler, In.uv);
	
    colorTex.a *= alphaAdd;
	
    return colorTex * mulColor;
}