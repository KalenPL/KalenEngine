#ifndef GIZMO_H
#define GIZMO_H

#include "Utility.h"
#include "LocalCoordinateSystem.h"
#include "Mesh.h"
#include "Camera.h"
#include "../XNACollision/xnacollision.h"



//------------------------------------------------------------------------------------------
// Class Gizmo
//------------------------------------------------------------------------------------------
class Gizmo
{
public:
	Gizmo(ID3D11DeviceContext* immediateContext, Mesh* mesh, ID3D11Device* m_pDevice);
	~Gizmo();
	void Draw(Camera* camera, LocalCoordinateSystem* localObjectCoordinateSystem, ID3DX11EffectMatrixVariable* mFX_pWorld, ID3DX11EffectMatrixVariable* mFX_pWorldViewProjection, 
		ID3DX11EffectTechnique* mTech, ID3DX11Effect* mFX, ID3D11DepthStencilView* mDepthStencilView, ID3DX11EffectScalarVariable* mFX_SetLineColor, ID3DX11EffectMatrixVariable* mFX_pWorldInvTranspose, int w, int h);
	void DetectRayCollision(Camera* camera, wxPoint* mousePosition, XMMATRIX* worldBaseTransform, int m_ClientWidth, int m_ClientHeight);
	int GetGizmoSelected();

	XMFLOAT2 GetGizmoStartPoint2D();
	XMFLOAT2 GetGizmoAxisXPoint2D();
	XMFLOAT2 GetGizmoAxisYPoint2D();
	XMFLOAT2 GetGizmoAxisZPoint2D();

	XMFLOAT2 GetGizmoAxisXYPoint2D();
	XMFLOAT2 GetGizmoAxisXZPoint2D();
	XMFLOAT2 GetGizmoAxisYZPoint2D();
	XMFLOAT2 GetGizmoAxisXYZPoint2D();

	float GetDistanceToGizmo();
	XMFLOAT3 GetStartPoint();
	XMFLOAT3 GetStartPoint2();
	XMVECTOR GetNormalVectorToPlane();

	void FindNearPointOfRotation(XMFLOAT2 startTransformPoint, Camera* camera, XMMATRIX* worldBaseTransform, int clientWidth, int clientHeight);
	void DrawRotationAngle(int angle);
	void SaveMatrixCircleRotation(LocalCoordinateSystem* localObjectCoordinateSystem);
	void FindStartPointOfTransform(XMFLOAT2 startTransformPoint, Camera* camera, XMMATRIX* worldBaseTransform, int clientWidth, int clientHeight, LocalCoordinateSystem* localObjectCoordinateSystem, int type);



private:
	XMMATRIX mMatrix;
	XMMATRIX mMatrixCircleRotation; //to save matix (use to draw rotation surface) 
	XMFLOAT3 mGizmoPosition;
	//XMFLOAT4 mQuaternion;
	XMVECTOR mQuaternionVector;
	ID3D11Buffer*		mVertexBuffer;
	ID3D11Buffer*		mIndicesBuffer;
	MeshData			mMesh;
	UINT mStride = sizeof(Vertex);
	UINT mOffset = 0;
	LocalCoordinateSystem* mLocalObjectCoordinateSystem;
	ID3D11DeviceContext*	m_pImmediateContext;
	float mDistance;
	int mGizmoSelected = 0; 
	int mAngle = 0;

	XMFLOAT3 mStartPoint = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	XMFLOAT3 mStartPoint2 = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	XMVECTOR mNormalVectorToPlane;

	int mStartVertexCircle = -1;

	// 0 - null; 1 - x; 2 - y; 3 - z; 4 - xy; 5 - xz; 6 - yz 
	// 10 - null; 11 - x; 12 - y; 13 - z
	// 30 - null; 31 - x; 32 - y; 33 - z; 34 - xy; 35 - xz; 36 - yz; 37 - xyz

	//Values to XNA_COLLISION	
	OrientedBox mBoxMoveX;		OrientedBox mBoxMoveXSave;
	OrientedBox mBoxMoveY;		OrientedBox mBoxMoveYSave;
	OrientedBox mBoxMoveZ;		OrientedBox mBoxMoveZSave;
	OrientedBox mBoxMoveXY;		OrientedBox mBoxMoveXYSave;
	OrientedBox mBoxMoveXY2;	OrientedBox mBoxMoveXY2Save;
	OrientedBox mBoxMoveXZ;		OrientedBox mBoxMoveXZSave;
	OrientedBox mBoxMoveXZ2;	OrientedBox mBoxMoveXZ2Save;
	OrientedBox mBoxMoveYZ;		OrientedBox mBoxMoveYZSave;
	OrientedBox mBoxMoveYZ2;	OrientedBox mBoxMoveYZ2Save;

	OrientedBox mBoxRotateX;		OrientedBox mBoxRotateXSave;
	OrientedBox mBoxRotateY;		OrientedBox mBoxRotateYSave;
	OrientedBox mBoxRotateZ;		OrientedBox mBoxRotateZSave;

	OrientedBox mBoxScaleX;		OrientedBox mBoxScaleXSave;
	OrientedBox mBoxScaleY;		OrientedBox mBoxScaleYSave;
	OrientedBox mBoxScaleZ;		OrientedBox mBoxScaleZSave;


	XMFLOAT2 mGizmoStartPoint2D;
	XMFLOAT2 mGizmoAxisXPoint2D;
	XMFLOAT2 mGizmoAxisYPoint2D;
	XMFLOAT2 mGizmoAxisZPoint2D;

	XMFLOAT2 mGizmoAxisXYPoint2D;
	XMFLOAT2 mGizmoAxisXZPoint2D;
	XMFLOAT2 mGizmoAxisYZPoint2D;
	XMFLOAT2 mGizmoAxisXYZPoint2D;
	float mDistanceToGizmo;

};
#endif //!GIZMO_H
