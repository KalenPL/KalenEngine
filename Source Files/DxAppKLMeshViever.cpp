#include "../Header Files/DxAppKLMeshViever.h"

extern bool gFilterDebugGrid;
extern bool gFilterRenderMesh;
extern bool gFilterDebugWireframe;
extern bool gFilterRenderTessellation;
extern bool gFilterRenderRefraction;

BEGIN_EVENT_TABLE(DxAppKLMeshViever, wxPanel)
	EVT_SIZE(DxAppKLMeshViever::OnSize)
	EVT_PAINT(DxAppKLMeshViever::OnPaint)
	EVT_ERASE_BACKGROUND(DxAppKLMeshViever::OnEraseBackground)
	EVT_IDLE(DxAppKLMeshViever::OnIdle)
	EVT_LEFT_DOWN(DxAppKLMeshViever::OnMouseLeftDown)
	EVT_LEFT_UP(DxAppKLMeshViever::OnMouseLeftUp)
	EVT_RIGHT_DOWN(DxAppKLMeshViever::OnMouseRightDown)
	EVT_RIGHT_UP(DxAppKLMeshViever::OnMouseRightUp)
	EVT_MOTION(DxAppKLMeshViever::OnMouseMotion)
	EVT_KEY_DOWN(DxAppKLMeshViever::OnKeyDown)
	EVT_KEY_UP(DxAppKLMeshViever::OnKeyUp)
END_EVENT_TABLE()

