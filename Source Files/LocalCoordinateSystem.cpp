#include "../Header Files/LocalCoordinateSystem.h"


//**************************************************************************************************************
// Constructor 
//**************************************************************************************************************
LocalCoordinateSystem::LocalCoordinateSystem()
{

	mMatrix = XMMatrixIdentity();  

	mLocalCoordinatePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mLocalAxisX = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);

	mGlobalAxisX = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mGlobalAxisY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mGlobalAxisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);

	mScaleVector = XMFLOAT3(1.0f, 1.0f, 1.0f);

	mPitch = 0.0f;
	mYaw = 0.0f;
	mRoll = 0.0f;
}


//********************************************************************************************************************
// Destructor
//********************************************************************************************************************
LocalCoordinateSystem::~LocalCoordinateSystem()
{
}


//********************************************************************************************************************
// Return position
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetLocalCoordinatePosition()const
{
	return mLocalCoordinatePosition;
}

//********************************************************************************************************************
// Return position as VECTOR
//********************************************************************************************************************
XMVECTOR LocalCoordinateSystem::GetLocalCoordinatePositionVECTOR()const
{
	return XMLoadFloat3(&mLocalCoordinatePosition);
}


//********************************************************************************************************************
// Set position as X, Y, Z
//********************************************************************************************************************
void LocalCoordinateSystem::SetLocalCoordinatePosition(float x, float y, float z)
{
	mLocalCoordinatePosition = XMFLOAT3(x, y, z);
}

//********************************************************************************************************************
// Set position as vector
//********************************************************************************************************************
void LocalCoordinateSystem::SetLocalCoordinatePosition(const XMFLOAT3& vector)
{
	mLocalCoordinatePosition = vector;
}



//********************************************************************************************************************************************************************
// LOCAL COORDINATE SYSTEM
//********************************************************************************************************************************************************************

//********************************************************************************************************************
// Set LocalAxisX as vector
//********************************************************************************************************************
void LocalCoordinateSystem::setLocalAxisX(const XMFLOAT3& vx)
{
	mLocalAxisX = vx;
}

//********************************************************************************************************************
// Set LocalAxisX
//********************************************************************************************************************
void LocalCoordinateSystem::setLocalAxisX(float x, float y, float z)
{
	mLocalAxisX = XMFLOAT3(x, y, z);
}

//********************************************************************************************************************
// Return LocalAxisX
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetLocalAxisX()const
{
	return mLocalAxisX;
}


//********************************************************************************************************************
// Set LocalAxisY as vector
//********************************************************************************************************************
void LocalCoordinateSystem::setLocalAxisY(const XMFLOAT3& vy)
{
	mLocalAxisY = vy;
}


//********************************************************************************************************************
// Set LocalAxisX
//********************************************************************************************************************
void LocalCoordinateSystem::setLocalAxisY(float x, float y, float z)
{
	mLocalAxisY = XMFLOAT3(x, y, z);
}

//********************************************************************************************************************
// Return LocalAxisY
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetLocalAxisY()const
{
	return mLocalAxisY;
}

//********************************************************************************************************************
// Set LocalAxisZ as vector
//********************************************************************************************************************
void LocalCoordinateSystem::setLocalAxisZ(const XMFLOAT3& vz)
{
	mLocalAxisZ = vz;
}

//********************************************************************************************************************
// Set LocalAxisX
//********************************************************************************************************************
void LocalCoordinateSystem::setLocalAxisZ(float x, float y, float z)
{
	mLocalAxisZ = XMFLOAT3(x, y, z);
}

//********************************************************************************************************************
// Return LocalAxisZ
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetLocalAxisZ()const
{
	return mLocalAxisZ;
}

//********************************************************************************************************************
// Rotate Local X
//********************************************************************************************************************
void LocalCoordinateSystem::Pitch(float angle) //X axis
{
	XMMATRIX Rotate = XMMatrixRotationAxis(XMLoadFloat3(&mLocalAxisX), angle);

	XMStoreFloat3(&mLocalAxisY, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisY), Rotate));
	XMStoreFloat3(&mLocalAxisZ, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisZ), Rotate));
}

//********************************************************************************************************************
// Rotate Local Y
//********************************************************************************************************************
void LocalCoordinateSystem::Yaw(float angle) //Y axis
{
	//XMMATRIX Rotate = XMMatrixRotationY(angle);
	XMMATRIX Rotate = XMMatrixRotationAxis(XMLoadFloat3(&mLocalAxisY), angle);

	XMStoreFloat3(&mLocalAxisX, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisX), Rotate));
	XMStoreFloat3(&mLocalAxisZ, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisZ), Rotate));
}


