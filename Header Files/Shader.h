#ifndef SHADER_H
#define SHADER_H

#include "Utility.h"
#include "DXApp.h"
using namespace DirectX;

class Shader
{
public:
	Shader();
	~Shader();



	HRESULT CompileShaderFromFileNew(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob, _Outptr_ ID3DBlob** blobError);

	std::vector<char> ReadShaderFromBinaryFileFXO(_In_ LPCWSTR srcFile, int &size);



	ID3D11Device* CreateVertexShader(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR profile, ID3D11Device** m_pDevice, ID3D11VertexShader** pVertexShader);
	ID3D11Device* CreatePixelShader(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR profile, ID3D11Device** m_pDevice, ID3D11ClassLinkage* pPSClassLinkage, ID3D11PixelShader** pPixelShader);

	ID3DBlob* getVSBlob();
	ID3DBlob* getPSBlob();

private:

	HRESULT CompileShaderFromFile(LPCWSTR, LPCSTR, LPCSTR, ID3DBlob**, ID3DBlob**);

	ID3DBlob* pVSBlob;
	ID3DBlob* pVSBlobError;

	ID3DBlob* pPSBlob;
	ID3DBlob* pPSBlobError;

}; 


#endif //SHADER_H