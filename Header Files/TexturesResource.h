#ifndef TEXTURES_RESOURCE_H
#define TEXTURES_RESOURCE_H

#include "Utility.h"


struct TexturesStruct
{
	TexturesStruct() { ZeroMemory(this, sizeof(this)); }

	ID3D11ShaderResourceView* Texture;		//Pointer to texture
	std::string Name;						//Name of texture
	UINT Counter;							//Count number of textures
};


class TexturesResource
{
public:
	TexturesResource();
	TexturesResource(ID3D11Device* m_pDevice);
	~TexturesResource();
	ID3D11ShaderResourceView* AddTextureToBuffor(std::string nameTexture); //Return a pointer to texture resource view 
private:
	std::vector<TexturesStruct> mTexturesStructVector;

	ID3D11Device*	m_pDevice;  // Device;

};
#endif //TEXTURES_RESOURCE_H

