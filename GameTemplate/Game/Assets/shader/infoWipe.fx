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
    float2 wipeDirectionX; //ワイプ方向X
    float2 wipeDirectionY; //ワイプ方向Y
    float wipeSizeX; //ワイプサイズX
    float wipeSizeY; //ワイプサイズY
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
	
    float X = dot(wipeDirectionX, In.pos.xy);
    clip(X - wipeSizeX);
    
    float Y = dot(wipeDirectionY, In.pos.xy);
    clip(Y - wipeSizeY);
    
    return color;
}