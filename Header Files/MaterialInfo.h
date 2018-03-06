#ifndef __MATERIAL_INFO__
#define  __MATERIAL_INFO__

#include "Utility.h"

class MaterialInfo
{
public:
	MaterialInfo();
	~MaterialInfo();
	wxString GetMatName();
	wxString GetShaderName();
	wxString GetTextureDiffuseName();
	wxString GetTextureNormalName();
	wxString GetTextureASHName();
	wxString GetTextureCubeName();
//	wxString GetTextureHeightName();
//	wxString GetTextureAlphaName();
	wxColour GetAmbientColour();
	wxColour GetDiffuseColour();
	wxColour GetSpecularColour();
	wxColour GetReflectColour();
	float GetSpecularPower();
	float GetRoughness();
	float GetFresnelFactor();
	float GetTessellationHeightScale();
	float GetTessellationMaxDistance();
	float GetTessellationMinDistance();
	float GetTessellationMaxFactor();
	float GetTessellationMinFactor();
	float GetTransparency();
	bool GetCubeMapDynamic();
	
	unsigned int GetNumberOfTriangles();
	unsigned int GetNumberOfVertices();
	unsigned int GetNumberOfIndices();

	void SetMatName(wxString name);
	void SetShaderName(wxString name);
	void SetTextureDiffuseName(wxString name);
	void SetTextureNormalName(wxString name);
	void SetTextureASHName(wxString name);
	void SetTextureCubeName(wxString name);
	//void SetTextureHeightName(wxString name);
	//void SetTextureAlphaName(wxString name);
	void SetAmbientColour(wxColour name);
	void SetDiffuseColour(wxColour name);
	void SetSpecularColour(wxColour name);
	void SetReflectColour(wxColour name);
	void SetTransparency(float value);
	void SetSpecularPower(float value);
	void SetRoughness(float value);
	void SetFresnelFactor(float value);
	void SetTessellationHeightScale(float value);
	void SetTessellationMaxDistance(float value);
	void SetTessellationMinDistance(float value);
	void SetTessellationMaxFactor(float value);
	void SetTessellationMinFactor(float value);
	void SetNumberOfTriangles(unsigned int value);
	void SetNumberOfVertices(unsigned int value);
	void SetNumberOfIndices(unsigned int value);
	void SetCubeMapDynamic(bool signal);

private:

	wxString mMatName;
	wxString mShaderName;
	wxString mTextureDiffuseName;
	wxString mTextureNormalName;
	wxString mTextureASHName;
	wxString mTextureCubeName;
//	wxString mTextureHeightName;
	//wxString mTextureAlphaName;
	wxColour mAmbientColour;
	wxColour mDiffuseColour;
	wxColour mSpecularColour;
	wxColour mReflectColour;
	float mTransparency;
	float mSpecularPower;
	float mRoughness;
	float mFresnelFactor;
	float mTessellationHeightScale;
	float mTessellationMaxDistance;
	float mTessellationMinDistance;
	float mTessellationMaxFactor;
	float mTessellationMinFactor;
	bool mCubeMapDynamic;

	unsigned int mNumberOfTriangles;
	unsigned int mNumberOfVertices;
	unsigned int mNumberOfIndices;

};

#endif // !__MATERIAL_INFO__