#ifndef OBJECT_H
#define OBJECT_H

#include "Utility.h"
#include "LocalCoordinateSystem.h"
#include "KLMESH.h"
#include "../XNACollision/xnacollision.h"
#include "Light.h"



//------------------------------------------------------------------------------------------
// Class Object
//------------------------------------------------------------------------------------------
class Object
{
public:
	Object();
	Object(std::string name);
	Object(std::string name, std::string tag, KLMESH* klmesh, AxisAlignedBox axisAlignedBox, LocalCoordinateSystem coordinate, bool isCastShadow, float shadowDistance, bool isVisible, int type, PointLight* light, int forceLODLevel);// , wxTreeItemId treeItemId);
	//type 0 = object
	//type 1 = light
	~Object();

	//Name
	void SetFileName(std::string fileName);
	std::string GetFileName();

	//Tag
	void SetTAG(std::string tag);
	std::string GetTAG();

	//KLMESH
	KLMESH* GetKLMESH();
	void SetKLMESH(KLMESH* klmesh);

	//AxisAlignedBoundingBox
	void SetAxisAlignedBoundingBox(AxisAlignedBox aabb);
	AxisAlignedBox GetAxisAlignedBoundingBox();

	//Coordinate
	void SetCoordinate(LocalCoordinateSystem coordinate);
	LocalCoordinateSystem* GetCoordinate();

	//Cast shadow
	void SetIsCastShadow(bool isCastShadow);
	bool GetIsCastShadow();

	//Shadow Distance - overrided value
	void SetShadowDistance(float shadowDistance);
	float GetShadowDistance();

	//IsVisible
	void SetIsVisible(bool visible);
	bool GetIsVisible();

	//Selected
	void SetSelected(bool selected);
	bool GetSelected();

	//TreeItemID
	wxTreeItemId GetTreeItemId();
	void SetTreeItemId(wxTreeItemId treeItemId);

	//Transform AxisAlignedBoundingBox
	void TransformAABB();

	void SetType(int type);
	int GetType();

	void SetForceLODLevel(int type);
	int GetForceLODLevel();

	void SetTempDistance(float distance);
	float GetTempDistance();

	PointLight* GetPointLight();
	void SetPointLight(PointLight* light);


private:
	std::string mName; 
	std::string mTAG;
	KLMESH* mKLMESH;
	AxisAlignedBox mAxisAlignedBox; //Select detector
	LocalCoordinateSystem* mCoordinate;
	bool mIsCastShadow;
	float  mShadowDistance;
	bool mIsVisible;
	bool mIsSelected;
	wxTreeItemId mTreeItemId;
	int mForceLODLevel;
	float mTempDistance;

	int mType; //-1 NULL, 0 KLMESH, 1 LIHGHT
	PointLight* mPointLight;
};
#endif //OBJECT_H