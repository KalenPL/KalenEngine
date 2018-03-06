#include "../Header Files/KalenEngine.h"
//*******************************************************************************
//Global values
//*******************************************************************************
ID3D11Device*	m_pDevice123;  // md3dDevice;


extern bool gGlobalSpace;
extern int gTransformGizmoType;
extern bool gFilterRenderMesh;
extern bool gMultiLayers;
extern wxTextCtrl	*mTextOututWindow123;

klmeshFileStruct mKLMeshGLOBAL;


//************************************************************************************************************************************************
//************************************************************************************************************************************************
//C-tor
KalenEngine::KalenEngine(wxWindow* parent, wxWindowID id, klmeshFileStruct klmesh) : DXApp(parent, id)
{
	mFX = nullptr;
	mTech = nullptr;
	mFX_pWorldViewProjection = nullptr;
	mFX_pWorldViewProjectionTex = nullptr;
	mFX_pWorld = nullptr;
	mFX_pWorldInvTranspose = nullptr;
	mFX_pTexTransform = nullptr;
	mFX_pShadowTransform = nullptr;

	mFX_pEyePosition = nullptr;
	mFX_pPointLight = nullptr;
	mFX_pDirectionalLight = nullptr;
	mFX_pMaterial = nullptr;
	mEyePosW = XMFLOAT3(100.0f, 0.0f, 0.0f);

	mFX_pDiffuseMap = nullptr;

	//***********
	//Tessellation
	//***********
	mFXTess = nullptr;
	mTechTess = nullptr;
	mFXTess_pWorldViewProjection = nullptr;
	mFXTess_pWorld = nullptr;
	mFXTess_pWorldInvTranspose = nullptr;
	mFXTess_pTexTransform = nullptr;
	mFXTess_pShadowTransform = nullptr;

	mFXTess_pEyePosition = nullptr;
	mFXTess_pPointLight = nullptr;
	mFXTess_pMaterial = nullptr;

	//***********
	//Shadow
	//***********
	mFXShadow = nullptr;
	mTechShadow = nullptr;
	mFXShadow_pWorldViewProjection = nullptr;
	mFXShadow_pWorld = nullptr;
	mFXShadow_pWorldInvTranspose = nullptr;
	mFXShadow_pViewProjection = nullptr;
	mFXShadow_pTexTransform = nullptr;
	mFXShadow_pEyePosition = nullptr;

	//***********
	//Shadow Tessellation
	//***********
	mFXShadowTess = nullptr;
	mTechShadowTess = nullptr;
	mFXShadowTess_pWorldViewProjection = nullptr;
	mFXShadowTess_pWorld = nullptr;
	mFXShadowTess_pWorldInvTranspose = nullptr;
	mFXShadowTess_pViewProjection = nullptr;
	mFXShadowTess_pTexTransform = nullptr;
	mFXShadowTess_pEyePosition = nullptr;


	mFX_mSSAO_ND_NormalDepthTech = nullptr;
	mSSAO_ND_NormalDepthTech = nullptr;
	mSSAO_ND_NormalDepthAlphaClipTech = nullptr;
	mSSAO_ND_WorldView = nullptr;
	mSSAO_ND_WorldInvTransposeView = nullptr;
	mSSAO_ND_WorldViewProj = nullptr;
	mSSAO_ND_TexTransform = nullptr;
	mSSAO_ND_DiffuseMap = nullptr;

	//SSAO
	mFX_mSSAO = nullptr;
	SsaoTech = nullptr;
	mSSAO_ViewToTexSpace = nullptr;
	mSSAO_OffsetVectors = nullptr;
	mSSAO_FrustumCorners = nullptr;
	mSSAO_OcclusionRadius = nullptr;
	mSSAO_OcclusionFadeStart = nullptr;
	mSSAO_OcclusionFadeEnd = nullptr;
	mSSAO_SurfaceEpsilon = nullptr;
	mSSAO_NormalDepthMap = nullptr;
	mSSAO_RandomVecMap = nullptr;

	//SsaoBlur
	mFX_mSSAOBlur = nullptr;
	m_SSAO_BLUR_HorzBlurTech = nullptr;
	m_SSAO_BLUR_VertBlurTech = nullptr;
	m_SSAO_BLUR_TexelWidth = nullptr;
	m_SSAO_BLUR_TexelHeight = nullptr;
	m_SSAO_BLUR_NormalDepthMap = nullptr;
	m_SSAO_BLUR_InputImage = nullptr;

	//CubeMap
	mDynamicCubeMapDSV = nullptr;
	mDynamicCubeMapSRV = nullptr;
	BuildCubeFaceCamera(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 6; ++i)
	{
		mDynamicCubeMapRTV[i] = 0;
	}



	mWireframeRS = nullptr;
	mSolidframeRS = nullptr;
	mTwoSideTriangleFrameRS = nullptr;

	mInputLayout = nullptr;
	//mInputLayoutShadow = nullptr;

	g_pVertexBufferGrid = nullptr;
	mVerticesBufferLightSphere = nullptr;
	mIndicesBufferLightSphere = nullptr;
	m_pVerticesBufferQuad = nullptr;
	m_pIndicesBufferQuad = nullptr;
	m_pVerticesBufferBoundingBox = nullptr;
	m_pIndicesBufferBoundingBox = nullptr;

	pVSBlob = nullptr;
	pVSBlobError = nullptr;
	

	g_World = XMMatrixIdentity();
	g_View = XMMatrixIdentity();
	g_Projection = XMMatrixIdentity();


	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity()); // Macierz dla wsp. tekstur - jako jednostkowa


	//Shadow
	mShadowMapResolution = 2048;
	mShadow = nullptr;


	__show_type_wire = false;
	__show_type_wire_through = false;
	__show_type_wire_new = 2;
	__normal_view = false;
	__diffuseTexture = true;
	__normalTexture = true; 
	__specularTexture = true;
	__move_Unit = 1; //1 cm - default
	 
	__rotate_Degree = 45;
	__castShadow = false;
	__showShadowMap = false;

	mTmpLightPosition = XMFLOAT3(0.0f, 0.0f, -20.0f);

	//Zmienna do zatrzymywania czasu w renderze
	__stop_time = false;


	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 0.0f, 50.0f);
	XMVECTOR Eye = XMVectorSet(150.0f, 100.0f, 250.0f, 0.0f); //+50 
	XMVECTOR At = XMVectorSet(0.0f, 50.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mCam.LookAt(Eye, At, Up);

	


	Grid_Count = 0;
	Grid.Vertices.clear();
	mLightSphere.Indices.clear();
	mLightSphere.Vertices.clear();

	m_worldBaseTransform = XMMatrixIdentity();

	//!!!!!!!!!!!!!
	//LIGHT TEST
	//!!!!!!!!!!!!!

	mDirectionalLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirectionalLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectionalLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectionalLight.Direction = XMFLOAT3(-0.19245f, -0.96225f, -0.19245f);

	/*mPointLight[0].Range = 0.0f;
	mPointLight[1].Range = 0.0f;
	mPointLight[2].Range = 0.0f;
	mPointLight[3].Range = 0.0f;
	mPointLight[4].Range = 0.0f;
	mPointLight[5].Range = 0.0f;
	mPointLight[6].Range = 0.0f;
	mPointLight[7].Range = 0.0f;
	mPointLight[8].Range = 0.0f;
	mPointLight[9].Range = 0.0f;
	*/

	mPointLight[0].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight[0].Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	mPointLight[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 128.0f); //kolor malego specular'a
	mPointLight[0].Att = XMFLOAT3(0.0f, 0.000f, 0.001f); //by³o wszedzie 0.01f
	mPointLight[0].Range = 0.0f;

	mPointLight[1].Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mPointLight[1].Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight[1].Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 128.0f); //kolor malego specular'a
	mPointLight[1].Att = XMFLOAT3(0.01f, 0.01f, 0.01f);
	mPointLight[1].Range = 0.0f;

	mPointLight[1].Position = XMFLOAT3(100.0f, 0.0f, 0.0f);
	mPointLight[0].Position = XMFLOAT3(0.0f, 100.0f, 100.0f);

	mPointLight[0].Range = 0.0f;
	mPointLight[1].Range = 0.0f;
	mPointLight[2].Range = 0.0f;
	mPointLight[3].Range = 0.0f;
	mPointLight[4].Range = 0.0f;
	mPointLight[5].Range = 0.0f;
	mPointLight[6].Range = 0.0f;
	mPointLight[7].Range = 0.0f;
	mPointLight[8].Range = 0.0f;
	mPointLight[9].Range = 0.0f;
	//mPointLight[8].Range = 0.0f;
	//mPointLight[4].Range = 0.0f;

	
	//!!!!!!!!!!!!!
	//Material 
	//!!!!!!!!!!!!!


	mSelectedMaterial.ambientColour = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.1f);
	mSelectedMaterial.diffuseColour = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	mSelectedMaterial.specularColour = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f); //kolor odblasku
	


	mFX_pPointLight = nullptr;





}

