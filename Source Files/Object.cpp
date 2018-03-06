#include "../Header Files/Object.h"
#include "../Header Files/Utility.h"
#include "../Header Files/File.h"
#include "../Header Files/Light.h"


//*************************************************************************************************************************************
Object::Object()
{
}
Object::Object(std::string name)
{
	this->mName = name;
}
Object::Object(std::string name, std::string tag, KLMESH* klmesh, AxisAlignedBox axisAlignedBox, LocalCoordinateSystem coordinate, bool isCastShadow, float shadowDistance, bool isVisible, int type, PointLight* light, int forceLODLevel)// , wxTreeItemId treeItemId)
{
	this->mName = name;
	this->mTAG = tag;
	this->mKLMESH = klmesh;
	this->mAxisAlignedBox = axisAlignedBox;
	mCoordinate = new LocalCoordinateSystem();
	mCoordinate->setLocalAxisX(coordinate.GetLocalAxisX());
	mCoordinate->setLocalAxisY(coordinate.GetLocalAxisY());
	mCoordinate->setLocalAxisZ(coordinate.GetLocalAxisZ());
	mCoordinate->SetLocalCoordinatePosition(coordinate.GetLocalCoordinatePosition());
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, coordinate.GetMatrix());
	mCoordinate->setMatrix(matrix);
	mCoordinate->SetPitch(coordinate.GetPitch());
	mCoordinate->SetYaw(coordinate.GetYaw());
	mCoordinate->SetRoll(coordinate.GetRoll());
	mCoordinate->SetScale(coordinate.GetScale());

	this->mIsCastShadow = isCastShadow;
	this->mShadowDistance = shadowDistance;
	this->mIsVisible = isVisible;
	this->mIsSelected = false;
	this->mForceLODLevel = forceLODLevel;
	//this->mTreeItemId = treeItemId;
	this->mType = type;
	this->mTempDistance = -1.0f;
	PointLight* lightNew = new PointLight();
	if (light != nullptr)
	{
		lightNew->Ambient = light->Ambient;
		lightNew->Diffuse = light->Diffuse;
		lightNew->Specular = light->Specular;
		lightNew->Att = light->Att;
		lightNew->Pad = light->Pad;
		lightNew->Position = light->Position;
		lightNew->Range = light->Range;
		this->mPointLight = lightNew;
	}
	else
	{
		this->mPointLight = nullptr;
	}

}
//*************************************************************************************************************************************
Object::~Object()
{
}
//*************************************************************************************************************************************
void Object::SetFileName(std::string fileName)
{
	this->mName = fileName;
}
void Object::SetTAG(std::string tag)
{
	this->mTAG = tag;
}
void Object::SetKLMESH(KLMESH* klmesh)
{
	this->mKLMESH = klmesh;
}
void Object::SetAxisAlignedBoundingBox(AxisAlignedBox aabb)
{
	this->mAxisAlignedBox = aabb;
}
void Object::SetCoordinate(LocalCoordinateSystem coordinate)
{
	mCoordinate = new LocalCoordinateSystem();
	mCoordinate->setLocalAxisX(coordinate.GetLocalAxisX());
	mCoordinate->setLocalAxisY(coordinate.GetLocalAxisY());
	mCoordinate->setLocalAxisZ(coordinate.GetLocalAxisZ());
	mCoordinate->SetLocalCoordinatePosition(coordinate.GetLocalCoordinatePosition());
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, coordinate.GetMatrix());
	mCoordinate->setMatrix(matrix);
	mCoordinate->SetPitch(coordinate.GetPitch());
	mCoordinate->SetYaw(coordinate.GetYaw());
	mCoordinate->SetRoll(coordinate.GetRoll());
	mCoordinate->SetScale(coordinate.GetScale());
}
void Object::SetIsCastShadow(bool isCastShadow)
{
	this->mIsCastShadow = isCastShadow;
}
void Object::SetShadowDistance(float shadowDistance)
{
	this->mShadowDistance = shadowDistance;
}
void Object::SetIsVisible(bool visible)
{
	this->mIsVisible= visible;
}
void Object::SetSelected(bool selected)
{
	this->mIsSelected = selected;
}
void Object::SetTreeItemId(wxTreeItemId treeItemId)
{
	this->mTreeItemId = treeItemId;
}
//*************************************************************************************************************************************
std::string Object::GetFileName()
{
	return this->mName;
}
std::string Object::GetTAG()
{
	return this->mTAG;
}
KLMESH* Object::GetKLMESH()
{
	return this->mKLMESH;
}
AxisAlignedBox Object::GetAxisAlignedBoundingBox()
{
	return this->mAxisAlignedBox;
}
LocalCoordinateSystem* Object::GetCoordinate()
{
	return mCoordinate;
}
bool Object::GetIsCastShadow()
{
	return this->mIsCastShadow;
}
float Object::GetShadowDistance()
{
	return this->mShadowDistance;
}
bool Object::GetIsVisible()
{
	return this->mIsVisible;
}
bool Object::GetSelected()
{
	return this->mIsSelected;
}
wxTreeItemId Object::GetTreeItemId()
{
	return this->mTreeItemId;
}
//*************************************************************************************************************************************
void Object::TransformAABB()
{

}
//*************************************************************************************************************************************
void Object::SetType(int type)
{
	this->mType = type;
}
int Object::GetType()
{
	return this->mType;
}
//*************************************************************************************************************************************
void Object::SetForceLODLevel(int type)
{
	this->mForceLODLevel = type;
}
int Object::GetForceLODLevel()
{
	return this->mForceLODLevel;
}
//*************************************************************************************************************************************
void Object::SetTempDistance(float distance)
{
	this->mTempDistance = distance;
}
float Object::GetTempDistance()
{
	return this->mTempDistance;
}
//*************************************************************************************************************************************
PointLight* Object::GetPointLight()
{
	return this->mPointLight;
}
void Object::SetPointLight(PointLight* light)
{
	this->mPointLight = light;
}

