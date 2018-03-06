#ifndef __KALEN_ENGINE__
#define  __KALEN_ENGINE__

#include "DXApp.h"  
#include "Light.h" 
#include "Shader.h"
#include "Mesh.h"
#include "WaitDialog.h"
#include "Material.h"
#include "LocalCoordinateSystem.h"
#include "Object.h"
#include "TexturesResource.h"
#include "File.h"
#include "Shadow.h"
#include "Gizmo.h"
#include "BinaryFileRead.h"
#include "Layer.h"
#include "Octree.h"
#include "Utility.h"

//*******************************************************************************************************************************************
//*******************************************************************************************************************************************


struct BoundingSphere
{
	BoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
	XMFLOAT3 Center;
	float Radius;
};




//************************************************************************************************************************************************
//************************************************************************************************************************************************
class KalenEngine : public DXApp
{
public:


	KalenEngine(wxWindow* parent, wxWindowID id, klmeshFileStruct klmesh);
	~KalenEngine();

	bool Init();
	void Update(float dt) override;
	void Render(float dt) override;
	void OnResize() override;

	void OnPaint(wxPaintEvent& evt) override;
	void OnSize(wxSizeEvent& evt) override;
	void OnEraseBackground(wxEraseEvent& evt) override;
	void OnIdle(wxIdleEvent& evt) override;

	void OnMouseLeftDown(wxMouseEvent& evt) override;
	void OnMouseLeftUp(wxMouseEvent& evt) override;
	void OnMouseRightDown(wxMouseEvent& evt) override;
	void OnMouseRightUp(wxMouseEvent& evt) override;
	void OnMouseMotion(wxMouseEvent& evt) override;
	void OnKeyDown(wxKeyEvent& evt) override;
	void OnKeyUp(wxKeyEvent& evt) override;

	void OnMouseMenuCase1(wxCommandEvent& evt) override;
	void OnMouseMenuCase2(wxCommandEvent& evt) override;
	void OnMouseMenuCase3(wxCommandEvent& evt) override;
	void OnMouseMenuCase4(wxCommandEvent& evt) override;
	void OnMouseMenuCase5(wxCommandEvent& evt) override;

	float GetFPS();
	wxPoint GetMousePosition();

	void BuildShadowTransform();
	void RenderShadowMap();
	void RenderSSAOMap();
	void DrawScreenQuad(); //Place to render textures
	void DrawBoundingBox(Object* object);
	void DrawCollisionMesh(Object* object);

	void HomeView();
	void TopView();
	void FrontView();
	void Pick(UINT x, UINT y);

	void WireView();
	void SolidView();
	void WireFrameSolidView();
	void NormalView();
	void SetMoveUnit(float unit);
	void SetRotateDegree(float degree);
	void SetTextureType(bool, bool, bool);

	void SetShowDiffuseMap(bool signal);
	void SetShowNormalMap(bool signal);
	void SetShowSpecularMap(bool signal);
	void SetShowAllTexturesMap(bool signal);
	void SetShowShadowMap(bool signal);

	void DrawMesh(Camera& camera, bool cubeMap);
	bool ReadShaderFromBinaryFile();
	bool CompileShader();
	Object* GetObject();
	DirectionalLight* GetSun();
	void SetSelectedObject(Object* obj);
	ID3D11Device* getDevice();
	TexturesResource* GetTexturesResource();
	Camera* GetCamera();
	std::vector<Object*>* GetRenderObjectsBuffer();
	bool GetIsAddedLight();
	void SetIsAddedLight(bool signal);
	XMMATRIX GetWorldBaseTransform();
	void CountRay(XMVECTOR *rayDir, XMVECTOR *RayOrigin);
	void CountRay(XMFLOAT3 *pos, int MouseX, int MouseY);
	void RebuildBoundingBox(Object* object);
	void SetLayerBuffer(std::vector<Layer*>* layerBuffer);
	void ConvertObjectToObjectStruct(objectStruct& objectStruct, Object* object);
	void ConvertObjectStructToObject(Object* object, objectStruct objectStruct);
	void AllowRender(bool signal);
	int GetCopyPasteCutDelete();
	void SetCopyPasteCutDelete(int signal);
	XMFLOAT3 GetCubeMapPosition();
	void SetCubeMapPosition(XMFLOAT3 pos);
	Octree* GetOctree();


	bool mKeyPressA = false;
	bool mKeyPressS = false;
	bool mKeyPressD = false;
	bool mKeyPressQ = false;
	bool mKeyPressW = false;
	bool mKeyPressE = false;
	bool mKeyPressShift = false;

private:
	//-----------------------------------
	//Private functions
	//-----------------------------------
	void BuildVertexLayout();
	void BuildScene();
	void WireThroughView();
	void BuildCubeFaceCamera(float x, float y, float z);
	void BuildDynamicCubeMapViews();