//********************************************************************************************************************
// Rotate Local Z
//********************************************************************************************************************
void LocalCoordinateSystem::Roll(float angle) //Z axis
{
	XMMATRIX Rotate = XMMatrixRotationAxis(XMLoadFloat3(&mLocalAxisZ), angle);

	XMStoreFloat3(&mLocalAxisX, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisX), Rotate));
	XMStoreFloat3(&mLocalAxisY, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisY), Rotate));
	
}

//********************************************************************************************************************
// Move Local X
//********************************************************************************************************************
void LocalCoordinateSystem::MoveX(float x)
{
	XMVECTOR s = XMVectorReplicate(x);
	XMVECTOR r = XMLoadFloat3(&mLocalAxisX);
	XMVECTOR position = XMLoadFloat3(&mLocalCoordinatePosition);
	XMStoreFloat3(&mLocalCoordinatePosition, XMVectorMultiplyAdd(s, r, position));
}


//********************************************************************************************************************
// Move Local Y
//********************************************************************************************************************
void LocalCoordinateSystem::MoveY(float y)
{
	XMVECTOR s = XMVectorReplicate(y);
	XMVECTOR l = XMLoadFloat3(&mLocalAxisY);
	XMVECTOR position = XMLoadFloat3(&mLocalCoordinatePosition);
	XMStoreFloat3(&mLocalCoordinatePosition, XMVectorMultiplyAdd(s, l, position));
}


//********************************************************************************************************************
// Move Local Z
//********************************************************************************************************************
void LocalCoordinateSystem::MoveZ(float z)
{
	XMVECTOR s = XMVectorReplicate(z);
	XMVECTOR l = XMLoadFloat3(&mLocalAxisZ);
	XMVECTOR position = XMLoadFloat3(&mLocalCoordinatePosition);
	XMStoreFloat3(&mLocalCoordinatePosition, XMVectorMultiplyAdd(s, l, position));
}



//********************************************************************************************************************
// Get Local Matrix
//********************************************************************************************************************
XMMATRIX LocalCoordinateSystem::GetLocalMatrix()const
{
	return XMLoadFloat4x4(&mLocalCoordinateSystem);
}


XMMATRIX LocalCoordinateSystem::GetMatrixToDrawGizmo()
{
	XMVECTOR AxisX = XMLoadFloat3(&mLocalAxisX);
	XMVECTOR AxisY = XMLoadFloat3(&mLocalAxisY);
	XMVECTOR AxisZ = XMLoadFloat3(&mLocalAxisZ);
	XMVECTOR Position = XMLoadFloat3(&mLocalCoordinatePosition);

	XMFLOAT4X4 matrix4x4;
	XMMATRIX matrix = XMMatrixIdentity();
	XMStoreFloat4x4(&matrix4x4, matrix);
	//Ortonormalizacja wektorów 
	AxisZ = XMVector3Normalize(AxisZ); //Dlugosc 1
	AxisY = XMVector3Normalize(XMVector3Cross(AxisZ, AxisX)); //Normalizuj dla wektora Y

	//Normalizuj dla wektora X 
	// ||YxZ|| = ||Y||||Z||sin90 = 1
	AxisX = XMVector3Cross(AxisY, AxisZ);

	// Wypelnianie 
	float x = -XMVectorGetX(XMVector3Dot(Position, AxisX));
	float y = -XMVectorGetX(XMVector3Dot(Position, AxisY));
	float z = -XMVectorGetX(XMVector3Dot(Position, AxisZ));

	XMStoreFloat3(&mLocalAxisX, AxisX);
	XMStoreFloat3(&mLocalAxisY, AxisY);
	XMStoreFloat3(&mLocalAxisZ, AxisZ);

	matrix4x4(0, 0) = mLocalAxisX.x;
	matrix4x4(1, 0) = mLocalAxisX.y;
	matrix4x4(2, 0) = mLocalAxisX.z;
	matrix4x4(3, 0) = x;

	matrix4x4(0, 1) = mLocalAxisY.x;
	matrix4x4(1, 1) = mLocalAxisY.y;
	matrix4x4(2, 1) = mLocalAxisY.z;
	matrix4x4(3, 1) = y;

	matrix4x4(0, 2) = mLocalAxisZ.x;
	matrix4x4(1, 2) = mLocalAxisZ.y;
	matrix4x4(2, 2) = mLocalAxisZ.z;
	matrix4x4(3, 2) = z;

	matrix4x4(0, 3) = 0.0f;
	matrix4x4(1, 3) = 0.0f;
	matrix4x4(2, 3) = 0.0f;
	matrix4x4(3, 3) = 1.0f;

	return XMLoadFloat4x4(&matrix4x4);
}