//************************************************************************************************************************************************
//************************************************************************************************************************************************
//D-tor
KalenEngine::~KalenEngine()
{
	//this->mObjectsVector.clear();
	ReleaseCOM(mInputLayout);
	//ReleaseCOM(mInputLayoutShadow);
	ReleaseCOM(mWireframeRS);
	ReleaseCOM(mSolidframeRS);
	ReleaseCOM(mTwoSideTriangleFrameRS);
	ReleaseCOM(mFX);
	ReleaseCOM(mDynamicCubeMapDSV);
	ReleaseCOM(mDynamicCubeMapSRV);
	for (int i = 0; i < 6; ++i)
		ReleaseCOM(mDynamicCubeMapRTV[i]);

}
//************************************************************************************************************************************************
bool KalenEngine::CompileShader()
{
	//-------------------------------------
	//Tworzenie efektu mFX
	//-------------------------------------

	//C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngine - KLMESH Viewer/KalenEngine - KLMESH Viewer/fx
	//HRESULT hr = shaderStandard.CompileShaderFromFileNew(L"fx/FX_Shader.fx", NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	HRESULT hr = shaderStandard.CompileShaderFromFileNew(
		//L"C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngineDX11/KalenEngineDX11/fx/FX_Shader.fx"
		L"fx/FX_Shader.fx"
		, NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'FX_Shader.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFX);
	if (mFX == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}

	//-------------------------------------
	//Tworzenie efektu mFXTess - teselacji
	//-------------------------------------
	/*hr = shaderStandard.CompileShaderFromFileNew(
		//L"C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngineDX11/KalenEngineDX11/fx/FX_Shader_Tessellation.fx",
		L"fx/FX_Shader_Tessellation.fx",
		NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'FX_Shader_Tessellation.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFXTess);
	if (mFXTess == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader_Tessellation.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}*/

	//-------------------------------------
	//Tworzenie efektu mFXShadow - basic shadow
	//-------------------------------------
	hr = shaderStandard.CompileShaderFromFileNew(
		//L"C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngineDX11/KalenEngineDX11/fx/FX_Shader_Shadow.fx",
		L"fx/FX_Shader_Shadow.fx",
		NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'FX_Shader_Shadow.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFXShadow);
	if (mFXShadow == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader_Shadow.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}

	//-------------------------------------
	//Tworzenie efektu mFXShadowTess - basic shadow
	//-------------------------------------
	/*hr = shaderStandard.CompileShaderFromFileNew(
		//L"C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngineDX11/KalenEngineDX11/fx/FX_Shader_Shadow_Tessellation.fx",
		L"fx/FX_Shader_Shadow_Tessellation.fx",
		NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'FX_Shader_Shadow_Tessellation.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFXShadowTess);
	if (mFXShadow == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader_Shadow_Tessellation.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}

	//-------------------------------------
	//SsaoNormalDepth
	//-------------------------------------

	hr = shaderStandard.CompileShaderFromFileNew( L"fx/SsaoNormalDepth.fx", NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'SsaoNormalDepth.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFX_mSSAO_ND_NormalDepthTech);
	if (mFX == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}

	//-------------------------------------
	//Ssao
	//-------------------------------------

	hr = shaderStandard.CompileShaderFromFileNew(L"fx/Ssao.fx", NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'Ssao.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFX_mSSAO);
	if (mFX == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}

	//-------------------------------------
	//SsaoBlur
	//-------------------------------------

	hr = shaderStandard.CompileShaderFromFileNew(L"fx/SsaoBlur.fx", NULL, "fx_5_0", &pVSBlob, &pVSBlobError);
	if (FAILED(hr))
	{
		wxMessageBox("Cannot compile shader: 'SsaoBlur.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	D3DX11CreateEffectFromMemory(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), 0, m_pDevice, &mFX_mSSAOBlur);
	if (mFX == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader.fx'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}



	*/










	return true;
}
//************************************************************************************************************************************************
bool KalenEngine::ReadShaderFromBinaryFile()
{
	//-------------------------------------
	//Reading shader from binary file
	//-------------------------------------
	int size = 0;
	std::vector<char> compiledShaderVector = shaderStandard.ReadShaderFromBinaryFileFXO(
		L"fx/FX_Shader.fxo", size);

	D3DX11CreateEffectFromMemory(&compiledShaderVector[0], size, 0, m_pDevice, &mFX);
	if (mFX == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader.fxo'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	//-------------------------------------
	//Reading shader from binary file
	//-------------------------------------
	/*size = 0;
	compiledShaderVector = shaderStandard.ReadShaderFromBinaryFileFXO(
		L"C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngineDX11/KalenEngineDX11/fx/FX_Shader_Tessellation.fxo", size);

	D3DX11CreateEffectFromMemory(&compiledShaderVector[0], size, 0, m_pDevice, &mFXTess);
	if (mFXTess == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader_Tessellation.fxo'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}*/
	//-------------------------------------
	//Reading shader from binary file
	//-------------------------------------
	size = 0;
	compiledShaderVector = shaderStandard.ReadShaderFromBinaryFileFXO(
		L"fx/FX_Shader_Shadow.fxo", size);

	D3DX11CreateEffectFromMemory(&compiledShaderVector[0], size, 0, m_pDevice, &mFXShadow);
	if (mFXShadow == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader_Shadow.fxo'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	//-------------------------------------
	//Reading shader from binary file
	//-------------------------------------
	/*size = 0;
	compiledShaderVector = shaderStandard.ReadShaderFromBinaryFileFXO(
		L"C:/Users/Kamil/Documents/Visual Studio 2015/Projects/KalenEngineDX11/KalenEngineDX11/fx/FX_Shader_Shadow_Tessellation.fxo", size);

	D3DX11CreateEffectFromMemory(&compiledShaderVector[0], size, 0, m_pDevice, &mFXShadowTess);
	if (mFXShadowTess == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader_Shadow_Tessellation.fxo'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	*/
	return true;
}
//************************************************************************************************************************************************

bool KalenEngine::Init()
{
	if (!DXApp::Init())
	{
		wxMessageBox("Cannot initialize DirectX!", "Error", wxICON_ERROR | wxOK);
		return false;
	}



	/*if (CompileShader() == false) //Any changes in the shader code?
	{
		wxMessageBox("Cannot compile shader!", "Error", wxICON_ERROR | wxOK);
		return false;
	}

	ReadShaderFromBinaryFile
	*/

	if (ReadShaderFromBinaryFile() == false) 
	{
		wxMessageBox("Cannot laod shaders!", "Error", wxICON_ERROR | wxOK);
		return false;
	}

	//--------------------
	// Create buffers
	//--------------------
	mTexturesResource = new TexturesResource(m_pDevice);
	mRenderObjectsBuffer = new std::vector<Object*>;
	mRenderObjectsTransparencyBuffer = new std::vector<Object*>;
	mLayerBuffer = new std::vector<Layer*>;


	m_pDevice123 = m_pDevice;


	

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS);


	D3D11_RASTERIZER_DESC wireThroughframeDesc;
	ZeroMemory(&wireThroughframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireThroughframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireThroughframeDesc.CullMode = D3D11_CULL_NONE;
	wireThroughframeDesc.FrontCounterClockwise = false;
	wireThroughframeDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&wireThroughframeDesc, &mWireThroughFrameRS);


	D3D11_RASTERIZER_DESC solidframeDesc;
	ZeroMemory(&solidframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidframeDesc.FillMode = D3D11_FILL_SOLID;
	solidframeDesc.CullMode = D3D11_CULL_BACK;
	solidframeDesc.FrontCounterClockwise = false;
	solidframeDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&solidframeDesc, &mSolidframeRS);

	D3D11_RASTERIZER_DESC twoSideTriangleFrameDesc;
	ZeroMemory(&twoSideTriangleFrameDesc, sizeof(D3D11_RASTERIZER_DESC));
	twoSideTriangleFrameDesc.FillMode = D3D11_FILL_SOLID;
	twoSideTriangleFrameDesc.CullMode = D3D11_CULL_NONE;
	twoSideTriangleFrameDesc.FrontCounterClockwise = false;
	twoSideTriangleFrameDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&twoSideTriangleFrameDesc, &mTwoSideTriangleFrameRS);


	mFX_pWorldViewProjection = mFX->GetVariableByName("WorldViewProjection")->AsMatrix();
	mFX_pWorldViewProjectionTex = mFX->GetVariableByName("WorldViewProjectionTex")->AsMatrix();
	mFX_pWorld = mFX->GetVariableByName("World")->AsMatrix();
	mFX_pWorldInvTranspose = mFX->GetVariableByName("WorldInvTranspose")->AsMatrix();
	mFX_pTexTransform = mFX->GetVariableByName("TexTransform")->AsMatrix();
	mFX_pShadowTransform = mFX->GetVariableByName("gShadowTransform")->AsMatrix();

	mFX_pEyePosition = mFX->GetVariableByName("gEyePosW")->AsVector();
	mFX_pPointLight = mFX->GetVariableByName("gPointLight");
	mFX_pDirectionalLight = mFX->GetVariableByName("gDirectionalLight");
	mFX_pMaterial = mFX->GetVariableByName("gMaterial");
	mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");

	mFX_pDiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mFX_pNormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	mFX_pASHMap = mFX->GetVariableByName("gASHMap")->AsShaderResource();
	mFX_pShadowMap = mFX->GetVariableByName("gShadowMap")->AsShaderResource(); //set mShadow->DepthMapSRV()
	mFX_pCubeMap = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	mFX_pSSAOMap = mFX->GetVariableByName("gSSAOMap")->AsShaderResource();
	
	mFX_AllowDiffuse = mFX->GetVariableByName("__allowDiffuse")->AsScalar();
	mFX_AllowNormal = mFX->GetVariableByName("__allowNormal")->AsScalar();
	mFX_AllowSpecular = mFX->GetVariableByName("__allowSpecular")->AsScalar();
	mFX_AllowAlpha = mFX->GetVariableByName("__allowAlpha")->AsScalar();
	mFX_SetLineColor = mFX->GetVariableByName("__color")->AsScalar();
	mFX_AllowCubeMap = mFX->GetVariableByName("__allowCubeMap")->AsScalar();
	mFX_AllowShadowMap = mFX->GetVariableByName("__allowShadowMap")->AsScalar();
	mFX_AllowSSAOMap = mFX->GetVariableByName("__allowSSAOMap")->AsScalar();
	mFX_SetLineColor->SetInt(0);

	mFX_Transparency = mFX->GetVariableByName("__transparency")->AsScalar();
	mFX_Transparency->SetFloat(1.0f);


	mFX_AllowDiffuse->SetBool(true);
	mFX_AllowNormal->SetBool(true);
	mFX_AllowSpecular->SetBool(true);
	mFX_AllowCubeMap->SetBool(true);
	mFX_AllowShadowMap->SetBool(true);
	mFX_AllowSSAOMap->SetBool(true);


	/*mFXTess_pWorldViewProjection = mFXTess->GetVariableByName("WorldViewProjection")->AsMatrix();
	mFXTess_pWorld = mFXTess->GetVariableByName("World")->AsMatrix();
	mFXTess_pWorldInvTranspose = mFXTess->GetVariableByName("WorldInvTranspose")->AsMatrix();
	mFXTess_pTexTransform = mFXTess->GetVariableByName("TexTransform")->AsMatrix();
	mFXTess_pShadowTransform = mFXTess->GetVariableByName("gShadowTransform")->AsMatrix();

	mFXTess_pEyePosition = mFXTess->GetVariableByName("gEyePosW")->AsVector();
	mFXTess_pPointLight = mFXTess->GetVariableByName("gPointLight");
	mFXTess_pMaterial = mFXTess->GetVariableByName("gMaterial");
	mTechTess = mFXTess->GetTechniqueByName("FeatureLevel11Tessellation");

	mFXTess_pDiffuseMap = mFXTess->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mFXTess_pNormalMap = mFXTess->GetVariableByName("gNormalMap")->AsShaderResource();
	mFXTess_pSpecularMap = mFXTess->GetVariableByName("gSpecularMap")->AsShaderResource();
	mFXTess_pHeightMap = mFXTess->GetVariableByName("gHeightMap")->AsShaderResource();
	mFXTess_pShadowMap = mFXTess->GetVariableByName("gShadowMap")->AsShaderResource();

	mFXTess_AllowDiffuse = mFXTess->GetVariableByName("__allowDiffuse")->AsScalar();
	mFXTess_AllowNormal = mFXTess->GetVariableByName("__allowNormal")->AsScalar();
	mFXTess_AllowSpecular = mFXTess->GetVariableByName("__allowSpecular")->AsScalar();

	//------------------------------------------
	//Teselacja - nadanie zmiennych:
	mFXTess_HeightScale = mFXTess->GetVariableByName("gHeightScale")->AsScalar();
	mFXTess_MaxTessDistance = mFXTess->GetVariableByName("gMaxTessDistance")->AsScalar();
	mFXTess_MinTessDistance = mFXTess->GetVariableByName("gMinTessDistance")->AsScalar();
	mFXTess_MinTessFactor = mFXTess->GetVariableByName("gMinTessFactor")->AsScalar();
	mFXTess_MaxTessFactor = mFXTess->GetVariableByName("gMaxTessFactor")->AsScalar();

	//Teselacja - ustawianie wartosci:
	//mFX_HeightScale->SetFloat(0.07f);
	mFXTess_HeightScale->SetFloat(0.07f);
	mFXTess_MaxTessDistance->SetFloat(1.0f);
	mFXTess_MinTessDistance->SetFloat(25.0f);
	mFXTess_MinTessFactor->SetFloat(1.0f);
	mFXTess_MaxTessFactor->SetFloat(5.0f);*/
	//------------------------------------------
	
	//------------------------
	//Shadow
	//------------------------
	mFXShadow_pWorldViewProjection = mFXShadow->GetVariableByName("WorldViewProjection")->AsMatrix();
	mFXShadow_pWorld = mFXShadow->GetVariableByName("World")->AsMatrix();
	mFXShadow_pWorldInvTranspose = mFXShadow->GetVariableByName("WorldInvTranspose")->AsMatrix();
	mFXShadow_pTexTransform = mFXShadow->GetVariableByName("TexTransform")->AsMatrix();
	mFXShadow_pViewProjection = mFXShadow->GetVariableByName("ViewProj")->AsMatrix();
	mFXShadow_pEyePosition = mFXShadow->GetVariableByName("gEyePosW")->AsVector();
	mTechShadow = mFXShadow->GetTechniqueByName("BuildShadowMapTechNoAlpha");
	mFXShadow_pAlphaMap = mFXShadow->GetVariableByName("gAlphaMap")->AsShaderResource();

	//------------------------
	//Shadow Tessellation
	//------------------------
	/*mFXShadowTess_pWorldViewProjection = mFXShadowTess->GetVariableByName("WorldViewProjection")->AsMatrix();
	mFXShadowTess_pWorld = mFXShadowTess->GetVariableByName("World")->AsMatrix();
	mFXShadowTess_pWorldInvTranspose = mFXShadowTess->GetVariableByName("WorldInvTranspose")->AsMatrix();
	mFXShadowTess_pTexTransform = mFXShadowTess->GetVariableByName("TexTransform")->AsMatrix();
	mFXShadowTess_pViewProjection = mFXShadowTess->GetVariableByName("ViewProj")->AsMatrix();
	mFXShadowTess_pEyePosition = mFXShadowTess->GetVariableByName("gEyePosW")->AsVector();
	mTechShadowTess = mFXShadowTess->GetTechniqueByName("BuildShadowMapTechNoAlphaTess");
	mFXShadowTess_pAlphaMap = mFXShadowTess->GetVariableByName("gAlphaMap")->AsShaderResource();
	mFXShadowTess_pNormalMap = mFXShadowTess->GetVariableByName("gNormalMap")->AsShaderResource();
	mFXShadowTess_pHeightMap = mFXShadowTess->GetVariableByName("gHeightMap")->AsShaderResource();
	mFXShadowTess_HeightScale = mFXShadowTess->GetVariableByName("gHeightScale")->AsScalar();
	mFXShadowTess_MaxTessDistance = mFXShadowTess->GetVariableByName("gMaxTessDistance")->AsScalar();
	mFXShadowTess_MinTessDistance = mFXShadowTess->GetVariableByName("gMinTessDistance")->AsScalar();
	mFXShadowTess_MinTessFactor = mFXShadowTess->GetVariableByName("gMinTessFactor")->AsScalar();
	mFXShadowTess_MaxTessFactor = mFXShadowTess->GetVariableByName("gMaxTessFactor")->AsScalar();

	*/

	//----------------------------
	//SSAO NormalDepth
	//----------------------------
	/*mSSAO_ND_NormalDepthTech = mFX_mSSAO_ND_NormalDepthTech->GetTechniqueByName("NormalDepth");
	mSSAO_ND_NormalDepthAlphaClipTech = mFX_mSSAO_ND_NormalDepthTech->GetTechniqueByName("NormalDepthAlphaClip");
	mSSAO_ND_WorldView = mFX_mSSAO_ND_NormalDepthTech->GetVariableByName("gWorldView")->AsMatrix();
	mSSAO_ND_WorldInvTransposeView = mFX_mSSAO_ND_NormalDepthTech->GetVariableByName("gWorldInvTransposeView")->AsMatrix();
	mSSAO_ND_WorldViewProj = mFX_mSSAO_ND_NormalDepthTech->GetVariableByName("gWorldViewProj")->AsMatrix();
	mSSAO_ND_TexTransform = mFX_mSSAO_ND_NormalDepthTech->GetVariableByName("gTexTransform")->AsMatrix();
	mSSAO_ND_DiffuseMap = mFX_mSSAO_ND_NormalDepthTech->GetVariableByName("gDiffuseMap")->AsShaderResource();

	//----------------------------
	//SSAO
	//----------------------------
	SsaoTech = mFX_mSSAO->GetTechniqueByName("Ssao");
	mSSAO_ViewToTexSpace = mFX_mSSAO->GetVariableByName("gViewToTexSpace")->AsMatrix();
	mSSAO_OffsetVectors = mFX_mSSAO->GetVariableByName("gOffsetVectors")->AsVector();
	mSSAO_FrustumCorners = mFX_mSSAO->GetVariableByName("gFrustumCorners")->AsVector();
	mSSAO_OcclusionRadius = mFX_mSSAO->GetVariableByName("gOcclusionRadius")->AsScalar();
	mSSAO_OcclusionFadeStart = mFX_mSSAO->GetVariableByName("gOcclusionFadeStart")->AsScalar();
	mSSAO_OcclusionFadeEnd = mFX_mSSAO->GetVariableByName("gOcclusionFadeEnd")->AsScalar();
	mSSAO_SurfaceEpsilon = mFX_mSSAO->GetVariableByName("gSurfaceEpsilon")->AsScalar();
	mSSAO_NormalDepthMap = mFX_mSSAO->GetVariableByName("gNormalDepthMap")->AsShaderResource();
	mSSAO_RandomVecMap = mFX_mSSAO->GetVariableByName("gRandomVecMap")->AsShaderResource();

	//----------------------------
	//SSAO Blur
	//----------------------------
	m_SSAO_BLUR_HorzBlurTech = mFX_mSSAOBlur->GetTechniqueByName("HorzBlur");
	m_SSAO_BLUR_VertBlurTech = mFX_mSSAOBlur->GetTechniqueByName("VertBlur");
	m_SSAO_BLUR_TexelWidth = mFX_mSSAOBlur->GetVariableByName("gTexelWidth")->AsScalar();
	m_SSAO_BLUR_TexelHeight = mFX_mSSAOBlur->GetVariableByName("gTexelHeight")->AsScalar();
	m_SSAO_BLUR_NormalDepthMap = mFX_mSSAOBlur->GetVariableByName("gNormalDepthMap")->AsShaderResource();
	m_SSAO_BLUR_InputImage = mFX_mSSAOBlur->GetVariableByName("gInputImage")->AsShaderResource();



	mSsaoEffects.SsaoTech = SsaoTech;
	mSsaoEffects.mSSAO_ViewToTexSpace = mSSAO_ViewToTexSpace;
	mSsaoEffects.mSSAO_OffsetVectors = mSSAO_OffsetVectors;
	mSsaoEffects.mSSAO_FrustumCorners = mSSAO_FrustumCorners;
	mSsaoEffects.mSSAO_NormalDepthMap = mSSAO_NormalDepthMap;
	mSsaoEffects.mSSAO_RandomVecMap = mSSAO_RandomVecMap;
	mSsaoEffects.m_SSAO_BLUR_TexelWidth = m_SSAO_BLUR_TexelWidth;
	mSsaoEffects.m_SSAO_BLUR_TexelHeight = m_SSAO_BLUR_TexelHeight;
	mSsaoEffects.m_SSAO_BLUR_NormalDepthMap = m_SSAO_BLUR_NormalDepthMap;
	mSsaoEffects.m_SSAO_BLUR_InputImage = m_SSAO_BLUR_InputImage;
	mSsaoEffects.m_SSAO_BLUR_HorzBlurTech = m_SSAO_BLUR_HorzBlurTech;
	mSsaoEffects.m_SSAO_BLUR_VertBlurTech = m_SSAO_BLUR_VertBlurTech;

	*/
	BuildVertexLayout();
	BuildDynamicCubeMapViews();

	mShadow = new Shadow(m_pDevice, mShadowMapResolution, mShadowMapResolution);
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 0.1f, 10000.0f);
	//mSSAO = new Ssao(m_pDevice, m_pImmediateContext, m_ClientWidth, m_ClientHeight, mCam.GetFovY(), mCam.GetFarZ(), mSsaoEffects, mInputLayout);
	
	mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSceneBounds.Radius = sqrtf(1500.0f*1500.0f + 500.0f*500.0f);



	//****************
	//Octree
	//****************
	//mOctree = new Octree(3, XMFLOAT3(5000.0f, 5000.0f, 5000.0f), XMFLOAT3(-5000.0f, -5000.0f, -5000.0f));
	//mOctree = new Octree(4, XMFLOAT3(8192.0f, 8192.0f, 8192.0f), XMFLOAT3(-8192.0f, -8192.0f, -8192.0f));
	//mOctree = new Octree(1, XMFLOAT3(5000.0f, 5000.0f, 5000.0f), XMFLOAT3(-5000.0f, -5000.0f, -5000.0f));
	mOctree = new Octree(2, XMFLOAT3(5000.0f, 5000.0f, 5000.0f), XMFLOAT3(-5000.0f, -5000.0f, -5000.0f));


	//--------------------
	// Mesh
	//--------------------
	mesh.CreateFullscreenQuad(mQuadScreen); //Create QuadScreen
	mesh.CreateBoundingBoxView(mKLMeshGLOBAL.boundingBoxMax, mKLMeshGLOBAL.boundingBoxMin, mBoundingBoxMesh);
	mesh.CreateGridWorld(20, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), Grid);



	//--------------------
	// GIZMO
	//--------------------
	mGizmo = new Gizmo(m_pImmediateContext, &mesh, m_pDevice);

	//--------------------
	// LightSphere
	//--------------------
	mesh.CreateSphere(1.0f, 12, 12, mLightSphere);

	//--------------------
	// Build scene
	//--------------------
	BuildScene();

	return true;
}

