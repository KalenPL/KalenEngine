#include "../Header Files/Layer.h"



extern wxTextCtrl	*mTextOututWindow123;
//**************************************************************************************************************
// Constructor 
//**************************************************************************************************************
Layer::Layer(wxString name, wxTreeItemId rootTreeID)
{
	this->mName = name;
	this->mIsActive = false;
	this->mIsShow = true;
	this->mObjectsContainer = new std::vector<Object*>();
	this->mRootTreeId = rootTreeID;
	this->mIsLoad = false;
}
//**************************************************************************************************************
// Get layer name 
//**************************************************************************************************************
wxString Layer::GetName()
{
	return this->mName;
}
//**************************************************************************************************************
// Set layer name 
//**************************************************************************************************************
void Layer::SetName(wxString name)
{
	this->mName = name;
}
//**************************************************************************************************************
// Get root ID as "wxTreeItemId"
//**************************************************************************************************************
wxTreeItemId Layer::GetRootTreeId()
{
	return this->mRootTreeId;
}
//**************************************************************************************************************
// Set root ID as "wxTreeItemId"
//**************************************************************************************************************
void Layer::SetRootTreeId(wxTreeItemId root)
{
	this->mRootTreeId = root;
}
//**************************************************************************************************************
// Load files
//**************************************************************************************************************
void Layer::LoadLayerFromFile(wxString fileName)
{
	//TODO
}
void Layer::SaveLayerToFile(wxString fileName)
{

}
//**************************************************************************************************************
// Add object to layer
//**************************************************************************************************************
void Layer::AddObject(Object* object, wxTreeItemId treeID)
{
	object->SetTreeItemId(treeID);
	mObjectsContainer->push_back(object);
}
//**************************************************************************************************************
// Add all objects to layer
//**************************************************************************************************************
void Layer::AddAllObjects(std::vector<Object*>* objectBuffer)
{

}
//**************************************************************************************************************
// Delete object from layer
//**************************************************************************************************************
void Layer::DeleteObject(wxTreeItemId treeID)
{
	int position = FindPositionByTreeId(treeID);
	if (position != -1)
	{
		mObjectsContainer->erase(mObjectsContainer->begin() + position);
	}
}
//**************************************************************************************************************
// Delete all objects from layer
//**************************************************************************************************************
void Layer::DeleteAllObjects()
{
	mObjectsContainer->clear();
}
//**************************************************************************************************************
// Get object
//**************************************************************************************************************
Object* Layer::GetObject(wxTreeItemId treeID)
{
	int position = FindPositionByTreeId(treeID);
	if (position != -1)
	{
		return mObjectsContainer->at(position);
	}
	else
		return nullptr;
}
//**************************************************************************************************************
// Get all objects
//**************************************************************************************************************
std::vector<Object*>* Layer::GetAllObjects()
{
	return mObjectsContainer;
}
//**************************************************************************************************************
// Rename selected object
//**************************************************************************************************************
void Layer::RenameSelectedObjects()
{
	//TODO
}
//**************************************************************************************************************
// Hide all objects
//**************************************************************************************************************
void Layer::HideAllObjecys()
{
	for (int i = 0; i < (int)mObjectsContainer->size(); ++i)
		mObjectsContainer->at(i)->SetIsVisible(true);
}
//**************************************************************************************************************
// Show all objects
//**************************************************************************************************************
void Layer::ShowAllObjects()
{
	for (int i = 0; i < (int)mObjectsContainer->size(); ++i)
		mObjectsContainer->at(i)->SetIsVisible(false);
}
//**************************************************************************************************************
// Find position in the mObjectsContainer by 'wxTreeItemId'
//**************************************************************************************************************
int Layer::FindPositionByTreeId(wxTreeItemId treeID)
{
	int position = -1;
	for (int i = 0; i < (int)mObjectsContainer->size(); ++i)
	{
		if (mObjectsContainer->at(i)->GetTreeItemId() == treeID)
			position = i;
	}
	//return -1 if object does not exist.
	return position;
}

//**********************************************************************
void Layer::SetIsActive(bool active)
{
	this->mIsActive = active;
}
bool Layer::GetIsActive()
{
	return this->mIsActive;
}
//**********************************************************************
void Layer::SetIsShow(bool show)
{
	this->mIsShow = show;
}
bool Layer::GetIsShow()
{
	return this->mIsShow;
}
//**********************************************************************
void Layer::SetIsLoad(bool isLoad)
{
	this->mIsLoad = isLoad;
}
bool Layer::GetIsLoad()
{
	return this->mIsLoad;
}



