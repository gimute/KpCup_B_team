/*!
 * @brief	画像を表示するためのシェーダー
 */

cbuffer cb : register(b0){
    float4x4 mvp;		//MVP行列
    float4 mulColor;	//乗算カラー
};

//追加したリニアワイプのレジスタ
cbuffer alphaCb : register(b1)
{
    float2 wipeDirection; //ワイプ方向
    float wipeSize; //ワイプのサイズ
    float fontColor;//フォントの色
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
Texture2D<float4> remainingReloadAddTexture1 : register(t20); //フォント文字色変更用テクスチャ

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
    float4 color = colorTexture.Sample(Sampler, In.uv);
	
    float4 addTexture = remainingReloadAddTexture1.Sample(Sampler, In.uv);
	
    float t = dot(wipeDirection, In.pos.xy);
    clip(t - wipeSize);
	
    if (addTexture.r > 0.5f)
    {
        color.rgb = fontColor;
    }
	
    return color;
}