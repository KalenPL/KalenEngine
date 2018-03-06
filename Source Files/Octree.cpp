#include "../Header Files/Octree.h"


//**************************************************************************************************************
// OctreeNode
//**************************************************************************************************************

OctreeNode::OctreeNode(int levels, XMFLOAT3 vectroMax, XMFLOAT3 vectroMin, std::string name)
{
	mName = name;
	mArrayOctreeNode = new std::vector<OctreeNode*>();
	mBufferVector = new std::vector<Object*>();

	mLevel = levels;
	mVectroMax = vectroMax;
	mVectroMin = vectroMin;
	mVectroCentre.x = vectroMax.x + vectroMin.x;
	mVectroCentre.y = vectroMax.y + vectroMin.y;
	mVectroCentre.z = vectroMax.z + vectroMin.z;

	mVectroCentre.x /= 2.0f;
	mVectroCentre.y /= 2.0f;
	mVectroCentre.z /= 2.0f;

	//Create AABB
	mAABB.Center.x = (mVectroMin.x + mVectroMax.x) * 0.5f;
	mAABB.Center.y = (mVectroMin.y + mVectroMax.y) * 0.5f;
	mAABB.Center.z = (mVectroMin.z + mVectroMax.z) * 0.5f;

	mAABB.Extents.x = (mVectroMax.x - mVectroMin.x) * 0.5f;
	mAABB.Extents.y = (mVectroMax.y - mVectroMin.y) * 0.5f;
	mAABB.Extents.z = (mVectroMax.z - mVectroMin.z) * 0.5f;

	//Create children
	if (mLevel > 0)
	{
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroCentre.x, mVectroCentre.y, mVectroCentre.z), XMFLOAT3(mVectroMin.x, mVectroMin.y, mVectroMin.z), "---"));//---
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroMax.x, mVectroCentre.y, mVectroCentre.z), XMFLOAT3(mVectroCentre.x, mVectroMin.y, mVectroMin.z), "+--"));//+--
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroCentre.x, mVectroMax.y, mVectroCentre.z), XMFLOAT3(mVectroMin.x, mVectroCentre.y, mVectroMin.z), "-+-"));//-+-
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroCentre.x, mVectroCentre.y, mVectroMax.z), XMFLOAT3(mVectroMin.x, mVectroMin.y, mVectroCentre.z), "--+"));//--+

		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroMax.x, mVectroCentre.y, mVectroMax.z), XMFLOAT3(mVectroCentre.x, mVectroMin.y, mVectroCentre.z), "+-+"));//+-+
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroCentre.x, mVectroMax.y, mVectroMax.z), XMFLOAT3(mVectroMin.x, mVectroCentre.y, mVectroCentre.z), "-++"));//-++
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroMax.x, mVectroMax.y, mVectroCentre.z), XMFLOAT3(mVectroCentre.x, mVectroCentre.y, mVectroMin.z), "++-"));//++-
		mArrayOctreeNode->push_back(new OctreeNode(mLevel - 1, XMFLOAT3(mVectroMax.x, mVectroMax.y, mVectroMax.z), XMFLOAT3(mVectroCentre.x, mVectroCentre.y, mVectroCentre.z), "+++"));//+++
	}

}
//**************************************************************************************************************
std::string OctreeNode::GetName()
{
	return this->mName;
}
//**************************************************************************************************************
OctreeNode* OctreeNode::GetChildren(std::string name)
{
	for (int i = 0; i < (int)mArrayOctreeNode->size(); ++i)
		if (mArrayOctreeNode->at(i)->GetName() == name)
			return mArrayOctreeNode->at(i);

	return nullptr;
}
//**************************************************************************************************************
std::vector<OctreeNode*>* OctreeNode::GetNodes()
{
	return mArrayOctreeNode;
}
//**************************************************************************************************************
void OctreeNode::DeleteObject(Object* Object)
{
	for (int i = 0; i < (int)mBufferVector->size(); ++i)
		if (mBufferVector->at(i) == Object)
		{
			//Delete object
			mBufferVector->erase(mBufferVector->begin() + i);
		}
	if (mLevel > 0)
	{
		for (int i = 0; i < (int)mArrayOctreeNode->size(); ++i)
		{
			mArrayOctreeNode->at(i)->DeleteObject(Object);
		}
	}
}
//**************************************************************************************************************
void OctreeNode::AddObject(Object* Object)
{
	for (int i = 0; i < (int)mBufferVector->size(); ++i)
		if (mBufferVector->at(i) == Object)
		{
			//Delete object
			mBufferVector->erase(mBufferVector->begin() + i);
		}
	mBufferVector->push_back(Object);
	XMFLOAT3 AABB_Center = Object->GetAxisAlignedBoundingBox().Center;
	XMFLOAT3 AABB_Extents = Object->GetAxisAlignedBoundingBox().Extents;
	XMFLOAT3 maxVector;
	maxVector.x = AABB_Center.x + AABB_Extents.y;
	maxVector.y = AABB_Center.y + AABB_Extents.y;
	maxVector.z = AABB_Center.z + AABB_Extents.z;

	XMFLOAT3 minVector;
	minVector.x = AABB_Center.x - AABB_Extents.y;
	minVector.y = AABB_Center.y - AABB_Extents.y;
	minVector.z = AABB_Center.z - AABB_Extents.z;

	std::vector<XMFLOAT3>* vectorPoints = new std::vector<XMFLOAT3>();

	XMFLOAT3 point1 = XMFLOAT3(minVector.x, minVector.y, minVector.z);
	XMFLOAT3 point2 = XMFLOAT3(maxVector.x, minVector.y, minVector.z);
	XMFLOAT3 point3 = XMFLOAT3(maxVector.x, maxVector.y, minVector.z);
	XMFLOAT3 point4 = XMFLOAT3(minVector.x, maxVector.y, minVector.z);

	XMFLOAT3 point5 = XMFLOAT3(minVector.x, minVector.y, maxVector.z);
	XMFLOAT3 point6 = XMFLOAT3(maxVector.x, minVector.y, maxVector.z);
	XMFLOAT3 point7 = XMFLOAT3(maxVector.x, maxVector.y, maxVector.z);
	XMFLOAT3 point8 = XMFLOAT3(minVector.x, maxVector.y, maxVector.z);

	vectorPoints->push_back(point1);
	vectorPoints->push_back(point2);
	vectorPoints->push_back(point3);
	vectorPoints->push_back(point4);
	vectorPoints->push_back(point5);
	vectorPoints->push_back(point6);
	vectorPoints->push_back(point7);
	vectorPoints->push_back(point8);

	XMFLOAT3 NormalX = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 NormalY = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 NormalZ = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR N = XMLoadFloat3(&NormalX);
	XMVECTOR N2;
	XMVECTOR P;
	XMVECTOR K;
	XMVECTOR WynikX;
	XMVECTOR WynikY;
	XMVECTOR WynikZ;
	
	wxString text = "";

	if (mLevel > 0)
	{
		for (int i = 0; i < (int)vectorPoints->size(); ++i)
		{
			text = "";
			//X
			N = XMLoadFloat3(&NormalX);
			N2 = XMVectorNegate(N);
			P = XMLoadFloat3(&vectorPoints->at(i));
			K = XMLoadFloat3(&mVectroCentre);
			WynikX = XMVectorAdd(XMVector3Dot(N, P), XMVector3Dot(N2, K));

			//Y
			N = XMLoadFloat3(&NormalY);
			N2 = XMVectorNegate(N);
			P = XMLoadFloat3(&vectorPoints->at(i));
			K = XMLoadFloat3(&mVectroCentre);
			WynikY = XMVectorAdd(XMVector3Dot(N, P), XMVector3Dot(N2, K));

			//Z
			N = XMLoadFloat3(&NormalZ);
			N2 = XMVectorNegate(N);
			P = XMLoadFloat3(&vectorPoints->at(i));
			K = XMLoadFloat3(&mVectroCentre);
			WynikZ = XMVectorAdd(XMVector3Dot(N, P), XMVector3Dot(N2, K));

			if(XMVectorGetX(WynikX) >= 0)
				text << "+";
			else
				text << "-";

			if (XMVectorGetX(WynikY) >= 0)
				text << "+";
			else
				text << "-";

			if (XMVectorGetX(WynikZ) >= 0)
				text << "+";
			else
				text << "-";

			GetChildren((std::string)text)->AddObject(Object);
		}
	}


}
//**************************************************************************************************************
std::vector<Object*>* OctreeNode::Test(Frustum frustum)
{
	int value = IntersectAxisAlignedBoxFrustum(&mAABB, &frustum);
	if (value == 0)//no intersection
	{
		return nullptr;
	}
	if (value == 1)//intersection
	{
		if(mLevel == 0)
			return mBufferVector;
		bool isExists = false;
		std::vector<Object*>* sumVector = new std::vector<Object*>();
		std::vector<Object*>* tempVector = new std::vector<Object*>();
		for (int i = 0; i < (int)mArrayOctreeNode->size(); ++i)
		{
			//Get objects
			tempVector = mArrayOctreeNode->at(i)->Test(frustum);
			//Compare and add
			if(tempVector != nullptr)
				for (int k = 0; k < (int)tempVector->size(); ++k)
				{
					/*isExists = false;
					for (int j = 0; j < (int)sumVector->size(); ++j)
						if (tempVector->at(k) == sumVector->at(j))
						{
							isExists = true;
						}

					if (isExists == false)*/
						sumVector->push_back(tempVector->at(k));
				}
		}
		return sumVector;
		//return nullptr;
	}
	if (value == 2)//box is completely inside frustum
	{
		return mBufferVector; //Return all objects inside this box and inside in the sub-boxes
	}

	return nullptr;
}










//**************************************************************************************************************
// Octree
//**************************************************************************************************************


Octree::Octree(int levels, XMFLOAT3 vectroMax, XMFLOAT3 vectroMin)
{
	mFirstNode = new OctreeNode(levels, vectroMax, vectroMin, "root");
}
void Octree::AddObject(Object* Object)
{
	mFirstNode->AddObject(Object);
}

void Octree::DeleteObject(Object* Object)
{
	mFirstNode->DeleteObject(Object);
}
std::vector<Object*>* Octree::Test(Frustum frustum)
{
	return mFirstNode->Test(frustum);
}

OctreeNode* Octree::GetRootNode()
{
	return  this->mFirstNode;
}