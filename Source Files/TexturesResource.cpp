#include "../Header Files/TexturesResource.h"


//**************************************************************************************************************
// Constructor 
//**************************************************************************************************************
TexturesResource::TexturesResource()
{
}
TexturesResource::TexturesResource(ID3D11Device* m_pDevice)
{
	this->m_pDevice = m_pDevice;
}
//**************************************************************************************************************
// Destructor
//**************************************************************************************************************
TexturesResource::~TexturesResource()
{

}

//**************************************************************************************************************
// Add texture to resource (return a pointer to texture)
//**************************************************************************************************************
ID3D11ShaderResourceView* TexturesResource::AddTextureToBuffor(std::string nameTexture)
{
	for (int i = 0; i < (int)mTexturesStructVector.size(); ++i)
	{
		if (mTexturesStructVector[i].Name == nameTexture)
		{
			mTexturesStructVector[i].Counter += 1;
		//	INFO_MESSAGE("This texture already exist in the texturesBuffer");
			return mTexturesStructVector[i].Texture;
			break;
		}
	}

	ID3D11ShaderResourceView*  textureMapTemp;
	ScratchImage image;
	ScratchImage imageMipMap;
	TexMetadata metadata;


	std::wstring stemp = std::wstring(nameTexture.begin(), nameTexture.end());
	LPCWSTR name = stemp.c_str();

	std::string format = "";
	format += nameTexture.at(nameTexture.size() - 3);
	format += nameTexture.at(nameTexture.size() - 2);
	format += nameTexture.at(nameTexture.size() - 1);
	HRESULT hr;
	if (format != "tga" && format != "TGA")
	{
		hr = D3DX11CreateShaderResourceViewFromFile(m_pDevice, name, 0, 0, &textureMapTemp, 0);
		if (FAILED(hr))
		{
			wxMessageBox("Cannot load texture: " + nameTexture, "Error!", wxICON_ERROR | wxOK);
			return NULL;
		}
	}
	else
	{
		//Load image from TGA texture
		hr = LoadFromTGAFile(name, &metadata, image);
		if (FAILED(hr))
		{
			wxMessageBox("Cannot load texture: " + nameTexture, "Error!", wxICON_ERROR | wxOK);
			return NULL;
		}
		//Generate MipMaps
		GenerateMipMaps(image.GetImages(), image.GetImageCount(), metadata, TEX_FILTER_DEFAULT, 0, imageMipMap);
		if (FAILED(hr))
		{
			wxMessageBox("Cannot create mipmaps for texture: " + nameTexture, "Error!", wxICON_ERROR | wxOK);
			return NULL;
		}
		//Create texture
		hr = CreateShaderResourceView(m_pDevice, imageMipMap.GetImages(), imageMipMap.GetImageCount(), imageMipMap.GetMetadata(), &textureMapTemp);
		if (FAILED(hr))
		{
			wxMessageBox("Cannot create texture: " + nameTexture, "Error!", wxICON_ERROR | wxOK);
			return NULL;
		}
		
	}
		




	//Create object
	TexturesStruct texture;
	texture.Counter = 0;
	texture.Name = nameTexture;
	texture.Texture = textureMapTemp;

	//Add to vector
	mTexturesStructVector.push_back(texture);


	return textureMapTemp;
}