	void GizmoMove2(wxPoint mousePosition);

	void GizmoMove(wxPoint mousePosition);
	void GizmoRotate(wxPoint mousePosition);
	void GizmoScale(wxPoint mousePosition);
	void QuaternionToEulerianAngle(XMVECTOR quaternion, float* pitch, float* roll, float* yaw);
	void TransformLightSphere(float radius, XMFLOAT4 colour);

	void QuicksortObjects(std::vector<Object*>* objectsBuffer, int p, int r);
	int PartitionQuicksort(std::vector<Object*>* objectsBuffer, int p, int r);

	//-----------------------------------
	//Private values
	//-----------------------------------
	ID3D11DepthStencilView* mDynamicCubeMapDSV;
	ID3D11RenderTargetView* mDynamicCubeMapRTV[6];
	ID3D11ShaderResourceView* mDynamicCubeMapSRV;
	D3D11_VIEWPORT mCubeMapViewport;
	static const int CubeMapSize = 256;
	Camera mCubeMapCamera[6];
	SsaoEffects mSsaoEffects;
	//Ssao* mSSAO;

	KLMESH* mKlmesh;
	bool mIsAddedLight = false;
	int mCopyPasteCutDelete = -1;
	XMFLOAT3 mCubeMapPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

	TexturesResource* mTexturesResource;

	std::vector<Object*>* mRenderObjectsBuffer;
	std::vector<Object*>* mRenderObjectsTransparencyBuffer;
	std::vector<Layer*>* mLayerBuffer;
	Object* mSelectedObject;

	XMFLOAT2 mStartTransformPoint; //Use to move object
	XMFLOAT2 mCurrentTransformPoint; //Use to move object

	wxLongLong mRightClickTimer = 0.0;
	wxLongLong mLeftClickTimer = 0.0;
	bool mLockLeftClickMove = false;
	bool mAllowRender = true;
	float mAngle = 0.0f;

	Shader shaderWire;
	Shader shaderStandard;
	Mesh mesh;
	File mFile;
	HINSTANCE mhInstance;
	//Camera
	Camera mCam;
	Frustum mCamFrustum;

	//Shadow
	UINT mShadowMapResolution;
	Shadow* mShadow;
	BoundingSphere mSceneBounds;
	XMFLOAT4X4 mLightView;
	XMFLOAT4X4 mLightProj;
	XMFLOAT4X4 mShadowTransform;

	//Value to compile HLSL (FX)
	ID3DBlob* pVSBlob;
	ID3DBlob* pVSBlobError;

	//*******************
	//Effects
	//*******************
	ID3DX11Effect*  mFX;
	ID3DX11EffectTechnique*  mTech;
	ID3DX11EffectMatrixVariable*         mFX_pWorldViewProjection;
	ID3DX11EffectMatrixVariable*         mFX_pWorldViewProjectionTex;
	ID3DX11EffectMatrixVariable*         mFX_pWorld;
	ID3DX11EffectMatrixVariable*		 mFX_pWorldInvTranspose;
	ID3DX11EffectMatrixVariable*		 mFX_pTexTransform;
	ID3DX11EffectMatrixVariable*		 mFX_pShadowTransform;
	ID3DX11EffectVectorVariable* mFX_pEyePosition;
	ID3DX11EffectVariable* mFX_pDirectionalLight;
	ID3DX11EffectVariable* mFX_pPointLight;
	ID3DX11EffectVariable* mFX_pMaterial;
	ID3DX11EffectShaderResourceVariable* mFX_pDiffuseMap;
	ID3DX11EffectShaderResourceVariable* mFX_pNormalMap;
	ID3DX11EffectShaderResourceVariable* mFX_pASHMap;
	ID3DX11EffectShaderResourceVariable* mFX_pShadowMap;
	ID3DX11EffectShaderResourceVariable* mFX_pSSAOMap;
	ID3DX11EffectScalarVariable* mFX_AllowDiffuse;
	ID3DX11EffectScalarVariable* mFX_AllowNormal;
	ID3DX11EffectScalarVariable* mFX_AllowSpecular;
	ID3DX11EffectScalarVariable* mFX_AllowAlpha;
	ID3DX11EffectScalarVariable* mFX_Transparency;
	ID3DX11EffectScalarVariable* mFX_AllowShadowMap;
	ID3DX11EffectScalarVariable* mFX_AllowSSAOMap;