//********************************************************************************************************************
// Update Local Coordinate Matrix
//********************************************************************************************************************
void LocalCoordinateSystem::UpdateLocalCoordinateMatrix()
{

	XMVECTOR AxisX = XMLoadFloat3(&mLocalAxisX);
	XMVECTOR AxisY = XMLoadFloat3(&mLocalAxisY);
	XMVECTOR AxisZ = XMLoadFloat3(&mLocalAxisZ);
	XMVECTOR Position = XMLoadFloat3(&mLocalCoordinatePosition);

	//Ortonormalizacja wektorów 
	AxisZ = XMVector3Normalize(AxisZ); //Dlugosc 1
	AxisY = XMVector3Normalize(XMVector3Cross(AxisZ, AxisX)); //Normalizuj dla wektora Y

	//Normalizuj dla wektora X 
	// ||YxZ|| = ||Y||||Z||sin90 = 1
	AxisX = XMVector3Cross(AxisY, AxisZ);

	// Wypelnianie 
	float x = -XMVectorGetX(XMVector3Dot(Position, AxisX));
	float y = -XMVectorGetX(XMVector3Dot(Position, AxisY));
	float z = -XMVectorGetX(XMVector3Dot(Position, AxisZ));

	x *= mScaleVector.x;
	y *= mScaleVector.y;
	z *= mScaleVector.z;

	XMStoreFloat3(&mLocalAxisX, AxisX);
	XMStoreFloat3(&mLocalAxisY, AxisY);
	XMStoreFloat3(&mLocalAxisZ, AxisZ);

	mLocalCoordinateSystem(0, 0) = mLocalAxisX.x * mScaleVector.x;
	mLocalCoordinateSystem(1, 0) = mLocalAxisX.y * mScaleVector.x;
	mLocalCoordinateSystem(2, 0) = mLocalAxisX.z * mScaleVector.x;
	mLocalCoordinateSystem(3, 0) = x;
	//mLocalCoordinateSystem(3, 0) = 0;

	mLocalCoordinateSystem(0, 1) = mLocalAxisY.x * mScaleVector.y;
	mLocalCoordinateSystem(1, 1) = mLocalAxisY.y * mScaleVector.y;
	mLocalCoordinateSystem(2, 1) = mLocalAxisY.z * mScaleVector.y;
	mLocalCoordinateSystem(3, 1) = y;
	//mLocalCoordinateSystem(3, 1) = 0;

	mLocalCoordinateSystem(0, 2) = mLocalAxisZ.x * mScaleVector.z;
	mLocalCoordinateSystem(1, 2) = mLocalAxisZ.y * mScaleVector.z;
	mLocalCoordinateSystem(2, 2) = mLocalAxisZ.z * mScaleVector.z;
	mLocalCoordinateSystem(3, 2) = z;
	//mLocalCoordinateSystem(3, 2) = 0;

	mLocalCoordinateSystem(0, 3) = 0.0f;
	mLocalCoordinateSystem(1, 3) = 0.0f;
	mLocalCoordinateSystem(2, 3) = 0.0f;
	mLocalCoordinateSystem(3, 3) = 1.0f;

/*	XMVECTOR quaternion;
	XMVECTOR scale;
	XMVECTOR translation;
	XMVECTOR point;

	XMMatrixDecompose(&scale, &quaternion, &translation, XMLoadFloat4x4(&mLocalCoordinateSystem));

	point = XMLoadFloat3(&XMFLOAT3(0, 0, 0));
	scale = XMLoadFloat3(&mScaleVector);

	XMStoreFloat4x4(&mLocalCoordinateSystem, XMMatrixAffineTransformation(scale, point, quaternion, translation));

	XMFLOAT4X4 scaleMatrix;
	XMMATRIX matrix = XMMatrixIdentity();
	XMStoreFloat4x4(&scaleMatrix, matrix);
	scaleMatrix(0, 0) = mScaleVector.x;
	scaleMatrix(1, 1) = mScaleVector.y;
	scaleMatrix(2, 2) = mScaleVector.z;*/

	/*	XMStoreFloat4x4(&mLocalCoordinateSystem, XMMatrixMultiply(XMLoadFloat4x4(&scaleMatrix), XMLoadFloat4x4(&mLocalCoordinateSystem)));

	//mLocalCoordinateSystem(3, 0) = x;
	//mLocalCoordinateSystem(3, 1) = y;
	//mLocalCoordinateSystem(3, 2) = z;

	//Move
	XMStoreFloat4x4(&scaleMatrix, matrix);
	scaleMatrix(3, 0) = x;
	scaleMatrix(3, 1) = y;
	scaleMatrix(3, 2) = z;

	XMStoreFloat4x4(&mLocalCoordinateSystem, XMMatrixMultiply(XMLoadFloat4x4(&scaleMatrix), XMLoadFloat4x4(&mLocalCoordinateSystem)));

	*/
}
//********************************************************************************************************************************************************************
// GLOBAL COORDINATE SYSTEM
//********************************************************************************************************************************************************************