//**********************************************************************************************************************************************************************************
DxAppKLMeshViever::DxAppKLMeshViever(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{

	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pRenderTargetView = nullptr;
	m_pSwapChain2 = nullptr;
	g_driverType = D3D_DRIVER_TYPE_NULL;
	mDepthStencilBuffer = nullptr;
	mDepthStencilView = nullptr;
	//Get width and height of window
	GetClientSize(&m_ClientWidth, &m_ClientHeight);
	mWindowDirectXD = (HWND)GetHWND(); //Get HWND from this window

	mInputLayout = nullptr;
	//mInputLayoutShadow = nullptr;

	g_pVertexBufferGrid = nullptr;

	mFX = nullptr;
	mTech = nullptr;
	mFX_pWorldViewProjection = nullptr;
	mFX_pWorld = nullptr;
	mFX_pWorldInvTranspose = nullptr;
	mFX_pTexTransform = nullptr;
	mFX_pShadowTransform = nullptr;

	mFX_pDirectionalLight = nullptr;
	mFX_pEyePosition = nullptr;
	mFX_pPointLight = nullptr;
	mFX_pMaterial = nullptr;

	mFX_pDiffuseMap = nullptr;

	g_World = XMMatrixIdentity();
	g_View = XMMatrixIdentity();
	g_Projection = XMMatrixIdentity();
	g_WorldViewProjection = XMMatrixIdentity();
	g_WorldInvTranspose = XMMatrixIdentity();
	m_worldBaseTransform = XMMatrixIdentity();
	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mKLMESH = nullptr;
	mTexturesResource = nullptr;

	mColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mColorSpecular = XMFLOAT3(1.0f, 1.0f, 1.0f);

	mCam = new Camera();

	//mCam.SetPosition(50.0f, 50.0f, 50.0f);
	//mCam->UpdateViewMatrix();


	mCam->SetPosition(XMFLOAT3(0.0f, 0.0f, 2.0f));

	mLocalCoordinateSystem = new LocalCoordinateSystem();

	/*XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCam->LookAt(Eye, At, Up);
	mCam->UpdateViewMatrix();*/

}
//**********************************************************************************************************************************************************************************
DxAppKLMeshViever::~DxAppKLMeshViever()
{
	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain2);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);

	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);

	Memory::SafeRelease(mDepthStencilBuffer);
	Memory::SafeRelease(mRenderTargetView);


	//Memory::SafeDelete(mKLMESH);
	//Memory::SafeDelete(mTexturesResource);


	/*ReleaseCOM(mFX);
	Memory::SafeDelete(mTech);
	Memory::SafeDelete(mFX_pWorldViewProjection);
	Memory::SafeDelete(mFX_pWorld);
	Memory::SafeDelete(mFX_pWorldInvTranspose);
	Memory::SafeDelete(mFX_pTexTransform);
	Memory::SafeDelete(mFX_pShadowTransform);
	Memory::SafeDelete(mFX_pEyePosition);
	Memory::SafeDelete(mFX_pPointLight);
	Memory::SafeDelete(mFX_pMaterial);
	Memory::SafeDelete(mFX_pDiffuseMap);
	Memory::SafeDelete(mFX_pNormalMap);
	Memory::SafeDelete(mFX_pASHMap);
	Memory::SafeDelete(mFX_pShadowMap);
	Memory::SafeDelete(mFX_AllowDiffuse);
	Memory::SafeDelete(mFX_AllowNormal);
	Memory::SafeDelete(mFX_AllowSpecular);
	Memory::SafeDelete(mFX_AllowAlpha);
	Memory::SafeDelete(mFX_SetLineColor);
	Memory::SafeDelete(mFXTess);
	Memory::SafeDelete(mTechTess);
	Memory::SafeDelete(mWireframeRS);
	Memory::SafeDelete(mSolidframeRS);
	Memory::SafeDelete(mWireThroughFrameRS);
	Memory::SafeDelete(mTwoSideTriangleFrameRS);
	Memory::SafeDelete(mInputLayout);
	Memory::SafeDelete(g_pVertexBufferGrid);
	Memory::SafeDelete(pVSBlob);
	Memory::SafeDelete(pVSBlobError);

	;*/

}
//**********************************************************************************************************************************************************************************
bool DxAppKLMeshViever::Init()
{
	mTimer.Reset(); //Reset time (FPS)
	mAppPaused = false;

	if (InitDirect3D() == false)
	{
		wxMessageBox("Cannot initialize DirectX!", "Error", wxICON_ERROR | wxOK);
		return false;
	}
	if (ReadShaderFromBinaryFile() == false)
	{
		wxMessageBox("Cannot read shader!", "Error", wxICON_ERROR | wxOK);
		return false;
	}

	BuildVertexLayout();


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

	mFX_pWorld = mFX->GetVariableByName("World")->AsMatrix();
	mFX_pWorldInvTranspose = mFX->GetVariableByName("WorldInvTranspose")->AsMatrix();
	mFX_pTexTransform = mFX->GetVariableByName("TexTransform")->AsMatrix();
	mFX_pShadowTransform = mFX->GetVariableByName("gShadowTransform")->AsMatrix();


	mFX_pEyePosition = mFX->GetVariableByName("gEyePosW")->AsVector();
	mFX_pPointLight = mFX->GetVariableByName("gPointLight");
	mFX_pMaterial = mFX->GetVariableByName("gMaterial");
	mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");

	mFX_pDirectionalLight = mFX->GetVariableByName("gDirectionalLight");
	mFX_pDiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mFX_pNormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	mFX_pASHMap = mFX->GetVariableByName("gASHMap")->AsShaderResource();
	mFX_pCubeMap = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	mFX_pShadowMap = mFX->GetVariableByName("gShadowMap")->AsShaderResource(); //set mShadow->DepthMapSRV()

	mFX_AllowDiffuse = mFX->GetVariableByName("__allowDiffuse")->AsScalar();
	mFX_AllowNormal = mFX->GetVariableByName("__allowNormal")->AsScalar();
	mFX_AllowSpecular = mFX->GetVariableByName("__allowSpecular")->AsScalar();
	mFX_AllowCubeMap = mFX->GetVariableByName("__allowCubeMap")->AsScalar();
	mFX_AllowShadowMap = mFX->GetVariableByName("__allowShadowMap")->AsScalar();

	mFX_AllowAlpha = mFX->GetVariableByName("__allowAlpha")->AsScalar();
	mFX_SetLineColor = mFX->GetVariableByName("__color")->AsScalar();
	mFX_SetLineColor->SetInt(0);

	mFX_Transparency = mFX->GetVariableByName("__transparency")->AsScalar();
	mFX_Transparency->SetFloat(1.0f);









	mFX_AllowDiffuse->SetBool(true);
	mFX_AllowNormal->SetBool(true);
	mFX_AllowSpecular->SetBool(true);
	mFX_AllowCubeMap->SetBool(true);
	mFX_AllowShadowMap->SetBool(false);

	//Build functions



	mesh.CreateGridWorld(6, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), Grid);

	BuildScene();






	/*mPointLight[0].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPointLight[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPointLight[0].Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 128.0f);
	mPointLight[0].Att = XMFLOAT3(0.0f, 0.000f, 0.001f);
	mPointLight[0].Range = 5000.0f;
	//	mPointLight[0].Range = 0.0f;
	mPointLight[0].Position = XMFLOAT3(200.0f, 200.0f, 200.0f);



	mPointLight[1].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPointLight[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPointLight[1].Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 128.0f);
	mPointLight[1].Att = XMFLOAT3(0.00f, 0.00f, 0.001f);
	mPointLight[1].Range = 5000.0f;
	//	mPointLight[1].Range = 0.0f;
	mPointLight[1].Position = XMFLOAT3(50.0f, 50.0f, 50.0f);
	*/

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






	mDirectionalLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirectionalLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectionalLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirectionalLight.Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);





	/*XMVECTOR Eye = XMVectorSet(150.0f, 100.0f, 250.0f, 0.0f); //+50 
	XMVECTOR At = XMVectorSet(0.0f, 50.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mCam.LookAt(Eye, At, Up);
	*/



	return true;
}
//**********************************************************************************************************************************************************************************
bool DxAppKLMeshViever::InitDirect3D()
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG

	//DRIVER_TYPES
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//FEATURE_LEVELS
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));
	swapDesc.BufferCount = 1; //double bufered
	swapDesc.BufferDesc.Width = m_ClientWidth;//
	swapDesc.BufferDesc.Height = m_ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = mWindowDirectXD;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; 
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;



	HRESULT result;
	for (int i = 0; i < (int)numDriverTypes; ++i)
	{
		g_driverType = driverTypes[i];
		result = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain2, &m_pDevice, &m_FeatureLevel, &m_pImmediateContext);

		if (SUCCEEDED(result))
		{
			break;
		}
	}


	if (FAILED(result))
	{
		OutputDebugString(L"FAILED TO CREATE DEVICE AND SWAP CHAIN");
		MessageBox(NULL, L"FAILED TO CREATE DEVICE AND SWAP CHAIN", L"ERROR !!!", NULL);
		return false;
	}

	IDXGIDevice* dxgiDevice = 0;
	(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_pDevice, &swapDesc, &m_pSwapChain2);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize();

	return true;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnResize()
{
	//Get new values (width and height)
	GetClientSize(&m_ClientWidth, &m_ClientHeight);

	assert(m_pImmediateContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);

	// Resize the swap chain and recreate the render target view.
	m_pSwapChain2->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* backBuffer;
	m_pSwapChain2->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);
	ReleaseCOM(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_ClientWidth;
	depthStencilDesc.Height = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	m_pDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, mDepthStencilView);

	//Viewport
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = (FLOAT)m_ClientWidth;
	m_Viewport.Height = (FLOAT)m_ClientHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::BuildVertexLayout()
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

	mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");

	D3DX11_PASS_SHADER_DESC VsPassDesc2;
	D3DX11_EFFECT_SHADER_DESC VsDesc;
	mTech->GetPassByIndex(0)->GetVertexShaderDesc(&VsPassDesc2);
	VsPassDesc2.pShaderVariable->GetShaderDesc(VsPassDesc2.ShaderIndex, &VsDesc);
	hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), VsDesc.pBytecode, VsDesc.BytecodeLength, &mInputLayout);
	
	if (FAILED(hr))
	{
		wxMessageBox("Cannot build vertex layout!", "Error", wxICON_ERROR | wxOK);
		PostQuitMessage(0);
	}
}
//**********************************************************************************************************************************************************************************
float DxAppKLMeshViever::AspectRatio()const
{
	return static_cast<float>(m_ClientWidth) / m_ClientHeight;
}
//**********************************************************************************************************************************************************************************
bool DxAppKLMeshViever::ReadShaderFromBinaryFile()
{
	/*HRESULT hr = shaderStandard.CompileShaderFromFileNew(
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
	}*/
	//-------------------------------------
	//Reading shader from binary file
	//-------------------------------------
	int size = 0;
	HRESULT hr;
	std::vector<char> compiledShaderVector = shaderStandard.ReadShaderFromBinaryFileFXO(
		L"fx/FX_Shader.fxo", size);

	hr = D3DX11CreateEffectFromMemory(&compiledShaderVector[0], size, 0, m_pDevice, &mFX);
	if (mFX == nullptr)
	{
		wxMessageBox("Cannot create D3DX11Effect for: 'FX_Shader.fxo'", "Error!", wxICON_ERROR | wxOK);
		return false;
	}
	
	return true;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::BuildScene()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex)* Grid.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = vertices;
	InitData.pSysMem = &Grid.Vertices[0];
	m_pDevice->CreateBuffer(&bd, &InitData, &g_pVertexBufferGrid);
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnKeyDown(wxKeyEvent& evt)
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
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnKeyUp(wxKeyEvent& evt)
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
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnMouseLeftDown(wxMouseEvent& evt)
{
	evt.Skip();
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnMouseLeftUp(wxMouseEvent& evt)
{
	evt.Skip();
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnMouseRightDown(wxMouseEvent& evt)
{
	evt.Skip();
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnMouseRightUp(wxMouseEvent& evt)
{
	evt.Skip();
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnMouseMotion(wxMouseEvent& evt)
{
	float x = evt.GetX();
	float y = evt.GetY();

	mMousePosition.x = x;
	mMousePosition.y = y;
	if (evt.RightIsDown())
	{
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		mCam->Pitch(dy);
		mCam->RotateY(dx);
	}

	if (evt.LeftIsDown())
	{
		float xx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float yy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		mCam->Walk(yy * 50 * (-1));
		mCam->RotateY(xx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnIdle(wxIdleEvent &evt)
{
	evt.RequestMore(true);
	if (HasFocus() == true || mRender == true) //if this window is active!!!
	{
		mTimer.Tick();
		if (!mAppPaused)
		{
			Update(mTimer.DeltaTime());
			Render(0.0f);
		}
		else
		{
			Sleep(100);
		}
		mRender = false;
	}
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetRender(bool signal)
{
	this->mRender = signal;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC dc(this);
	Render(0.0f);
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::OnSize(wxSizeEvent& WXUNUSED(evt))
{
	OnResize();
	mCam->SetLens(0.25f*MathHelper::Pi, AspectRatio(), 0.1f, 10000.0f);
}

void DxAppKLMeshViever::OnEraseBackground(wxEraseEvent& evt)
{
	Render(0.0f);
	evt.Skip();
}
//**********************************************************************************************************************************************************************************
wxWindow* DxAppKLMeshViever::GetWindow()
{
	return GetWindowChild(GetId());
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::Render(float dt)
{
	//wxMessageBox("Wa", "WWW", wxICON_QUESTION | wxOK);
	m_pImmediateContext->RSSetState(0);

	ID3D11RenderTargetView* renderTargets[1] = { m_pRenderTargetView };
	m_pImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthStencilView);

	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::Black)); 

	m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// Set the input layout
	m_pImmediateContext->IASetInputLayout(mInputLayout);
	m_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBufferGrid, &stride, &offset);

	mEyePosW = XMFLOAT3(mCam->GetPosition().x, mCam->GetPosition().y, mCam->GetPosition().z);
//	mFX_pPointLight->SetRawValue(&mPointLight, 0, sizeof(mPointLight));
	mFX_pEyePosition->SetRawValue(&mEyePosW, 0, sizeof(mEyePosW));
	mFX_pDirectionalLight->SetRawValue(&mDirectionalLight, 0, sizeof(mDirectionalLight));


	mCam->UpdateViewMatrix();
	XMMATRIX view = mCam->View();
	XMMATRIX proj = mCam->Proj();

	//Macierz jednostkowa! -- taka, jak dla obiektu
	//mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&m_worldBaseTransform));


	g_WorldInvTranspose = MathHelper::InverseTranspose(m_worldBaseTransform);
	mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));


	g_WorldViewProjection = m_worldBaseTransform * view * proj;
	mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));


	mFX_pTexTransform->SetMatrix(reinterpret_cast<float*>(&mTexTransform));

	mTech = mFX->GetTechniqueByName("FeatureLevel11NORMAL");
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);


	//For grid -- show
	if (gFilterDebugGrid == true)
	{
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		m_pImmediateContext->Draw(Grid.Vertices.size(), 0);
	}

	



	
	//-----------------------------
	//Draw KLMESH
	//-----------------------------
	if (mAllowRender == true)//clock render when object is loading
	if (gFilterRenderMesh == true)
	{
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		mCam->UpdateViewMatrix();
		view = mCam->View();
		proj = mCam->Proj();
		float distance = 0.0;
		XMVECTOR distanceVector;


		m_pImmediateContext->IASetVertexBuffers(0, 1, mKLMESH->GetVertexBuffer(), &stride, &offset);
		m_pImmediateContext->IASetIndexBuffer(mKLMESH->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);

		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//Wylaczenie teselacji
		m_pImmediateContext->HSSetShader(0, 0, 0);
		m_pImmediateContext->DSSetShader(0, 0, 0);


		mLocalCoordinateSystem->UpdateCoordinateMatrix();
		//XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, mLocalCoordinateSystem->GetMatrix());
		XMMATRIX odwrotnoscMacierzyPrzejscia = mLocalCoordinateSystem->GetMatrix();
		mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia));

		g_WorldInvTranspose = MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia);
		mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));

		g_WorldViewProjection = odwrotnoscMacierzyPrzejscia  * view * proj;
		mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
		mFX_pShadowTransform->SetMatrix(reinterpret_cast<const float*>(&XMMatrixMultiply(odwrotnoscMacierzyPrzejscia, XMMatrixIdentity())));

		distanceVector = XMVector3Length((mCam->GetPositionXM()));
		distance = XMVectorGetX(distanceVector);
		if (mForceLODLevel == 0)
		{
			distance = 0.1f;
		}
		if (mForceLODLevel == 1)
		{
			distance = mKLMESH->GetDistanceLOD1() + 1.0f;
		}
		if (mForceLODLevel == 2)
		{
			distance = mKLMESH->GetDistanceLOD2() + 1.0f;
		}

		int matPos = -1;
		if(mKLMESH->GetMeshToDraw(distance) != nullptr)
			for (int j = 0; j < (int)mKLMESH->GetMeshToDraw(distance)->size(); ++j)//rendering chunks
			{
				matPos = mKLMESH->GetMeshToDraw(distance)->at(j)->materialPosition;

				if (mKLMESH->GetMaterialBuffer()->at(matPos).diffuseMapName == "NULL")
					mFX_AllowDiffuse->SetBool(false);
				else
					mFX_AllowDiffuse->SetBool(true);

				if (mKLMESH->GetMaterialBuffer()->at(matPos).ashMapName == "NULL")
					mFX_AllowSpecular->SetBool(false);
				else
					mFX_AllowSpecular->SetBool(true);

				if (mKLMESH->GetMaterialBuffer()->at(matPos).normalMapName == "NULL")
					mFX_AllowNormal->SetBool(false);
				else
					mFX_AllowNormal->SetBool(true);


				//Cubemap render flag
				if (gFilterRenderRefraction == false)
					mFX_AllowCubeMap->SetBool(false);
				else
					mFX_AllowCubeMap->SetBool(true);
				//Object - Cubemap
				if (mKLMESH->GetMaterialBuffer()->at(matPos).cubeMapName == "NULL")
					mFX_AllowCubeMap->SetBool(false);
				else
					if (gFilterRenderRefraction == false)
						mFX_AllowCubeMap->SetBool(false);
					else
						mFX_AllowCubeMap->SetBool(true);




				mFX_pDiffuseMap->SetResource(mKLMESH->GetMaterialBuffer()->at(matPos).diffuseMap);
				mFX_pNormalMap->SetResource(mKLMESH->GetMaterialBuffer()->at(matPos).normalMap);
				mFX_pASHMap->SetResource(mKLMESH->GetMaterialBuffer()->at(matPos).ashMap);
				mFX_pCubeMap->SetResource(mKLMESH->GetMaterialBuffer()->at(matPos).cubeMap);
				mFX_pShadowMap->SetResource(mKLMESH->GetMaterialBuffer()->at(matPos).ashMap);


				if (mIsCheckBoxSelected == true && matPos == mSelectedMaterialPos)
				{

					mFX_AllowSpecular->SetBool(false);
					mSelectedMaterialHlsl = mKLMESH->GetMaterialBuffer()->at(mSelectedMaterialPos).materialHlsl;
					mSelectedMaterialHlsl.Ambient.x = 0.2f;
					mSelectedMaterialHlsl.Ambient.y = 0.9f;
					mSelectedMaterialHlsl.Ambient.z = 0.2f;

					mFX_pMaterial->SetRawValue(&mSelectedMaterialHlsl, 0, sizeof(mSelectedMaterialHlsl));
				}
				else
				{
					mFX_pMaterial->SetRawValue(&mKLMESH->GetMaterialBuffer()->at(matPos).materialHlsl, 0, sizeof(mKLMESH->GetMaterialBuffer()->at(matPos).materialHlsl));
				}
			
				//Transparency
				mFX_Transparency->SetFloat(mKLMESH->GetMaterialBuffer()->at(matPos).transparency);

				if (mKLMESH->GetMaterialBuffer()->at(matPos).shaderName == "Phong")
					mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");
				else if (mKLMESH->GetMaterialBuffer()->at(matPos).shaderName == "Phong Transparency")
					mTech = mFX->GetTechniqueByName("FeatureLevel11PhongTransparency");
				else if (mKLMESH->GetMaterialBuffer()->at(matPos).shaderName == "Phong Tessellation")
				{
					if (gFilterRenderTessellation == true)
					{
						mTech = mFX->GetTechniqueByName("FeatureLevel11PhongTessellation");
						m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
						g_WorldViewProjection = mCam->ViewProj();
						mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
					}
					else
						mTech = mFX->GetTechniqueByName("FeatureLevel11Phong");
				}
				else if (mKLMESH->GetMaterialBuffer()->at(matPos).shaderName == "Cook-Torrance Transparency")
					mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorranceTransparency");
				else if (mKLMESH->GetMaterialBuffer()->at(matPos).shaderName == "Cook-Torrance Tessellation")
				{
					if (gFilterRenderTessellation == true)
					{
						mTech = mFX->GetTechniqueByName("FeatureLevel11Tessellation");
						m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
						g_WorldViewProjection = mCam->ViewProj();
						mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
					}
					else
						mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");
				}
				else
					mTech = mFX->GetTechniqueByName("FeatureLevel11CookTorrance");

				//Wireframe
				if(gFilterDebugWireframe == true)
					m_pImmediateContext->RSSetState(mWireframeRS);
				else
					m_pImmediateContext->RSSetState(mSolidframeRS);

				//Update
				mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

				//Draw
				if (mKLMESH != nullptr)
					m_pImmediateContext->DrawIndexed(mKLMESH->GetMeshToDraw(distance)->at(j)->numberOfIndicesToDraw,
						mKLMESH->GetMeshToDraw(distance)->at(j)->startIndex,
						mKLMESH->GetMeshToDraw(distance)->at(j)->startVertex);


				//Wylaczenie teselacji
				m_pImmediateContext->HSSetShader(0, 0, 0);
				m_pImmediateContext->DSSetShader(0, 0, 0);
			}
	}

	//-----------------------------
	//Draw BoundingBox
	//-----------------------------
	if (mBoundingBoxDebug)
	{

		g_WorldViewProjection = mCam->ViewProj();
		mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));
		//Draw boundingBox
		RebuildBoundingBox();//rebuild
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		m_pImmediateContext->IASetVertexBuffers(0, 1, mKLMESH->GetBBoxVertexBuffer(), &stride, &offset);
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
		mFX_SetLineColor->SetInt(5); //Yellow colour
		mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
		m_pImmediateContext->Draw(24, 0);
	}
	
	//-----------------------------
	//Draw collisionMesh
	//-----------------------------
	if(mCollisionDebug == true)
		DrawCollisionMesh();
		

	//-----------------------------
	//Swap buffers
	//-----------------------------
	m_pImmediateContext->RSSetState(0);
	m_pImmediateContext->OMSetDepthStencilState(0, 0);

	// Odwi¹¿ mapê cieni jako wejœcie shadera, poniewa¿ wyrenderujemy do niej nastêpn¹ klatkê.
	// Cieñ mo¿e znajdowaæ siê w dowolnym gnieŸdzie, trzeba zatem wyczyœciæ wszystkie.
	ID3D11ShaderResourceView* nullSRV[16] = { 0 };
	m_pImmediateContext->PSSetShaderResources(0, 16, nullSRV);
	m_pSwapChain2->Present(0, 0);
	//m_pSwapChain2->Present(1, 0); //lock 60 fps
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::DrawCollisionMesh()
{
	//m_pVerticesBufferCollision
	mCam->UpdateViewMatrix();
	XMMATRIX view = mCam->View();
	XMMATRIX proj = mCam->Proj();

	m_pImmediateContext->IASetVertexBuffers(0, 1, mKLMESH->GetVertexBufferCollsion(), &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mKLMESH->GetIndexBufferCollsion(), DXGI_FORMAT_R16_UINT, 0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//TESSELLATION OFF
	m_pImmediateContext->HSSetShader(0, 0, 0);
	m_pImmediateContext->DSSetShader(0, 0, 0);

	mTech = mFX->GetTechniqueByName("FeatureLevel11COLLISION_RENDER");

	//mLocalCoordinateSystem->UpdateCoordinateMatrix();
	//XMMATRIX macierzPrzejscia2 = XMMatrixInverse(nullptr, mLocalCoordinateSystem->GetMatrix());
	XMMATRIX macierzPrzejscia2 = mLocalCoordinateSystem->GetMatrix();

	mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&macierzPrzejscia2));
	g_WorldInvTranspose = MathHelper::InverseTranspose(macierzPrzejscia2);
	mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&g_WorldInvTranspose));

	g_WorldViewProjection = macierzPrzejscia2  * view * proj;
	mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&g_WorldViewProjection));

	m_pImmediateContext->RSSetState(mSolidframeRS);
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(mKLMESH->GetMeshCollision().Indices.size(), 0, 0);

	mTech = mFX->GetTechniqueByName("FeatureLevel11RedWireFrame");
	m_pImmediateContext->RSSetState(mWireframeRS);
	mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(mKLMESH->GetMeshCollision().Indices.size(), 0, 0);
}