//************************************************************************************************************************************************
//************************************************************************************************************************************************
void KalenEngine::BuildScene()
{

	//Count Vertex
	Grid_Count = Grid.Vertices.size();
	Grid_Offset = 0;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* Grid_Count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = vertices;
	InitData.pSysMem = &Grid.Vertices[0];
	m_pDevice->CreateBuffer(&bd, &InitData, &g_pVertexBufferGrid);

	// Setup constant buffers
	D3D11_BUFFER_DESC Desc;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;


	//------------------
	//Create quads buffer
	//------------------
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* mQuadScreen.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &mQuadScreen.Vertices[0];
	m_pDevice->CreateBuffer(&bd, &InitData, &m_pVerticesBufferQuad);


	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD)* mQuadScreen.Indices.size();
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	InitData.pSysMem = &mQuadScreen.Indices[0];
	m_pDevice->CreateBuffer(&bd2, &InitData, &m_pIndicesBufferQuad);






	//------------------
	//Create boundingBox buffer
	//------------------
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* mBoundingBoxMesh.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &mBoundingBoxMesh.Vertices[0];
	m_pDevice->CreateBuffer(&bd, &InitData, &m_pVerticesBufferBoundingBox);


	//D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD)* mBoundingBoxMesh.Indices.size();
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	InitData.pSysMem = &mBoundingBoxMesh.Indices[0];
	m_pDevice->CreateBuffer(&bd2, &InitData, &m_pIndicesBufferBoundingBox);




	//------------------
	//Create LightSphere buffer - DYNAMIC BUFFER
	//------------------

	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DYNAMIC;
	bd2.ByteWidth = sizeof(Vertex)* mLightSphere.Vertices.size();
	bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd2.MiscFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &mLightSphere.Vertices[0];
	m_pDevice->CreateBuffer(&bd2, &InitData, &mVerticesBufferLightSphere);

	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD)* mLightSphere.Indices.size();
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	InitData.pSysMem = &mLightSphere.Indices[0];
	m_pDevice->CreateBuffer(&bd2, &InitData, &mIndicesBufferLightSphere);


}
//*******************************************************************************************************************************************
//*******************************************************************************************************************************************


void KalenEngine::BuildVertexLayout()
{

	HRESULT hr = S_OK;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};
	UINT numElements = ARRAYSIZE(layout);

	D3DX11_PASS_SHADER_DESC VsPassDesc123;

	mTech->GetPassByIndex(0)->GetVertexShaderDesc(&VsPassDesc123);

	D3DX11_PASS_SHADER_DESC VsPassDesc;
	D3DX11_EFFECT_SHADER_DESC VsDesc;
	mTech->GetPassByIndex(0)->GetVertexShaderDesc(&VsPassDesc);
	//mTechTess->GetPassByIndex(0)->GetVertexShaderDesc(&VsPassDesc); //_______________________________________________________________
	VsPassDesc.pShaderVariable->GetShaderDesc(VsPassDesc.ShaderIndex, &VsDesc);
	hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), VsDesc.pBytecode, VsDesc.BytecodeLength, &mInputLayout);



	if (FAILED(hr))
	{
		MessageBox(NULL, L"B³¹d przy tworzeniu warstwy wejœcia", L"B³¹d!", MB_OK);
		PostQuitMessage(0);
	}


}
//*******************************************************************************************************************************************
//*******************************************************************************************************************************************




//************************************************************************************************************************************************
//************************************************************************************************************************************************

