#include "../Header Files/DXApp.h"

BEGIN_EVENT_TABLE(DXApp, wxPanel)
	EVT_SIZE(DXApp::OnSize)
	EVT_PAINT(DXApp::OnPaint)
	EVT_ERASE_BACKGROUND(DXApp::OnEraseBackground)
	EVT_IDLE(DXApp::OnIdle)
	EVT_LEFT_DOWN(DXApp::OnMouseLeftDown)
	EVT_LEFT_UP(DXApp::OnMouseLeftUp)
	EVT_RIGHT_DOWN(DXApp::OnMouseRightDown)
	EVT_RIGHT_UP(DXApp::OnMouseRightUp)
	EVT_MOTION(DXApp::OnMouseMotion)
	EVT_KEY_DOWN(DXApp::OnKeyDown)
	EVT_KEY_UP(DXApp::OnKeyUp)
	EVT_MENU(ID_MouseRightMenuSlot1, DXApp::OnMouseMenuCase1)
	EVT_MENU(ID_MouseRightMenuSlot2, DXApp::OnMouseMenuCase2)
	EVT_MENU(ID_MouseRightMenuSlot3, DXApp::OnMouseMenuCase3)
	EVT_MENU(ID_MouseRightMenuSlot4, DXApp::OnMouseMenuCase4)
	EVT_MENU(ID_MouseRightMenuSlot5, DXApp::OnMouseMenuCase5)
END_EVENT_TABLE()




std::vector<float> parametersFloatBufferMat;



//**************************************************************************************************************************************************
// C - tor
//**************************************************************************************************************************************************
DXApp::DXApp(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)//, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTR_MULTIPLE, wxString("DirectX Window"))
{
	//Create window and set into the main frame
	gOutputWindowDirectX = GetWindowChild(GetId());
	mWindowDirectXD = (HWND)GetHWND(); //Get HWND from this window
	//DragAcceptFiles(true);

	//Reset values
	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pRenderTargetView = nullptr;
	m_pSwapChain = nullptr;
	g_driverType = D3D_DRIVER_TYPE_NULL;
	mDepthStencilBuffer = nullptr;
	mDepthStencilView = nullptr;
	mMousePosition = wxPoint(0, 0);

	//Get width and height of window
	GetClientSize(&m_ClientWidth, &m_ClientHeight);
}
//**************************************************************************************************************************************************
// D - tor
//**************************************************************************************************************************************************
DXApp::~DXApp()
{
	//CLEANUP DIRECT3D
	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);

	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


}

//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
bool DXApp::Init()
{
	mTimer.Reset(); //Reset time (FPS)
	mAppPaused = false;
	if (!InitDirect3D())
	{
		return false;
	}
	
	return true;
}


//*************************************************************************************************************************************************
//*************************************************************************************************************************************************
bool DXApp::InitDirect3D()
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
	swapDesc.BufferDesc.Width = m_ClientWidth;// -m_PanelRightWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = mWindowDirectXD;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true; //praca w oknie, false --> fullscreen na starcie.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //atl+enter to fullscreen !
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // tego nie ma w dobrej wersji
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;  // tego nie ma w dobrej wersji



	HRESULT result;
	for (int i = 0; i < (int)numDriverTypes; ++i)
	{
		g_driverType = driverTypes[i];
		result = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, &m_FeatureLevel, &m_pImmediateContext);

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

	dxgiFactory->CreateSwapChain(m_pDevice, &swapDesc, &m_pSwapChain);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize();
	
	return true;
}


//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
void DXApp::OnResize()
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
	m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* backBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
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


//**********************************************************************************************
float DXApp::AspectRatio()const
{
	return static_cast<float>(m_ClientWidth) / m_ClientHeight;
}
//**********************************************************************************************
//FPS
//**********************************************************************************************
void DXApp::CalculateFrameStats()
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
//**********************************************************************************************


