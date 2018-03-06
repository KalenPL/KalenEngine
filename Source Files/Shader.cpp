#include "../Header Files/Shader.h"



//******************************************************************************************************************************
//Constructor
//******************************************************************************************************************************
Shader::Shader()
{
	pVSBlob = nullptr;
	pVSBlobError = nullptr;
	pPSBlob = nullptr;
	pPSBlobError = nullptr;


}


//******************************************************************************************************************************
//Destructor
//******************************************************************************************************************************
Shader::~Shader(){}

//******************************************************************************************************************************
// Compile Shader From File
//******************************************************************************************************************************
HRESULT Shader::CompileShaderFromFile(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob, _Outptr_ ID3DBlob** blobError)
{

	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;
	*blobError = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	//Temporary values
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//Try to compile file...
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);



	//Any error?
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (shaderBlob)
			shaderBlob->Release();
		return hr;
	}

	//Output value
	*blob = shaderBlob;
	*blobError = errorBlob;


	//return HRESULT
	return hr;

}
//******************************************************************************************************************************
// Create VERTEX Shader -- return pointer to the device
//******************************************************************************************************************************
ID3D11Device* Shader::CreateVertexShader(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR profile, ID3D11Device** m_pDevice, ID3D11VertexShader** pVertexShader)
{
	ID3D11Device* pDevice = nullptr;

	pDevice = *m_pDevice;
	HRESULT hr = S_OK;


	//---------------------------------------------------------
	// COMPILING VERTEX SHADER...
	//---------------------------------------------------------
	hr = CompileShaderFromFile(fileName, entryPoint, profile, &pVSBlob, &pVSBlobError);

	if (FAILED(hr))
	{
		std::wostringstream yy__;
		yy__.precision(6);
		yy__ << "'" << fileName << "' - Failed compiling vertex shader!";
		MessageBox(NULL, yy__.str().c_str(), L"Error!", MB_OK);
		PostQuitMessage(0);
	}
	//else
	//if (pVSBlobError != NULL)
	//{
	//MessageBox(NULL, L"Failed compiling vertex shader .fx - Unknown error", L"B³¹d!", MB_OK);
	//PostQuitMessage(0);
	//}

	// Create vertex shader
	hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, pVertexShader);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"B³¹d przy tworzeniu shader'a vertex'ów", L"B³¹d!", MB_OK);
		pVSBlob->Release();
		PostQuitMessage(0);
	}


	//return pointer to the Device
	return pDevice;
}
//******************************************************************************************************************************
// Create PIXEL Shader -- return pointer to the device
//******************************************************************************************************************************
ID3D11Device* Shader::CreatePixelShader(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR profile, ID3D11Device** m_pDevice, ID3D11ClassLinkage* pPSClassLinkage, ID3D11PixelShader** pPixelShader)
{
	ID3D11Device* pDevice = nullptr;

	pDevice = *m_pDevice;
	HRESULT hr = S_OK;


	//---------------------------------------------------------
	// COMPILING VERTEX SHADER...
	//---------------------------------------------------------
	hr = CompileShaderFromFile(fileName, entryPoint, profile, &pPSBlob, &pPSBlobError);

	if (FAILED(hr))
	{
		std::wostringstream yy__;
		yy__.precision(6);
		yy__ << "'" << fileName << "' - Failed compiling pixels shader!";
		MessageBox(NULL, yy__.str().c_str(), L"Error!", MB_OK);
		PostQuitMessage(0);
	}
	//else
	//if (pVSBlobError != NULL)
	//{
	//MessageBox(NULL, L"Failed compiling vertex shader .fx - Unknown error", L"B³¹d!", MB_OK);
	//PostQuitMessage(0);
	//}




	// Create vertex shader
	//Dodalem tutaj pPsClassLinkage -> Sluzy do linkowania shader'ow
	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), pPSClassLinkage, pPixelShader);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"B³¹d przy tworzeniu shader'a pixel'a", L"B³¹d!", MB_OK);
		pPSBlob->Release();
		PostQuitMessage(0);
	}


	//return pointer to the Device
	return pDevice;
}






//******************************************************************************************************************************
//******************************************************************************************************************************






HRESULT Shader::CompileShaderFromFileNew(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob, _Outptr_ ID3DBlob** blobError)
{

	//if (!srcFile || !entryPoint || !profile || !blob)
	if (!srcFile  || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;
	*blobError = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	//Temporary values
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//Try to compile file...
	//HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);
	//HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);
	//HRESULT hr = D3DCompileFromFile(srcFile, defines, 0, 0, profile, flags, 0, &shaderBlob, &errorBlob);
	HRESULT hr = D3DCompileFromFile(srcFile, 0, 0, NULL, profile, flags, 0, &shaderBlob, &errorBlob);


	//Any error?
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (shaderBlob)
			shaderBlob->Release();
		return hr;
	}

	//Output value
	*blob = shaderBlob;
	*blobError = errorBlob;


	//return HRESULT
	return hr;

}

//******************************************************************************************************************************
// Get VSBlob -- return pointer to pVSBlob
//******************************************************************************************************************************
ID3DBlob* Shader::getVSBlob()
{
	return this->pVSBlob;
}

//******************************************************************************************************************************
// Get PSBlob -- return pointer to pPSBlob
//******************************************************************************************************************************
ID3DBlob* Shader::getPSBlob()
{
	return this->pPSBlob;
}


//******************************************************************************************************************************
// Read binary shader (return - vector of shader)
//******************************************************************************************************************************
std::vector<char> Shader::ReadShaderFromBinaryFileFXO(_In_ LPCWSTR srcFile, int &size)
{
	std::ifstream fin(srcFile, ios::binary); 
	std::vector<char> compiledShaderVector;

	if (fin)
	{
		fin.seekg(0, ios_base::end);
		size = (int)fin.tellg();
		fin.seekg(0, ios_base::beg);
		compiledShaderVector.resize(size);

		//read
		fin.read(&compiledShaderVector[0], size);

	}
	else
	{
		MessageBox(NULL, L"ReadShaderFromBinaryFileFXO - file error", L"INPUT FILE ERROR", MB_OK);
	}
	fin.close();


	return compiledShaderVector;




}



