#ifndef OCTREE_H
#define OCTREE_H

#include "Utility.h"
#include "Object.h"
#include "../XNACollision/xnacollision.h"

//------------------------------------------------------------------------------------------
// Class OctreeNode
//------------------------------------------------------------------------------------------
class OctreeNode
{
public:

	OctreeNode(int levels, XMFLOAT3 vectroMax, XMFLOAT3 vectroMin, std::string name);
	~OctreeNode();
	void AddObject(Object* object);
	void DeleteObject(Object* object);
	//void ClearObjects();
	//AxisAlignedBox GetAxisAlignedBox();
	//int GetLevel();
	//XMFLOAT3 GetVectroMax();
	//XMFLOAT3 GetVectroMin();
	//XMFLOAT3 GetVectroCentre();

	std::string GetName();
	std::vector<OctreeNode*>* GetNodes();
	OctreeNode* GetChildren(std::string name);

	std::vector<Object*>* Test(Frustum frustum);
private:
	std::string mName;
	AxisAlignedBox mAABB;
	std::vector<OctreeNode*>* mArrayOctreeNode;//8 children
	std::vector<Object*>* mBufferVector;
	int mLevel;
	XMFLOAT3 mVectroMax;
	XMFLOAT3 mVectroMin;
	XMFLOAT3 mVectroCentre;


};


//------------------------------------------------------------------------------------------
// Class Octree
//------------------------------------------------------------------------------------------
class Octree
{
public:

	Octree(int levels, XMFLOAT3 vectroMax, XMFLOAT3 vectroMin);
	~Octree();
	std::vector<Object*>* Test(Frustum frustum);//return all visible objects
	void AddObject(Object* Object);
	void DeleteObject(Object* Object);
	OctreeNode* GetRootNode();
private:
	OctreeNode* mFirstNode;
};
#endif //!OCTREE_H