void KalenEngine::Update(float dt)
{

	bool rotate = false;
	float normalSpeed = 100.0f;
	float shiftPower = 4.0f;
	XMVECTOR length;

	int gizmoTransformType = -1;

	if(mKeyPressW)
		if(mKeyPressShift)
			mCam.Walk(normalSpeed * shiftPower * dt);
		else 
			mCam.Walk(normalSpeed * dt);

	if (mKeyPressS)
		if (mKeyPressShift)
			mCam.Walk(-normalSpeed * shiftPower * dt);
		else
			mCam.Walk(-normalSpeed * dt);

	if (mKeyPressA)
		if (mKeyPressShift)
			mCam.Strafe(-normalSpeed * shiftPower * dt);
		else
			mCam.Strafe(-normalSpeed * dt);

	if (mKeyPressD)
		if (mKeyPressShift)
			mCam.Strafe(normalSpeed * shiftPower * dt);
		else
			mCam.Strafe(normalSpeed * dt);

	if (mKeyPressQ)
		if (mKeyPressShift)
			mCam.WalkUP(normalSpeed * shiftPower * dt);
		else
			mCam.WalkUP(normalSpeed * dt);

	if (mKeyPressE)
		if (mKeyPressShift)
			mCam.WalkUP(-normalSpeed * shiftPower * dt);
		else
			mCam.WalkUP(-normalSpeed * dt);


	if (mLockLeftClickMove == true) //Transform mode in ON
	{
		gizmoTransformType = mGizmo->GetGizmoSelected();

		XMVECTOR startVector;
		XMVECTOR currentVector;
		startVector = XMVectorSet(mStartTransformPoint.x, mStartTransformPoint.y, 0, 0);
		currentVector = XMVectorSet(mCurrentTransformPoint.x, mCurrentTransformPoint.y, 0, 0);

		length = XMVectorSubtract(currentVector, startVector);
		length = XMVector2Length(length);
			//(XMLoadFloat2(&XMFLOAT2(mStartTransformPoint.x, mStartTransformPoint.y)) - XMLoadFloat2(&XMFLOAT2(mCurrentTransformPoint.x, mCurrentTransformPoint.y)));
		length = XMVectorAbs(length);


	}

	mFX_pDirectionalLight->SetRawValue(&mDirectionalLight, 0, sizeof(mDirectionalLight));
	mFX_pPointLight->SetRawValue(&mPointLight, 0, sizeof(mPointLight));
	mFX_pEyePosition->SetRawValue(&mEyePosW, 0, sizeof(mEyePosW));
	mFXShadow_pEyePosition->SetRawValue(&mEyePosW, 0, sizeof(mEyePosW));

	BuildShadowTransform();
}
//*******************************************************************************************************************************************
void KalenEngine::BuildShadowTransform()
{
	//mSceneBounds.Radius = sqrtf(500.0f*500.0f + 400.0f*400.0f);
	//mSceneBounds.Radius = sqrtf(1000.0f*1000.0f + 300.0f*300.0f);


	mSceneBounds.Radius = sqrtf(3000.0f*3000.0f + 400.0f*400.0f);

	mSceneBounds.Center = XMFLOAT3(mCam.GetPosition().x, mCam.GetPosition().y, mCam.GetPosition().z);

	//mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMVECTOR lightDir = XMLoadFloat3(&mDirectionalLight.Direction);
	XMVECTOR lightPos = -2.0f * mSceneBounds.Radius * lightDir;
	XMVECTOR targetPos = XMLoadFloat3(&mSceneBounds.Center);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);

	// Przekszta³æ sferê otaczaj¹c¹ do przestrzeni œwiat³a.
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));

	// Prostopad³oœcian widzenia w przestrzeni œwiat³a obejmuje ca³¹ scenê.
	float l = sphereCenterLS.x - mSceneBounds.Radius;
	float b = sphereCenterLS.y - mSceneBounds.Radius;
	float n = sphereCenterLS.z - mSceneBounds.Radius;
	float r = sphereCenterLS.x + mSceneBounds.Radius;
	float t = sphereCenterLS.y + mSceneBounds.Radius;
	float f = sphereCenterLS.z + mSceneBounds.Radius;
	XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Przekszta³æ przestrzeñ NDC [-1,+1]^2 do przestrzeni tekstury [0,1]^2
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX S = V*P*T;

	XMStoreFloat4x4(&mLightView, V);
	XMStoreFloat4x4(&mLightProj, P);
	XMStoreFloat4x4(&mShadowTransform, S);
}
//*******************************************************************************************************************************************
void KalenEngine::CountRay(XMVECTOR *rayDirOut, XMVECTOR *RayOriginOut)
{
	XMMATRIX P = mCam.Proj();
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f * mMousePosition.x / m_ClientWidth - 1.0f) / p(0, 0);
	float vy = (-2.0f * mMousePosition.y / m_ClientHeight + 1.0f) / p(1, 1);


	// Definicja promienia w przestrzeni widoku.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Przekszta³æ promieñ do lokalnej przestrzeni siatki.
	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = m_worldBaseTransform;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Nadaj kierunkowi promienia d³ugoœæ 1 przed testami przeciêcia.
	rayDir = XMVector3Normalize(rayDir);

	rayDirOut = &rayDir;
	RayOriginOut = &rayOrigin;
}
//*******************************************************************************************************************************************
void KalenEngine::CountRay(XMFLOAT3 *pos, int MouseX, int MouseY)
{

	XMMATRIX P = mCam.Proj();
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f * MouseX / m_ClientWidth - 1.0f) / p(0, 0);
	float vy = (-2.0f * MouseY / m_ClientHeight + 1.0f) / p(1, 1);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);
	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
	XMMATRIX W = m_worldBaseTransform;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);
	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);
	rayDir = XMVector3Normalize(rayDir);

	//Plane
	XMVECTOR normalvec = XMVector3Normalize(XMVectorNegate(mCam.GetLookXM()));
	XMVECTOR CurrentPoint;
	XMVECTOR tempVec;

	CurrentPoint = XMVector3Dot(normalvec, rayOrigin); // (N dot S)
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	position = mCam.GetPosition();

	position.x += mCam.GetLook().x * 200;
	position.y += mCam.GetLook().y * 200;
	position.z += mCam.GetLook().z * 200;

	tempVec = XMVector3Dot(XMVectorNegate(normalvec), XMLoadFloat3(&position)); // a = (-N dot K)
	CurrentPoint = XMVectorAdd(CurrentPoint, tempVec); // (N dot S) + a = (N dot S) + (-N dot K)
	tempVec = XMVector3Dot(normalvec, rayDir); // (N dot V)
	CurrentPoint = XMVectorDivide(CurrentPoint, tempVec); // ((N dot S) + (-N dot K)) / (N dot V)
	CurrentPoint = XMVectorMultiply(CurrentPoint, rayDir); // (((N dot S) + (-N dot K)) / (N dot V)) * V
	CurrentPoint = XMVectorSubtract(rayOrigin, CurrentPoint); // S - ((((N dot S) + (-N dot K)) / (N dot V)) * V)

	pos->x = XMVectorGetX(CurrentPoint);
	pos->y = XMVectorGetY(CurrentPoint);
	pos->z = XMVectorGetZ(CurrentPoint);
}
//*******************************************************************************************************************************************
void KalenEngine::Pick(UINT sx, UINT sy)
{
	XMMATRIX P = mCam.Proj();
	XMFLOAT4X4 p; 
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f * sx / m_ClientWidth - 1.0f) / p(0, 0);
	float vy = (-2.0f * sy / m_ClientHeight + 1.0f) / p(1, 1);


	// Definicja promienia w przestrzeni widoku.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Przekszta³æ promieñ do lokalnej przestrzeni siatki.
	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = m_worldBaseTransform;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Nadaj kierunkowi promienia d³ugoœæ 1 przed testami przeciêcia.
	rayDir = XMVector3Normalize(rayDir);
	float tmin = 0.0f;
	

	std::vector<Object*>* selectedObjects = new std::vector<Object*>;

	if (gMultiLayers == true)
	{
		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)//layers
			if (mLayerBuffer->at(i)->GetIsShow() == true)
				for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)//objects
				{
					if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetIsVisible() == true)
					{
						if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetType() == 0) //KLMESH
						{
							RebuildBoundingBox(mLayerBuffer->at(i)->GetAllObjects()->at(j));
							if (IntersectRayAxisAlignedBox(rayOrigin, rayDir, &mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetAxisAlignedBoundingBox(), &tmin))
							{
								mLayerBuffer->at(i)->GetAllObjects()->at(j)->SetTempDistance(tmin);
								selectedObjects->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));
							}
						}
					}
				}
	}
	else
	{
		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)//layers
		{
			if(mLayerBuffer->at(i)->GetIsActive() == true)
				if (mLayerBuffer->at(i)->GetIsShow() == true)
					for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)//objects
					{
						if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetIsVisible() == true)
						{
							if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetType() == 0) //KLMESH
							{
								RebuildBoundingBox(mLayerBuffer->at(i)->GetAllObjects()->at(j));
								if (IntersectRayAxisAlignedBox(rayOrigin, rayDir, &mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetAxisAlignedBoundingBox(), &tmin))
								{
									mLayerBuffer->at(i)->GetAllObjects()->at(j)->SetTempDistance(tmin);
									selectedObjects->push_back(mLayerBuffer->at(i)->GetAllObjects()->at(j));
								}
							}
						}
					}
		}
	}
	if (selectedObjects->size() > 0)
	{
		QuicksortObjects(selectedObjects, 0, (int)selectedObjects->size() - 1);
		Object* nearObject = new Object();

		tmin = MathHelper::Infinity;

		for (int j = 0; j < (int)selectedObjects->size(); ++j)
		{
			//Chunks of the object
			for (int k = 0; k < (int)selectedObjects->at(j)->GetKLMESH()->GetMeshToDraw(0.0f)->size(); ++k)
			{
				int startIndex = selectedObjects->at(j)->GetKLMESH()->GetMeshToDraw(0.0f)->at(k)->startIndex;
				int maxIndex = selectedObjects->at(j)->GetKLMESH()->GetMeshToDraw(0.0f)->at(k)->numberOfIndicesToDraw;
				for (int i = startIndex; i < maxIndex - 1; i += 3) //step = 3
				{
					//Get three indices of the triangle
					UINT i0 = selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Indices[i + 0];
					UINT i1 = selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Indices[i + 1];
					UINT i2 = selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Indices[i + 2];

					//Get three vertices of the triangle
					XMVECTOR v0 = XMLoadFloat3(&selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Vertices[i0].Pos);
					XMVECTOR v1 = XMLoadFloat3(&selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Vertices[i1].Pos);
					XMVECTOR v2 = XMLoadFloat3(&selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Vertices[i2].Pos);

					//Count normal to triangle
					XMVECTOR normalToTriangle = XMLoadFloat3(&selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Vertices[i0].Normal);
					normalToTriangle = XMVectorAdd(normalToTriangle, XMLoadFloat3(&selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Vertices[i1].Normal));
					normalToTriangle = XMVectorAdd(normalToTriangle, XMLoadFloat3(&selectedObjects->at(j)->GetKLMESH()->GetAllMeshData()->Vertices[i2].Normal));
					normalToTriangle = XMVector3Normalize(normalToTriangle);

					normalToTriangle = XMVector3Dot(normalToTriangle, mCam.GetLookXM());

					if (XMVectorGetX(normalToTriangle) >= 0.0f)
					{
						//Get matrix
						XMMATRIX matrix = XMMatrixInverse(nullptr, selectedObjects->at(j)->GetCoordinate()->GetMatrix());

						//Transform three vertices 
						v0 = XMVector3Transform(v0, matrix);
						v1 = XMVector3Transform(v1, matrix);
						v2 = XMVector3Transform(v2, matrix);

						float t = 0.0f;
						if (IntersectRayTriangle(rayOrigin, rayDir, v0, v1, v2, &t))
						{
							if (t < tmin)
							{
								tmin = t;
								nearObject = selectedObjects->at(j);
							}
						}
					}
				}
			}
		}
		if (nearObject->GetKLMESH() != nullptr)
			mSelectedObject = nearObject;
		else//null
			mSelectedObject = nullptr;
	}
	else if (selectedObjects->size() == 0)
	{
		mSelectedObject = nullptr;
	}

}

//************************************************************************************************************************************************
//************************************************************************************************************************************************

void KalenEngine::OnResize()
{
	DXApp::OnResize();
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 0.1f, 10000.0f);

	/*if (mSSAO)
	{
		mSSAO->OnSize(m_ClientWidth, m_ClientHeight, mCam.GetFovY(), mCam.GetFarZ());
	}*/
	ComputeFrustumFromProjection(&mCamFrustum, &mCam.Proj());
}

//************************************************************************************************************************************************
void KalenEngine::HomeView()
{ 
	XMVECTOR Eye = XMVectorSet(11.0f, 5.0f, 12.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCam.LookAt(Eye, At, Up);
}
//************************************************************************************************************************************************
void KalenEngine::TopView()
{
	XMVECTOR Eye = XMVectorSet(0.0f, 15.0f, 0.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	mCam.LookAt(Eye, At, Up);
}
//************************************************************************************************************************************************
void KalenEngine::FrontView()
{
	//m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -15.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCam.LookAt(Eye, At, Up);
}
//************************************************************************************************************************************************


//************************************************************************************************************************************************
void KalenEngine::WireView()
{
		//__show_type_wire = true;
		__show_type_wire_new = 0;
}

//************************************************************************************************************************************************
void KalenEngine::WireThroughView()
{

	__show_type_wire_new = 1;
}

//************************************************************************************************************************************************
void KalenEngine::SolidView()
{
	//__show_type_wire = false;
	__show_type_wire_new = 2;

}

//************************************************************************************************************************************************
void KalenEngine::WireFrameSolidView()
{
	__show_type_wire_new = 3;
}


//************************************************************************************************************************************************

void KalenEngine::NormalView()
{
	if (__normal_view)
		__normal_view = false;
	else
		__normal_view = true;
}


//************************************************************************************************************************************************
void KalenEngine::SetMoveUnit(float unit)
{
	__move_Unit = unit;
}
//************************************************************************************************************************************************
void KalenEngine::SetRotateDegree(float degree)
{
	__rotate_Degree = degree;
}

//************************************************************************************************************************************************
void KalenEngine::SetTextureType(bool diffuse, bool normal, bool specular)
{
	if (diffuse && normal && specular)
	{
		if (__diffuseTexture || __normalTexture || __specularTexture)
		{
			__diffuseTexture = false;
			__normalTexture = false;
			__specularTexture = false;
		}
		else
		{
			__diffuseTexture = true;
			__normalTexture = true;
			__specularTexture = true;
		}
	}
	else
	{
		if (diffuse && !normal && !specular)
			if (__diffuseTexture)
				__diffuseTexture = false;
			else
				__diffuseTexture = true;

		if (!diffuse && normal && !specular)
			if (__normalTexture)
				__normalTexture = false;
			else
				__normalTexture = true;

		if (!diffuse && !normal && specular)
			if (__specularTexture)
				__specularTexture = false;
			else
				__specularTexture = true;
	}


}

void KalenEngine::SetShowDiffuseMap(bool signal)
{
	this->__diffuseTexture = signal;
}
void KalenEngine::SetShowNormalMap(bool signal)
{
	this->__normalTexture = signal;
}
void KalenEngine::SetShowSpecularMap(bool signal)
{
	this->__specularTexture= signal;
}
void KalenEngine::SetShowAllTexturesMap(bool signal)
{
	this->__diffuseTexture = signal;
	this->__normalTexture = signal;
	this->__specularTexture = signal;
}



void KalenEngine::SetShowShadowMap(bool signal)
{
	this->__showShadowMap = signal;
}




void KalenEngine::OnIdle(wxIdleEvent &evt)
{
	evt.RequestMore(true);

	//if (HasFocus() == true) //if this window is active!!!
	//{
		mTimer.Tick();
		if (!mAppPaused)
		{
			CalculateFrameStats();
			Update(mTimer.DeltaTime());
			Render(0.0f);
		}
		else
		{
			Sleep(100);
		}
	/*}
	else if (mAllowRender == true)
	{
		mTimer.Tick();
		if (!mAppPaused)
		{
			CalculateFrameStats();
			Update(mTimer.DeltaTime());
			Render(0.0f);
		}
		else
		{
			Sleep(100);
		}
		mAllowRender = false;
	}
	*/
}

void KalenEngine::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC dc(this);
	Render(0.0f);
}

void KalenEngine::OnSize(wxSizeEvent& WXUNUSED(evt))
{
	DXApp::OnResize();
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 0.1f, 10000.0f);
	ComputeFrustumFromProjection(&mCamFrustum, &mCam.Proj());
}

void KalenEngine::OnEraseBackground(wxEraseEvent& evt)
{
	Render(0.0f);
	evt.Skip();
}


float KalenEngine::GetFPS()
{
	return this->mFPS;
}
wxPoint KalenEngine::GetMousePosition()
{
	return this->mMousePosition;
}


