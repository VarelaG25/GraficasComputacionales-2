Texture2D colorMap : register(t0);
Texture2D colorMap2 : register(t1);
Texture2D blendMap : register(t2);
Texture2D normalMap : register(t3);
SamplerState colorSampler : register(s0);

cbuffer cbChangerEveryFrame : register(b0)
{
	matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
	matrix projMatrix;
};

cbuffer cbChangesOccasionally : register(b3)
{
    float time;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangente : NORMAL1;
	float3 binormal : NORMAL2;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float2 blendTex : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangent : NORMAL1;
	float3 binorm : NORMAL2;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	vsOut.blendTex = vertex.blendTex;
	vsOut.normal = normalize(mul(vertex.normal, worldMatrix));
	vsOut.tangent = normalize(mul(vertex.tangente, worldMatrix));
	vsOut.binorm = normalize(mul(vertex.binormal, worldMatrix));

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
    float4 fColor = float4(1, 0, 0, 1);

    float3 ambient = float3(0.1f, 0.1f, 0.1f);

    // Texturas base y mezcla
    float4 text = colorMap.Sample(colorSampler, pix.tex0);
    float4 text2 = colorMap2.Sample(colorSampler, pix.tex0);
    float4 alphaBlend = blendMap.Sample(colorSampler, pix.blendTex);
    float4 textf = (text * alphaBlend) + ((1.0 - alphaBlend) * text2);

    // Dirección de la luz
    float3 lightDir = normalize(float3(0.5f, -1.0f, 0.0f)); // O puedes hacerla uniforme

    // Normal map
    float3 normalSample = normalMap.Sample(colorSampler, pix.tex0).rgb;
    normalSample = normalSample * 2.0f - 1.0f; // Expandir de [0,1] a [-1,1]

    float3x3 TBN = float3x3(normalize(pix.tangent), normalize(pix.binorm), normalize(pix.normal));
    float3 normalWorld = mul(normalSample, TBN);

    // Color de la luz difusa afectado por el tiempo
    float3 DiffuseColor = float3(1.0f, 1.0f, 1.0f);
    if (time > 30.0f && time < 60.0f)
    {
        DiffuseColor = float3(61.0f / 255.0f, 133.0f / 255.0f, 1.0f); // Azul claro
    }

    // Iluminación difusa
    float NdotL = saturate(dot(normalWorld, -lightDir));
    float3 diffuse = saturate(NdotL * DiffuseColor + ambient);

    fColor = float4(textf.rgb * diffuse, 1.0f);

    return fColor;
}
