#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utility.h"



struct MaterialHLSL
{
	MaterialHLSL() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;

	float roughness;
	float fresnelFactor;
	float specularPower;

	float heightScaleTess;//ok
	float minFactorTess;
	float minDistanceTess; //ok
	float maxDistanceTess;
	float maxFactorTess;//ok
};






struct Material
{

	// C-tor set default data
	Material()
	{
		this->materialName = "";
		this->shaderName = "Phong";
		XMFLOAT4 empty = XMFLOAT4(1.0, 1.0, 1.0, 1.0); //default set on white
		this->diffuseColour = empty;
		this->ambientColour = empty;
		this->specularColour = empty;
		this->reflectColour = empty;
		this->materialHlsl.Ambient = empty;
		this->materialHlsl.Diffuse = empty;
		this->materialHlsl.Specular = empty;
		this->materialHlsl.Reflect = empty;

		this->materialHlsl.heightScaleTess = 1.5;
		this->materialHlsl.maxDistanceTess = 150.0;
		this->materialHlsl.minDistanceTess = 300.0;
		this->materialHlsl.maxFactorTess = 1.0;
		this->materialHlsl.minFactorTess = 8.0;



		this->alphaColour = 1.0;
		//this->alphaMapName = "NULL";
		this->diffuseMapName = "NULL";
		//this->heightMapName = "NULL";
		this->normalMapName = "NULL";
		this->ashMapName = "NULL";
		this->cubeMapName = "NULL";

		this->diffuseMap = nullptr;
		this->normalMap = nullptr;
		this->ashMap = nullptr;
		this->cubeMap = nullptr;
		//this->alphaMap = nullptr;
		//this->heightMap = nullptr;

		this->hasTessellation = false;
		this->heightScaleTess = 1.5;
		this->maxDistanceTess = 150.0;
		this->minDistanceTess = 300.0;
		this->maxFactorTess = 1.0;
		this->minFactorTess = 8.0;

		this->roughness = 0.5f;
		this->fresnelFactor = 1.0f;
		this->specularPower = 64.0f;
		this->cubeMapDynamic = false;


	}
	string shaderName;
	string materialName;
	MaterialHLSL materialHlsl;

	// Colour information
	XMFLOAT4	ambientColour;
	XMFLOAT4	diffuseColour;
	XMFLOAT4	specularColour;
	XMFLOAT4	reflectColour;
	float		alphaColour;

	// Textures information:
	string  diffuseMapName;
	string	normalMapName;
	string	ashMapName;
	string	cubeMapName;

	ID3D11ShaderResourceView* diffuseMap;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* ashMap;
	ID3D11ShaderResourceView* cubeMap;

	float roughness;
	float fresnelFactor;
	float specularPower;
	float transparency;

	//Tessellation
	bool hasTessellation;
	float heightScaleTess;
	short maxDistanceTess;
	short minDistanceTess;
	short maxFactorTess;
	short minFactorTess;

	bool cubeMapDynamic;

};


#endif //MATERIAL_H