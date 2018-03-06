#include "../Header Files/Gizmo.h"

extern bool gFilterDebugGizmo;
extern bool gGlobalSpace;
extern int gTransformGizmoType;

extern wxTextCtrl	*mTextOututWindow123;
//**************************************************************************************************************
// Constructor 
//**************************************************************************************************************


Gizmo::Gizmo(ID3D11DeviceContext* immediateContext, Mesh* mesh, ID3D11Device* m_pDevice)
{
	this->m_pImmediateContext = immediateContext;
	mMesh.Vertices.clear();
	mMesh.Indices.clear();
	mesh->CreateAxisArrows(5.0f, mMesh);

	mMatrixCircleRotation = XMMatrixIdentity();

	mDistance = 0.0f;

	//----------------------
	//Vertices buffer
	//----------------------
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex)* mMesh.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	m_pDevice->CreateBuffer(&vbd, 0, &mVertexBuffer);


	D3D11_BUFFER_DESC bd2;
	ZeroMemory(&bd2, sizeof(bd2));
	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = sizeof(WORD)* mMesh.Indices.size();
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &mMesh.Indices[0];
	m_pDevice->CreateBuffer(&bd2, &InitData, &mIndicesBuffer);


	//----------------------
	//OrientationBoundingBoxes
	//----------------------
	mBoxMoveX.Center = XMFLOAT3(9.0f, 0.0f, 0.0f);//root position
	mBoxMoveX.Extents = XMFLOAT3(8.5f, 1.0f, 1.0f);
	mBoxMoveX.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveXSave = mBoxMoveX;

	mBoxMoveY.Center = XMFLOAT3(0.0f, 9.0f, 0.0f);//root position
	mBoxMoveY.Extents = XMFLOAT3(1.0f, 8.5f, 1.0f);
	mBoxMoveY.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveYSave = mBoxMoveY;

	mBoxMoveZ.Center = XMFLOAT3(0.0f, 0.0f, 9.0f);//root position
	mBoxMoveZ.Extents = XMFLOAT3(1.0f, 1.0f, 8.5f);
	mBoxMoveZ.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveZSave = mBoxMoveZ;

	//XY
	mBoxMoveXY.Center = XMFLOAT3(7.4f, 5.0f, 0.0f);//root position
	mBoxMoveXY.Extents = XMFLOAT3(1.0f, 4.2f, 1.0f);
	mBoxMoveXY.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveXYSave = mBoxMoveXY;

	mBoxMoveXY2.Center = XMFLOAT3(5.0f, 7.4f, 0.0f);//root position
	mBoxMoveXY2.Extents = XMFLOAT3(4.2f, 1.0f, 1.0f);
	mBoxMoveXY2.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveXY2Save = mBoxMoveXY2;

	//XZ
	mBoxMoveXZ.Center = XMFLOAT3(7.4f, 0.0f, 5.0f);//root position
	mBoxMoveXZ.Extents = XMFLOAT3(1.0f, 1.0f, 4.2f);
	mBoxMoveXZ.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveXZSave = mBoxMoveXZ;

	mBoxMoveXZ2.Center = XMFLOAT3(5.0f, 0.0f, 7.4f);//root position
	mBoxMoveXZ2.Extents = XMFLOAT3(4.2f, 1.0f, 1.0f);
	mBoxMoveXZ2.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveXZ2Save = mBoxMoveXZ2;

	//YZ
	mBoxMoveYZ.Center = XMFLOAT3(0.0, 5.0f, 7.4f);//root position
	mBoxMoveYZ.Extents = XMFLOAT3(1.0f, 4.2f, 1.0f);
	mBoxMoveYZ.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveYZSave = mBoxMoveYZ;

	mBoxMoveYZ2.Center = XMFLOAT3(0.0f, 7.4f, 5.0f);//root position
	mBoxMoveYZ2.Extents = XMFLOAT3(1.0f, 1.0f, 4.2f);
	mBoxMoveYZ2.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxMoveYZ2Save = mBoxMoveYZ2;

	//-------- 
	//Rotate gizmo
	//--------
	mBoxRotateX.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);//root position
	mBoxRotateX.Extents = XMFLOAT3(2.0f, 15.0f, 15.0f);
	mBoxRotateX.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxRotateXSave = mBoxRotateX;

	mBoxRotateY.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);//root position
	mBoxRotateY.Extents = XMFLOAT3(15.0f, 2.0f, 15.0f);
	mBoxRotateY.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxRotateYSave = mBoxRotateY;

	mBoxRotateZ.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);//root position
	mBoxRotateZ.Extents = XMFLOAT3(15.0f, 15.0f, 2.0f);
	mBoxRotateZ.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxRotateZSave = mBoxRotateZ;

	//***************
	//Scale
	//***************
	mBoxScaleX.Center = XMFLOAT3(9.0f, 0.0f, 0.0f);//root position
	mBoxScaleX.Extents = XMFLOAT3(8.5f, 1.0f, 1.0f);
	mBoxScaleX.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxScaleXSave = mBoxScaleX;

	mBoxScaleY.Center = XMFLOAT3(0.0f, 9.0f, 0.0f);//root position
	mBoxScaleY.Extents = XMFLOAT3(1.0f, 8.5f, 1.0f);
	mBoxScaleY.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxScaleYSave = mBoxScaleY;

	mBoxScaleZ.Center = XMFLOAT3(0.0f, 0.0f, 9.0f);//root position
	mBoxScaleZ.Extents = XMFLOAT3(1.0f, 1.0f, 8.5f);
	mBoxScaleZ.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mBoxScaleZSave = mBoxScaleZ;




	mMatrix = XMMatrixIdentity();
	mGizmoPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//**************************************************************************************************************
