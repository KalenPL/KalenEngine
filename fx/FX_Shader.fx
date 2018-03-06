//--------------------------------------------------------------------------------------
// Struktury FX
//--------------------------------------------------------------------------------------
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;

	float roughness;
	float fresnelFactor;
	float specularPower;

	float gHeightScale;
	float gMinTessFactor;
	float gMinTessDistance;
	float gMaxTessDistance;
	float gMaxTessFactor;
};

BlendState AlphaBlendingOn
{
	BlendEnable[0] = TRUE;
	DestBlend = INV_SRC_ALPHA;
	SrcBlend = SRC_ALPHA;
};

BlendState AlphaBlendingOFF
{
	BlendEnable[0] = FALSE;
	DestBlend = INV_SRC_ALPHA;
	SrcBlend = SRC_ALPHA;
};


struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};




struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Position;
	float Range;
	float3 Att;
	float pad;
};


struct VS_INPUT
{
	float3 PositionL	: POSITION;
	float3 NormalL		: NORMAL;
	float2 Tex			: TEXCOORD;
	float3 TangentL		: TANGENT;
};

struct VS_OUTPUT
{
	float4 PositionH    : SV_POSITION;
	float3 PositionW	: POSITION;
	float3 NormalW		: NORMAL;//TEXCOORD0;
	float3 TangentW		: TANGENT;
	float2 Tex			: TEXCOORD;
	float4 ShadowPosH   : TEXCOORD1;
	float4 SSAOPosH   : TEXCOORD2;
};

struct VS_OUTPUT_TESS
{
	float3 PositionW	: POSITION;
	float3 NormalW		: NORMAL;
	float3 TangentW		: TANGENT;
	float2 Tex			: TEXCOORD;
	float  TessFactor : TESS; //Zmienna dla teselacji obiektu
	float4 ShadowPosH   : TEXCOORD1;
};

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP;
};


SamplerState samTriLinearSam
{
	Filter = MiN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	ComparisonFunc = LESS_EQUAL;
};


Texture2D gDiffuseMap;
Texture2D gNormalMap;
Texture2D gASHMap;
Texture2D gShadowMap;
TextureCube gCubeMap;
Texture2D gSSAOMap;


bool __allowDiffuse;
bool __allowNormal;
bool __allowSpecular;
bool __allowAlpha;
bool __allowCubeMap;
bool __allowShadowMap;
bool __allowSSAOMap;
int __color;
float __transparency;
//--------------------------------------------------------------------------------------
// Sta³e bufory danych
//--------------------------------------------------------------------------------------

cbuffer ConstantBuffer : register(b0)
{
	float4x4 World;
	float4x4 WorldViewProjection;
	float4x4 WorldInvTranspose;
	float4x4 TexTransform;
	float4x4 gShadowTransform;
	float4x4 WorldViewProjectionTex;
	Material gMaterial;

}

cbuffer cbPerFrame : register(b1)
{
	PointLight gPointLight[10];
	DirectionalLight gDirectionalLight;
	float3 gEyePosW;
};

//--------------------------------------------------------------------------------------
//Prototypy funkcji
//--------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower);
void ComputeDirectionalLightCookTorrance(Material mat, DirectionalLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower);

void ComputePointLight(Material mat, PointLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower);
void ComputePointLightCookTorrance(Material mat, PointLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower);
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW);
float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH);


//***************************************************************************************************************************

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input, uniform bool AllowWire)
{
	VS_OUTPUT output;

	// Przekszta³cenie do przestrzeni œwiata.
	output.PositionW = mul(float4(input.PositionL, 1.0f), World).xyz;


	if (!AllowWire)
	{
		output.NormalW = mul(input.NormalL, (float3x3)WorldInvTranspose);
	}
	else
		output.NormalW = input.NormalL;

	output.TangentW = mul(input.TangentL, (float3x3) World);


	// Przekszta³cenie do jednorodnej przestrzeni obcinania.
	output.PositionH = mul(float4(input.PositionL, 1.0f), WorldViewProjection);

	output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), TexTransform).xy;

	// Generuj wspó³rzêdne rzutowe tekstury, aby zrzutowaæ mapê cieni na scenê.
	output.ShadowPosH = mul(float4(input.PositionL, 1.0f), gShadowTransform);

	output.SSAOPosH = mul(float4(input.PositionL, 1.0f), WorldViewProjectionTex);

	return output;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_LINES(VS_INPUT input)
{
	VS_OUTPUT output;

	// Przekszta³cenie do przestrzeni œwiata.
	output.PositionW = mul(float4(input.PositionL, 1.0f), World).xyz;
	output.NormalW = input.NormalL;
	output.TangentW = input.TangentL;


	// Przekszta³cenie do jednorodnej przestrzeni obcinania.
	output.PositionH = mul(float4(input.PositionL, 1.0f), WorldViewProjection);
	output.Tex = input.Tex;

	// Generuj wspó³rzêdne rzutowe tekstury, aby zrzutowaæ mapê cieni na scenê.
	output.ShadowPosH = mul(float4(input.PositionL, 1.0f), gShadowTransform);


	return output;
}



