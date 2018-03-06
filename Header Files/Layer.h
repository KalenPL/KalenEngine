#ifndef LAYER_H
#define LAYER_H


#include "Utility.h"
#include "Object.h"

//------------------------------------------------------------------------------------------
// Class Layer
//------------------------------------------------------------------------------------------
class Layer
{
public:
	Layer(wxString name, wxTreeItemId rootTreeID);
	//Name
	wxString GetName();
	void SetName(wxString name);

	//RootTreeId
	wxTreeItemId GetRootTreeId();
	void SetRootTreeId(wxTreeItemId root);

	void LoadLayerFromFile(wxString fileName);
	void SaveLayerToFile(wxString fileName);

	void AddObject(Object* object, wxTreeItemId treeID);
	void AddAllObjects(std::vector<Object*>* objectBuffer);
	void DeleteObject(wxTreeItemId treeID);
	void DeleteAllObjects();
	Object* GetObject(wxTreeItemId treeID);
	std::vector<Object*>* GetAllObjects();
	void RenameSelectedObjects();

	void HideAllObjecys();
	void ShowAllObjects();

	void SetIsActive(bool active);
	bool GetIsActive();

	void SetIsShow(bool show);
	bool GetIsShow();

	void SetIsLoad(bool show);
	bool GetIsLoad();

private:
	//functions
	int FindPositionByTreeId(wxTreeItemId);
	
	//values
	wxString mName;
	wxTreeItemId mRootTreeId;
	bool mIsActive;
	bool mIsShow;
	bool mIsLoad;
	std::vector<Object*>* mObjectsContainer;


};
#endif //!LAYER_H