//***********************************************************************************************************************
void KalenEngine::OnMouseLeftDown(wxMouseEvent& evt)
{
	mLeftClickTimer = wxGetLocalTimeMillis();
	if (mGizmo->GetGizmoSelected() != 0 && mGizmo->GetGizmoSelected() != 10 && mGizmo->GetGizmoSelected() != 30)
	{
		mLockLeftClickMove = true;
		mStartTransformPoint = XMFLOAT2(evt.GetX(), evt.GetY());


		mGizmoStartPoint2D = mGizmo->GetGizmoStartPoint2D();
		mGizmoAxisXPoint2D = mGizmo->GetGizmoAxisXPoint2D();
		mGizmoAxisYPoint2D = mGizmo->GetGizmoAxisYPoint2D();
		mGizmoAxisZPoint2D = mGizmo->GetGizmoAxisZPoint2D();

		mGizmoAxisXYPoint2D = mGizmo->GetGizmoAxisXYPoint2D();
		mGizmoAxisXZPoint2D = mGizmo->GetGizmoAxisXZPoint2D();
		mGizmoAxisYZPoint2D = mGizmo->GetGizmoAxisYZPoint2D();
		mGizmoAxisXYZPoint2D = mGizmo->GetGizmoAxisYZPoint2D();

		//mCurrentTransformPoint = XMFLOAT2(mStartTransformPoint.x - mGizmoStartPoint2D.x, mStartTransformPoint.y - mGizmoStartPoint2D.y);
		mCurrentTransformPoint = XMFLOAT2(evt.GetX(), evt.GetY());

		
		if (mSelectedObject != nullptr)
		{
			mGizmo->FindStartPointOfTransform(mStartTransformPoint, &mCam, &m_worldBaseTransform, m_ClientWidth, m_ClientHeight, mSelectedObject->GetCoordinate(), 0);
			//Rotate case
			if (mGizmo->GetGizmoSelected() == 11 || mGizmo->GetGizmoSelected() == 12 || mGizmo->GetGizmoSelected() == 13)
			{
				mGizmo->FindNearPointOfRotation(mStartTransformPoint, &mCam, &m_worldBaseTransform, m_ClientWidth, m_ClientHeight);
				mAngle = 0.0f; //Set angle to draw rotation
				mGizmo->DrawRotationAngle(mAngle);
				mGizmo->SaveMatrixCircleRotation(mSelectedObject->GetCoordinate());
			}
			//Save old value of scale
			mScaleSaveValue = mSelectedObject->GetCoordinate()->GetScale();
		}
	}
	else
	{

		mLockLeftClickMove = false;
	}

	evt.Skip();
}
//***********************************************************************************
void KalenEngine::OnMouseLeftUp(wxMouseEvent& evt)
{
	wxLongLong time = wxGetLocalTimeMillis();


	if ((time - mLeftClickTimer) < 200) //Detecting click 
	{
		//pick object
		if (mGizmo->GetGizmoSelected() == 0 || mGizmo->GetGizmoSelected() == 10 || mGizmo->GetGizmoSelected() == 30)
			Pick(evt.GetX(), evt.GetY());
	}
	if(mSelectedObject != nullptr)
	{ 
		mScaleSaveValue = mSelectedObject->GetCoordinate()->GetScale();
		//Update in the octree
		mOctree->AddObject(mSelectedObject);
	}
	mLockLeftClickMove = false;
	mAngle = 0.0f;
	mGizmo->DrawRotationAngle(mAngle);
	evt.Skip();
}
//***********************************************************************************************************************
void KalenEngine::OnMouseRightDown(wxMouseEvent& evt)
{
	mRightClickTimer = wxGetLocalTimeMillis();
	evt.Skip();
}
//***********************************************************************************
void KalenEngine::OnMouseRightUp(wxMouseEvent& evt)
{
	mLockLeftClickMove = false;
	wxLongLong time = wxGetLocalTimeMillis();
	if ((time - mRightClickTimer) < 200) //Detecting click 
	{
		mLockLeftClickMove = true;
		wxMenu menu(wxT(""));

		wxPoint point = wxPoint(evt.GetX(), evt.GetY());
		point.x = point.x + 5;
		point.y = point.y + 5;

		menu.Append(ID_MouseRightMenuSlot1, wxT("Add - Point Light"));
		
		if (mSelectedObject != nullptr)
		{
			menu.AppendSeparator();
			menu.Append(ID_MouseRightMenuSlot2, wxT("Copy"));
			//menu.Append(ID_MouseRightMenuSlot3, wxT("Cut"));
		}
		
		menu.Append(ID_MouseRightMenuSlot4, wxT("Paste"));

		if (mSelectedObject != nullptr)
		{
			menu.AppendSeparator();
			menu.Append(ID_MouseRightMenuSlot5, wxT("Delete"));
		}


		PopupMenu(&menu, point);
	}
	else
	{
		mLockLeftClickMove = false;
	}
	evt.Skip();

}
int KalenEngine::GetCopyPasteCutDelete()
{
	return this->mCopyPasteCutDelete;
}
void KalenEngine::SetCopyPasteCutDelete(int signal)
{
	this->mCopyPasteCutDelete = signal;
}
void KalenEngine::OnMouseMenuCase1(wxCommandEvent& WXUNUSED(evt))
{
	mIsAddedLight = true;
}
void KalenEngine::OnMouseMenuCase2(wxCommandEvent& WXUNUSED(evt))
{
	mCopyPasteCutDelete = 1; //copy
}
void KalenEngine::OnMouseMenuCase3(wxCommandEvent& WXUNUSED(evt))
{
	mCopyPasteCutDelete = 2; //cut
}
void KalenEngine::OnMouseMenuCase4(wxCommandEvent& WXUNUSED(evt))
{
	mCopyPasteCutDelete = 3; //paste
}
void KalenEngine::OnMouseMenuCase5(wxCommandEvent& WXUNUSED(evt))
{
	mCopyPasteCutDelete = 4; //delete
}
//***********************************************************************************************************************
void KalenEngine::OnMouseMotion(wxMouseEvent& evt)
{
	float x = evt.GetX();
	float y = evt.GetY();

	mMousePosition.x = x;
	mMousePosition.y = y;
	if (evt.RightIsDown() && mLockLeftClickMove == false)
	{
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	if (evt.LeftIsDown() && mLockLeftClickMove == false)
	{
		float xx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float yy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		mCam.Walk(yy * 50 * (-1));
		mCam.RotateY(xx);
	}
	if (mLockLeftClickMove == true)
	{
		if (mSelectedObject != nullptr)
		{
			if (gTransformGizmoType == 1) // MOVE CASE
				GizmoMove2(wxPoint(evt.GetX(), evt.GetY()));
			else
				if (gTransformGizmoType == 2) // ROTATE CASE
					GizmoRotate(wxPoint(evt.GetX(), evt.GetY()));
				else
					if (gTransformGizmoType == 3) // SCALE CASE
						GizmoScale(wxPoint(evt.GetX(), evt.GetY()));
		}
	}
		

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
//***********************************************************************************************************************
void KalenEngine::OnKeyDown(wxKeyEvent& evt)
{
	wxChar inputChar = evt.GetUnicodeKey();
	
	switch (inputChar)
	{
	case (int)'W':
		mKeyPressW = true;
		break;
	case (int)'S':
		mKeyPressS = true;
		break;
	case (int)'A':
		mKeyPressA = true;
		break;
	case (int)'D':
		mKeyPressD = true;
		break;
	case (int)'Q':
		mKeyPressQ = true;
		break;
	case (int)'E':
		mKeyPressE = true;
		break;
	default:
		//evt.Skip();
		break;
	}

	if (evt.ShiftDown() == true)
		mKeyPressShift = true;
	evt.Skip();
}
//*********************************************************
void KalenEngine::OnKeyUp(wxKeyEvent& evt)
{
	wxChar inputChar = evt.GetUnicodeKey();

	switch (inputChar)
	{
	case (int)'W':
		mKeyPressW = false;
		break;
	case (int)'S':
		mKeyPressS = false;
		break;
	case (int)'A':
		mKeyPressA = false;
		break;
	case (int)'D':
		mKeyPressD = false;
		break;
	case (int)'Q':
		mKeyPressQ = false;
		break;
	case (int)'E':
		mKeyPressE = false;
		break;
	default:
		//evt.Skip();
		break;
	}

	if (evt.ShiftDown() == false)
		mKeyPressShift = false;
	evt.Skip();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
//Gizmo MOVE - private function to move object 
//-------------------------------------------------------------------------------------------------------------------------------------------
void KalenEngine::GizmoMove(wxPoint mousePosition)
{

	XMVECTOR tempVector;
	XMVECTOR mousePos;
	XMVECTOR mousePosNorm;
	XMVECTOR normalLine;
	XMVECTOR cosine;
	XMVECTOR cosineSecondAxis;
	float length = 0.0f;
	float lengthSecondAxis = 0.0f;


	int gizmoTransformType = mGizmo->GetGizmoSelected();
	float distanceToGizmo;

	distanceToGizmo = mGizmo->GetDistanceToGizmo();

	XMFLOAT2 tempAxisPoint = XMFLOAT2(0, 0);
	XMFLOAT2 tempSecondAxisPoint = XMFLOAT2(0, 0);


	//Set axis
	if (mGizmo->GetGizmoSelected() == 1) //X
		tempAxisPoint = mGizmoAxisXPoint2D;
	else
		if (mGizmo->GetGizmoSelected() == 2) //Y
			tempAxisPoint = mGizmoAxisYPoint2D;
		else
			if (mGizmo->GetGizmoSelected() == 3) //Z
				tempAxisPoint = mGizmoAxisZPoint2D;
			else
				if (mGizmo->GetGizmoSelected() == 4) //XY
				{
					tempAxisPoint = mGizmoAxisXPoint2D;
					tempSecondAxisPoint = mGizmoAxisYPoint2D;
				}
				else
					if (mGizmo->GetGizmoSelected() == 5) //XZ
					{
						tempAxisPoint = mGizmoAxisXPoint2D;
						tempSecondAxisPoint = mGizmoAxisZPoint2D;
					}
					else
						if (mGizmo->GetGizmoSelected() == 6) //YZ
						{
							tempAxisPoint = mGizmoAxisYPoint2D;
							tempSecondAxisPoint = mGizmoAxisZPoint2D;
						}

	//Normalize line between GizmoStartPoint (center of gizmo) and one of axis (X,Y,Z)
	normalLine = XMLoadFloat2(&XMFLOAT2(tempAxisPoint.x - mGizmoStartPoint2D.x, tempAxisPoint.y - mGizmoStartPoint2D.y));
	normalLine = XMVector2Normalize(normalLine);

	//Normalize line between mouse position and StartTransformPoint (clicked point)
	mousePos = XMLoadFloat2(&XMFLOAT2(mousePosition.x - mStartTransformPoint.x, mousePosition.y - mStartTransformPoint.y));
	mousePosNorm = XMVector2Normalize(mousePos);

	//Counting cosine 
	cosine = XMVector2Dot(normalLine, mousePosNorm);

	//Proj = ( (V dot W) / ||V||^2 ) * V
	//V = normalLine
	//W = mousePos
	tempVector = XMVector2Dot(normalLine, mousePos);
	tempVector = XMVectorGetX(tempVector) * mousePosNorm;

	//Counting lenght of the projected vector
	length = sqrt((XMVectorGetX(tempVector) * XMVectorGetX(tempVector)) + (XMVectorGetY(tempVector) *  XMVectorGetY(tempVector)));

	//Adjustment values (screen values and 3D space values)
	//length = ((length * abs(XMVectorGetX(cosine))) * (distanceToGizmo / 500));
	if (XMVectorGetX(cosine) < 0.2 && XMVectorGetX(cosine) > -0.2)
		length = ((length * abs(XMVectorGetX(cosine))) * (distanceToGizmo / 500));
	else
		length = (length * (distanceToGizmo / 500));



	//---------------------	
	//FOR 2 SELECTED AXIS
	//---------------------	
	normalLine = XMLoadFloat2(&XMFLOAT2(tempSecondAxisPoint.x - mGizmoStartPoint2D.x, tempSecondAxisPoint.y - mGizmoStartPoint2D.y));
	normalLine = XMVector2Normalize(normalLine);

	//Counting second cosine 
	cosineSecondAxis = XMVector2Dot(normalLine, mousePosNorm);

	//Proj = ( (V dot W) / ||V||^2 ) * V
	//V = normalLine
	//W = mousePos
	tempVector = XMVector2Dot(normalLine, mousePos);
	tempVector = XMVectorGetX(tempVector) * mousePosNorm;

	//Counting lenght of the projected vector
	lengthSecondAxis = sqrt((XMVectorGetX(tempVector) * XMVectorGetX(tempVector)) + (XMVectorGetY(tempVector) *  XMVectorGetY(tempVector)));

	//Adjustment values (screen values and 3D space values)
	if (XMVectorGetX(cosineSecondAxis) < 0.2 && XMVectorGetX(cosineSecondAxis) > -0.2)
		lengthSecondAxis = ((lengthSecondAxis * abs(XMVectorGetX(cosineSecondAxis))) * (distanceToGizmo / 500));
	else
		lengthSecondAxis = (lengthSecondAxis * (distanceToGizmo / 500));



	//Print info and get&set new values
	if (length >= __move_Unit || lengthSecondAxis >= __move_Unit)
	{
		mStartTransformPoint = XMFLOAT2(mousePosition.x, mousePosition.y);
		mGizmoStartPoint2D = mGizmo->GetGizmoStartPoint2D();
		mGizmoAxisXPoint2D = mGizmo->GetGizmoAxisXPoint2D();
		mGizmoAxisYPoint2D = mGizmo->GetGizmoAxisYPoint2D();
		mGizmoAxisZPoint2D = mGizmo->GetGizmoAxisZPoint2D();

		/*wxString text = "Position: X = ";
		text << mObject.manyObjectsVector[0].coordinate.GetLocalCoordinatePosition().x;
		text << "; Y = ";
		text << mObject.manyObjectsVector[0].coordinate.GetLocalCoordinatePosition().z;
		text << "; Z = ";
		text << mObject.manyObjectsVector[0].coordinate.GetLocalCoordinatePosition().y;
		(*mTextOututWindow123) << "\n<<" << text;*/
	}


	if (length >= __move_Unit) //if length is bigger then step value
	{
		length = (int)(length / __move_Unit);

		//inverse 
		if (XMVectorGetX(cosine) <= 0)
			length *= -1;

		if (gizmoTransformType == 1) //MOVE X
		{
			if (gGlobalSpace == true) //Global space
			{
				mSelectedObject->GetCoordinate()->UpdateGlobalCoordinateMatrix();
				mSelectedObject->GetCoordinate()->MoveGlobalX(length *__move_Unit);
			}
			else //Local Space
				mSelectedObject->GetCoordinate()->MoveX(length * __move_Unit);
		}
		else
			if (gizmoTransformType == 2) //MOVE Y
			{
				if (gGlobalSpace == true) //Global space
					mSelectedObject->GetCoordinate()->MoveGlobalY(length * __move_Unit);
				else //Local Space
					mSelectedObject->GetCoordinate()->MoveY(length * __move_Unit);
			}
			else
				if (gizmoTransformType == 3) //MOVE Z
				{
					if (gGlobalSpace == true) //Global space
						mSelectedObject->GetCoordinate()->MoveGlobalZ(length * __move_Unit);
					else //Local Space
						mSelectedObject->GetCoordinate()->MoveZ(length * __move_Unit);
				}
				else
					if (gizmoTransformType == 4) //MOVE XY - only X
					{
						if (gGlobalSpace == true) //Global space
							mSelectedObject->GetCoordinate()->MoveGlobalX(length * __move_Unit);
						else //Local Space
							mSelectedObject->GetCoordinate()-> MoveX(length * __move_Unit);
					}
					else
						if (gizmoTransformType == 5) //MOVE XZ - only X
						{
							if (gGlobalSpace == true) //Global space
								mSelectedObject->GetCoordinate()->MoveGlobalX(length * __move_Unit);
							else //Local Space
								mSelectedObject->GetCoordinate()->MoveX(length * __move_Unit);
						}
						else
							if (gizmoTransformType == 6) //MOVE YZ - only Y
							{
								if (gGlobalSpace == true) //Global space
									mSelectedObject->GetCoordinate()->MoveGlobalY(length * __move_Unit);
								else //Local Space
									mSelectedObject->GetCoordinate()->MoveY(length * __move_Unit);
							}
	}
	//For two lines (XY, XZ, YZ)
	if (lengthSecondAxis >= __move_Unit)
	{
		lengthSecondAxis = (int)(lengthSecondAxis / __move_Unit);

		//inverse 
		if (XMVectorGetX(cosineSecondAxis) <= 0)
			lengthSecondAxis *= -1;


		if (gizmoTransformType == 4) //MOVE XY
		{
			if (gGlobalSpace == true) //Global space
				mSelectedObject->GetCoordinate()->MoveGlobalY(lengthSecondAxis * __move_Unit);
			else //Local Space
				mSelectedObject->GetCoordinate()->MoveY(lengthSecondAxis * __move_Unit);
		}
		else
			if (gizmoTransformType == 5) //MOVE XZ
			{
				if (gGlobalSpace == true) //Global space
					mSelectedObject->GetCoordinate()->MoveGlobalZ(lengthSecondAxis * __move_Unit);
				else //Local Space
					mSelectedObject->GetCoordinate()->MoveZ(lengthSecondAxis * __move_Unit);
			}
			else
				if (gizmoTransformType == 6) //MOVE YZ
				{
					if (gGlobalSpace == true) //Global space
						mSelectedObject->GetCoordinate()->MoveGlobalZ(lengthSecondAxis * __move_Unit);
					else //Local Space
						mSelectedObject->GetCoordinate()->MoveZ(lengthSecondAxis * __move_Unit);
				}
	}

}
//-------------------------------------------------------------------------------------------------------------------------------------------
//Gizmo ROTATE - private function to rotate object 
//-------------------------------------------------------------------------------------------------------------------------------------------
void KalenEngine::GizmoRotate(wxPoint mousePosition)
{
	XMVECTOR tempVector;
	XMVECTOR linePos;
	XMVECTOR linePosNorm;
	XMVECTOR mousePos2;
	XMVECTOR mousePos2Norm;
	XMVECTOR cosine;
	XMVECTOR cosine2;
	float length = 0.0f;
	int gizmoTransformType = mGizmo->GetGizmoSelected();
	XMFLOAT2 tempAxisPoint = XMFLOAT2(0, 0);

	mGizmoStartPoint2D = mGizmo->GetGizmoStartPoint2D();

	//Create line from StartTransformPoint (first clicked point)
	linePos = XMLoadFloat2(&XMFLOAT2(mStartTransformPoint.x - mGizmoStartPoint2D.x, mStartTransformPoint.y - mGizmoStartPoint2D.y));


	float sinT = sin(XMConvertToRadians(90));
	float cosT = cos(XMConvertToRadians(90));

	linePos = XMLoadFloat2(&XMFLOAT2(XMVectorGetX(linePos) * cosT - XMVectorGetY(linePos) * sinT, XMVectorGetX(linePos) * sinT + XMVectorGetY(linePos) * cosT));





	//Normalize - Direction line (use to compute cosine between this line and mouse vector) 
	linePosNorm = XMVector2Normalize(linePos);

	//MousePoint - normal vector
	mousePos2 = XMLoadFloat2(&XMFLOAT2(mousePosition.x - mCurrentTransformPoint.x, mousePosition.y - mCurrentTransformPoint.y));
	mousePos2Norm = XMVector2Normalize(mousePos2);

	//Counting cosine 
	cosine = XMVector2Dot(linePosNorm, mousePos2Norm);

	//Proj = ( (V dot W) / ||V||^2 ) * V
	//V = normalLine
	//W = mousePos2
	tempVector = XMVector2Dot(linePosNorm, mousePos2);
	tempVector = XMVectorGetX(tempVector) * mousePos2Norm;

	//Counting lenght of the projected vector
	length = sqrt((XMVectorGetX(tempVector) * XMVectorGetX(tempVector)) + (XMVectorGetY(tempVector) *  XMVectorGetY(tempVector)));

	//Adjustment values (screen values and 3D space values)
	length = length * abs(XMVectorGetX(cosine));



	//---------------------------
	//linePos - now it is a vector to describe axis (X or Y or Z) 
	if (gizmoTransformType == 11) //Rotate X
		linePos = XMLoadFloat3(&XMFLOAT3(mSelectedObject->GetCoordinate()->GetLocalAxisX()));
	else
		if (gizmoTransformType == 12) //Rotate Y
			linePos = XMLoadFloat3(&XMFLOAT3(mSelectedObject->GetCoordinate()->GetLocalAxisY()));
		else
			if (gizmoTransformType == 13) //Rotate Z
				linePos = XMLoadFloat3(&XMFLOAT3(mSelectedObject->GetCoordinate()->GetLocalAxisZ()));

	tempVector = XMVector3Normalize(mCam.GetPositionXM() - mSelectedObject->GetCoordinate()->GetLocalCoordinatePositionVECTOR());

	//Counting cosine between camera vector and local axis
	cosine2 = XMVector3Dot(tempVector, linePos);
	//---------------------------

	XMFLOAT3 axis = XMFLOAT3(0.0f, 0.0f, 0.0f);
	if (length >= __rotate_Degree)
	{
		float rotation = (int)(length / __rotate_Degree);
		//inverse 
		if (XMVectorGetX(cosine) <= 0)
			rotation *= -1;

		//Cosine to the camera and one of axis
		if (XMVectorGetX(cosine2) <= 0)
			rotation *= -1;


		mAngle += (rotation * __rotate_Degree); //To draw rotation 
		mGizmo->DrawRotationAngle((int)mAngle);

		if (gGlobalSpace == true) //Global space
			mSelectedObject->GetCoordinate()->UpdateGlobalCoordinateMatrix();
		else
			mSelectedObject->GetCoordinate()->UpdateLocalCoordinateMatrix();

		if (gizmoTransformType == 11) //Rotate X
		{
			if (gGlobalSpace == true) //Global space
			{
				mSelectedObject->GetCoordinate()->Pitch_GlobalAxis(XMConvertToRadians(rotation * __rotate_Degree));
				axis = mSelectedObject->GetCoordinate()->GetGlobalAxisX();
			}
			else //Local Space
			{
				mSelectedObject->GetCoordinate()->Pitch(XMConvertToRadians(rotation * __rotate_Degree));
				axis = mSelectedObject->GetCoordinate()->GetLocalAxisX();
			}	
		}
		if (gizmoTransformType == 12) //Rotate Y
		{
			if (gGlobalSpace == true) //Global space
			{
				mSelectedObject->GetCoordinate()->Yaw_GlobalAxis(XMConvertToRadians(rotation * __rotate_Degree));
				axis = mSelectedObject->GetCoordinate()->GetGlobalAxisY();
			}
			else //Local Space
			{
				mSelectedObject->GetCoordinate()->Yaw(XMConvertToRadians(rotation * __rotate_Degree));
				axis = mSelectedObject->GetCoordinate()->GetLocalAxisY();
			}
		}
		if (gizmoTransformType == 13) //Rotate Z
		{
			if (gGlobalSpace == true) //Global space
			{
				mSelectedObject->GetCoordinate()->Roll_GlobalAxis(XMConvertToRadians(rotation * __rotate_Degree));
				axis = mSelectedObject->GetCoordinate()->GetGlobalAxisZ();
			}	
			else //Local Space
			{
				mSelectedObject->GetCoordinate()->Roll(XMConvertToRadians(rotation * __rotate_Degree));
				axis = mSelectedObject->GetCoordinate()->GetLocalAxisZ();
			}
		}

		if (gGlobalSpace == true) //Global space
			mSelectedObject->GetCoordinate()->UpdateGlobalCoordinateMatrix();
		else
			mSelectedObject->GetCoordinate()->UpdateLocalCoordinateMatrix();

		//Update value
		mCurrentTransformPoint = XMFLOAT2(mousePosition.x, mousePosition.y);

		//Print values
		wxString text = "Rotation: Axis: (";
		text << axis.x;
		text << ", ";
		text << axis.z;
		text << ", ";
		text << axis.y;
		text << "); Angle: ";
		text << mAngle;
		text << "\xB0";//         GLOBAL: Pitch = ";
		

		//Get values (Pitch, Roll, Yaw) from matrix
		mSelectedObject->GetCoordinate()->UpdateLocalCoordinateMatrix();
		XMMatrixDecompose(&tempVector, &linePos, &tempVector, XMMatrixInverse(nullptr, mSelectedObject->GetCoordinate()->GetLocalMatrix()));
		float roll = 0.0f;
		float yaw = 0.0f;
		float pitch = 0.0f;

		QuaternionToEulerianAngle(linePos, &pitch,  &roll, &yaw);

		roll = roundf(XMConvertToDegrees(roll) * 100) / 100;
		yaw = roundf(XMConvertToDegrees(yaw) * 100) / 100;
		pitch = roundf(XMConvertToDegrees(pitch) * 100) / 100;

		//mSelectedObject->GetCoordinate()->SetPitch(pitch);
		//mSelectedObject->GetCoordinate()->SetYaw(yaw);
		//mSelectedObject->GetCoordinate()->SetRoll(roll);

		mSelectedObject->GetCoordinate()->SetPitch(roll);
		mSelectedObject->GetCoordinate()->SetYaw(pitch);
		mSelectedObject->GetCoordinate()->SetRoll(yaw);

	/*	text << roll;
		text << "\xB0 Roll = ";
		text << yaw;
		text << "\xB0 Yaw = ";
		text << pitch;
		text << "\xB0";*/
		(*mTextOututWindow123) << "\n<<" << text;
	}
	
	RebuildBoundingBox(mSelectedObject);

}
//-------------------------------------------------------------------------------------------------------------------------------------------
//Gizmo SCALE - private function to scale object 
//-------------------------------------------------------------------------------------------------------------------------------------------
void KalenEngine::GizmoScale(wxPoint mousePosition)
{
	//mCurrentTransformPoint = XMFLOAT2(mousePosition.x, mousePosition.y);
	XMVECTOR tempVector;
	XMVECTOR mousePos;
	XMVECTOR mousePosNorm;
	XMVECTOR normalLine;
	XMVECTOR cosine;
	XMVECTOR axis;
	float length = 0.0f;
	float lengthSecondAxis = 0.0f;


	int gizmoTransformType = mGizmo->GetGizmoSelected();
	float distanceToGizmo;

	distanceToGizmo = mGizmo->GetDistanceToGizmo();

	XMFLOAT2 tempAxisPoint = XMFLOAT2(0, 0);
	XMFLOAT2 tempSecondAxisPoint = XMFLOAT2(0, 0);


	//Set axis
	if (mGizmo->GetGizmoSelected() == 31) //X
	{
		tempAxisPoint = mGizmoAxisXPoint2D;
		if (gGlobalSpace != true)
			axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
		else
			axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
	}
		
	else
		if (mGizmo->GetGizmoSelected() == 32) //Y
		{
			tempAxisPoint = mGizmoAxisYPoint2D;
			if (gGlobalSpace != true)
				axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY()); //Global Y
			else
				axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY()); //Local Y
		}
		else
			if (mGizmo->GetGizmoSelected() == 33) //Z
			{
				tempAxisPoint = mGizmoAxisZPoint2D;
				if (gGlobalSpace != true)
					axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ()); //Global Z
				else
					axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ()); //Local Z
			}
			else
				if (mGizmo->GetGizmoSelected() == 34) //XY
				{
					tempAxisPoint = mGizmoAxisXYPoint2D;
					if (gGlobalSpace != true)
					{
						axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
						axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY())); //Add global Y to global X
						axis = XMVector3Normalize(axis);
					}
					else
					{
						axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
						axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY())); //Add local Y to local X
						axis = XMVector3Normalize(axis);
					}
				}
				else
					if (mGizmo->GetGizmoSelected() == 35) //XZ
					{
						tempAxisPoint = mGizmoAxisXZPoint2D;
						if (gGlobalSpace != true)
						{
							axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
							axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ())); //Add global Z to global X
							axis = XMVector3Normalize(axis);
						}
						else
						{
							axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
							axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ())); //Add local Z to local X
							axis = XMVector3Normalize(axis);
						}
					}
					else
						if (mGizmo->GetGizmoSelected() == 36) //YZ
						{
							tempAxisPoint = mGizmoAxisYZPoint2D;
							if (gGlobalSpace != true)
							{
								axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY()); //Global Y
								axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ())); //Add global Z to global Y
								axis = XMVector3Normalize(axis);
							}
							else
							{
								axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY()); //Local Y
								axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ())); //Add local Z to local Y
								axis = XMVector3Normalize(axis);
							}
						}
						else
							if (mGizmo->GetGizmoSelected() == 37) //XYZ
							{
								tempAxisPoint = mGizmoAxisXYZPoint2D;
								if (gGlobalSpace != true)
								{
									axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
									axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY())); //Add global Y to global X
									axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ())); //Add global Z to global XY
									axis = XMVector3Normalize(axis);
								}
								else
								{
									axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
									axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY())); //Add local Y to local X
									axis = XMVectorAdd(axis, XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ())); //Add local Z to local XY
									axis = XMVector3Normalize(axis);
								}
							}

	//Normalize line between GizmoStartPoint (center of gizmo) and one of axis (X,Y,Z)
	normalLine = XMLoadFloat2(&XMFLOAT2(tempAxisPoint.x - mGizmoStartPoint2D.x, tempAxisPoint.y - mGizmoStartPoint2D.y));
	normalLine = XMVector2Normalize(normalLine);

	//Normalize line between mouse position and StartTransformPoint (clicked point)
	mousePos = XMLoadFloat2(&XMFLOAT2(mousePosition.x - mStartTransformPoint.x, mousePosition.y - mStartTransformPoint.y));
	mousePosNorm = XMVector2Normalize(mousePos);

	//Counting cosine 
	cosine = XMVector2Dot(normalLine, mousePosNorm);

	//Proj = ( (V dot W) / ||V||^2 ) * V
	//V = normalLine
	//W = mousePos
	tempVector = XMVector2Dot(normalLine, mousePos);
	tempVector = XMVectorGetX(tempVector) * mousePosNorm;

	//Counting lenght of the projected vector
	length = sqrt((XMVectorGetX(tempVector) * XMVectorGetX(tempVector)) + (XMVectorGetY(tempVector) *  XMVectorGetY(tempVector)));

	//Adjustment values (screen values and 3D space values)
	if (XMVectorGetX(cosine) < 0.2 && XMVectorGetX(cosine) > -0.2)
		length = ((length * abs(XMVectorGetX(cosine))) * (distanceToGizmo / 20000));
	else
		length = (length * (distanceToGizmo / 20000));

	//inverse 
	if (XMVectorGetX(cosine) <= 0)
		length *= -1;

	//Normal vector * length
	axis *= length;

	//Add vector to scale.
	mSelectedObject->GetCoordinate()->SetScaleInverse(XMFLOAT3((1.0f / mScaleSaveValue.x) + XMVectorGetX(axis), (1.0f / mScaleSaveValue.y) + XMVectorGetY(axis), (1.0f / mScaleSaveValue.z) + XMVectorGetZ(axis)));

	//mSelectedObject->GetCoordinate()->SetScale(XMFLOAT3(1.0f / (mScaleSaveValue.x + XMVectorGetX(axis)), 1.0f / (mScaleSaveValue.y + XMVectorGetY(axis)), 1.0f / (mScaleSaveValue.z + XMVectorGetZ(axis)));
	if (mSelectedObject->GetCoordinate()->GetScale().x < 0)
		mSelectedObject->GetCoordinate()->SetScaleInverse(XMFLOAT3(0.001f, mSelectedObject->GetCoordinate()->GetScale().y, mSelectedObject->GetCoordinate()->GetScale().z));
	if (mSelectedObject->GetCoordinate()->GetScale().y < 0)
		mSelectedObject->GetCoordinate()->SetScaleInverse(XMFLOAT3(mSelectedObject->GetCoordinate()->GetScale().x, 0.001f, mSelectedObject->GetCoordinate()->GetScale().z));
	if (mSelectedObject->GetCoordinate()->GetScale().z < 0)
		mSelectedObject->GetCoordinate()->SetScaleInverse(XMFLOAT3(mSelectedObject->GetCoordinate()->GetScale().x, mSelectedObject->GetCoordinate()->GetScale().y, 0.001f));
	


	RebuildBoundingBox(mSelectedObject);
	//Print value
	/*wxString text = "Scale: (";
	text << mObject.manyObjectsVector[0].coordinate.GetScale().x;
	text << ", ";
	text << mObject.manyObjectsVector[0].coordinate.GetScale().z;
	text << ", ";
	text << mObject.manyObjectsVector[0].coordinate.GetScale().y;
	text << ")";
	text << length;
	text << ") Scale: (";
	text << XMVectorGetX(axis);
	text << ", ";
	text << XMVectorGetY(axis);
	text << ", ";
	text << XMVectorGetZ(axis);
	text << ")";
	
	(*mTextOututWindow123) << "\n<<" << text;

	*/
}
//-------------------------------------------------------------------------------------------------------------------------------------------
//Gizmo Move - private function to move object 
//-------------------------------------------------------------------------------------------------------------------------------------------
void KalenEngine::GizmoMove2(wxPoint mousePosition)
{
	//Counting ray
	XMMATRIX P = mCam.Proj();
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f * mousePosition.x / m_ClientWidth - 1.0f) / p(0, 0);
	float vy = (-2.0f * mousePosition.y / m_ClientHeight + 1.0f) / p(1, 1);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = m_worldBaseTransform;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	rayDir = XMVector3Normalize(rayDir);


	//----------------------------------------------------------------------

	XMVECTOR normalvec = mGizmo->GetNormalVectorToPlane();

	XMVECTOR CurrentPoint;
	XMVECTOR ProjCurrentPoint;
	XMVECTOR PerpCurrentPoint;

	XMVECTOR StartPoint;
	XMVECTOR ProjStartPoint;
	XMVECTOR PerpStartPoint;
	XMVECTOR StartPoint2;

	XMVECTOR axis; //First axis, use: X, Y, Z, XY, XZ, YZ
	XMVECTOR axis2; //Second axis, use: XY, XZ, YZ

	XMVECTOR tempVec;
	XMVECTOR cosine; //First cosine
	XMVECTOR cosine2; //Second cosine

	float lengthTransform = 0.0f;
	float lengthTransform2 = 0.0f;

	//----------------------------------------------------
	//Equation - ray and plane - Cunting 'CurrentPoint' 
	//----------------------------------------------------
	//CurrentPoint = S - (((N dot S) + a / (N dot V)) * V)
	// a = -(N dot K)
	// K --> Gizmo position
	// S --> rayOrigin
	// N --> normalvec
	// V --> rayDir

	//Counting dot (N dot S)
	CurrentPoint = XMVector3Dot(normalvec, rayOrigin); // (N dot S)

	//Counting 'a'
	tempVec = XMVector3Dot(XMVectorNegate(normalvec), XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalCoordinatePosition())); // a = (-N dot K)

	//Add 'a'
	CurrentPoint = XMVectorAdd(CurrentPoint, tempVec); // (N dot S) + a = (N dot S) + (-N dot K)

	//Counting dot (N dot V)
	tempVec = XMVector3Dot(normalvec, rayDir); // (N dot V)

	//Divide
	CurrentPoint = XMVectorDivide(CurrentPoint, tempVec); // ((N dot S) + (-N dot K)) / (N dot V)

	//Multiply
	CurrentPoint = XMVectorMultiply(CurrentPoint, rayDir); // (((N dot S) + (-N dot K)) / (N dot V)) * V

	//Substract
	CurrentPoint = XMVectorSubtract(rayOrigin, CurrentPoint); // S - ((((N dot S) + (-N dot K)) / (N dot V)) * V)


	//Set axis 
	if (mGizmo->GetGizmoSelected() == 1) //X
	{
		if(gGlobalSpace == true)
			axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
		else
			axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
	}	
	else
		if (mGizmo->GetGizmoSelected() == 2) //Y
		{
			if (gGlobalSpace == true)
				axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY()); //Global Y
			else
				axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY()); //Local Y
		}
		else
			if (mGizmo->GetGizmoSelected() == 3) //Z
			{
				if (gGlobalSpace == true)
					axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ()); //Global Z
				else
					axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ()); //Local Z
			}
			else
				if (mGizmo->GetGizmoSelected() == 4) //XY
				{
					if (gGlobalSpace == true)
					{
						axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
						axis2 = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY()); //Global Y
					}
					else
					{
						axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
						axis2 = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY()); //Local Y
					}	
				}
				else
					if (mGizmo->GetGizmoSelected() == 5) //XZ
					{
						if (gGlobalSpace == true)
						{
							axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisX()); //Global X
							axis2 = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ()); //Global Z
						}
						else
						{
							axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisX()); //Local X
							axis2 = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ()); //Local Z
						}
					}
					else
						if (mGizmo->GetGizmoSelected() == 6) //YZ
						{
							if (gGlobalSpace == true)
							{
								axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisY()); //Global Y
								axis2 = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetGlobalAxisZ()); //Global Z
							}
							else
							{
								axis = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisY()); //Local Y
								axis2 = XMLoadFloat3(&mSelectedObject->GetCoordinate()->GetLocalAxisZ()); //Local Z
							}
						}

	//StartPoint - set value
	StartPoint = XMLoadFloat3(&XMFLOAT3(mGizmo->GetStartPoint().x, mGizmo->GetStartPoint().y, mGizmo->GetStartPoint().z));
	StartPoint2 = XMLoadFloat3(&XMFLOAT3(mGizmo->GetStartPoint2().x, mGizmo->GetStartPoint2().y, mGizmo->GetStartPoint2().z));

	//----------------------------------------------------
	//Equation - Project StartPoint to axis.
	//----------------------------------------------------
	//ProjStartPoint = (V dot W) * V
	//V --> Axis
	//W --> StartPoint
	ProjStartPoint = XMVectorMultiply(XMVector3Dot(axis, StartPoint), axis);
	//PerpStartPoint = XMVectorMultiply(XMVector3Dot(axis2, StartPoint), axis2);

	PerpStartPoint = XMVectorMultiply(XMVector3Dot(axis2, StartPoint2), axis2);

	//Counting cosine between 'StatrtPoint' and 'CurrentPoint' 
	//Normalize
	tempVec = XMVectorSubtract(CurrentPoint, StartPoint);
	tempVec = XMVector3Normalize(tempVec);//CurrentPoint normalize

	//Counting cosine 
	cosine = XMVector3Dot(axis, tempVec); //First cosine
	cosine2 = XMVector3Dot(axis2, tempVec); //Second cosine
	//----------------------------------------------------
	//Equation - Project CurrentPoint to axis.
	//----------------------------------------------------
	//ProjCurrentPoint = (V dot W) * V
	//V --> Axis
	//W --> CurrentPoint
	ProjCurrentPoint = XMVectorMultiply(XMVector3Dot(axis, CurrentPoint), axis);
	PerpCurrentPoint = XMVectorMultiply(XMVector3Dot(axis2, CurrentPoint), axis2);

	//-------------
	//First length
	//-------------
	tempVec = XMVectorSubtract(ProjCurrentPoint, ProjStartPoint); //ProjCurrentPoint - ProjStartPoint
	XMVECTOR length = XMVector3Length(tempVec); //Counting length 
	lengthTransform = XMVectorGetX(length); //Vector to scalar

	//-------------
	//Second length
	//-------------
	tempVec = XMVectorSubtract(PerpCurrentPoint, PerpStartPoint);//PerpCurrentPoint - PerpStartPoint
	length = XMVector3Length(tempVec); //Counting length 
	lengthTransform2 = XMVectorGetX(length); //Vector to scalar

	if (lengthTransform >= __move_Unit) //if length is bigger then step value
	{
		lengthTransform = (int)(lengthTransform / __move_Unit);
		//inverse 
		if (XMVectorGetX(cosine) < 0)
		{
			lengthTransform *= -1;
		}
		
		if (mGizmo->GetGizmoSelected() == 1) //MOVE X
		{
			if (gGlobalSpace == true) //Global space
			{
				mSelectedObject->GetCoordinate()->UpdateGlobalCoordinateMatrix();
				mSelectedObject->GetCoordinate()->MoveGlobalX(lengthTransform *__move_Unit);
			}
			else //Local Space
				mSelectedObject->GetCoordinate()->MoveX(lengthTransform * __move_Unit);
		}
		else
			if (mGizmo->GetGizmoSelected() == 2) //MOVE Y
			{
				if (gGlobalSpace == true) //Global space
					mSelectedObject->GetCoordinate()->MoveGlobalY(lengthTransform * __move_Unit);
				else //Local Space
					mSelectedObject->GetCoordinate()->MoveY(lengthTransform * __move_Unit);
			}
			else
				if (mGizmo->GetGizmoSelected() == 3) //MOVE Z
				{
					if (gGlobalSpace == true) //Global space
						mSelectedObject->GetCoordinate()->MoveGlobalZ(lengthTransform * __move_Unit);
					else //Local Space
						mSelectedObject->GetCoordinate()->MoveZ(lengthTransform * __move_Unit);
				}
				else
					if (mGizmo->GetGizmoSelected() == 4) //MOVE XY - only X
					{
						if (gGlobalSpace == true) //Global space
							mSelectedObject->GetCoordinate()->MoveGlobalX(lengthTransform * __move_Unit);
						else //Local Space
							mSelectedObject->GetCoordinate()->MoveX(lengthTransform * __move_Unit);
					}
					else
						if (mGizmo->GetGizmoSelected() == 5) //MOVE XZ - only X
						{
							if (gGlobalSpace == true) //Global space
								mSelectedObject->GetCoordinate()->MoveGlobalX(lengthTransform * __move_Unit);
							else //Local Space
								mSelectedObject->GetCoordinate()->MoveX(lengthTransform * __move_Unit);
						}
						else
							if (mGizmo->GetGizmoSelected() == 6) //MOVE YZ - only Y
							{
								if (gGlobalSpace == true) //Global space
									mSelectedObject->GetCoordinate()->MoveGlobalY(lengthTransform * __move_Unit);
								else //Local Space
									mSelectedObject->GetCoordinate()->MoveY(lengthTransform * __move_Unit);
							}

		mGizmo->FindStartPointOfTransform(XMFLOAT2(mousePosition.x, mousePosition.y), &mCam, &m_worldBaseTransform, m_ClientWidth, m_ClientHeight, mSelectedObject->GetCoordinate(), 1);
	}
	//For second axis(XY, XZ, YZ)
	if (lengthTransform2 >= __move_Unit)
	{
		lengthTransform2 = (int)(lengthTransform2 / __move_Unit);

		//inverse 
		if (XMVectorGetX(cosine2) <= 0)
			lengthTransform2 *= -1;


		if (mGizmo->GetGizmoSelected() == 4) //MOVE XY
		{
			if (gGlobalSpace == true) //Global space
				mSelectedObject->GetCoordinate()->MoveGlobalY(lengthTransform2 * __move_Unit);
			else //Local Space
				mSelectedObject->GetCoordinate()->MoveY(lengthTransform2 * __move_Unit);
		}
		else
			if (mGizmo->GetGizmoSelected() == 5) //MOVE XZ
			{
				if (gGlobalSpace == true) //Global space
					mSelectedObject->GetCoordinate()->MoveGlobalZ(lengthTransform2 * __move_Unit);
				else //Local Space
					mSelectedObject->GetCoordinate()->MoveZ(lengthTransform2 * __move_Unit);
			}
			else
				if (mGizmo->GetGizmoSelected() == 6) //MOVE YZ
				{
					if (gGlobalSpace == true) //Global space
						mSelectedObject->GetCoordinate()->MoveGlobalZ(lengthTransform2 * __move_Unit);
					else //Local Space
						mSelectedObject->GetCoordinate()->MoveZ(lengthTransform2 * __move_Unit);
				}

		mGizmo->FindStartPointOfTransform(XMFLOAT2(mousePosition.x, mousePosition.y), &mCam, &m_worldBaseTransform, m_ClientWidth, m_ClientHeight, mSelectedObject->GetCoordinate(), 2);
	}



	RebuildBoundingBox(mSelectedObject);
	
	//Print value
	/*wxString text = "Position: (";
	text << mObject.manyObjectsVector[0].coordinate.GetLocalCoordinatePosition().x;
	text << ", ";
	text << mObject.manyObjectsVector[0].coordinate.GetLocalCoordinatePosition().z;
	text << ", ";
	text << mObject.manyObjectsVector[0].coordinate.GetLocalCoordinatePosition().y;
	
	text << ")";
	(*mTextOututWindow123) << "\n<<" << text;*/	
}
//-------------------------------------------------------------------------------------------------------------------------------------------
//Quaternion to Eulerian Angle
//-------------------------------------------------------------------------------------------------------------------------------------------
void KalenEngine::QuaternionToEulerianAngle(XMVECTOR quaternion, float* pitch, float* roll, float* yaw)
{
	double ysqr = XMVectorGetY(quaternion) * XMVectorGetY(quaternion);

	// roll (x-axis rotation)
	double t0 = +2.0 * (XMVectorGetW(quaternion) * XMVectorGetX(quaternion) + XMVectorGetY(quaternion) * XMVectorGetZ(quaternion));
	double t1 = +1.0 - 2.0 * (XMVectorGetX(quaternion) * XMVectorGetX(quaternion) + ysqr);
	*roll = std::atan2(t0, t1);

	// pitch (y-axis rotation)
	double t2 = +2.0 * (XMVectorGetW(quaternion) * XMVectorGetY(quaternion) - XMVectorGetZ(quaternion) * XMVectorGetX(quaternion));
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	*pitch = std::asin(t2);

	// yaw (z-axis rotation)
	double t3 = +2.0 * (XMVectorGetW(quaternion) * XMVectorGetZ(quaternion) + XMVectorGetX(quaternion) * XMVectorGetY(quaternion));
	double t4 = +1.0 - 2.0 * (ysqr + XMVectorGetZ(quaternion) * XMVectorGetZ(quaternion));
	*yaw = std::atan2(t3, t4);
}