//--------------------------------------------------------------------------------------
// Vertex Shader - Tessallation
//--------------------------------------------------------------------------------------
VS_OUTPUT_TESS VS_TESS(VS_INPUT input)
{
	VS_OUTPUT_TESS outputTess;
	// Przekszta³cenie do przestrzeni œwiata.
	outputTess.PositionW = mul(float4(input.PositionL, 1.0f), World).xyz;

	outputTess.NormalW = mul(input.NormalL, (float3x3)WorldInvTranspose);

	outputTess.TangentW = mul(input.TangentL, (float3x3) World);
	outputTess.Tex = mul(float4(input.Tex, 0.0f, 1.0f), TexTransform).xy;

	float dist = distance(outputTess.PositionW, gEyePosW);
	float tess = saturate((gMaterial.gMinTessDistance - dist) / (gMaterial.gMinTessDistance - gMaterial.gMaxTessDistance));

	// Przeskaluj [0, 1]-->[gMinTessFactor, gMaxTessFactor].
	outputTess.TessFactor = gMaterial.gMinTessFactor + tess*(gMaterial.gMaxTessFactor - gMaterial.gMinTessFactor);


	outputTess.ShadowPosH = mul(float4(input.PositionL, 1.0f), gShadowTransform);
	return outputTess;
}







//--------------------------------------------------------------------------------------
// Pixel Shader Phong
//--------------------------------------------------------------------------------------
float4 PS_Phong(VS_OUTPUT input) : SV_Target
{
	//Normalize
	input.NormalW = normalize(input.NormalW);
	//To eye
	float3 toEyeW = normalize(gEyePosW - input.PositionW);

	//Values
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float SpecularPower = 0.0f;
	float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (__allowSpecular)
		SpecularPower = gASHMap.Sample(samAnisotropic, input.Tex).g;
	else
		SpecularPower = 0.0f;

	//----------------------
	//Check alpha
	//----------------------
	//if (__allowAlpha)
	//{
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex).a;
		clip(texColor.rgb - 0.1f);
	//}

	//----------------------
	//Shadow
	//----------------------
	float3 shadow = float3(1.0f, 1.0f, 1.0f);
	if(__allowShadowMap == true)
		shadow[0] = CalcShadowFactor(samShadow, gShadowMap, input.ShadowPosH);
	
	//--------------------------------------
	//Mapowanie normalnych
	//--------------------------------------
	float3 bumpedNormalW = float3(0.0f, 0.0f, 0.0f);

	//------
	//SSAO
	//------
	/*input.SSAOPosH /= input.SSAOPosH.w;
	float ambientSSAO = 1.0f;
	if(__allowSSAOMap == true)
		ambientSSAO = gSSAOMap.SampleLevel(samTriLinearSam, input.SSAOPosH.xy, 0.0f).r;
		*/
	float ambientSSAO = 1.0f;
	float SpecularMul = 1.0f;
	if (__allowSpecular)
	{
		ambientSSAO = gASHMap.Sample(samAnisotropic, input.Tex).r;
		SpecularMul = gASHMap.Sample(samAnisotropic, input.Tex).g;
	}


	if (__allowNormal)
	{
		float3 normalMapSample = gNormalMap.Sample(samAnisotropic, input.Tex).rgb;
		bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.NormalW, input.TangentW);
		bumpedNormalW = normalize(bumpedNormalW); //normalize bumpedNormal

		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLight(gMaterial, gDirectionalLight, input.PositionW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLight(gMaterial, gPointLight[i], input.PositionW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) * ambientSSAO *Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * SpecularMul * Specular;
				}
				else
				{
					ambient += ambientSSAO *Ambient;
					diffuse += Diffuse;
					spec += SpecularMul * Specular;
				}
			}
		}
	}
	else
	{
		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLight(gMaterial, gDirectionalLight, input.PositionW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLight(gMaterial, gPointLight[i], input.PositionW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) *ambientSSAO * Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * SpecularMul * Specular;
				}
				else
				{
					ambient += ambientSSAO * Ambient;
					diffuse += Diffuse;
					spec += SpecularMul * Specular;
				}
			}
		}
	}
	//set diffuse colour from material
	texColor = float4(gMaterial.Diffuse.r, gMaterial.Diffuse.g, gMaterial.Diffuse.b, 1.0f);

	//Diffuse texture
	if (__allowDiffuse)
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex);

	FinalColor = texColor*(ambient + diffuse) + spec;

	//Set alpha
	FinalColor.a = gMaterial.Diffuse.a;

	//CubeMapping
	if (__allowCubeMap == true)
	{
		float3 incident = -toEyeW;
		float3 reflectionVector = float3(0.0f, 0.0f, 0.0f);
		if (__allowNormal)
			reflectionVector = reflect(incident, bumpedNormalW);
		else
			reflectionVector = reflect(incident, input.NormalW);

		float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
		FinalColor += gMaterial.Reflect * reflectionColor;
	}
	

	FinalColor.a = __transparency * gDiffuseMap.Sample(samAnisotropic, input.Tex).a;
	return FinalColor;

}