	//*******************
	// Tessellation
	//*******************
	ID3DX11Effect*  mFXTess;
	ID3DX11EffectTechnique*  mTechTess;
	ID3DX11EffectMatrixVariable*         mFXTess_pWorldViewProjection;
	ID3DX11EffectMatrixVariable*         mFXTess_pWorld;
	ID3DX11EffectMatrixVariable*		 mFXTess_pWorldInvTranspose;
	ID3DX11EffectMatrixVariable*		 mFXTess_pTexTransform;
	ID3DX11EffectMatrixVariable*		 mFXTess_pShadowTransform; //______________________________
	ID3DX11EffectVectorVariable* mFXTess_pEyePosition;
	ID3DX11EffectVariable* mFXTess_pPointLight;
	ID3DX11EffectVariable* mFXTess_pMaterial;
	ID3DX11EffectShaderResourceVariable* mFXTess_pDiffuseMap;
	ID3DX11EffectShaderResourceVariable* mFXTess_pNormalMap;
	ID3DX11EffectShaderResourceVariable* mFXTess_pSpecularMap;
	ID3DX11EffectShaderResourceVariable* mFXTess_pHeightMap;
	ID3DX11EffectShaderResourceVariable* mFXTess_pShadowMap;
	ID3DX11EffectShaderResourceVariable* mFX_pCubeMap;
	ID3DX11EffectScalarVariable* mFXTess_AllowDiffuse;
	ID3DX11EffectScalarVariable* mFXTess_AllowNormal;
	ID3DX11EffectScalarVariable* mFXTess_AllowSpecular;
	ID3DX11EffectScalarVariable* mFX_AllowCubeMap;
	ID3DX11EffectScalarVariable* mFX_SetLineColor;


	ID3DX11EffectScalarVariable*  mFXTess_HeightScale;
	ID3DX11EffectScalarVariable*  mFXTess_MaxTessDistance;
	ID3DX11EffectScalarVariable*  mFXTess_MinTessDistance;
	ID3DX11EffectScalarVariable*  mFXTess_MinTessFactor;
	ID3DX11EffectScalarVariable*  mFXTess_MaxTessFactor;

	//*******************
	// Shadow
	//*******************
	ID3DX11Effect*						 mFXShadow;
	ID3DX11EffectTechnique*				 mTechShadow;
	ID3DX11EffectMatrixVariable*         mFXShadow_pWorldViewProjection;
	ID3DX11EffectMatrixVariable*         mFXShadow_pWorld;
	ID3DX11EffectMatrixVariable*		 mFXShadow_pWorldInvTranspose;
	ID3DX11EffectMatrixVariable*         mFXShadow_pViewProjection;
	ID3DX11EffectMatrixVariable*		 mFXShadow_pTexTransform;
	ID3DX11EffectVectorVariable*		 mFXShadow_pEyePosition;
	ID3DX11EffectShaderResourceVariable* mFXShadow_pAlphaMap;

	//*******************
	// Shadow Tessellation
	//*******************
	ID3DX11Effect*						 mFXShadowTess;
	ID3DX11EffectTechnique*				 mTechShadowTess;
	ID3DX11EffectMatrixVariable*         mFXShadowTess_pWorldViewProjection;
	ID3DX11EffectMatrixVariable*         mFXShadowTess_pWorld;
	ID3DX11EffectMatrixVariable*		 mFXShadowTess_pWorldInvTranspose;
	ID3DX11EffectMatrixVariable*         mFXShadowTess_pViewProjection;
	ID3DX11EffectMatrixVariable*		 mFXShadowTess_pTexTransform;
	ID3DX11EffectVectorVariable*		 mFXShadowTess_pEyePosition;
	ID3DX11EffectShaderResourceVariable* mFXShadowTess_pAlphaMap;
	ID3DX11EffectShaderResourceVariable* mFXShadowTess_pNormalMap;
	ID3DX11EffectShaderResourceVariable* mFXShadowTess_pHeightMap;

	ID3DX11EffectScalarVariable*  mFXShadowTess_HeightScale;
	ID3DX11EffectScalarVariable*  mFXShadowTess_MaxTessDistance;
	ID3DX11EffectScalarVariable*  mFXShadowTess_MinTessDistance;
	ID3DX11EffectScalarVariable*  mFXShadowTess_MinTessFactor;
	ID3DX11EffectScalarVariable*  mFXShadowTess_MaxTessFactor;



	//*******************
	// SSAO
	//*******************