Object* KalenEngine::GetObject()
{
	return mSelectedObject;
}

ID3D11Device* KalenEngine::getDevice()
{
	return this->m_pDevice;
}
TexturesResource* KalenEngine::GetTexturesResource()
{
	return this->mTexturesResource;
}

void KalenEngine::SetSelectedObject(Object* obj)
{
	//if(obj->GetFileName() == "1")
		mSelectedObject = obj;
	//else
	//	mSelectedObject = mObject2;
}
Camera* KalenEngine::GetCamera()
{
	return &mCam;
}

std::vector<Object*>* KalenEngine::GetRenderObjectsBuffer()
{
	return this->mRenderObjectsBuffer;
}

bool KalenEngine::GetIsAddedLight()
{
	return this->mIsAddedLight;
}
void KalenEngine::SetIsAddedLight(bool signal)
{
	this->mIsAddedLight = signal;
}
void KalenEngine::TransformLightSphere(float radius, XMFLOAT4 colour)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	m_pImmediateContext->Map(mVerticesBufferLightSphere, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);

	for (UINT i = 0; i < mLightSphere.Vertices.size(); ++i)
	{
		v[i].Pos.x = mLightSphere.Vertices[i].Pos.x * radius;
		v[i].Pos.y = mLightSphere.Vertices[i].Pos.y * radius;
		v[i].Pos.z = mLightSphere.Vertices[i].Pos.z * radius;
		v[i].Normal = XMFLOAT3(colour.x, colour.y, colour.z);
	}
	m_pImmediateContext->Unmap(mVerticesBufferLightSphere, 0);
}
DirectionalLight* KalenEngine::GetSun()
{
	return &mDirectionalLight;
}
XMMATRIX KalenEngine::GetWorldBaseTransform()
{
	return this->m_worldBaseTransform;
}