//--------------------------------------------------------------------------------------
// Pixel Shader Cook-Torrance
//--------------------------------------------------------------------------------------

float4 PS_CookTorrance(VS_OUTPUT input) : SV_Target
{
	input.NormalW = normalize(input.NormalW);

	float3 toEyeW = normalize(gEyePosW - input.PositionW);

	// Rozpocznij sumowanie od zera.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 texColor = float4(1, 1, 1, 1);

	float SpecularPower = 0.0f;

	//----------------------
	//Check alpha
	//----------------------
	//if (__allowAlpha)
	//{
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex).a;
		clip(texColor.rgb - 0.1f);
	//}


	float4 Ambient, Diffuse, Specular;


	//----------------------
	//Shadow
	//----------------------
	float3 shadow = float3(1.0f, 1.0f, 1.0f);
	if (__allowShadowMap == true)
		shadow[0] = CalcShadowFactor(samShadow, gShadowMap, input.ShadowPosH);


	//------
	//SSAO
	//------
	/*input.SSAOPosH /= input.SSAOPosH.w;
	float ambientSSAO = 1.0f;
	if (__allowSSAOMap == true)
		ambientSSAO = gSSAOMap.SampleLevel(samTriLinearSam, input.SSAOPosH.xy, 0.0f).r;
	*/
	float ambientSSAO = 1.0f;
	float SpecularMul = 1.0f;
	if (__allowSpecular)
	{
		ambientSSAO = gASHMap.Sample(samAnisotropic, input.Tex).r;
		SpecularMul = gASHMap.Sample(samAnisotropic, input.Tex).g;
	}


	//--------------------------------------
	//Mapowanie normalnych
	//--------------------------------------
	float3 bumpedNormalW = float3(0.0f, 0.0f, 0.0f);
	if (__allowNormal)
	{
		float3 normalMapSample = gNormalMap.Sample(samAnisotropic, input.Tex).rgb;
		bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.NormalW, input.TangentW);
		bumpedNormalW = normalize(bumpedNormalW); //normalize bumpedNormal

		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLightCookTorrance(gMaterial, gDirectionalLight, input.PositionW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLightCookTorrance(gMaterial, gPointLight[i], input.PositionW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * Specular;
				}
				else
				{
					ambient += ambientSSAO * Ambient;
					diffuse += Diffuse;
					spec += Specular;
				}
			}
		}
	}
	else
	{
		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLightCookTorrance(gMaterial, gDirectionalLight, input.PositionW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------  
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLightCookTorrance(gMaterial, gPointLight[i], input.PositionW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * SpecularMul * Specular;
				}
				else
				{
					ambient += ambientSSAO * Ambient;
					diffuse += Diffuse;
					spec += SpecularMul * Specular;
				}
			}
		}
	}
	float4 litColor;


	texColor = float4(gMaterial.Diffuse.r, gMaterial.Diffuse.g, gMaterial.Diffuse.b, 1.0f);//set default colour

	 //Diffuse texture
	if (__allowDiffuse)
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex);

	litColor = texColor*(ambient + diffuse) + spec;

	//Set alpha
	litColor.a = gMaterial.Diffuse.a;

	//CubeMapping
	if (__allowCubeMap == true)
	{
		float3 incident = -toEyeW;
		float3 reflectionVector = float3(0.0f, 0.0f, 0.0f);
		if (__allowNormal)
			reflectionVector = reflect(incident, bumpedNormalW);
		else
			reflectionVector = reflect(incident, input.NormalW);

		float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
		litColor += gMaterial.Reflect * reflectionColor;
	}

		litColor.a = __transparency * gDiffuseMap.Sample(samAnisotropic, input.Tex).a;
	return litColor;
}




//--------------------------------------------------------------------------------------
// Pixel Shader NormalView
//--------------------------------------------------------------------------------------

float4 PS_NormalView(VS_OUTPUT input, uniform bool AllowNormal) : SV_Target
{
	float3 normalColor = input.NormalW;
	return float4(normalColor, 1.0f);
}


