//--------------------------------------------------------------------------------------
// Struktury FX
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 PositionL	: POSITION;
	float3 NormalL		: NORMAL;
	float2 Tex			: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 PositionH    : SV_POSITION;
	float2 Tex			: TEXCOORD;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D gAlphaMap;


//--------------------------------------------------------------------------------------
// Sta³e bufory danych
//--------------------------------------------------------------------------------------

cbuffer ConstantBuffer : register(b0)
{
	float4x4 World;
	float4x4 WorldViewProjection;
	float4x4 WorldInvTranspose;
	float4x4 TexTransform;
	float4x4 ViewProj;

}

cbuffer cbPerFrame : register(b1)
{
	float3 gEyePosW;
};

//--------------------------------------------------------------------------------------
//Prototypy funkcji
//--------------------------------------------------------------------------------------


//***************************************************************************************************************************

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	// Przekszta³cenie do jednorodnej przestrzeni obcinania.
	output.PositionH = mul(float4(input.PositionL, 1.0f), WorldViewProjection);
	output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), TexTransform).xy;

	return output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader 
//--------------------------------------------------------------------------------------
void PS(VS_OUTPUT input)
{

	//PS jest stosowana tylko do usuwania pikseli, na ktorych jest kanal alfa.
	float4 diffuse = gAlphaMap.Sample(samLinear, input.Tex);
	// Usuwanie przy pomocy kanalu alfa
	clip(diffuse.rgb - 0.10f);
}




//--------------------------------------------------------------------------------------
// MSDN 
//--------------------------------------------------------------------------------------
RasterizerState Depth
{
	// Optymalne wartoœci dla sceny nale¿y znaleŸæ metod¹ prób i b³êdów.
	//DepthBias = 100000;
	/*DepthBias = 40000;
	DepthBiasClamp = 1.1f;
	SlopeScaledDepthBias = 0.8f;

	*/
	/*DepthBias = 35000;
	DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 0.1f;*/

	DepthBias = 50000;
	DepthBiasClamp = 0.5f;
	SlopeScaledDepthBias = 0.4f;
};




//***************************************************************************************************************************
technique11 BuildShadowMapTechNoAlpha
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
		SetRasterizerState(Depth);
	}
}

technique11 BuildShadowMapTechAlpha
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