//********************************************************************************************************************
// Return GlobalAxisX
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetGlobalAxisX()const
{
	return mGlobalAxisX;
}

//********************************************************************************************************************
// Return GlobalAxisY
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetGlobalAxisY()const
{
	return mGlobalAxisY;
}


//********************************************************************************************************************
// Return GlobalAxisZ
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetGlobalAxisZ()const
{
	return mGlobalAxisZ;
}

//********************************************************************************************************************
// Rotate Global X
//********************************************************************************************************************
void LocalCoordinateSystem::Pitch_GlobalAxis(float angle) //Global X axis
{
	mGlobalAxisX = XMFLOAT3(1.0f, 0.0f, 0.0f);

	XMMATRIX Rotate = XMMatrixRotationAxis(XMLoadFloat3(&mGlobalAxisX), angle);
	XMStoreFloat3(&mLocalAxisX, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisX), Rotate));
	XMStoreFloat3(&mLocalAxisY, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisY), Rotate));
	XMStoreFloat3(&mLocalAxisZ, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisZ), Rotate));
}

//********************************************************************************************************************
// Rotate Global Y
//********************************************************************************************************************
void LocalCoordinateSystem::Yaw_GlobalAxis(float angle) //Global Y axis
{
	mGlobalAxisY = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMMATRIX Rotate = XMMatrixRotationAxis(XMLoadFloat3(&mGlobalAxisY), angle);
	XMStoreFloat3(&mLocalAxisX, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisX), Rotate));
	XMStoreFloat3(&mLocalAxisY, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisY), Rotate));
	XMStoreFloat3(&mLocalAxisZ, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisZ), Rotate));
}


//********************************************************************************************************************
// Rotate Global Z
//********************************************************************************************************************
void LocalCoordinateSystem::Roll_GlobalAxis(float angle) //Global Z axis
{
	mGlobalAxisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMMATRIX Rotate = XMMatrixRotationAxis(XMLoadFloat3(&mGlobalAxisZ), angle);
	XMStoreFloat3(&mLocalAxisX, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisX), Rotate));
	XMStoreFloat3(&mLocalAxisY, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisY), Rotate));
	XMStoreFloat3(&mLocalAxisZ, XMVector3TransformNormal(XMLoadFloat3(&mLocalAxisZ), Rotate));

}


//********************************************************************************************************************
// Move Global X
//********************************************************************************************************************
void LocalCoordinateSystem::MoveGlobalX(float x)
{
	//XMFLOAT3 AxisXa = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMVECTOR s = XMVectorReplicate(x);
	XMVECTOR r = XMLoadFloat3(&mGlobalAxisX);
//	XMVECTOR r = XMLoadFloat3(&AxisXa);
	XMVECTOR position = XMLoadFloat3(&mLocalCoordinatePosition);
	XMStoreFloat3(&mLocalCoordinatePosition, XMVectorMultiplyAdd(s, r, position));
}


//********************************************************************************************************************
// Move Global Y
//********************************************************************************************************************
void LocalCoordinateSystem::MoveGlobalY(float y)
{

	//XMFLOAT3 AxisY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMVECTOR s = XMVectorReplicate(y);
	XMVECTOR l = XMLoadFloat3(&mGlobalAxisY);
	//XMVECTOR l = XMLoadFloat3(&AxisY);
	XMVECTOR position = XMLoadFloat3(&mLocalCoordinatePosition);
	XMStoreFloat3(&mLocalCoordinatePosition, XMVectorMultiplyAdd(s, l, position));
}