//--------------------------------------------------------------------------------------
// Pixel Shader LINES
//--------------------------------------------------------------------------------------
float4 PS_LINE(VS_OUTPUT input, uniform bool AllowClip) : SV_Target
{
	if (AllowClip)
	{
		input.NormalW = normalize(input.NormalW);
		float3 toEyeW = normalize(gEyePosW - input.PositionW);
		float angle = dot(-toEyeW, input.NormalW);

		if (angle > 0.2)
			clip(-1);
	}
	if (__color == 0) //default color
	{
		float3 normalColor = input.NormalW;
		return float4(normalColor, 1.0f);
	}
	else if (__color == 1) //red
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	else if (__color == 2) //green
		return float4(0.0f, 1.0f, 0.0f, 1.0f);
	else if (__color == 3) //blue
		return float4(0.0f, 0.0f, 1.0f, 1.0f);
	else if (__color == 4) //yellow with alphaBlend
		return float4(1.0f, 1.0f, 0.0f, 0.6f);
	else if (__color == 6) //grey
		return float4(0.5f, 0.5f, 0.5f, 1.0f);

	else if (__color == 7) //red with alphaBlend
		return float4(1.0f, 0.0f, 0.0f, 0.6f);
	else if (__color == 8) //greeen with alphaBlend
		return float4(0.0f, 1.0f, 0.0f, 0.6f);
	else if (__color == 9) //blue with alphaBlend
		return float4(0.0f, 0.0f, 1.0f, 0.6f);

	//Yellow - selected line (no alphaBlend!
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}


//--------------------------------------------------------------------------------------
// Pixel Shader COLLISION
//--------------------------------------------------------------------------------------

float4 PS_RenderCollision(VS_OUTPUT input, uniform bool AllowNormal) : SV_Target
{
	return float4(0.5f, 0.0f, 0.88f, 1.0f);
}


//--------------------------------------------------------------------------------------
// Pixel Shader SelectedWire
//--------------------------------------------------------------------------------------

float4 PS_SelectedWire(VS_OUTPUT input) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}


//--------------------------------------------------------------------------------------
// Pixel Shader SelectedWire
//--------------------------------------------------------------------------------------

float4 PS_WhiteWireFrame(VS_OUTPUT input) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------------
// Pixel Shader BoxSelectedWire
//--------------------------------------------------------------------------------------

float4 PS_RedWireFrame(VS_OUTPUT input) : SV_Target
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}









//--------------------------------------------------------------------------------------
// Pixel Shader 
//--------------------------------------------------------------------------------------
float4 PS_DRAW_SHADOW_TEXTURES(VS_OUTPUT input) : SV_Target
{
	float4 color = gDiffuseMap.Sample(samAnisotropic, input.Tex).r;

	return float4(color.rrr, 1);
}







