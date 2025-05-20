Texture2D textures;
Texture2D textureNight : register(t1);
SamplerState colorSampler : register(s0);

cbuffer cbChangesOccasionally : register(b2)
{
    float time;
};

cbuffer gradient : register(b3)
{
    float gradient;
}

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
    float gradient : TEXCOORD1;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.gradient = saturate(gradient);

	vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
    float4 finalColor;
    float4 final;
    float4 dia = textures.Sample(colorSampler, pix.tex0);
    float4 noche = textureNight.Sample(colorSampler, pix.tex0);
        
    if (time > 0.0f && time < 30.0f)
    {
        finalColor = noche * (gradient) + dia;
    }
    else
    {
        finalColor = noche + dia * abs(1.0f - (gradient));
        
    }

	return finalColor;
}
