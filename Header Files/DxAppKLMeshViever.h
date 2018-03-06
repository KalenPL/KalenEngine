#ifndef DX_APP_KLMESH_VIEVER_H
#define DX_APP_KLMESH_VIEVER_H

#include "Utility.h"
#include "Shader.h"
#include "Camera.h"
#include "KLMESH.h"
#include "TexturesResource.h"
#include "Light.h"
#include "LocalCoordinateSystem.h"



//------------------------------------------------------------------------------------------
// Class DxAppKLMeshViever
//------------------------------------------------------------------------------------------
class DxAppKLMeshViever : public wxPanel
{
public:
	DxAppKLMeshViever(wxWindow* parent, wxWindowID id);
	~DxAppKLMeshViever();

	void OnKeyDown(wxKeyEvent& evt);
	void OnKeyUp(wxKeyEvent& evt);
	void OnMouseLeftDown(wxMouseEvent& evt);
	void OnMouseLeftUp(wxMouseEvent& evt);
	void OnMouseRightDown(wxMouseEvent& evt);
	void OnMouseRightUp(wxMouseEvent& evt);
	void OnMouseMotion(wxMouseEvent& evt);

	wxWindow* GetWindow();
	bool Init();
	bool InitDirect3D();
	void OnResize();
	float AspectRatio()const;
	bool ReadShaderFromBinaryFile();
	void BuildVertexLayout();
	void BuildScene();

	void Render(float dt);
	void Update(float dt);

	//Render functions
	void OnPaint(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void OnEraseBackground(wxEraseEvent& evt);
	void OnIdle(wxIdleEvent& evt);
	void SetKLMESH(KLMESH* klmesh);
	KLMESH* CreateKLMESH_ToRender(klmeshFileStruct* klmeshStruct);
	TexturesResource* CreateTexturesResourceToRender();
	void SetLightPosition(XMFLOAT3 pos);
	void SetLightColorR(int value);
	void SetLightColorG(int value);
	void SetLightColorB(int value);
	void SetLightColorSpecR(int value);
	void SetLightColorSpecG(int value);
	void SetLightColorSpecB(int value);
	void SetLightPower(int value);
	void SetLightSpec(int value);
	void SetRotation(int value);
	void SetCameraView();
	void RebuildBoundingBox();
	void ShowBoundingBox(bool signal);
	void ShowCollision(bool signal);
	void SetForceLODLevel(int LOD);
	void SetSelectedMaterialPos(int pos);
	void SetIsCheckBoxSelected(bool signal);
	void SetRender(bool signal);
	void SetAllowRender(bool signal);
private:

	void DrawCollisionMesh();
	HWND			mWindowDirectXD;
	int mForceLODLevel = -1;
	int mSelectedMaterialPos = 0;
	bool mIsCheckBoxSelected = false;
	MaterialHLSL mSelectedMaterialHlsl;
	bool mRender = true;

	ID3D11Device*					m_pDevice;  // md3dDevice;
	ID3D11DeviceContext*			m_pImmediateContext; // md3dImmediateContext;
	IDXGISwapChain*					m_pSwapChain2;  //mSwapChain;
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

	ID3DX11Effect*  mFX;
	ID3DX11EffectTechnique*  mTech;
	ID3DX11EffectMatrixVariable*         mFX_pWorldViewProjection;
	ID3DX11EffectMatrixVariable*         mFX_pWorld;
	ID3DX11EffectMatrixVariable*		 mFX_pWorldInvTranspose;
	ID3DX11EffectMatrixVariable*		 mFX_pTexTransform;
	ID3DX11EffectMatrixVariable*		 mFX_pShadowTransform;
	ID3DX11EffectVectorVariable*		 mFX_pEyePosition;
	ID3DX11EffectVariable*				 mFX_pDirectionalLight;
	ID3DX11EffectVariable*					mFX_pPointLight;
	ID3DX11EffectVariable*					mFX_pMaterial;
	ID3DX11EffectShaderResourceVariable*	mFX_pDiffuseMap;
	ID3DX11EffectShaderResourceVariable*	mFX_pNormalMap;
	ID3DX11EffectShaderResourceVariable*	mFX_pASHMap;
	ID3DX11EffectShaderResourceVariable*	mFX_pShadowMap;
	ID3DX11EffectShaderResourceVariable*	mFX_pCubeMap;
	ID3DX11EffectScalarVariable*	mFX_AllowDiffuse;
	ID3DX11EffectScalarVariable*	mFX_AllowNormal;
	ID3DX11EffectScalarVariable*	mFX_AllowSpecular;
	ID3DX11EffectScalarVariable*	mFX_AllowCubeMap;
	ID3DX11EffectScalarVariable*	mFX_AllowShadowMap;
	ID3DX11EffectScalarVariable* mFX_AllowAlpha;
	ID3DX11EffectScalarVariable* mFX_SetLineColor;
	ID3DX11EffectScalarVariable* mFX_Transparency;


	ID3DX11Effect*  mFXTess;
	ID3DX11EffectTechnique*  mTechTess;

	Shader shaderStandard;

	//RasterizerState
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidframeRS;
	ID3D11RasterizerState* mWireThroughFrameRS;
	ID3D11RasterizerState* mTwoSideTriangleFrameRS;

	//Layout
	ID3D11InputLayout* mInputLayout;

	//Buffers
	ID3D11Buffer*           g_pVertexBufferGrid;

	ID3DBlob* pVSBlob;
	ID3DBlob* pVSBlobError;

	MeshData Grid;
	Mesh mesh;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	XMFLOAT3 mEyePosW;

	XMMATRIX   g_World;
	XMMATRIX   g_View;
	XMMATRIX   g_Projection;
	XMMATRIX   g_WorldViewProjection;
	XMMATRIX   g_WorldInvTranspose;
	XMMATRIX   m_worldBaseTransform;
	XMFLOAT4X4 mTexTransform;

	XMFLOAT3 mColor;
	XMFLOAT3 mColorSpecular;

	//FPS
	void CalculateFrameStats();
	GameTimer mTimer;
	bool      mAppPaused;
	float mFPS = 0.0; //display FPS in the title of the main window

	Camera* mCam;

	bool mKeyPressA = false;
	bool mKeyPressS = false;
	bool mKeyPressD = false;
	bool mKeyPressQ = false;
	bool mKeyPressW = false;
	bool mKeyPressE = false;
	bool mKeyPressShift = false;

	bool mBoundingBoxDebug = false;
	bool mCollisionDebug = false;
	bool mAllowRender = true;

	wxPoint mMousePosition;
	POINT mLastMousePos;

	int m_ClientWidth = 0;
	int m_ClientHeight = 0;

	PointLight mPointLight[10];
	DirectionalLight mDirectionalLight; //Main light

	KLMESH* mKLMESH;
	TexturesResource* mTexturesResource;
	LocalCoordinateSystem* mLocalCoordinateSystem;

	DECLARE_EVENT_TABLE();
};


#endif //DX_APP_KLMESH_VIEVER_H