//***************************************************************************************************************************
//Definicje funkcji
//***************************************************************************************************************************
void ComputeDirectionalLight(Material mat, DirectionalLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Wektor do Ÿród³a
	float3 lightVec = -Light.Direction;

	// Ambient
	ambient = mat.Ambient * Light.Ambient;

	// Dodaj sk³adniki œwiat³a rozproszonego i odbitego pod warunkiem, ¿e pada ono na powierzchniê.
	float diffuseFactor = dot(lightVec, normal);

	// Sp³aszcz, aby unikn¹æ dynamicznego rozga³êziania.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal); //funkcja odbicia wektora
		float specFactor = pow(max(dot(v, toEye), 0.0f), (mat.specularPower));// *(SpecularPower + 0.1f)));
		diffuse = diffuseFactor * mat.Diffuse * Light.Diffuse;
		spec = specFactor * mat.Specular * Light.Specular;
	}
}
void ComputeDirectionalLightCookTorrance(Material mat, DirectionalLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float specular = 0.0f;

	float3 lightVec = -Light.Direction;
	float N_dot_L = dot(lightVec, normal);

	[flatten]
	if (N_dot_L > 0.0f)
	{
		float3 eyeDir = normalize(toEye);
		float3 half_vec = normalize(lightVec + eyeDir);
		float N_dot_H = dot(normal, half_vec);
		float N_dot_V = dot(normal, eyeDir);
		float V_dot_H = dot(eyeDir, half_vec);
		float r_squared = mat.roughness * mat.roughness;

		//ok
		float _2_N_dot_H = 2.0f * N_dot_H;
		float Ga = 1.0f;
		float Gb = (_2_N_dot_H * N_dot_V) / V_dot_H;
		float Gc = (_2_N_dot_H * N_dot_L) / V_dot_H;
		float G = min(Ga, min(Gb, Gc));

		//ok
		float r1 = 1.0f / (4.0f * r_squared * pow(N_dot_H, 4.0f));
		float r2 = (N_dot_H * N_dot_H - 1.0f) / (r_squared * N_dot_H * N_dot_H);
		float D = r1 * exp(r2);
		//ok
		float F = mat.fresnelFactor + ((1.0f - mat.fresnelFactor) * pow(1.0f - V_dot_H, 5.0f));

		specular = (D * F * G) / (3.14 * N_dot_V * N_dot_L);

		diffuse = N_dot_L * mat.Diffuse * Light.Diffuse;
		//spec = specular * mat.Specular * Light.Specular;
		spec = (SpecularPower + specular * (1.0f - SpecularPower)) * mat.Specular * Light.Specular;

	}
	// Ambient
	ambient = mat.Ambient * Light.Ambient;
}
//*********************************************************
//PointLight
//*********************************************************
void ComputePointLight(Material mat, PointLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Wektor do Ÿród³a
	float3 lightVec = Light.Position - pos;

	// Odleg³oœæ miêdzy powierzchni¹ a Ÿród³em
	float distance = length(lightVec);

	// Test zasiêgu
	if (distance > Light.Range)
		return;

	//Normalizacja wekstora œwiat³a
	lightVec /= distance;

	// Ambient
	ambient = mat.Ambient * Light.Ambient;

	// Dodaj sk³adniki œwiat³a rozproszonego i odbitego pod warunkiem, ¿e pada ono na powierzchniê.

	float diffuseFactor = dot(lightVec, normal); //Iloczyn skalarny

	// Sp³aszcz, aby unikn¹æ dynamicznego rozga³êziania.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal); //funkcja odbicia wektora

		float specFactor = pow(max(dot(v, toEye), 0.0f), (mat.specularPower));// *(SpecularPower + 0.1f)));

		diffuse = diffuseFactor * mat.Diffuse * Light.Diffuse;
		spec = specFactor * mat.Specular * Light.Specular;
	}

	// Wygaszanie - ATTENUATION
	float attAmbient = 1.0f / dot(Light.Att, float3(1.0f, distance, distance * distance));
	ambient *= attAmbient;

	float attDiffuse = 1.0f / dot(Light.Att, float3(1.0f, distance, distance * distance));
	diffuse *= attDiffuse;

	float attSpecular = 1.0f / dot(Light.Att, float3(1.0f, distance, distance * distance));
	spec *= attSpecular;
}
void ComputePointLightCookTorrance(Material mat, PointLight Light, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec, float SpecularPower)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float specular = 0.0f;

	float3 lightVec = Light.Position - pos;
	float distance = length(lightVec);

	if (distance > Light.Range)
		return;

	lightVec /= distance;

	float N_dot_L = dot(lightVec, normal);
	
	[flatten]
	if (N_dot_L > 0.0f)
	{
		float3 eyeDir = normalize(toEye);
		float3 half_vec = normalize(lightVec + eyeDir);
		float N_dot_H = dot(normal, half_vec);
		float N_dot_V = dot(normal, eyeDir);
		float V_dot_H = dot(eyeDir, half_vec);
		float r_squared = mat.roughness * mat.roughness;

		//ok
		float _2_N_dot_H = 2.0f * N_dot_H;
		float Ga = 1.0f;
		float Gb = (_2_N_dot_H * N_dot_V) / V_dot_H;
		float Gc = (_2_N_dot_H * N_dot_L) / V_dot_H;
		float G = min(Ga, min(Gb, Gc));

		//ok
		float r1 = 1.0f / (4.0f * r_squared * pow(N_dot_H, 4.0f)); 
		float r2 = (N_dot_H * N_dot_H - 1.0f) / (r_squared * N_dot_H * N_dot_H);
		float D = r1 * exp(r2);
		//ok
		float F = mat.fresnelFactor + ((1.0f - mat.fresnelFactor) * pow(1.0f - V_dot_H, 5.0f));

		specular = (D * F * G) / (3.14 * N_dot_V * N_dot_L);

		diffuse = N_dot_L * mat.Diffuse * Light.Diffuse;
		//spec = specular * mat.Specular * Light.Specular;
		spec = (SpecularPower + specular * (1.0f - SpecularPower)) * mat.Specular * Light.Specular;

	}
	// Ambient
	ambient = mat.Ambient * Light.Ambient;

	// Wygaszanie
	float attAmbient = 1.0f / dot(Light.Att, float3(1.0f, distance * distance, distance * distance));
	ambient *= attAmbient;

	float attDiffuse = 1.0f / dot(Light.Att, float3(1.0f, distance * distance, distance * distance));
	diffuse *= attDiffuse;

	float attSpecular = 1.0f / dot(Light.Att, float3(1.0f, distance * distance, distance * distance));
	spec *= attSpecular;

}







//***************************************************************************************************************************
// NormalSampleToWorldSpace
//***************************************************************************************************************************
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Dekompresuj ka¿d¹ sk³adow¹ z [0,1] do [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f;

		// Buduj ortonormaln¹ bazê.
		float3 N = unitNormalW;
		float3 T = normalize(tangentW - dot(tangentW, N)*N);
		float3 B = cross(N, T);

		float3x3 TBN = float3x3(T, B, N);

		// Przekszta³æ z przestrzeni stycznej do przestrzeni œwiata.
		float3 bumpedNormalW = mul(normalT, TBN);

		return bumpedNormalW;
}


//---------------------------------------------------------------------------------------
// Wykonuje test zacienienia, aby sprawdziæ czy piksel znajduje siê w cieniu.
//---------------------------------------------------------------------------------------

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowPosH)
{
	// Finalizuj rzutowanie dzieleniem przez w.
	shadowPosH.xyz /= shadowPosH.w;

	// G³êbokoœæ w przestrzeni NDC.
	float depth = shadowPosH.z;

	// Rozmiar teksela.
	const float dx = SMAP_DX;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}












//************************************************************************************************************************************
// Sktruktura 'PatchTess'
//************************************************************************************************************************************
struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};