//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::Update(float dt)
{

	bool rotate = false;
	float normalSpeed = 100.0f;
	float shiftPower = 4.0f;

	int gizmoTransformType = -1;

	if (mKeyPressW)
		if (mKeyPressShift)
			mCam->Walk(normalSpeed * shiftPower * dt);
		else
			mCam->Walk(normalSpeed * dt);

	if (mKeyPressS)
		if (mKeyPressShift)
			mCam->Walk(-normalSpeed * shiftPower * dt);
		else
			mCam->Walk(-normalSpeed * dt);

	if (mKeyPressA)
		if (mKeyPressShift)
			mCam->Strafe(-normalSpeed * shiftPower * dt);
		else
			mCam->Strafe(-normalSpeed * dt);

	if (mKeyPressD)
		if (mKeyPressShift)
			mCam->Strafe(normalSpeed * shiftPower * dt);
		else
			mCam->Strafe(normalSpeed * dt);

	if (mKeyPressQ)
		if (mKeyPressShift)
			mCam->WalkUP(normalSpeed * shiftPower * dt);
		else
			mCam->WalkUP(normalSpeed * dt);

	if (mKeyPressE)
		if (mKeyPressShift)
			mCam->WalkUP(-normalSpeed * shiftPower * dt);
		else
			mCam->WalkUP(-normalSpeed * dt);
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		mFPS = (float)frameCnt;

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
//**********************************************************************************************************************************************************************************	
void DxAppKLMeshViever::SetKLMESH(KLMESH* klmesh)
{
	this->mKLMESH = klmesh;
}
//**********************************************************************************************************************************************************************************
KLMESH* DxAppKLMeshViever::CreateKLMESH_ToRender(klmeshFileStruct* klmeshStruct)
{
	mKLMESH = new KLMESH(klmeshStruct, m_pDevice);
	mKLMESH->CreateBufferVertexAndIndex(m_pDevice);
	//mTexturesResource = new TexturesResource(m_pDevice);

	XMFLOAT3 boundingBoxMax = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 boundingBoxMin = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMStoreFloat3(&boundingBoxMax, mKLMESH->GetBoundingBoxMax());
	XMStoreFloat3(&boundingBoxMin, mKLMESH->GetBoundingBoxMin());

	float radius = max(max(abs(boundingBoxMax.x) * 4.0f, abs(boundingBoxMin.x) * 4.0f), max(abs(boundingBoxMax.z) * 4.0f, abs(boundingBoxMin.z) * 4.0f));
	float x = radius * cos(XMConvertToRadians(0.0f));


	float y = boundingBoxMax.y + ((boundingBoxMax.y + boundingBoxMin.y) * 2.0f);
	float z = radius * sin(XMConvertToRadians(0.0f));
	//mPointLight[1].Position = XMFLOAT3(x, y, z);

	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(-x, -y, -z))));
	mDirectionalLight.Direction = dir;



	return mKLMESH;
}
//**********************************************************************************************************************************************************************************
TexturesResource* DxAppKLMeshViever::CreateTexturesResourceToRender()
{
	mTexturesResource = new TexturesResource(m_pDevice);
	return mTexturesResource;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetLightPosition(XMFLOAT3 pos)
{
	//mPointLight[1].Position = pos;

	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(-pos.x, -pos.y, -pos.z))));
	mDirectionalLight.Direction = dir;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetLightColorR(int value)
{
	mColor.x = (float)value / 255.0f;
	//mPointLight[1].Diffuse.x = (float)value / 255.0f;
	mDirectionalLight.Diffuse.x = (float)value / 255.0f;
}
void DxAppKLMeshViever::SetLightColorG(int value)
{
	mColor.y = (float)value / 255.0f;
	//mPointLight[1].Diffuse.y = (float)value / 255.0f;
	mDirectionalLight.Diffuse.y = (float)value / 255.0f;
}
void DxAppKLMeshViever::SetLightColorB(int value)
{
	mColor.z = (float)value / 255.0f;
	//mPointLight[1].Diffuse.z = (float)value / 255.0f;
	mDirectionalLight.Diffuse.z = (float)value / 255.0f;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetLightColorSpecR(int value)
{
	mColorSpecular.x = (float)value / 255.0f;
	//mPointLight[1].Specular.x = (float)value / 255.0f;
	mDirectionalLight.Specular.x = (float)value / 255.0f;
}
void DxAppKLMeshViever::SetLightColorSpecG(int value)
{
	mColorSpecular.y = (float)value / 255.0f;
	//mPointLight[1].Specular.y = (float)value / 255.0f;
	mDirectionalLight.Specular.y = (float)value / 255.0f;
}
void DxAppKLMeshViever::SetLightColorSpecB(int value)
{
	mColorSpecular.z = (float)value / 255.0f;
	//mPointLight[1].Specular.z = (float)value / 255.0f;
	mDirectionalLight.Specular.z = (float)value / 255.0f;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetLightPower(int value)
{
	float power = (float)value / 100.0f;
	//mPointLight[1].Diffuse.x = mColor.x * power;
	//mPointLight[1].Diffuse.y = mColor.y * power;
	//mPointLight[1].Diffuse.z = mColor.z * power;

	mDirectionalLight.Diffuse.x = mColor.x * power;
	mDirectionalLight.Diffuse.y = mColor.y * power;
	mDirectionalLight.Diffuse.z = mColor.z * power;
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetLightSpec(int value)
{
	float power = (float)value / 100.0f;
	//mPointLight[1].Specular.x = mColorSpecular.x * power;
	//mPointLight[1].Specular.y = mColorSpecular.y * power;
	//mPointLight[1].Specular.z = mColorSpecular.z * power;

	mDirectionalLight.Specular.x = mColorSpecular.x * power;
	mDirectionalLight.Specular.y = mColorSpecular.y * power;
	mDirectionalLight.Specular.z = mColorSpecular.z * power;
	
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetRotation(int value)
{
	mLocalCoordinateSystem->SetMatrixRotationRollPitchYaw(0.0f, (float)value, 0.0f);
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::SetCameraView()
{
	XMFLOAT3 vec;

	XMStoreFloat3(&vec, mKLMESH->GetBoundingBoxMax());
	XMVECTOR Eye = XMVectorSet(-vec.x * 1.5f, vec.y * 1.5f, -vec.z * 1.5f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	mCam->LookAt(Eye, At, Up);
	mCam->UpdateViewMatrix();
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::RebuildBoundingBox()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	m_pImmediateContext->Map(*mKLMESH->GetBBoxVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);

	XMFLOAT3 vectorMin = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vectorMax = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	XMVECTOR rotation;
	XMVECTOR translation;
	XMVECTOR scale;

	//XMMatrixInverse(nullptr, object->GetCoordinate()->GetMatrix());
	//XMMatrixDecompose(&temp, &rotation, &temp, object->GetCoordinate()->GetMatrix());
	XMMatrixDecompose(&scale, &rotation, &translation, mLocalCoordinateSystem->GetMatrix());
	//	temp = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));

	/*XMStoreFloat3(&vectorMax, scale);
	vectorMax.x = 1.0f / vectorMax.x;
	vectorMax.y = 1.0f / vectorMax.y;
	vectorMax.z = 1.0f / vectorMax.z;

	scale = XMLoadFloat3(&vectorMax);*/

	//vectorMax = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//Counting new max and min vector of this mesh
	AxisAlignedBox AABBTemp;
	TransformAxisAlignedBox(&AABBTemp, mKLMESH->GetAxisAlignedBoxOrigin(), 1.0f, scale, rotation, translation);

	vectorMax.x = AABBTemp.Center.x + AABBTemp.Extents.x;
	vectorMax.y = AABBTemp.Center.y + AABBTemp.Extents.y;
	vectorMax.z = AABBTemp.Center.z + AABBTemp.Extents.z;

	vectorMin.x = AABBTemp.Center.x - AABBTemp.Extents.x;
	vectorMin.y = AABBTemp.Center.y - AABBTemp.Extents.y;
	vectorMin.z = AABBTemp.Center.z - AABBTemp.Extents.z;
	
	v[0].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	v[1].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);

	v[2].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	v[3].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);

	v[4].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	v[5].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);

	v[6].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	v[7].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);

	v[8].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMin.z);
	v[9].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);

	v[10].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMin.z);
	v[11].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);

	v[12].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMin.z);
	v[13].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);

	v[14].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMin.z);
	v[15].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);

	v[16].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);
	v[17].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);

	v[18].Pos = XMFLOAT3(vectorMax.x, vectorMin.y, vectorMax.z);
	v[19].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);

	v[20].Pos = XMFLOAT3(vectorMax.x, vectorMax.y, vectorMax.z);
	v[21].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);

	v[22].Pos = XMFLOAT3(vectorMin.x, vectorMax.y, vectorMax.z);
	v[23].Pos = XMFLOAT3(vectorMin.x, vectorMin.y, vectorMax.z);

	m_pImmediateContext->Unmap(*mKLMESH->GetBBoxVertexBuffer(), 0);
}
//**********************************************************************************************************************************************************************************
void DxAppKLMeshViever::ShowBoundingBox(bool signal)
{
	this->mBoundingBoxDebug = signal;
}
void DxAppKLMeshViever::ShowCollision(bool signal)
{
	this->mCollisionDebug = signal;
}
void DxAppKLMeshViever::SetForceLODLevel(int LOD)
{
	this->mForceLODLevel = LOD;
}
void DxAppKLMeshViever::SetSelectedMaterialPos(int pos)
{
	this->mSelectedMaterialPos = pos;
}
void DxAppKLMeshViever::SetIsCheckBoxSelected(bool signal)
{
	this->mIsCheckBoxSelected = signal;
}
void DxAppKLMeshViever::SetAllowRender(bool signal)
{
	this->mAllowRender = signal;
}