void KalenEngine::QuicksortObjects(std::vector<Object*>* objectsBuffer, int p, int r)
{
	int q = -1;

	if (p < r)
	{
		q = PartitionQuicksort(objectsBuffer, p, r);
		QuicksortObjects(objectsBuffer, p, q);
		QuicksortObjects(objectsBuffer, q + 1, r);
	}
}
int KalenEngine::PartitionQuicksort(std::vector<Object*>* objectsBuffer, int p, int r)
{
	Object* obj = new Object();
	obj = objectsBuffer->at(p);

	Object* objSwap = new Object();


	int i = p;
	int j = r;
	int w = -1;

	while (true) 
	{
		while (objectsBuffer->at(j)->GetTempDistance() > obj->GetTempDistance()) 
			j--;
		while (objectsBuffer->at(i)->GetTempDistance() < obj->GetTempDistance())
			i++;

		if (i < j)
		{
			objSwap = objectsBuffer->at(i);
			objectsBuffer->at(i) = objectsBuffer->at(j);
			objectsBuffer->at(j) = objSwap;
			i++;
			j--;
		}
		else
			return j;
	}
	return 0;
}

void KalenEngine::SetLayerBuffer(std::vector<Layer*>* layerBuffer)
{
	this->mLayerBuffer = layerBuffer;
}
//***********************************************************************************************
void KalenEngine::ConvertObjectToObjectStruct(objectStruct& objectStruct, Object* object)
{
	object->GetCoordinate()->UpdateLocalCoordinateMatrix();
	if (object->GetKLMESH() != nullptr)
		objectStruct.linkToFile = object->GetKLMESH()->GetLinkToFile();
	else
		objectStruct.linkToFile = "";
	objectStruct.mIsCastShadow = object->GetIsCastShadow();
	objectStruct.mIsVisible = object->GetIsVisible();
	objectStruct.mLocalAxisX = object->GetCoordinate()->GetLocalAxisX();
	objectStruct.mLocalAxisY = object->GetCoordinate()->GetLocalAxisY();
	objectStruct.mLocalAxisZ = object->GetCoordinate()->GetLocalAxisZ();
	objectStruct.mLocalCoordinatePosition = object->GetCoordinate()->GetLocalCoordinatePosition();
	objectStruct.mPitch = object->GetCoordinate()->GetPitch();
	objectStruct.mRoll = object->GetCoordinate()->GetRoll();
	objectStruct.mYaw = object->GetCoordinate()->GetYaw();
	objectStruct.mScaleVector = object->GetCoordinate()->GetScale();
	objectStruct.mShadowDistance = object->GetShadowDistance();
	objectStruct.name = object->GetFileName();
	objectStruct.tag = object->GetTAG();
	objectStruct.mType = object->GetType();
	if (object->GetType() == 1)//Light
	{
		objectStruct.Ambient = object->GetPointLight()->Ambient;
		objectStruct.Diffuse = object->GetPointLight()->Diffuse;
		objectStruct.Specular = object->GetPointLight()->Specular;
		objectStruct.Position = object->GetPointLight()->Position;
		objectStruct.Att = object->GetPointLight()->Att;
		objectStruct.Range = object->GetPointLight()->Range;
	}
	else //klmesh
	{
		objectStruct.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		objectStruct.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		objectStruct.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		objectStruct.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		objectStruct.Att = XMFLOAT3(0.0f, 0.0f, 0.0f);
		objectStruct.Range = 0.0f;
	}
}
//***********************************************************************************************
void KalenEngine::ConvertObjectStructToObject(Object* object, objectStruct objectStruct)
{

	LocalCoordinateSystem* localSystem = new LocalCoordinateSystem();
	localSystem->SetLocalCoordinatePosition(objectStruct.mLocalCoordinatePosition);
	localSystem->setLocalAxisX(objectStruct.mLocalAxisX);
	localSystem->setLocalAxisY(objectStruct.mLocalAxisY);
	localSystem->setLocalAxisZ(objectStruct.mLocalAxisZ);
	localSystem->SetPitch(objectStruct.mPitch);
	localSystem->SetRoll(objectStruct.mRoll);
	localSystem->SetYaw(objectStruct.mYaw);
	localSystem->SetScale(objectStruct.mScaleVector);
	localSystem->UpdateLocalCoordinateMatrix();
	object->SetCoordinate(*localSystem);

	object->SetFileName(objectStruct.name);
	object->SetIsCastShadow(objectStruct.mIsCastShadow);
	object->SetIsVisible(objectStruct.mIsVisible);
	//object->SetIsVisible(false);
	object->SetShadowDistance(objectStruct.mShadowDistance);
	object->SetTAG(objectStruct.tag);
	object->SetType(objectStruct.mType);

	
	if (objectStruct.mType == 1) //light
	{
		PointLight* lightNew = new PointLight();
		lightNew->Ambient = objectStruct.Ambient;
		lightNew->Diffuse = objectStruct.Diffuse;
		lightNew->Specular = objectStruct.Specular;
		lightNew->Position = objectStruct.Position;
		lightNew->Pad = 0.0f;
		lightNew->Range = objectStruct.Range;
		lightNew->Att = objectStruct.Att;
		object->SetPointLight(lightNew);
	}
	else //klmesh
	{
		object->SetPointLight(nullptr);
	}
}