// Destructor
//**************************************************************************************************************
Gizmo::~Gizmo()
{
}
//**************************************************************************************************************
void Gizmo::Draw(Camera* camera, LocalCoordinateSystem* localObjectCoordinateSystem, ID3DX11EffectMatrixVariable* mFX_pWorld, ID3DX11EffectMatrixVariable* mFX_pWorldViewProjection,
	ID3DX11EffectTechnique* mTech, ID3DX11Effect* mFX, ID3D11DepthStencilView* mDepthStencilView, ID3DX11EffectScalarVariable* mFX_SetLineColor, ID3DX11EffectMatrixVariable* mFX_pWorldInvTranspose, int clientWidth, int clientHeight)
{
	camera->UpdateViewMatrix();
	XMMATRIX view = camera->View();
	XMMATRIX proj = camera->Proj();
	XMMATRIX worldViewProjection = XMMatrixIdentity();

	XMMATRIX matLookAtCamera = XMMatrixIdentity();
	XMVECTOR W;
	XMVECTOR U;
	XMVECTOR V;
	int offset = 0;
	wxString text = "";

	int angle = 0;

	XMFLOAT3 point2D;
	//wxString text;

	if (gFilterDebugGizmo == true)
	{
		XMMATRIX macierzPrzejscia2 = XMMatrixIdentity();
		XMMATRIX odwrotnoscMacierzyPrzejscia = XMMatrixInverse(nullptr, localObjectCoordinateSystem->GetMatrixToDrawGizmo());
		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, odwrotnoscMacierzyPrzejscia);

		//--------------------------------------------
		//Quaternion - OrientedBoundingBox
		XMFLOAT4 orientation;


		if (gGlobalSpace)
		{ //global space
			localObjectCoordinateSystem->UpdateGlobalPositionCoordinateMatrix(matrix);
			macierzPrzejscia2 = localObjectCoordinateSystem->GetGlobalPositionMatrix();

			mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&macierzPrzejscia2));
			mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&MathHelper::InverseTranspose(macierzPrzejscia2)));
			worldViewProjection = macierzPrzejscia2 * view * proj;
			mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&worldViewProjection));

			//--------------------------------------------
			//Quaternion - OrientedBoundingBox - GlobalSpace
			mMatrix = XMMatrixIdentity(); //Set identity matrix (global space - rotate)
			mQuaternionVector = XMQuaternionRotationMatrix(XMMatrixInverse(nullptr, mMatrix)); //Inverse object matrix (box -> world)
			orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); //Set value

		}
		else //local space
		{
			localObjectCoordinateSystem->UpdateCoordinateMatrix();
			//localObjectCoordinateSystem->UpdateGlobalPositionCoordinateMatrix(matrix);
			macierzPrzejscia2 = localObjectCoordinateSystem->GetMatrixToDrawGizmo();//ok


			XMMATRIX odwrotnoscMacierzyPrzejscia2 = XMMatrixInverse(nullptr, macierzPrzejscia2);
			mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia2));
			mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&MathHelper::InverseTranspose(odwrotnoscMacierzyPrzejscia2)));
			worldViewProjection = odwrotnoscMacierzyPrzejscia2 * view * proj;
			mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&worldViewProjection));

			//--------------------------------------------
			//Quaternion - OrientedBoundingBox - LocalSpace
			localObjectCoordinateSystem->UpdateLocalCoordinateMatrix(); //Update coordinate matrix 
			mMatrix = localObjectCoordinateSystem->GetMatrixToDrawGizmo(); //Get object matrix
			mQuaternionVector = XMQuaternionRotationMatrix(XMMatrixInverse(nullptr, mMatrix)); //Inverse object matrix (box -> world)
			orientation = XMFLOAT4(XMVectorGetX(mQuaternionVector), XMVectorGetY(mQuaternionVector), XMVectorGetZ(mQuaternionVector), XMVectorGetW(mQuaternionVector)); //Set value

		}


		//Get new position of the Gizmo
		mGizmoPosition = localObjectCoordinateSystem->GetLocalCoordinatePosition();

		//Set new values for OrientedBoundingBoxes (Gizmo type: move, rotate, scale)
		mBoxMoveX.Orientation = orientation;
		mBoxMoveY.Orientation = orientation;
		mBoxMoveZ.Orientation = orientation;
		mBoxMoveXY.Orientation = orientation;
		mBoxMoveXY2.Orientation = orientation;
		mBoxMoveXZ.Orientation = orientation;
		mBoxMoveXZ2.Orientation = orientation;
		mBoxMoveYZ.Orientation = orientation;
		mBoxMoveYZ2.Orientation = orientation;

		mBoxRotateX.Orientation = orientation;
		mBoxRotateY.Orientation = orientation;
		mBoxRotateZ.Orientation = orientation;

		mBoxScaleX.Orientation = orientation;
		mBoxScaleY.Orientation = orientation;
		mBoxScaleZ.Orientation = orientation;

		//--------------------------------------------
		// Cast 3D gizmo to 2D screen (as 2D points)
		//--------------------------------------------
		XMVECTOR vectorPoint = XMVectorSet(0, 0, 0, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoStartPoint2D.x = point2D.x;	mGizmoStartPoint2D.y = point2D.y;
		mDistanceToGizmo = XMVectorGetZ(vectorPoint);

		vectorPoint = XMVectorSet(10, 0, 0, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisXPoint2D.x = point2D.x;	mGizmoAxisXPoint2D.y = point2D.y;

		vectorPoint = XMVectorSet(0, 10, 0, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisYPoint2D.x = point2D.x;	mGizmoAxisYPoint2D.y = point2D.y;

		vectorPoint = XMVectorSet(0, 0, 10, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisZPoint2D.x = point2D.x;	mGizmoAxisZPoint2D.y = point2D.y;

		//*************************************************//XY
		vectorPoint = XMVectorSet(5, 5, 0, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisXYPoint2D.x = point2D.x;	mGizmoAxisXYPoint2D.y = point2D.y;

		//*************************************************//XZ
		vectorPoint = XMVectorSet(5, 0, 5, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisXZPoint2D.x = point2D.x;	mGizmoAxisXZPoint2D.y = point2D.y;

		//*************************************************//YZ
		vectorPoint = XMVectorSet(0, 5, 5, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisYZPoint2D.x = point2D.x;	mGizmoAxisYZPoint2D.y = point2D.y;

		//*************************************************//XYZ
		vectorPoint = XMVectorSet(5, 5, 5, 1);
		vectorPoint = XMVector3Transform(vectorPoint, worldViewProjection);
		point2D = XMFLOAT3(XMVectorGetX(vectorPoint), XMVectorGetY(vectorPoint), XMVectorGetZ(vectorPoint));
		point2D.x /= XMVectorGetW(vectorPoint);
		point2D.y /= XMVectorGetW(vectorPoint);
		point2D.x = ((point2D.x + 1.0f) / 2.0f) * clientWidth;
		point2D.y = ((-point2D.y + 1.0f) / 2.0f) * clientHeight;
		mGizmoAxisXYZPoint2D.x = point2D.x;	mGizmoAxisXYZPoint2D.y = point2D.y;

		//--------------------------------------------
		//Rescale gizmo size
		//--------------------------------------------

		//Clear buffer
		m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//Counting distance between camera and gizmo
		XMVECTOR distanceVector = XMVector3Length((camera->GetPositionXM() - localObjectCoordinateSystem->GetLocalCoordinatePositionVECTOR()));
		float distance = XMVectorGetX(distanceVector);
		distance /= 95;

		mDistance = distance; //Value to scale trinagles used to detect collision with ray. 
		D3D11_MAPPED_SUBRESOURCE mappedData;
		m_pImmediateContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		Vertex* v = reinterpret_cast<Vertex*>(mappedData.pData);
		for (UINT i = 0; i < mMesh.Vertices.size(); ++i)
		{
			v[i].Pos.x = mMesh.Vertices[i].Pos.x * distance;
			v[i].Pos.y = mMesh.Vertices[i].Pos.y * distance;
			v[i].Pos.z = mMesh.Vertices[i].Pos.z * distance;
			v[i].Normal = mMesh.Vertices[i].Normal;
		}
		m_pImmediateContext->Unmap(mVertexBuffer, 0);

		mMatrix.m[3][0] = 0.0f;
		mMatrix.m[3][1] = 0.0f;
		mMatrix.m[3][2] = 0.0f;


		XMVECTOR vect;
		if (gTransformGizmoType == 1) //move gizmo case
		{
			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveXSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveX.Extents.x = mBoxMoveXSave.Extents.x * distance;
			mBoxMoveX.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveX.Extents.y = mBoxMoveXSave.Extents.y * distance;
			mBoxMoveX.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveX.Extents.z = mBoxMoveXSave.Extents.z * distance;
			mBoxMoveX.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveYSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveY.Extents.x = mBoxMoveYSave.Extents.x * distance;
			mBoxMoveY.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveY.Extents.y = mBoxMoveYSave.Extents.y * distance;
			mBoxMoveY.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveY.Extents.z = mBoxMoveYSave.Extents.z * distance;
			mBoxMoveY.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;
			
			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveZSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveZ.Extents.x = mBoxMoveZSave.Extents.x * distance;
			mBoxMoveZ.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveZ.Extents.y = mBoxMoveZSave.Extents.y * distance;
			mBoxMoveZ.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveZ.Extents.z = mBoxMoveZSave.Extents.z * distance;
			mBoxMoveZ.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;
			
			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveXYSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveXY.Extents.x = mBoxMoveXYSave.Extents.x * distance;
			mBoxMoveXY.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveXY.Extents.y = mBoxMoveXYSave.Extents.y * distance;
			mBoxMoveXY.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveXY.Extents.z = mBoxMoveXYSave.Extents.z * distance;
			mBoxMoveXY.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveXY2Save.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveXY2.Extents.x = mBoxMoveXY2Save.Extents.x * distance;
			mBoxMoveXY2.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveXY2.Extents.y = mBoxMoveXY2Save.Extents.y * distance;
			mBoxMoveXY2.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveXY2.Extents.z = mBoxMoveXY2Save.Extents.z * distance;
			mBoxMoveXY2.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveXZSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveXZ.Extents.x = mBoxMoveXZSave.Extents.x * distance;
			mBoxMoveXZ.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveXZ.Extents.y = mBoxMoveXZSave.Extents.y * distance;
			mBoxMoveXZ.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveXZ.Extents.z = mBoxMoveXZSave.Extents.z * distance;
			mBoxMoveXZ.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveXZ2Save.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveXZ2.Extents.x = mBoxMoveXZ2Save.Extents.x * distance;
			mBoxMoveXZ2.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveXZ2.Extents.y = mBoxMoveXZ2Save.Extents.y * distance;
			mBoxMoveXZ2.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveXZ2.Extents.z = mBoxMoveXZ2Save.Extents.z * distance;
			mBoxMoveXZ2.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveYZSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveYZ.Extents.x = mBoxMoveYZSave.Extents.x * distance;
			mBoxMoveYZ.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveYZ.Extents.y = mBoxMoveYZSave.Extents.y * distance;
			mBoxMoveYZ.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveYZ.Extents.z = mBoxMoveYZSave.Extents.z * distance;
			mBoxMoveYZ.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxMoveYZ2Save.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxMoveYZ2.Extents.x = mBoxMoveYZ2Save.Extents.x * distance;
			mBoxMoveYZ2.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxMoveYZ2.Extents.y = mBoxMoveYZ2Save.Extents.y * distance;
			mBoxMoveYZ2.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxMoveYZ2.Extents.z = mBoxMoveYZ2Save.Extents.z * distance;
			mBoxMoveYZ2.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;
		}
		else if (gTransformGizmoType == 2)//Rotate gizmo case
		{
			vect = XMVector3Transform(XMLoadFloat3(&mBoxRotateXSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxRotateX.Extents.x = mBoxRotateXSave.Extents.x * distance;
			mBoxRotateX.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxRotateX.Extents.y = mBoxRotateXSave.Extents.y * distance;
			mBoxRotateX.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxRotateX.Extents.z = mBoxRotateXSave.Extents.z * distance;
			mBoxRotateX.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxRotateYSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxRotateY.Extents.x = mBoxRotateYSave.Extents.x * distance;
			mBoxRotateY.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxRotateY.Extents.y = mBoxRotateYSave.Extents.y * distance;
			mBoxRotateY.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxRotateY.Extents.z = mBoxRotateYSave.Extents.z * distance;
			mBoxRotateY.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxRotateZSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxRotateZ.Extents.x = mBoxRotateZSave.Extents.x * distance;
			mBoxRotateZ.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxRotateZ.Extents.y = mBoxRotateZSave.Extents.y * distance;
			mBoxRotateZ.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxRotateZ.Extents.z = mBoxRotateZSave.Extents.z * distance;
			mBoxRotateZ.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;
		}
		else if (gTransformGizmoType == 3)//Scale gizmo case
		{
			vect = XMVector3Transform(XMLoadFloat3(&mBoxScaleXSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxScaleX.Extents.x = mBoxScaleXSave.Extents.x * distance;
			mBoxScaleX.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxScaleX.Extents.y = mBoxScaleXSave.Extents.y * distance;
			mBoxScaleX.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxScaleX.Extents.z = mBoxScaleXSave.Extents.z * distance;
			mBoxScaleX.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;
			
			vect = XMVector3Transform(XMLoadFloat3(&mBoxScaleYSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxScaleY.Extents.x = mBoxScaleYSave.Extents.x * distance;
			mBoxScaleY.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxScaleY.Extents.y = mBoxScaleYSave.Extents.y * distance;
			mBoxScaleY.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxScaleY.Extents.z = mBoxScaleYSave.Extents.z * distance;
			mBoxScaleY.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;

			vect = XMVector3Transform(XMLoadFloat3(&mBoxScaleZSave.Center), XMMatrixInverse(nullptr, mMatrix));
			mBoxScaleZ.Extents.x = mBoxScaleZSave.Extents.x * distance;
			mBoxScaleZ.Center.x = (XMVectorGetX(vect) * distance) + mGizmoPosition.x;
			mBoxScaleZ.Extents.y = mBoxScaleZSave.Extents.y * distance;
			mBoxScaleZ.Center.y = (XMVectorGetY(vect) * distance) + mGizmoPosition.y;
			mBoxScaleZ.Extents.z = mBoxScaleZSave.Extents.z * distance;
			mBoxScaleZ.Center.z = (XMVectorGetZ(vect) * distance) + mGizmoPosition.z;
		}


		m_pImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &mOffset);
		m_pImmediateContext->IASetIndexBuffer(mIndicesBuffer, DXGI_FORMAT_R16_UINT, 0);

		//-------------------------------------------------------------------------------------------------------------------------------------------
		switch (gTransformGizmoType)
		{
		case 1: //Move
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");

			if (mGizmoSelected != 0)
			{
				if (mGizmoSelected == 1) //X
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 0);

					mFX_SetLineColor->SetInt(0);//Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 18);
					m_pImmediateContext->Draw(4, 22);
					m_pImmediateContext->Draw(4, 26);
					m_pImmediateContext->Draw(18, 30);
					m_pImmediateContext->Draw(18, 48);
				}
				else if (mGizmoSelected == 2) //Y
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 30);

					mFX_SetLineColor->SetInt(0);//Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 0);
					m_pImmediateContext->Draw(4, 18);
					m_pImmediateContext->Draw(4, 22);
					m_pImmediateContext->Draw(4, 26);
					m_pImmediateContext->Draw(18, 48);
				}
				else if (mGizmoSelected == 3) //Z
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 48);

					mFX_SetLineColor->SetInt(0);//Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 0);
					m_pImmediateContext->Draw(4, 18);
					m_pImmediateContext->Draw(4, 22);
					m_pImmediateContext->Draw(4, 26);
					m_pImmediateContext->Draw(18, 30);
				}
				else if (mGizmoSelected == 4) //XY
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 18);

					mFX_SetLineColor->SetInt(0);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 0);
					m_pImmediateContext->Draw(4, 22);
					m_pImmediateContext->Draw(4, 26);
					m_pImmediateContext->Draw(18, 30);
					m_pImmediateContext->Draw(18, 48);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 430);
				}
				else if (mGizmoSelected == 5) //XZ
				{
					mFX_SetLineColor->SetInt(0);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 0);
					m_pImmediateContext->Draw(4, 18);
					m_pImmediateContext->Draw(4, 26);
					m_pImmediateContext->Draw(18, 30);
					m_pImmediateContext->Draw(18, 48);

					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 22);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 426);
				}
				else if (mGizmoSelected == 6) //YZ
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 26);

					mFX_SetLineColor->SetInt(0);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(18, 0);
					m_pImmediateContext->Draw(4, 18);
					m_pImmediateContext->Draw(4, 22);
					m_pImmediateContext->Draw(18, 30);
					m_pImmediateContext->Draw(18, 48);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 434);
				}
			}
			else
			{
				mFX_SetLineColor->SetInt(0);
				mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
				m_pImmediateContext->Draw(18, 0);
				m_pImmediateContext->Draw(4, 18);
				m_pImmediateContext->Draw(4, 22);
				m_pImmediateContext->Draw(4, 26);
				m_pImmediateContext->Draw(18, 30);
				m_pImmediateContext->Draw(18, 48);
			}
			break;
			//-------------------------------------------------------------------------------------------------------------------------------------------
		case 2: //
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES_CIRCLE");

			// X - Axis
			if (mGizmoSelected == 11)
				mFX_SetLineColor->SetInt(5); //Yellow colour
			else
				mFX_SetLineColor->SetInt(1); //Red colour
			mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
			m_pImmediateContext->Draw(90, 66);

			// Y - Axis
			if (mGizmoSelected == 12)
				mFX_SetLineColor->SetInt(5); //Yellow colour
			else
				mFX_SetLineColor->SetInt(3); //Blue colour
			mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
			m_pImmediateContext->Draw(90, 156);

			// Z - Axis
			if (mGizmoSelected == 13)
				mFX_SetLineColor->SetInt(5); //Yellow colour
			else
				mFX_SetLineColor->SetInt(2); //Green colour
			mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
			m_pImmediateContext->Draw(90, 246);



			//Draw small gizmo in the center of circles
			mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
			mFX_SetLineColor->SetInt(0);//Normal colour
			mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
			m_pImmediateContext->Draw(2, 1542);
			m_pImmediateContext->Draw(2, 1544);
			m_pImmediateContext->Draw(2, 1546);
			m_pImmediateContext->Draw(2, 1548);
			m_pImmediateContext->Draw(2, 1550);
			m_pImmediateContext->Draw(2, 1552);

			

			
			//------------------------------
			//Draw grey circle
			//------------------------------
			//W - normal vector from gizmo to camera point(eye)
			W = XMVector3Normalize(camera->GetPositionXM() - localObjectCoordinateSystem->GetLocalCoordinatePositionVECTOR());
			U = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&XMFLOAT3(0, 1, 0)), W)); //Vector up - orthogonal to W
			V = XMVector3Cross(W, U); //Vector right - orthogonal to W & U

			//Set values to matrix (create rotate matrix from 3 orthogonal vector U, V, W)
			matLookAtCamera.m[0][0] = XMVectorGetX(U);
			matLookAtCamera.m[0][1] = XMVectorGetY(U);
			matLookAtCamera.m[0][2] = XMVectorGetZ(U);

			matLookAtCamera.m[1][0] = XMVectorGetX(V);
			matLookAtCamera.m[1][1] = XMVectorGetY(V);
			matLookAtCamera.m[1][2] = XMVectorGetZ(V);

			matLookAtCamera.m[2][0] = XMVectorGetX(W);
			matLookAtCamera.m[2][1] = XMVectorGetY(W);
			matLookAtCamera.m[2][2] = XMVectorGetZ(W);

			//Set translations (X, Y, Z) (center points of the gizmo)
			matLookAtCamera.m[3][0] = localObjectCoordinateSystem->GetLocalCoordinatePosition().x;
			matLookAtCamera.m[3][1] = localObjectCoordinateSystem->GetLocalCoordinatePosition().y;
			matLookAtCamera.m[3][2] = localObjectCoordinateSystem->GetLocalCoordinatePosition().z;


			
			//Counting projecttion matrix
			mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&MathHelper::InverseTranspose(matLookAtCamera)));
			worldViewProjection = matLookAtCamera * view * proj;
			mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&worldViewProjection));

			//Set shader values
			mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
			mFX_SetLineColor->SetInt(6); //grey colour
			mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
			m_pImmediateContext->Draw(90, 246);

			//----------------
			//Draw rotation surface
			//----------------
			if (mAngle != 0)
			{
				if (gGlobalSpace) //global space
				{
					macierzPrzejscia2 = localObjectCoordinateSystem->GetGlobalPositionMatrix();

					mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&macierzPrzejscia2));
					mFX_pWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&MathHelper::InverseTranspose(macierzPrzejscia2)));
					worldViewProjection = macierzPrzejscia2 * view * proj;
					mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&worldViewProjection));
				}
				else //local space
				{
					XMMATRIX odwrotnoscMacierzyPrzejscia2 = XMMatrixInverse(nullptr, mMatrixCircleRotation);
					mFX_pWorld->SetMatrix(reinterpret_cast<float*>(&odwrotnoscMacierzyPrzejscia2));
					worldViewProjection = odwrotnoscMacierzyPrzejscia2 * view * proj;
					mFX_pWorldViewProjection->SetMatrix(reinterpret_cast<float*>(&worldViewProjection));
				}

				mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");
				m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				
				if (mGizmoSelected == 11)
				{
					mFX_SetLineColor->SetInt(7); //Red colour with alphaBlend
					offset = 0;
				}
				else
					if (mGizmoSelected == 12)
					{
						mFX_SetLineColor->SetInt(9); //green colour with alphaBlend
						offset = 360;
					}
					else
						if (mGizmoSelected == 13)
						{
							offset = 720;
							mFX_SetLineColor->SetInt(8); //blue colour with alphaBlend
						}
						
				mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
				if (mGizmoSelected == 11)
				{
					if (mAngle < 0) //Go forward
					{
						angle = mAngle;
						angle *= -1;//change direction
						if (angle > 360)//no loop
							angle -= 360;
						if (mStartVertexCircle - angle < 0) //mStartVertexCircle = (0, 360)
						{
							m_pImmediateContext->DrawIndexed(abs((-1 * (mStartVertexCircle - angle)) * 3), ((360 + (mStartVertexCircle - angle))) * 3, 0);
							m_pImmediateContext->DrawIndexed((mStartVertexCircle) * 3, 0, 0);
						}
						else
							m_pImmediateContext->DrawIndexed(angle * 3, ((mStartVertexCircle - angle) * 3), 0);
					}
					else //Go back
					{
						angle = mAngle;
						if (angle > 360)//no loop
							angle -= 360;
						if (mStartVertexCircle + angle > 360) //mStartVertexCircle = (0, 360)
						{
							m_pImmediateContext->DrawIndexed(((mStartVertexCircle + angle) - 360) * 3, 0, 0);
							m_pImmediateContext->DrawIndexed((angle - ((mStartVertexCircle + angle) % 360)) * 3, (mStartVertexCircle) * 3, 0);
						}
						else
							m_pImmediateContext->DrawIndexed(angle * 3, (mStartVertexCircle) * 3, 0);
					}
				}
				else
					if (mGizmoSelected == 12) //Y - circle
					{
						if (mAngle < 0) //Go back
						{
							angle = mAngle;
							angle *= -1;//change direction
							if (angle > 360)//no loop
								angle -= 360;

							if (mStartVertexCircle + angle > 360) //mStartVertexCircle = (0, 360)
							{
								m_pImmediateContext->DrawIndexed(((mStartVertexCircle + offset + angle) - 720) * 3, offset * 3, 0);
								m_pImmediateContext->DrawIndexed((angle - ((mStartVertexCircle + angle) % 360)) * 3, (mStartVertexCircle + offset) * 3, 0);
							}
							else
								m_pImmediateContext->DrawIndexed(angle * 3, (mStartVertexCircle + offset) * 3, 0);
						}
						else //Go forward
						{
							angle = mAngle;
							if (angle > 360)//no loop
								angle -= 360;

							if (mStartVertexCircle - angle < 0) //mStartVertexCircle = (0, 360)
							{
								m_pImmediateContext->DrawIndexed(abs((-1 * (mStartVertexCircle - angle)) * 3), ((360 + (mStartVertexCircle + offset - angle))) * 3, 0); //ok
								m_pImmediateContext->DrawIndexed(mStartVertexCircle * 3, (360 * 3), 0);
							}
							else
								m_pImmediateContext->DrawIndexed(angle * 3, ((mStartVertexCircle + offset - angle) * 3), 0);
						}
					}
				if (mGizmoSelected == 13) // Z - Circle
				{
					if (mAngle < 0) //Go forward
					{
						angle = mAngle;
						angle *= -1;//change direction
						if (angle > 360)//no loop
							angle -= 360;
						if (mStartVertexCircle - angle < 0) //mStartVertexCircle = (0, 360)
						{
							m_pImmediateContext->DrawIndexed(abs((-1 * (mStartVertexCircle - angle)) * 3), ((360 + (mStartVertexCircle + offset - angle))) * 3, 0);
							m_pImmediateContext->DrawIndexed((mStartVertexCircle) * 3, (720 * 3), 0);
						}
						else
							m_pImmediateContext->DrawIndexed(angle * 3, ((mStartVertexCircle + offset - angle) * 3), 0);
					}
					else //Go back
					{
						angle = mAngle;
						if (angle > 360)//no loop
							angle -= 360;
						if (mStartVertexCircle + angle > 360) //mStartVertexCircle = (0, 360)
						{
							m_pImmediateContext->DrawIndexed(((mStartVertexCircle + angle) - 360) * 3, (720 * 3), 0);
							m_pImmediateContext->DrawIndexed((angle - ((mStartVertexCircle + angle) % 360)) * 3, (mStartVertexCircle + offset) * 3, 0);
						}
						else
							m_pImmediateContext->DrawIndexed(angle * 3, (mStartVertexCircle + offset) * 3, 0);
					}
				}			
			}
			break;
			//-------------------------------------------------------------------------------------------------------------------------------------------
		case 3: //Scale
			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			mTech = mFX->GetTechniqueByName("FeatureLevel11_LINES");

			if (mGizmoSelected != 30)
			{
				if (mGizmoSelected == 31) //X
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					
					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 362);
					m_pImmediateContext->Draw(26, 388);
					m_pImmediateContext->Draw(2, 414);
					m_pImmediateContext->Draw(2, 416);
					m_pImmediateContext->Draw(2, 418);
					m_pImmediateContext->Draw(6, 420);
					
				}
				else if (mGizmoSelected == 32) //Y
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 362);

					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					m_pImmediateContext->Draw(26, 388);
					m_pImmediateContext->Draw(2, 414);
					m_pImmediateContext->Draw(2, 416);
					m_pImmediateContext->Draw(2, 418);
					m_pImmediateContext->Draw(6, 420);
				}
				else if (mGizmoSelected == 33) //Z
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 388);

					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					m_pImmediateContext->Draw(26, 362);
					m_pImmediateContext->Draw(2, 414);
					m_pImmediateContext->Draw(2, 416);
					m_pImmediateContext->Draw(2, 418);
					m_pImmediateContext->Draw(6, 420);
				}
				else if (mGizmoSelected == 34) //XY
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(2, 414);

					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					m_pImmediateContext->Draw(26, 362);
					m_pImmediateContext->Draw(26, 388);
					m_pImmediateContext->Draw(2, 416);
					m_pImmediateContext->Draw(2, 418);
					m_pImmediateContext->Draw(6, 420);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 450);
				}
				else if (mGizmoSelected == 35) //XZ
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(2, 416);

					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					m_pImmediateContext->Draw(26, 362);
					m_pImmediateContext->Draw(26, 388);
					m_pImmediateContext->Draw(2, 414);
					m_pImmediateContext->Draw(2, 418);
					m_pImmediateContext->Draw(6, 420);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 454);
				}
				else if (mGizmoSelected == 36) //YZ
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(2, 418);

					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					m_pImmediateContext->Draw(26, 362);
					m_pImmediateContext->Draw(26, 388);
					m_pImmediateContext->Draw(2, 414);
					m_pImmediateContext->Draw(2, 416);
					m_pImmediateContext->Draw(6, 420);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(4, 458); 
				}
				else if (mGizmoSelected == 37) //XYZ
				{
					mFX_SetLineColor->SetInt(5); //Yellow colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(6, 420);

					mFX_SetLineColor->SetInt(0); //Normal colour
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(26, 336);
					m_pImmediateContext->Draw(26, 362);
					m_pImmediateContext->Draw(26, 388);
					m_pImmediateContext->Draw(2, 414);
					m_pImmediateContext->Draw(2, 416);
					m_pImmediateContext->Draw(2, 418);

					mFX_SetLineColor->SetInt(4); //Yellow colour with alphaBlend
					m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
					m_pImmediateContext->Draw(3, 447);
				}
			}
			else //Normal
			{
				mFX_SetLineColor->SetInt(0); //Normal colour
				mTech->GetPassByIndex(0)->Apply(0, m_pImmediateContext);
				m_pImmediateContext->Draw(26, 336);
				m_pImmediateContext->Draw(26, 362);
				m_pImmediateContext->Draw(26, 388);
				m_pImmediateContext->Draw(2, 414);
				m_pImmediateContext->Draw(2, 416);
				m_pImmediateContext->Draw(2, 418);
				m_pImmediateContext->Draw(6, 420);
			}
			break;

		default:
			break;
		}


	}
}
//**************************************************************************************************************