//************************************************************************************************************************************
// PatchHS
//************************************************************************************************************************************
PatchTess PatchHS(InputPatch<VS_OUTPUT_TESS, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt; // Stuktura powyzej
	pt.EdgeTess[0] = (patch[1].TessFactor + patch[2].TessFactor) * 0.5f;
	pt.EdgeTess[1] = (patch[2].TessFactor + patch[0].TessFactor) * 0.5f;
	pt.EdgeTess[2] = (patch[0].TessFactor + patch[1].TessFactor) * 0.5f;
	pt.InsideTess = pt.EdgeTess[0];
	return pt; // Zwraca obiekt struktury
}

//************************************************************************************************************************************
// Sktruktura 'HullOut'
//************************************************************************************************************************************
struct HullOut
{
	float3 PosW     : POSITION;
	float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex      : TEXCOORD;
	float4 ShadowPosH : TEXCOORD1;
};


[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]

//************************************************************************************************************************************
// HullOut
//************************************************************************************************************************************
HullOut HS(InputPatch<VS_OUTPUT_TESS, 3> p, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	// Shader "przelotowy".
	hout.PosW = p[i].PositionW;
	hout.NormalW = p[i].NormalW;
	hout.TangentW = p[i].TangentW;
	hout.Tex = p[i].Tex;
	hout.ShadowPosH = p[i].ShadowPosH;

	return hout;
}



//************************************************************************************************************************************
// Sktruktura 'Domain OUT'
//************************************************************************************************************************************
struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
	float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex      : TEXCOORD;
	float4 ShadowPosH : TEXCOORD1;
};
[domain("tri")]