//********************************************************************************************************************
// Move Global Z
//********************************************************************************************************************
void LocalCoordinateSystem::MoveGlobalZ(float z)
{


	//XMFLOAT3 AxisZ = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR s = XMVectorReplicate(z);
	XMVECTOR l = XMLoadFloat3(&mGlobalAxisZ);
	//XMVECTOR l = XMLoadFloat3(&AxisZ);
	XMVECTOR position = XMLoadFloat3(&mLocalCoordinatePosition);
	XMStoreFloat3(&mLocalCoordinatePosition, XMVectorMultiplyAdd(s, l, position));
}



//********************************************************************************************************************
// Get Global Matrix
//********************************************************************************************************************
XMMATRIX LocalCoordinateSystem::GetGlobalMatrix()const
{
	return XMLoadFloat4x4(&mGlobalCoordinateSystem); 
}

//********************************************************************************************************************
// Update Global Coordinate Matrix
//********************************************************************************************************************
void LocalCoordinateSystem::UpdateGlobalCoordinateMatrix()
{

	//--------------------------------------------
	// Dla globalnych
	//--------------------------------------------


	XMVECTOR AxisX = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
	XMVECTOR AxisY = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
	XMVECTOR AxisZ = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 1.0f));

	XMVECTOR Position = XMLoadFloat3(&mLocalCoordinatePosition);



	// Wypelnianie 
	float x = -XMVectorGetX(XMVector3Dot(Position, AxisX));
	float y = -XMVectorGetX(XMVector3Dot(Position, AxisY));
	float z = -XMVectorGetX(XMVector3Dot(Position, AxisZ));

	XMStoreFloat3(&mGlobalAxisX, AxisX);
	XMStoreFloat3(&mGlobalAxisY, AxisY);
	XMStoreFloat3(&mGlobalAxisZ, AxisZ);

	mGlobalCoordinateSystem(0, 0) = mGlobalAxisX.x;
	mGlobalCoordinateSystem(1, 0) = mGlobalAxisX.y;
	mGlobalCoordinateSystem(2, 0) = mGlobalAxisX.z;
	mGlobalCoordinateSystem(3, 0) = x;

	mGlobalCoordinateSystem(0, 1) = mGlobalAxisY.x;
	mGlobalCoordinateSystem(1, 1) = mGlobalAxisY.y;
	mGlobalCoordinateSystem(2, 1) = mGlobalAxisY.z;
	mGlobalCoordinateSystem(3, 1) = y;

	mGlobalCoordinateSystem(0, 2) = mGlobalAxisZ.x;
	mGlobalCoordinateSystem(1, 2) = mGlobalAxisZ.y;
	mGlobalCoordinateSystem(2, 2) = mGlobalAxisZ.z;
	mGlobalCoordinateSystem(3, 2) = z;

	mGlobalCoordinateSystem(0, 3) = 0.0f;
	mGlobalCoordinateSystem(1, 3) = 0.0f;
	mGlobalCoordinateSystem(2, 3) = 0.0f;
	mGlobalCoordinateSystem(3, 3) = 1.0f;



}

//********************************************************************************************************************
// Update Global Coordinate Matrix ONLY POSITION
//********************************************************************************************************************
void LocalCoordinateSystem::UpdateGlobalPositionCoordinateMatrix(XMFLOAT4X4 m)
{

	mGlobalPositionCoordinateSystem = m;

	// Wypelnianie 
	float x = -mLocalCoordinatePosition.x;
	float y = -mLocalCoordinatePosition.y;
	float z = -mLocalCoordinatePosition.z;


	///*
	mGlobalPositionCoordinateSystem(0, 0) = 1.0f;
	mGlobalPositionCoordinateSystem(1, 0) = 0.0f;
	mGlobalPositionCoordinateSystem(2, 0) = 0.0f;
	mGlobalPositionCoordinateSystem(3, 0) = m(3,0); 
	//mGlobalPositionCoordinateSystem(3, 0) = x;

	mGlobalPositionCoordinateSystem(0, 1) = 0.0f;
	mGlobalPositionCoordinateSystem(1, 1) = 1.0f;
	mGlobalPositionCoordinateSystem(2, 1) = 0.0f;
    //mGlobalPositionCoordinateSystem(3, 1) = m(3,1);
	//mGlobalPositionCoordinateSystem(3, 1) = y;

	mGlobalPositionCoordinateSystem(0, 2) = 0.0f;
	mGlobalPositionCoordinateSystem(1, 2) = 0.0f;
	mGlobalPositionCoordinateSystem(2, 2) = 1.0f;
	//mGlobalPositionCoordinateSystem(3, 2) = m(3,2);
	//mGlobalPositionCoordinateSystem(3, 2) = z;

	mGlobalPositionCoordinateSystem(0, 3) = 0.0f;
	mGlobalPositionCoordinateSystem(1, 3) = 0.0f;
	mGlobalPositionCoordinateSystem(2, 3) = 0.0f;
	mGlobalPositionCoordinateSystem(3, 3) = 1.0f;
	//*/

}

