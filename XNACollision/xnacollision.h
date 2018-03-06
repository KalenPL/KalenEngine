//-------------------------------------------------------------------------------------
// XNACollision.h
//  
// An opimtized collision library based on XNAMath
//  
// Microsoft XNA Developer Connection
// Copyright (c) Microsoft Corporation. Wszelkie prawa zastrzeżone.
//-------------------------------------------------------------------------------------



#pragma once

#ifndef _XNA_COLLISION_H_
#define _XNA_COLLISION_H_
//namespace XNA
//{


#include <DirectXMath.h>
using namespace DirectX;
//#include "../xnamath.h"



struct AxisAlignedBox
{
	XMFLOAT3 Center;            // Center of the box.
	XMFLOAT3 Extents;           // Distance from the center to each side.
};

struct Frustum
{
	XMFLOAT3 Origin;            // Origin of the frustum (and projection).
	XMFLOAT4 Orientation;       // Unit quaternion representing rotation.

	FLOAT RightSlope;           // Positive X slope (X/Z).
	FLOAT LeftSlope;            // Negative X slope.
	FLOAT TopSlope;             // Positive Y slope (Y/Z).
	FLOAT BottomSlope;          // Negative Y slope.
	FLOAT Near, Far;            // Z of the near plane and far plane.
};

struct OrientedBox
{
	XMFLOAT3 Center;            // Center of the box.
	XMFLOAT3 Extents;           // Distance from the center to each side.
	XMFLOAT4 Orientation;       // Unit quaternion representing rotation (box -> world).
};

VOID TransformFrustum(Frustum* pOut, const Frustum* pIn, FLOAT Scale, FXMVECTOR Rotation, FXMVECTOR Translation);
INT IntersectAxisAlignedBoxFrustum(const AxisAlignedBox* pVolumeA, const Frustum* pVolumeB);
VOID ComputeFrustumFromProjection(Frustum* pOut, XMMATRIX* pProjection);
INT IntersectOrientedBoxFrustum(const OrientedBox* pVolumeA, const Frustum* pVolumeB);
VOID TransformAxisAlignedBox(AxisAlignedBox* pOut, const AxisAlignedBox* pIn, FLOAT Scale, FXMVECTOR scaleVector, FXMVECTOR Rotation, FXMVECTOR Translation);

bool IntersectRayAxisAlignedBox(FXMVECTOR Origin, FXMVECTOR Direction, const AxisAlignedBox* pVolume, FLOAT* pDist);
bool IntersectRayTriangle(FXMVECTOR Origin, FXMVECTOR Direction, FXMVECTOR V0, CXMVECTOR V1, CXMVECTOR V2, FLOAT* pDist);
bool IntersectRayOrientedBox(FXMVECTOR Origin, FXMVECTOR Direction, const OrientedBox* pVolume, FLOAT* pDist);

//}; // namespace
#endif