	//SSAO_NormalDepthEffect
	ID3DX11Effect*			mFX_mSSAO_ND_NormalDepthTech;
	ID3DX11EffectTechnique* mSSAO_ND_NormalDepthTech;
	ID3DX11EffectTechnique* mSSAO_ND_NormalDepthAlphaClipTech;
	ID3DX11EffectMatrixVariable* mSSAO_ND_WorldView;
	ID3DX11EffectMatrixVariable* mSSAO_ND_WorldInvTransposeView;
	ID3DX11EffectMatrixVariable* mSSAO_ND_WorldViewProj;
	ID3DX11EffectMatrixVariable* mSSAO_ND_TexTransform;
	ID3DX11EffectShaderResourceVariable* mSSAO_ND_DiffuseMap;

	//SSAO
	ID3DX11Effect*			mFX_mSSAO;
	ID3DX11EffectTechnique* SsaoTech;
	ID3DX11EffectMatrixVariable* mSSAO_ViewToTexSpace;
	ID3DX11EffectVectorVariable* mSSAO_OffsetVectors;
	ID3DX11EffectVectorVariable* mSSAO_FrustumCorners;
	ID3DX11EffectScalarVariable* mSSAO_OcclusionRadius;
	ID3DX11EffectScalarVariable* mSSAO_OcclusionFadeStart;
	ID3DX11EffectScalarVariable* mSSAO_OcclusionFadeEnd;
	ID3DX11EffectScalarVariable* mSSAO_SurfaceEpsilon;
	ID3DX11EffectShaderResourceVariable* mSSAO_NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* mSSAO_RandomVecMap;

	//SsaoBlur
	ID3DX11Effect*			mFX_mSSAOBlur;
	ID3DX11EffectTechnique* m_SSAO_BLUR_HorzBlurTech;
	ID3DX11EffectTechnique* m_SSAO_BLUR_VertBlurTech;
	ID3DX11EffectScalarVariable* m_SSAO_BLUR_TexelWidth;
	ID3DX11EffectScalarVariable* m_SSAO_BLUR_TexelHeight;
	ID3DX11EffectShaderResourceVariable* m_SSAO_BLUR_NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* m_SSAO_BLUR_InputImage;






	//LIGHT TEST
	XMFLOAT3 mEyePosW;
	Material mSelectedMaterial;
	MaterialHLSL mSelectedMaterialHlsl;
	DirectionalLight mDirectionalLight; //Main light

	PointLight mPointLight[10];
	XMFLOAT3 mTmpLightPosition;
	int mPointLightsCounter = 0;

	XMFLOAT4X4 mTexTransform;

	//Buffers
	ID3D11Buffer*           g_pVertexBufferGrid;
	ID3D11Buffer*           m_pVerticesBufferQuad;
	ID3D11Buffer*           m_pIndicesBufferQuad;
	ID3D11Buffer*			m_pVerticesBufferBoundingBox;
	ID3D11Buffer*			m_pIndicesBufferBoundingBox;
	ID3D11Buffer*			mVerticesBufferLightSphere;
	ID3D11Buffer*			mIndicesBufferLightSphere;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	//Layout
	ID3D11InputLayout* mInputLayout;
	//ID3D11InputLayout* mInputLayoutShadow;

	//Matrix
	XMMATRIX   g_World;
	XMMATRIX   g_View;
	XMMATRIX   g_Projection;
	XMMATRIX   g_WorldViewProjection;
	XMMATRIX   g_WorldInvTranspose;
	XMMATRIX   m_worldBaseTransform;


	//States
	bool __stop_time;
	bool __show_type_wire;
	bool __show_type_wire_through;
	UINT  __show_type_wire_new;
	bool __normal_view;
	bool __diffuseTexture;
	bool __normalTexture;
	bool __specularTexture;
	float __move_Unit;
	float __rotate_Degree;
	bool __castShadow;
	bool __showShadowMap;


	XMFLOAT2 mGizmoStartPoint2D;
	XMFLOAT2 mGizmoAxisXPoint2D;
	XMFLOAT2 mGizmoAxisYPoint2D;
	XMFLOAT2 mGizmoAxisZPoint2D;
	XMFLOAT2 mGizmoAxisXYPoint2D;
	XMFLOAT2 mGizmoAxisXZPoint2D;
	XMFLOAT2 mGizmoAxisYZPoint2D;
	XMFLOAT2 mGizmoAxisXYZPoint2D;

	XMFLOAT3 mScaleSaveValue = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//Camera mCam;
	POINT mLastMousePos;

	//Mesh
	MeshData mQuadScreen; //place to render shadow map
	MeshData mBoundingBoxMesh;
	MeshData Grid;
	MeshData mLightSphere;
	UINT Grid_Count;
	UINT Grid_Offset;

	//RasterizerState
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidframeRS;
	ID3D11RasterizerState* mWireThroughFrameRS;
	ID3D11RasterizerState* mTwoSideTriangleFrameRS;

	Gizmo* mGizmo;
	Octree* mOctree;
};


#endif // !__KALEN_ENGINE__