//************************************************************************************************************************************
// Domain OUT - Shader wierzcho³ków po teselacji
//************************************************************************************************************************************
DomainOut DS(PatchTess patchTess, float3 bary : SV_DomainLocation, const OutputPatch<HullOut, 3> tri)
{
	DomainOut dout;

	// Interpolacja atrybutów ³aty.
	dout.PosW = bary.x*tri[0].PosW + bary.y*tri[1].PosW + bary.z*tri[2].PosW;
	dout.NormalW = bary.x*tri[0].NormalW + bary.y*tri[1].NormalW + bary.z*tri[2].NormalW;
	dout.TangentW = bary.x*tri[0].TangentW + bary.y*tri[1].TangentW + bary.z*tri[2].TangentW;
	dout.Tex = bary.x*tri[0].Tex + bary.y*tri[1].Tex + bary.z*tri[2].Tex;
	dout.ShadowPosH = bary.x*tri[0].ShadowPosH + bary.y*tri[1].ShadowPosH + bary.z*tri[2].ShadowPosH;
	
	// Normalizuj ponownie normaln¹ (mog³a ulec denormalizacji w procesie interpolacji).
	dout.NormalW = normalize(dout.NormalW);
	//
	// Mapowanie przemieszczeñ.	
	//
	const float MipInterval = 50.0f; //Bylo 20.0f
	float mipLevel = clamp((distance(dout.PosW, gEyePosW) - MipInterval) / MipInterval, 0.0f, 6.0f);

	// Próbkuj mapê wysokoœci (przechowywan¹ w kanalach rgb).
	float h = gASHMap.SampleLevel(samAnisotropic, dout.Tex, mipLevel).b;
	
	// Przesuñ wierzcho³ek wzd³u¿ wektora normalnego.
	float3 normalMapSample123 = gNormalMap.SampleLevel(samAnisotropic, dout.Tex, mipLevel).rgb;
	dout.NormalW = NormalSampleToWorldSpace(normalMapSample123, dout.NormalW, dout.TangentW);
	dout.PosW += (gMaterial.gHeightScale*h)* dout.NormalW;

	// Generuj wspó³rzêdne rzutowe tekstury, aby zrzutowaæ mapê cieni na scenê.
	//dout.ShadowPosH = mul(float4(dout.PosW, 1.0f), gShadowTransform);
	//dout.ShadowPosH = mul(float4(dout.PosW, 1.0f), gShadowTransform);

	// Rzutuj do jednorodnej przestrzeni obcinania.
	dout.PosH = mul(float4(dout.PosW, 1.0f), WorldViewProjection);

	//dout.PosH = dout.PosH *2 ;

	
	return dout;
}
//--------------------------------------------------------------------------------------
// Pixel Shader Tessallation // Dla teselacja DomainOut 
//--------------------------------------------------------------------------------------
float4 PS_PHONG_TESS(DomainOut input) : SV_Target
{
	//Normalize
	input.NormalW = normalize(input.NormalW);
	//To eye
	float3 toEyeW = normalize(gEyePosW - input.PosW);

	//Values
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float SpecularPower = 0.0f;
	float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (__allowSpecular)
	SpecularPower = gASHMap.Sample(samAnisotropic, input.Tex).g;
	else
	SpecularPower = 0.0f;

	//----------------------
	//Check alpha
	//----------------------
	//if (__allowAlpha)
	//{
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex).a;
		clip(texColor.rgb - 0.1f);
	//}


	//----------------------
	//Shadow
	//----------------------
	float3 shadow = float3(1.0f, 1.0f, 1.0f);
	if (__allowShadowMap == true)
		shadow[0] = CalcShadowFactor(samShadow, gShadowMap, input.ShadowPosH);

	//------
	//SSAO
	//------
	/*input.SSAOPosH /= input.SSAOPosH.w;
	float ambientSSAO = 1.0f;
	if(__allowSSAOMap == true)
	ambientSSAO = gSSAOMap.SampleLevel(samTriLinearSam, input.SSAOPosH.xy, 0.0f).r;
	*/
	float ambientSSAO = 1.0f;
	float SpecularMul = 1.0f;
	if (__allowSpecular)
	{
		ambientSSAO = gASHMap.Sample(samAnisotropic, input.Tex).r;
		SpecularMul = gASHMap.Sample(samAnisotropic, input.Tex).g;
	}



	//--------------------------------------
	//Mapowanie normalnych
	//--------------------------------------
	float3 bumpedNormalW = float3(0.0f, 0.0f, 0.0f);

	if (__allowNormal)
	{
		float3 normalMapSample = gNormalMap.Sample(samAnisotropic, input.Tex).rgb;
		bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.NormalW, input.TangentW);
		bumpedNormalW = normalize(bumpedNormalW); //normalize bumpedNormal

												  //--------------------
												  //DirectionalLight
												  //--------------------
		ComputeDirectionalLight(gMaterial, gDirectionalLight, input.PosW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLight(gMaterial, gPointLight[i], input.PosW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) * ambientSSAO *Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * SpecularMul * Specular;
				}
				else
				{
					ambient += ambientSSAO *Ambient;
					diffuse += Diffuse;
					spec += SpecularMul * Specular;
				}
			}
		}
	}
	else
	{
		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLight(gMaterial, gDirectionalLight, input.PosW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLight(gMaterial, gPointLight[i], input.PosW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) *ambientSSAO * Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * SpecularMul * Specular;
				}
				else
				{
					ambient += ambientSSAO * Ambient;
					diffuse += Diffuse;
					spec += SpecularMul * Specular;
				}
			}
		}
	}
	//set diffuse colour from material
	texColor = float4(gMaterial.Diffuse.r, gMaterial.Diffuse.g, gMaterial.Diffuse.b, 1.0f);

	//Diffuse texture
	if (__allowDiffuse)
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex);

	FinalColor = texColor*(ambient + diffuse) + spec;

	//Set alpha
	FinalColor.a = gMaterial.Diffuse.a;

	//CubeMapping
	if (__allowCubeMap == true)
	{
		float3 incident = -toEyeW;
		float3 reflectionVector = float3(0.0f, 0.0f, 0.0f);
		if (__allowNormal)
			reflectionVector = reflect(incident, bumpedNormalW);
		else
			reflectionVector = reflect(incident, input.NormalW);

		float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
		FinalColor += gMaterial.Reflect * reflectionColor;
	}


	FinalColor.a = __transparency;
	return FinalColor;

}
//--------------------------------------------------------------------------------------
// Pixel Shader Tessallation // Dla teselacja DomainOut 
//--------------------------------------------------------------------------------------
float4 PS_TESS(DomainOut input) : SV_Target
{
	input.NormalW = normalize(input.NormalW);

	float3 toEyeW = normalize(gEyePosW - input.PosW);

	// Rozpocznij sumowanie od zera.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 texColor = float4(1, 1, 1, 1);

	float SpecularPower = 0.0f;

	//----------------------
	//Check alpha
	//----------------------
	//if (__allowAlpha)
	//{
		texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex).a;
		clip(texColor.rgb - 0.1f);
	//}


	float4 Ambient, Diffuse, Specular;


	//----------------------
	//Shadow
	//----------------------
	float3 shadow = float3(1.0f, 1.0f, 1.0f);
	if (__allowShadowMap == true)
	shadow[0] = CalcShadowFactor(samShadow, gShadowMap, input.ShadowPosH);


	//------
	//SSAO
	//------
	/*input.SSAOPosH /= input.SSAOPosH.w;
	float ambientSSAO = 1.0f;
	if (__allowSSAOMap == true)
	ambientSSAO = gSSAOMap.SampleLevel(samTriLinearSam, input.SSAOPosH.xy, 0.0f).r;
	*/
	float ambientSSAO = 1.0f;
	float SpecularMul = 1.0f;
	if (__allowSpecular)
	{
		ambientSSAO = gASHMap.Sample(samAnisotropic, input.Tex).r;
		SpecularMul = gASHMap.Sample(samAnisotropic, input.Tex).g;
	}


	//--------------------------------------
	//Mapowanie normalnych
	//--------------------------------------
	float3 bumpedNormalW = float3(0.0f, 0.0f, 0.0f);
	if (__allowNormal)
	{
		float3 normalMapSample = gNormalMap.Sample(samAnisotropic, input.Tex).rgb;
		bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.NormalW, input.TangentW);
		bumpedNormalW = normalize(bumpedNormalW); //normalize bumpedNormal

		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLightCookTorrance(gMaterial, gDirectionalLight, input.PosW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLightCookTorrance(gMaterial, gPointLight[i], input.PosW, bumpedNormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * Specular;
				}
				else
				{
					ambient += ambientSSAO * Ambient;
					diffuse += Diffuse;
					spec += Specular;
				}
			}
		}
	}
	else
	{
		//--------------------
		//DirectionalLight
		//--------------------
		ComputeDirectionalLightCookTorrance(gMaterial, gDirectionalLight, input.PosW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
		if (__allowShadowMap == true)
		{
			ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
			diffuse += shadow[0] * Diffuse;
			spec += shadow[0] * SpecularMul * Specular;
		}
		else
		{
			ambient += ambientSSAO * Ambient;
			diffuse += Diffuse;
			spec += SpecularMul * Specular;
		}
		//--------------------
		//PointLights
		//--------------------  
		[unroll]
		for (int i = 0; i < 10; ++i)
		{
			if (gPointLight[i].Range > 0.0f)
			{
				ComputePointLightCookTorrance(gMaterial, gPointLight[i], input.PosW, input.NormalW, toEyeW, Ambient, Diffuse, Specular, SpecularPower);
				if (__allowShadowMap == true)
				{
					ambient += (shadow[0] + 0.5) * ambientSSAO * Ambient;
					diffuse += shadow[0] * Diffuse;
					spec += shadow[0] * SpecularMul * Specular;
				}
				else
				{
					ambient += ambientSSAO * Ambient;
					diffuse += Diffuse;
					spec += SpecularMul * Specular;
				}
			}
		}
	}
	float4 litColor;


	texColor = float4(gMaterial.Diffuse.r, gMaterial.Diffuse.g, gMaterial.Diffuse.b, 1.0f);//set default colour

	//Diffuse texture
	if (__allowDiffuse)
	texColor = gDiffuseMap.Sample(samAnisotropic, input.Tex);

	litColor = texColor*(ambient + diffuse) + spec;

	//Set alpha
	litColor.a = gMaterial.Diffuse.a;

	//CubeMapping
	if (__allowCubeMap == true)
	{
		float3 incident = -toEyeW;
		float3 reflectionVector = float3(0.0f, 0.0f, 0.0f);
		if (__allowNormal)
			reflectionVector = reflect(incident, bumpedNormalW);
		else
			reflectionVector = reflect(incident, input.NormalW);

		float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
		litColor += gMaterial.Reflect * reflectionColor;
	}

	litColor.a = __transparency;
	return litColor;
}




































