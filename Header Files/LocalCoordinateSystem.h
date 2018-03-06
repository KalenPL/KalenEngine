#ifndef LOCAL_COORDINATE_SYSTEM_H
#define LOCAL_COORDINATE_SYSTEM_H

#include "Utility.h"
using namespace DirectX;


class LocalCoordinateSystem
{
public:
	LocalCoordinateSystem();
	~LocalCoordinateSystem();


	XMFLOAT3 GetLocalCoordinatePosition()const;
	XMVECTOR GetLocalCoordinatePositionVECTOR()const;
	void SetLocalCoordinatePosition(float x, float y, float z);
	void SetLocalCoordinatePosition(const XMFLOAT3& vector);
	

	//--------------------------------------------------
	// Local coordinate system
	//--------------------------------------------------

	XMFLOAT3 GetLocalAxisX()const;
	XMFLOAT3 GetLocalAxisY()const;
	XMFLOAT3 GetLocalAxisZ()const;

	void setLocalAxisX(const XMFLOAT3& vx);
	void setLocalAxisY(const XMFLOAT3& vy);
	void setLocalAxisZ(const XMFLOAT3& vz);

	void setLocalAxisX(float x, float y, float z);
	void setLocalAxisY(float x, float y, float z);
	void setLocalAxisZ(float x, float y, float z);

	void Pitch(float angle);	//X axis
	void Yaw(float angle);		//Y axis
	void Roll(float angle);		//Z axis



	void SetPitch(float value);
	void SetYaw(float value);
	void SetRoll(float value);
	float GetPitch();
	float GetYaw();
	float GetRoll();

	void SetMatrixRotationRollPitchYaw(float pitch, float yaw, float roll);

	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);

	XMMATRIX GetLocalMatrix() const;
	void UpdateLocalCoordinateMatrix();

	void setMatrix(XMFLOAT4X4 matrix);

	//--------------------------------------------------
	// Global coordinate system
	//--------------------------------------------------

	XMFLOAT3 GetGlobalAxisX()const;
	XMFLOAT3 GetGlobalAxisY()const;
	XMFLOAT3 GetGlobalAxisZ()const;

	void Pitch_GlobalAxis(float angle);		//X axis
	void Yaw_GlobalAxis(float angle);		//Y axis
	void Roll_GlobalAxis(float angle);		//Z axis

	void MoveGlobalX(float x);
	void MoveGlobalY(float y);
	void MoveGlobalZ(float z);

	void ScaleAdd(XMFLOAT3 scale);
	void SetScale(XMFLOAT3 scale);
	void SetScaleInverse(XMFLOAT3 scale);
	XMFLOAT3 GetScale();

	XMMATRIX GetGlobalMatrix() const;
	void UpdateGlobalCoordinateMatrix();

	void UpdateGlobalPositionCoordinateMatrix(XMFLOAT4X4 m); // Only position
	XMMATRIX GetGlobalPositionMatrix()const; // Only position
	//--------------------------------------------------
	// Global + Local coordinate system
	//--------------------------------------------------
	void UpdateCoordinateMatrix();
	XMMATRIX GetMatrix();

	XMMATRIX GetMatrixToDrawGizmo();
	
private:
	XMMATRIX mMatrix; //Final matrix!!!
	
	//For local coordinate system
	XMFLOAT3 mLocalCoordinatePosition;
	XMFLOAT3 mLocalAxisX; 
	XMFLOAT3 mLocalAxisY; 
	XMFLOAT3 mLocalAxisZ; 
    XMFLOAT4X4 mLocalCoordinateSystem;

	//For global coordinate system
	XMFLOAT3 mGlobalAxisX; 
	XMFLOAT3 mGlobalAxisY; 
	XMFLOAT3 mGlobalAxisZ; 
	XMFLOAT4X4 mGlobalCoordinateSystem;


	XMFLOAT4X4 mGlobalPositionCoordinateSystem;
	float mPitch;
	float mYaw;
	float mRoll;


	XMFLOAT3 mScaleVector;



};


#endif //LOCAL_COORDINATE_SYSTEM_H