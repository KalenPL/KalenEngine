#ifndef DXAPP_H
#define DXAPP_H



#pragma once  

#include "../Header Files/Utility.h"
#include "../Header Files/Camera.h"
#include "../Header Files/File.h"



extern wxWindow* gOutputWindowDirectX; //Window - place to render - global value 

class DXApp : public wxPanel//wxWindow
{
public:
	enum
	{
		ID_MouseRightMenuSlot1,
		ID_MouseRightMenuSlot2,
		ID_MouseRightMenuSlot3,
		ID_MouseRightMenuSlot4,
		ID_MouseRightMenuSlot5,
	};
	DXApp(wxWindow* parent, wxWindowID id);
	virtual ~DXApp();


	virtual void OnPaint(wxPaintEvent& evt) {}
	virtual void OnSize(wxSizeEvent& evt) { }
	virtual void OnEraseBackground(wxEraseEvent& evt) { }
	virtual void OnIdle(wxIdleEvent& evt) { }
	
	virtual void OnMouseLeftDown(wxMouseEvent& evt) { }
	virtual void OnMouseLeftUp(wxMouseEvent& evt) { }
	virtual void OnMouseRightDown(wxMouseEvent& evt) { }
	virtual void OnMouseRightUp(wxMouseEvent& evt) { }
	virtual void OnMouseMotion(wxMouseEvent& evt) { }
	virtual void OnKeyDown(wxKeyEvent& evt) { }
	virtual void OnKeyUp(wxKeyEvent& evt) { }
	virtual void OnMouseMenuCase1(wxCommandEvent& evt) { }
	virtual void OnMouseMenuCase2(wxCommandEvent& evt) { }
	virtual void OnMouseMenuCase3(wxCommandEvent& evt) { }
	virtual void OnMouseMenuCase4(wxCommandEvent& evt) { }
	virtual void OnMouseMenuCase5(wxCommandEvent& evt) { }


public:
	int m_ClientWidth = 0;
	int m_ClientHeight = 0;

	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual void OnResize();
	float     AspectRatio()const;

	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnLeftMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

	virtual void HomeView(){ }
	virtual void TopView(){ }
	virtual void FrontView(){ }
	virtual void WireView(){ }
	virtual void SolidView(){ }
	virtual void WireFrameSolidView(){}
	virtual void WireThroughView(){}
	virtual void NormalView(){}
	virtual void LockLight() {}
	virtual void SetLocalSpace(){}
	virtual void SetGlobalSpace(){}
	virtual void SetTransformType(UINT){}
	virtual void SetMoveUnit(UINT){}
	virtual void SetRotateDegree(UINT){}
	virtual void ResetRotate(){}
	virtual void SetTextureType(bool, bool, bool){}
	virtual void ShowGrid(){ }
	virtual void ShowAxis(){ }
	virtual void CopyObject(){}
	virtual void DeleteObject(){}
	virtual void SetShowGrid(bool signal) {};
	virtual void SetShowArrow(bool signal) {};
	virtual void SetShowDiffuseMap(bool signal) {};
	virtual void SetShowNormalMap(bool signal) {};
	virtual void SetShowSpecularMap(bool signal) {};
	virtual void SetShowAllTexturesMap(bool signal) {};
	virtual void SetShowLightCamera(bool signal) {};
	virtual void SetShowCastShadow(bool signal) {};
	virtual void SetShowShadowMap(bool signal) {};
	virtual void SetShowTessellation(bool signal) {};
	virtual void SetShowCookTorrace(bool signal) {};
	virtual void SetActualMaterial(int signal) {};
	virtual void SetShowBoundingBox(bool signal) {};
	virtual void SetShowCollision(bool signal) {};
	virtual void SetInputData(std::vector<float> &parametersFloatBuffer) {};
	virtual void SetInputDataMat(std::vector<float> &parametersFloatBuffer) {};
	

protected:

	//*********************************************
	//DirectX11
	HWND			mWindowDirectXD;

	ID3D11Device*					m_pDevice;  // md3dDevice;
	ID3D11DeviceContext*			m_pImmediateContext; // md3dImmediateContext;
	IDXGISwapChain*					m_pSwapChain;  //mSwapChain;
	ID3D11RenderTargetView*			m_pRenderTargetView;  //mRenderTargetView;
	D3D_DRIVER_TYPE					m_DriverType;
	D3D_FEATURE_LEVEL				m_FeatureLevel;
	D3D11_VIEWPORT					m_Viewport;  // mScreenViewport;
	D3D11_VIEWPORT					m_Viewport2;  // mScreenViewport;
	D3D_DRIVER_TYPE         g_driverType;

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;

	UINT      m4xMsaaQuality;


	//Init directx3D
	bool InitDirect3D();

	//FPS
	void CalculateFrameStats();
	GameTimer mTimer;
	bool      mAppPaused;
	float mFPS = 0.0; //display FPS in the title of the main window

	wxPoint mMousePosition;
	DECLARE_EVENT_TABLE();
};

#endif //DXAPP_H