//***************************************************************************************************************************PS_CookTorrance
technique11 FeatureLevel11CookTorrance //PS_CookTorrance  FeatureLevel11CookTorrance
{
	pass P0
	{
		SetBlendState(AlphaBlendingOFF, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_CookTorrance()));
	}
}
technique11 FeatureLevel11Phong
{
	pass P0
	{
		SetBlendState(AlphaBlendingOFF, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Phong()));
	}
}


technique11 FeatureLevel11PhongTransparency
{
	pass P0
	{
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Phong()));
	}
}

technique11 FeatureLevel11CookTorranceTransparency
{
	pass P0
	{
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_CookTorrance()));
	}
}


technique11 FeatureLevel11PhongTessellation
{
	pass P0
	{
		SetBlendState(AlphaBlendingOFF, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS_TESS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_PHONG_TESS()));
	}
}

technique11 FeatureLevel11Tessellation
{
	pass P0
	{
		SetBlendState(AlphaBlendingOFF, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS_TESS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_TESS()));
	}
}
















technique11 FeatureLevel11NotNormal
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Phong()));
	}
}


technique11 FeatureLevel11NotTexture
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Phong()));
	}
}


technique11 FeatureLevel11NotTextureNormal
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Phong()));
	}
}


technique11 FeatureLevel11_LINES
{
	pass P0
	{
		SetBlendState(AlphaBlendingOn, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_5_0, VS_LINES()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_LINE(false)));
	}
}

technique11 FeatureLevel11_LINES_CIRCLE
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(false)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_LINE(true)));
	}
}


technique11 FeatureLevel11NORMAL
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_LINES()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_NormalView(false)));
	}
}


technique11 FeatureLevel11COLLISION_RENDER
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_RenderCollision(false)));
	}
}


technique11 FeatureLevel11NORMAL_TEXTURE
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_NormalView(true)));
	}
}


technique11 FeatureLevel11WireSelected
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_SelectedWire()));
	}
}

technique11 FeatureLevel11WhiteWireFrame
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_WhiteWireFrame()));
	}
}

technique11 FeatureLevel11RedWireFrame
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_RedWireFrame()));
	}
}




technique11 FeatureLevel11_NEW
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS(true)));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_DRAW_SHADOW_TEXTURES()));
	}
}