void Gizmo::DetectRayCollision(Camera* camera, wxPoint* mousePosition, XMMATRIX* worldBaseTransform, int clientWidth, int clientHeight)
{
	XMMATRIX P = camera->Proj();
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f*mousePosition->x / clientWidth - 1.0f) / p(0, 0); 
	float vy = (-2.0f*mousePosition->y / clientHeight + 1.0f) / p(1, 1);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX View = camera->View();
	XMMATRIX inverseView = XMMatrixInverse(&XMMatrixDeterminant(View), View);

	rayOrigin = XMVector3TransformCoord(rayOrigin, inverseView);
	rayDir = XMVector3TransformNormal(rayDir, inverseView);
	rayDir = XMVector3Normalize(rayDir);

	/*wxString abc = "";
	XMFLOAT3 out;


	abc = "";
	abc << vx;
	abc << ", ";
	abc << vy;
	wxMessageBox(abc, "vxvy", wxICON_QUESTION | wxOK);


	abc = "";
	XMStoreFloat3(&out, rayDir);
	abc << out.x;
	abc << ", ";
	abc << out.y;
	abc << ", ";
	abc << out.z;
	wxMessageBox(abc, "rayDir", wxICON_QUESTION | wxOK);*/

	float time = FLT_MAX;
	float timeRotateX = FLT_MAX; //max value of float
	float timeRotateY = FLT_MAX;
	float timeRotateZ = FLT_MAX;

	float timeMoveXY1 = FLT_MAX;
	float timeMoveXY2 = FLT_MAX;
	float timeMoveXZ1 = FLT_MAX;
	float timeMoveXZ2 = FLT_MAX;
	float timeMoveYZ1 = FLT_MAX;
	float timeMoveYZ2 = FLT_MAX;
	float timeMoveXY_MIN = FLT_MAX;
	float timeMoveXZ_MIN = FLT_MAX;
	float timeMoveYZ_MIN = FLT_MAX;

	float timScaleXY_MIN = FLT_MAX;
	float timScaleXZ_MIN = FLT_MAX;
	float timScaleYZ_MIN = FLT_MAX;


	if (gTransformGizmoType == 1)// Move Gizmo
	{
		if (IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveX, &time) == true)
		{
			mGizmoSelected = 1;
		}
		else
			if (IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveY, &time) == true)
			{
				mGizmoSelected = 2;
			}
			else
				if (IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveZ, &time) == true)
				{
					mGizmoSelected = 3;
				}
				else
				{
					IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveXY, &timeMoveXY1);
					IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveXY2, &timeMoveXY2);
					IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveXZ, &timeMoveXZ1);
					IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveXZ2, &timeMoveXZ2);
					IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveYZ, &timeMoveYZ1);
					IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxMoveYZ2, &timeMoveYZ2);
					if (timeMoveXY1 <= timeMoveXY2)
						timeMoveXY_MIN = timeMoveXY1;
					else
						timeMoveXY_MIN = timeMoveXY2;

					if (timeMoveXZ1 <= timeMoveXZ2)
						timeMoveXZ_MIN = timeMoveXZ1;
					else
						timeMoveXZ_MIN = timeMoveXZ2;

					if (timeMoveYZ1 <= timeMoveYZ2)
						timeMoveYZ_MIN = timeMoveYZ1;
					else
						timeMoveYZ_MIN = timeMoveYZ2;

					if (timeMoveXY_MIN == FLT_MAX && timeMoveXZ_MIN == FLT_MAX && timeMoveYZ_MIN == FLT_MAX)
						mGizmoSelected = 0; //null
					else
					if(timeMoveXY_MIN < timeMoveXZ_MIN && timeMoveXY_MIN < timeMoveYZ_MIN)
						mGizmoSelected = 4;
					else
						if (timeMoveXZ_MIN < timeMoveXY_MIN && timeMoveXZ_MIN < timeMoveYZ_MIN)
						mGizmoSelected = 5;
						else
							if (timeMoveYZ_MIN < timeMoveXY_MIN && timeMoveYZ_MIN < timeMoveXZ_MIN)
								mGizmoSelected = 6;

				}
	}
	else
		if (gTransformGizmoType == 2)// Rotate Gizmo
		{
			IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxRotateX, &timeRotateX);
			IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxRotateY, &timeRotateY);
			IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxRotateZ, &timeRotateZ);

			if (timeRotateX == FLT_MAX && timeRotateY == FLT_MAX && timeRotateZ == FLT_MAX)
				mGizmoSelected = 10;
			else
			{
				if (timeRotateX < timeRotateY && timeRotateX < timeRotateZ)
					mGizmoSelected = 11;
				else
					if (timeRotateY < timeRotateX && timeRotateY < timeRotateZ)
						mGizmoSelected = 12;
					else
						if (timeRotateZ < timeRotateX && timeRotateZ < timeRotateY)
							mGizmoSelected = 13;
						else
							mGizmoSelected = 10;
			}
		}
	else 
		if (gTransformGizmoType == 3)// Scale Gizmo
		{
			XMVECTOR V1;
			XMVECTOR V2;
			XMVECTOR V3;
			XMFLOAT3 tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);

			V1 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(447).Pos), XMMatrixInverse(nullptr, mMatrix));
			tempPoint.x = (XMVectorGetX(V1) * mDistance) + mGizmoPosition.x;
			tempPoint.y = (XMVectorGetY(V1) * mDistance) + mGizmoPosition.y;
			tempPoint.z = (XMVectorGetZ(V1) * mDistance) + mGizmoPosition.z;
			V1 = XMLoadFloat3(&tempPoint);
			
			tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
			V2 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(448).Pos), XMMatrixInverse(nullptr, mMatrix));
			tempPoint.x = (XMVectorGetX(V2) * mDistance) + mGizmoPosition.x;
			tempPoint.y = (XMVectorGetY(V2) * mDistance) + mGizmoPosition.y;
			tempPoint.z = (XMVectorGetZ(V2) * mDistance) + mGizmoPosition.z;
			V2 = XMLoadFloat3(&tempPoint);

			tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
			V3 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(449).Pos), XMMatrixInverse(nullptr, mMatrix));
			tempPoint.x = (XMVectorGetX(V3) * mDistance) + mGizmoPosition.x;
			tempPoint.y = (XMVectorGetY(V3) * mDistance) + mGizmoPosition.y;
			tempPoint.z = (XMVectorGetZ(V3) * mDistance) + mGizmoPosition.z;
			V3 = XMLoadFloat3(&tempPoint);
			
			if (IntersectRayTriangle(rayOrigin, rayDir, V1, V2, V3, &time)) //XYZ triangle testing
			{
				mGizmoSelected = 37;
			}
			else
			if (IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxScaleX, &time) == true)
			{
				mGizmoSelected = 31;
			}
			else
				if (IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxScaleY, &time) == true)
				{
					mGizmoSelected = 32;
				}
				else
					if (IntersectRayOrientedBox(rayOrigin, rayDir, &mBoxScaleZ, &time) == true)
					{
						mGizmoSelected = 33;
					}
					else
						{
							//XZ
							V1 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(441).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V1) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V1) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V1) * mDistance) + mGizmoPosition.z;
							V1 = XMLoadFloat3(&tempPoint);

							tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
							V2 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(442).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V2) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V2) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V2) * mDistance) + mGizmoPosition.z;
							V2 = XMLoadFloat3(&tempPoint);

							tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
							V3 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(443).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V3) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V3) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V3) * mDistance) + mGizmoPosition.z;
							V3 = XMLoadFloat3(&tempPoint);

							IntersectRayTriangle(rayOrigin, rayDir, V1, V2, V3, &timScaleXZ_MIN);

							//XY
							V1 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(438).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V1) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V1) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V1) * mDistance) + mGizmoPosition.z;
							V1 = XMLoadFloat3(&tempPoint);

							tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
							V2 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(439).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V2) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V2) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V2) * mDistance) + mGizmoPosition.z;
							V2 = XMLoadFloat3(&tempPoint);

							tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
							V3 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(440).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V3) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V3) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V3) * mDistance) + mGizmoPosition.z;
							V3 = XMLoadFloat3(&tempPoint);

							IntersectRayTriangle(rayOrigin, rayDir, V1, V2, V3, &timScaleXY_MIN);


							//YZ
							V1 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(444).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V1) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V1) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V1) * mDistance) + mGizmoPosition.z;
							V1 = XMLoadFloat3(&tempPoint);

							tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
							V2 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(445).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V2) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V2) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V2) * mDistance) + mGizmoPosition.z;
							V2 = XMLoadFloat3(&tempPoint);

							tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
							V3 = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(446).Pos), XMMatrixInverse(nullptr, mMatrix));
							tempPoint.x = (XMVectorGetX(V3) * mDistance) + mGizmoPosition.x;
							tempPoint.y = (XMVectorGetY(V3) * mDistance) + mGizmoPosition.y;
							tempPoint.z = (XMVectorGetZ(V3) * mDistance) + mGizmoPosition.z;
							V3 = XMLoadFloat3(&tempPoint);


							IntersectRayTriangle(rayOrigin, rayDir, V1, V2, V3, &timScaleYZ_MIN);


							if (timScaleXZ_MIN == FLT_MAX && timScaleXY_MIN == FLT_MAX && timScaleYZ_MIN == FLT_MAX)
								mGizmoSelected = 30; //NO SELECTED
							else
							{
								if (timScaleXZ_MIN < timScaleXY_MIN && timScaleXZ_MIN < timScaleYZ_MIN)
									mGizmoSelected = 35;
								else
									if (timScaleXY_MIN < timScaleXZ_MIN && timScaleXY_MIN < timScaleYZ_MIN)
										mGizmoSelected = 34;
									else
										if (timScaleYZ_MIN < timScaleXZ_MIN && timScaleYZ_MIN < timScaleXY_MIN)
											mGizmoSelected = 36;
										else
											mGizmoSelected = 30; //NO SELECTED
							}
						}									
		}
}
//***********************************************************************************************************
int Gizmo::GetGizmoSelected()
{
	return this->mGizmoSelected;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoStartPoint2D()
{
	return this->mGizmoStartPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisXPoint2D()
{
	return this->mGizmoAxisXPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisYPoint2D()
{
	return this->mGizmoAxisYPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisZPoint2D()
{
	return this->mGizmoAxisZPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisXYPoint2D()
{
	return this->mGizmoAxisXYPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisXZPoint2D()
{
	return this->mGizmoAxisXZPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisYZPoint2D()
{
	return this->mGizmoAxisYZPoint2D;
}
//***********************************************************************************************************
XMFLOAT2 Gizmo::GetGizmoAxisXYZPoint2D()
{
	return this->mGizmoAxisXYZPoint2D;
}
//***********************************************************************************************************
float Gizmo::GetDistanceToGizmo()
{
	return this->mDistanceToGizmo;
}
//***********************************************************************************************************
void Gizmo::FindNearPointOfRotation(XMFLOAT2 startTransformPoint, Camera* camera, XMMATRIX* worldBaseTransform, int clientWidth, int clientHeight)
{
	XMMATRIX P = camera->Proj();
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f*startTransformPoint.x / clientWidth - 1.0f) / p(0, 0);
	float vy = (-2.0f*startTransformPoint.y / clientHeight + 1.0f) / p(1, 1);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX V = camera->View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = *worldBaseTransform;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	rayDir = XMVector3Normalize(rayDir);




	float nearestPoint = FLT_MAX;
	int nearestPointPosition = -1;
	int iterStart = 0;
	int iterMax = 0;


	if (mGizmoSelected == 11) //X - circle
	{
		iterStart = 461;
		iterMax = 821;
	}
	else
		if (mGizmoSelected == 12) //Y - circle
		{
			iterStart = 821;
			iterMax = 1181;
		}
		else
			if (mGizmoSelected == 13) //Z - circle
			{
				iterStart = 1181;
				iterMax = 1541;
			}
	//----------------------------------------------------
	//Find the nearest point between ray and circle
	//----------------------------------------------------
	//Description values
	//rayOrigin -> S 
	//pointCircle -> P
	//rayDir -> V   ||V|| = 1
	//tempVector -> (P-S)
	//lenghtToPoint -> d
	XMVECTOR pointCircle;
	XMVECTOR tempVector;
	XMFLOAT3 tempPoint = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float lenghtToPoint = -1.0f;

	//----------------------------------------------------
	//Equation
	//----------------------------------------------------
	//d = || ((P-S) cross V) / ||V|| ) ||
	for (int i = iterStart; i <= iterMax; ++i)
	{
		pointCircle = XMVector3Transform(XMLoadFloat3(&mMesh.Vertices.at(i).Pos), XMMatrixInverse(nullptr, mMatrix));
		tempPoint.x = (XMVectorGetX(pointCircle) * mDistance) + mGizmoPosition.x;
		tempPoint.y = (XMVectorGetY(pointCircle) * mDistance) + mGizmoPosition.y;
		tempPoint.z = (XMVectorGetZ(pointCircle) * mDistance) + mGizmoPosition.z;
		pointCircle = XMLoadFloat3(&tempPoint);

		//Compute
		tempVector = XMVectorSubtract(pointCircle, rayOrigin); //P-S 
		tempVector = XMVector3Cross(tempVector, rayDir); //P-S cross V
		tempVector = XMVector3Length(tempVector); // || (P-S) cross V ||

		lenghtToPoint = XMVectorGetX(tempVector); //Get value
		if (lenghtToPoint < nearestPoint)
		{
			nearestPoint = lenghtToPoint;
			nearestPointPosition = i;
			vertex = tempPoint;
		}
			
	}

	if (mGizmoSelected == 11) //X - circle
	{
		mStartVertexCircle = nearestPointPosition - 462;
	}
	else
		if (mGizmoSelected == 12) //Y - circle
		{
			mStartVertexCircle = nearestPointPosition - (462 + 360);
		}
		else
			if (mGizmoSelected == 13) //Z - circle
			{
				mStartVertexCircle = nearestPointPosition - (462 + 720);
			}

}
void Gizmo::DrawRotationAngle(int angle)
{
	mAngle = angle;
}
void Gizmo::SaveMatrixCircleRotation(LocalCoordinateSystem* localObjectCoordinateSystem)
{
	mMatrixCircleRotation = localObjectCoordinateSystem->GetMatrixToDrawGizmo();
}
//******************************************************************************************************************************************
void Gizmo::FindStartPointOfTransform(XMFLOAT2 startTransformPoint, Camera* camera, XMMATRIX* worldBaseTransform, int clientWidth, int clientHeight, LocalCoordinateSystem* localObjectCoordinateSystem, int type)
{
	//Counting ray
	XMMATRIX P = camera->Proj();
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, P);

	float vx = (+2.0f*startTransformPoint.x / clientWidth - 1.0f) / p(0, 0);
	float vy = (-2.0f*startTransformPoint.y / clientHeight + 1.0f) / p(1, 1);

	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	XMMATRIX V = camera->View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = *worldBaseTransform;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	rayDir = XMVector3Normalize(rayDir);

	//Values
	XMFLOAT3 normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMVECTOR temp1;
	XMVECTOR temp2;

	//Detect normal vector to plane 
	if (gGlobalSpace == false)
	{
		if (mGizmoSelected == 1) //Local x
		{
			temp1 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetLocalAxisY()));
			temp2 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetLocalAxisZ()));
			if (XMVectorGetIntX(temp1) > XMVectorGetIntX(temp2))
				normal = localObjectCoordinateSystem->GetLocalAxisY();
			else
				normal = localObjectCoordinateSystem->GetLocalAxisZ();
		}
		else
			if (mGizmoSelected == 2) //Local y
			{
				temp1 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetLocalAxisX()));
				temp2 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetLocalAxisZ()));
				if(XMVectorGetIntX(temp1) > XMVectorGetIntX(temp2))
					normal = localObjectCoordinateSystem->GetLocalAxisX();
				else
					normal = localObjectCoordinateSystem->GetLocalAxisZ();
			}
			else
				if (mGizmoSelected == 3) //Local z
				{
					temp1 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetLocalAxisX()));
					temp2 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetLocalAxisY()));
					if (XMVectorGetIntX(temp1) > XMVectorGetIntX(temp2))
						normal = localObjectCoordinateSystem->GetLocalAxisX();
					else
						normal = localObjectCoordinateSystem->GetLocalAxisY();
				}
				else
					if (mGizmoSelected == 4) //Local XY
					{
						normal = localObjectCoordinateSystem->GetLocalAxisZ();
					}
					else
						if (mGizmoSelected == 5) //Local XZ
						{
							normal = localObjectCoordinateSystem->GetLocalAxisY();
						}
						else
							if (mGizmoSelected == 6) //Local YZ
							{
								normal = localObjectCoordinateSystem->GetLocalAxisX();
							}
	}
	else
	{
		if (mGizmoSelected == 1) //Global x
		{
			temp1 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetGlobalAxisY()));
			temp2 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetGlobalAxisZ()));
			if (XMVectorGetIntX(temp1) > XMVectorGetIntX(temp2))
				normal = localObjectCoordinateSystem->GetGlobalAxisY();
			else
				normal = localObjectCoordinateSystem->GetGlobalAxisZ();
		}
		else
			if (mGizmoSelected == 2) //Global y
			{
				temp1 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetGlobalAxisX()));
				temp2 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetGlobalAxisZ()));
				if (XMVectorGetIntX(temp1) > XMVectorGetIntX(temp2))
					normal = localObjectCoordinateSystem->GetGlobalAxisX();
				else
					normal = localObjectCoordinateSystem->GetGlobalAxisZ();
			}
			else
				if (mGizmoSelected == 3) //Global z
				{
					temp1 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetGlobalAxisX()));
					temp2 = XMVector3Dot(rayDir, XMLoadFloat3(&localObjectCoordinateSystem->GetGlobalAxisY()));
					if (XMVectorGetIntX(temp1) > XMVectorGetIntX(temp2))
						normal = localObjectCoordinateSystem->GetGlobalAxisX();
					else
						normal = localObjectCoordinateSystem->GetGlobalAxisY();
				}
				else
					if (mGizmoSelected == 4) //Global XY
					{
						normal = localObjectCoordinateSystem->GetGlobalAxisZ();
					}
					else
						if (mGizmoSelected == 5) //Global XZ
						{
							normal = localObjectCoordinateSystem->GetGlobalAxisY();
						}
						else
							if (mGizmoSelected == 6) //Global YZ
							{
								normal = localObjectCoordinateSystem->GetGlobalAxisX();
							}
	}

	XMVECTOR normalvec = XMLoadFloat3(&normal);
	XMVECTOR Point;
	
	//----------------------------------------------------
	//Equation
	//----------------------------------------------------
	//Point = S - (((N dot S) + a / (N dot V)) * V)
	// a = (-N dot K)
	// K --> object position
	// S --> rayOrigin
	// N --> normalvec
	// V --> rayDir

	//Counting dots
	Point = XMVector3Dot(normalvec, rayOrigin); // (N dot S)

	temp1 = XMVector3Dot(XMVectorNegate(normalvec), XMLoadFloat3(&localObjectCoordinateSystem->GetLocalCoordinatePosition())); // a = (-N dot K)
	//Add 'a'
	Point = XMVectorAdd(Point, temp1); // (N dot S) + a = (N dot S) + (-N dot K)

	temp1 = XMVector3Dot(normalvec, rayDir); // (N dot V)

	//Divide
	Point = XMVectorDivide(Point, temp1); // (N dot S) + a / (N dot V)

	//Multiply
	Point = XMVectorMultiply(Point, rayDir); //  ((N dot S) + a / (N dot V)) * V

	//Substract
	Point = XMVectorSubtract(rayOrigin, Point); // S - (((N dot S) + a / (N dot V)) * V)

	 //Store vector to XMFLOAT3
	if (type == 0)
	{
		XMStoreFloat3(&mStartPoint, Point);
		XMStoreFloat3(&mStartPoint2, Point);
	}
	else
		if(type == 1)
			XMStoreFloat3(&mStartPoint, Point);
		else
			if (type == 2)
				XMStoreFloat3(&mStartPoint2, Point);

	// if (mGizmoSelected == 4 || mGizmoSelected == 5 || mGizmoSelected == 6)
	//	 mStartPoint = localObjectCoordinateSystem->GetLocalCoordinatePosition();

	 //Set normal vector to plane
	 mNormalVectorToPlane = normalvec;
}

XMFLOAT3 Gizmo::GetStartPoint()
{
	return mStartPoint;
}

XMFLOAT3 Gizmo::GetStartPoint2()
{
	return mStartPoint2;
}

XMVECTOR Gizmo::GetNormalVectorToPlane()
{
	return mNormalVectorToPlane;
}