void KalenEngine::AllowRender(bool signal)
{
	mAllowRender = signal;
}

XMFLOAT3 KalenEngine::GetCubeMapPosition()
{
	return this->mCubeMapPosition;
}
void KalenEngine::SetCubeMapPosition(XMFLOAT3 pos)
{
	this->mCubeMapPosition = pos;
}

//***************************************************************************************
// CubeMapDemo.cpp by Frank Luna (C) 2011
//***************************************************************************************
void KalenEngine::BuildCubeFaceCamera(float x, float y, float z)
{
	// Generate the cube map about the given position.
	XMFLOAT3 center(x, y, z);
	XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);

	// Look along each coordinate axis.
	XMFLOAT3 targets[6] =
	{
		XMFLOAT3(x + 1.0f, y, z), // +X
		XMFLOAT3(x - 1.0f, y, z), // -X
		XMFLOAT3(x, y + 1.0f, z), // +Y
		XMFLOAT3(x, y - 1.0f, z), // -Y
		XMFLOAT3(x, y, z + 1.0f), // +Z
		XMFLOAT3(x, y, z - 1.0f)  // -Z
	};

	// Use world up vector (0,1,0) for all directions except +Y/-Y.  In these cases, we
	// are looking down +Y or -Y, so we need a different "up" vector.
	XMFLOAT3 ups[6] =
	{
		XMFLOAT3(0.0f, 1.0f, 0.0f),  // +X
		XMFLOAT3(0.0f, 1.0f, 0.0f),  // -X
		XMFLOAT3(0.0f, 0.0f, -1.0f), // +Y
		XMFLOAT3(0.0f, 0.0f, +1.0f), // -Y
		XMFLOAT3(0.0f, 1.0f, 0.0f),	 // +Z
		XMFLOAT3(0.0f, 1.0f, 0.0f)	 // -Z
	};

	for (int i = 0; i < 6; ++i)
	{
		mCubeMapCamera[i].LookAt(center, targets[i], ups[i]);
		mCubeMapCamera[i].SetLens(0.5f*XM_PI, 1.0f, 0.1f, 1000.0f);
		mCubeMapCamera[i].UpdateViewMatrix();
	}
}

//***************************************************************************************
// CubeMapDemo.cpp by Frank Luna (C) 2011
//***************************************************************************************
void KalenEngine::BuildDynamicCubeMapViews()
{

	//
	// Cubemap is a special texture array with 6 elements.
	//

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = CubeMapSize;
	texDesc.Height = CubeMapSize;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 6;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* cubeTex = 0;
	m_pDevice->CreateTexture2D(&texDesc, 0, &cubeTex);

	//
	// Create a render target view to each cube map face 
	// (i.e., each element in the texture array).
	// 

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 1;
	rtvDesc.Texture2DArray.MipSlice = 0;

	for (int i = 0; i < 6; ++i)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		m_pDevice->CreateRenderTargetView(cubeTex, &rtvDesc, &mDynamicCubeMapRTV[i]);
	}

	//
	// Create a shader resource view to the cube map.
	//

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	m_pDevice->CreateShaderResourceView(cubeTex, &srvDesc, &mDynamicCubeMapSRV);

	ReleaseCOM(cubeTex);

	//
	// We need a depth texture for rendering the scene into the cubemap
	// that has the same resolution as the cubemap faces.  
	//

	D3D11_TEXTURE2D_DESC depthTexDesc;
	depthTexDesc.Width = CubeMapSize;
	depthTexDesc.Height = CubeMapSize;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;

	ID3D11Texture2D* depthTex = 0;
	m_pDevice->CreateTexture2D(&depthTexDesc, 0, &depthTex);

	// Create the depth stencil view for the entire cube
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = depthTexDesc.Format;
	dsvDesc.Flags = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	m_pDevice->CreateDepthStencilView(depthTex, &dsvDesc, &mDynamicCubeMapDSV);

	ReleaseCOM(depthTex);

	//
	// Viewport for drawing into cubemap.
	// 

	mCubeMapViewport.TopLeftX = 0.0f;
	mCubeMapViewport.TopLeftY = 0.0f;
	mCubeMapViewport.Width = (float)CubeMapSize;
	mCubeMapViewport.Height = (float)CubeMapSize;
	mCubeMapViewport.MinDepth = 0.0f;
	mCubeMapViewport.MaxDepth = 1.0f;
}

Octree* KalenEngine::GetOctree()
{
	return this->mOctree;
}
