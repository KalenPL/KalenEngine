#include "../Header Files/MaterialInfo.h"


MaterialInfo::MaterialInfo()
	:
	mMatName("Mat #?"), 
	mShaderName("Phong"),
	mTextureDiffuseName("diffuse.tga"),
	mTextureNormalName("normal.tga"),
	mTextureASHName("specular.tga"),
	mTextureCubeName("NULL"),
	//mTextureHeightName("height.tga"),
	//mTextureAlphaName("alpha.tga"),
	mAmbientColour(wxColour(255, 255, 255)),
	mDiffuseColour(wxColour(255, 255, 255)),
	mSpecularColour(wxColour(255, 255, 255)),
	mReflectColour(wxColour(255, 255, 255)),
	mSpecularPower(128.0),
	mTransparency(1.0),
	mRoughness(0.5),
	mFresnelFactor(1.0f),
	mTessellationHeightScale(4.0),
	mTessellationMaxDistance(40.0),
	mTessellationMinDistance(10.0),
	mTessellationMaxFactor(4.0),
	mTessellationMinFactor(1.0),
	mNumberOfTriangles(0),
	mNumberOfVertices(0),
	mNumberOfIndices(0),
	mCubeMapDynamic(false)
{

}
MaterialInfo::~MaterialInfo()
{

}
wxString MaterialInfo::GetMatName()
{
	return this->mMatName;
}
wxString MaterialInfo::GetShaderName()
{
	return this->mShaderName;
}
wxString MaterialInfo::GetTextureDiffuseName()
{
	return this->mTextureDiffuseName;
}
wxString MaterialInfo::GetTextureNormalName()
{
	return this->mTextureNormalName;
}
wxString MaterialInfo::GetTextureASHName()
{
	return this->mTextureASHName;
}
wxString MaterialInfo::GetTextureCubeName()
{
	return this->mTextureCubeName;
}
/*wxString MaterialInfo::GetTextureHeightName()
{
	return this->mTextureHeightName;
}
wxString MaterialInfo::GetTextureAlphaName()
{
	return this->mTextureAlphaName;
}*/
wxColour MaterialInfo::GetAmbientColour()
{
	return this->mAmbientColour;
}
wxColour MaterialInfo::GetDiffuseColour()
{
	return this->mDiffuseColour;
}
wxColour MaterialInfo::GetSpecularColour() 
{
	return this->mSpecularColour;
}
wxColour MaterialInfo::GetReflectColour()
{
	return this->mReflectColour;
}
float MaterialInfo::GetSpecularPower()
{
	return this->mSpecularPower;
}
float MaterialInfo::GetTransparency()
{
	return this->mTransparency;
}
float MaterialInfo::GetTessellationHeightScale()
{
	return this->mTessellationHeightScale;
}
float MaterialInfo::GetTessellationMaxDistance()
{
	return this->mTessellationMaxDistance;
}
float MaterialInfo::GetTessellationMinDistance()
{
	return this->mTessellationMinDistance;
}
float MaterialInfo::GetTessellationMaxFactor()
{
	return this->mTessellationMaxFactor;
}
float MaterialInfo::GetTessellationMinFactor()
{
	return this->mTessellationMinFactor;
}
unsigned int MaterialInfo::GetNumberOfTriangles()
{
	return this->mNumberOfTriangles;
}
unsigned int MaterialInfo::GetNumberOfVertices()
{
	return this->mNumberOfVertices;
}
unsigned int MaterialInfo::GetNumberOfIndices()
{
	return this->mNumberOfIndices;
}
float MaterialInfo::GetRoughness()
{
	return this->mRoughness;
}
float MaterialInfo::GetFresnelFactor()
{
	return this->mFresnelFactor;
}
bool MaterialInfo::GetCubeMapDynamic()
{
	return this->mCubeMapDynamic;
}

//***************************************************************************************************************************
void MaterialInfo::SetMatName(wxString name)
{
	this->mMatName = name;
}
void MaterialInfo::SetShaderName(wxString name)
{
	this->mShaderName = name;
}
void MaterialInfo::SetTextureDiffuseName(wxString name)
{
	this->mTextureDiffuseName = name;
}
void MaterialInfo::SetTextureNormalName(wxString name)
{
	this->mTextureNormalName = name;
}
void MaterialInfo::SetTextureASHName(wxString name)
{
	this->mTextureASHName = name;
}
void MaterialInfo::SetTextureCubeName(wxString name)
{
	this->mTextureCubeName = name;
}
/*void MaterialInfo::SetTextureHeightName(wxString name)
{
	this->mTextureHeightName = name;
}
void MaterialInfo::SetTextureAlphaName(wxString name)
{
	this->mTextureAlphaName = name;
}*/
void MaterialInfo::SetAmbientColour(wxColour colour)
{
	this->mAmbientColour = colour;
}
void MaterialInfo::SetDiffuseColour(wxColour colour)
{
	this->mDiffuseColour = colour;
}
void MaterialInfo::SetSpecularColour(wxColour colour)
{
	this->mSpecularColour = colour;
}
void MaterialInfo::SetReflectColour(wxColour colour)
{
	this->mReflectColour = colour;
}
void MaterialInfo::SetSpecularPower(float value)
{
	this->mSpecularPower = value;
}
void MaterialInfo::SetTransparency(float value)
{
	this->mTransparency = value;
}
void MaterialInfo::SetTessellationHeightScale(float value)
{
	this->mTessellationHeightScale = value;
}
void MaterialInfo::SetTessellationMaxDistance(float value)
{
	this->mTessellationMaxDistance = value;
}
void MaterialInfo::SetTessellationMinDistance(float value)
{
	this->mTessellationMinDistance = value;
}
void MaterialInfo::SetTessellationMaxFactor(float value)
{
	this->mTessellationMaxFactor= value;
}
void MaterialInfo::SetTessellationMinFactor(float value)
{
	this->mTessellationMinFactor = value;
}
void MaterialInfo::SetNumberOfTriangles(unsigned int value)
{
	this->mNumberOfTriangles = value;
}
void MaterialInfo::SetNumberOfVertices(unsigned int value)
{
	this->mNumberOfVertices = value;
}
void MaterialInfo::SetNumberOfIndices(unsigned int value)
{
	this->mNumberOfIndices = value;
}
void MaterialInfo::SetRoughness(float value)
{
	this->mRoughness = value;
}
void MaterialInfo::SetFresnelFactor(float value)
{
	this->mFresnelFactor = value;
}
void MaterialInfo::SetCubeMapDynamic(bool value)
{
	this->mCubeMapDynamic = value;
}