//********************************************************************************************************************
// Get Global Position Matrix
//********************************************************************************************************************
XMMATRIX LocalCoordinateSystem::GetGlobalPositionMatrix()const
{
	return XMLoadFloat4x4(&mGlobalPositionCoordinateSystem);
}




//********************************************************************************************************************************************************************
// GLOBAL + LOCAL COORDINATE SYSTEM
//********************************************************************************************************************************************************************


//********************************************************************************************************************
// Get Matrix
//********************************************************************************************************************
XMMATRIX LocalCoordinateSystem::GetMatrix()
{
	//mMatrix = XMMatrixMultiply(GetGlobalMatrix(), GetLocalMatrix());
	//return mMatrix;

	return GetLocalMatrix();
}

//********************************************************************************************************************
// Update Global Coordinate Matrix
//********************************************************************************************************************
void LocalCoordinateSystem::UpdateCoordinateMatrix()
{
	//UpdateGlobalCoordinateMatrix();
	UpdateLocalCoordinateMatrix();	
}
//********************************************************************************************************************
// Set Matrix
//********************************************************************************************************************
void LocalCoordinateSystem::setMatrix(XMFLOAT4X4 matrix)
{
	mMatrix = XMLoadFloat4x4(&matrix);
	//mMatrix = matrix;
}

//********************************************************************************************************************
//Add scale vector
//********************************************************************************************************************
void LocalCoordinateSystem::ScaleAdd(XMFLOAT3 scale)
{
	mScaleVector.x += scale.x;
	mScaleVector.y += scale.y;
	mScaleVector.z += scale.z;
}
//********************************************************************************************************************
//Add scale vector
//********************************************************************************************************************
void LocalCoordinateSystem::SetScaleInverse(XMFLOAT3 scale)
{
	scale.x = 1.0f / scale.x;
	scale.y = 1.0f / scale.y;
	scale.z = 1.0f / scale.z;
	mScaleVector = scale;
}
void LocalCoordinateSystem::SetScale(XMFLOAT3 scale)
{
	mScaleVector = scale;
}
//********************************************************************************************************************
//Add scale vector
//********************************************************************************************************************
XMFLOAT3 LocalCoordinateSystem::GetScale()
{
	return mScaleVector;
}


void LocalCoordinateSystem::SetPitch(float value)
{
	this->mPitch = value;
}
void LocalCoordinateSystem::SetYaw(float value)
{
	this->mYaw = value;
}
void LocalCoordinateSystem::SetRoll(float value)
{
	this->mRoll = value;
}
float LocalCoordinateSystem::GetPitch()
{
	return this->mPitch;
}
float LocalCoordinateSystem::GetYaw()
{
	return this->mYaw;
}
float LocalCoordinateSystem::GetRoll()
{
	return this->mRoll;
}

//********************************************************************************************************************
//Create rotation matrix (pitch, yaw , roll) and update normal vectors
//********************************************************************************************************************
void  LocalCoordinateSystem::SetMatrixRotationRollPitchYaw(float pitch, float yaw, float roll)
{
	pitch = XMConvertToRadians(pitch);
	yaw = XMConvertToRadians(yaw);
	roll = XMConvertToRadians(roll);

	//Counting new rotation matrix.
	XMMATRIX matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	SetPitch(XMConvertToDegrees(pitch));
	SetRoll(XMConvertToDegrees(roll));
	SetYaw(XMConvertToDegrees(yaw));

	XMStoreFloat3(&mLocalAxisX, XMVector3TransformNormal(XMLoadFloat3(&mGlobalAxisX), matrix));
	XMStoreFloat3(&mLocalAxisY, XMVector3TransformNormal(XMLoadFloat3(&mGlobalAxisY), matrix));
	XMStoreFloat3(&mLocalAxisZ, XMVector3TransformNormal(XMLoadFloat3(&mGlobalAxisZ), matrix));

	UpdateLocalCoordinateMatrix(); //Update rotation matrix.
}