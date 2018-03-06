#include "../Header Files/Utility.h"

wxWindow* gOutputWindowDirectX;
bool gFilterDebugGrid = true;
bool gFilterDebugGizmo = true;
bool gFilterDebugBBoxes = false;
bool gFilterDebugBSphereLight = false;
bool gFilterDebugCollision = false;
bool gFilterDebugSelection = false;
bool gFilterDebugWireframe = false;
bool gFilterRenderMesh = true;
bool gFilterRenderTessellation = true;
bool gFilterRenderRefraction = true;
bool gFilterRenderRefractionDynamic = true;
bool gGlobalSpace = true;
int gTransformGizmoType = 1;
bool gMultiLayers = true;
bool gFilterDebugSelected = false;
bool gFilterSSAO = true;
bool gFilterRenderShadow = true;


//**********************************************************************************************************
Node::Node(Node* parent, wxString name, std::vector<Node*>* allNodeArray, int level)
{
	this->mLevel = level;
	this->mParent = parent;
	wxString path = "";
	if (parent != NULL)
	{
		path = parent->getPathNameNode();
		if (name != "")
			path << "\\" << name;
	}
	else
	{
		path = wxGetCwd();
		path << "\\" << "data";
	}


	this->mPathName = path;
	this->mName = name;
	this->mIsVisited = false;
	this->mChildrenArray = new std::vector<Node*>();

	wxDir* dir = new wxDir(this->mPathName);
	wxString fileName = "";// this->mName;
	wxArrayString* arrayFoldersName = new wxArrayString();
	wxString fileFolderName = "";


	//detect dictionary 
	if (dir->IsOpened())
	{

		dir->GetFirst(&fileFolderName, wxString(), wxDIR_DIRS);
		arrayFoldersName->Add(fileFolderName);
		if (fileFolderName != "")
		{
			dir->GetNext(&fileName);
			for (bool cont = true; cont; cont = dir->GetNext(&fileName))// true-false for
			{
				arrayFoldersName->Add(fileName);
			}
		}
	}

	if (arrayFoldersName->GetCount() == 0)
		this->mIsVisited = true;

	if (this->mIsVisited == false)
	{
		//Create all children
		for (size_t i = 0; i < arrayFoldersName->GetCount(); ++i)
		{
			wxString name = this->getNameNode();
			name << "\\" << arrayFoldersName->Item(i);

			name = arrayFoldersName->Item(i);
			if (name != "") //Is there any folder? 
			{

				Node* nodeChild = new Node(this, name, allNodeArray, level);
				allNodeArray->push_back(nodeChild);
				this->mChildrenArray->push_back(nodeChild); //Add child to parent
				++level;
			}
		}
	}
}
//**********************************************************************************************************
Node::~Node()
{
}
//**********************************************************************************************************
Node* Node::getParent()
{
	return this->mParent;
}
//**********************************************************************************************************
std::vector<Node*>* Node::getChildrenArray()
{
	return this->mChildrenArray;
}

//**********************************************************************************************************
bool Node::getIsVisited()
{
	return this->mIsVisited;
}
//**********************************************************************************************************
wxString Node::getNameNode()
{
	return this->mName;
}
//**********************************************************************************************************
wxString Node::getPathNameNode()
{
	return this->mPathName;

}
//**********************************************************************************************************
int Node::getLevel()
{
	return this->mLevel;
}
//**********************************************************************************************************
void Node::setLevel(int level)
{
	this->mLevel = level;
}

//**********************************************************************************************************
int Node::getArrayFolderPosition()
{
	return this->mArrayFolderPosition;
}
//**********************************************************************************************************
void Node::setArrayFolderPosition(int pos)
{
	this->mArrayFolderPosition = pos;
}
//**********************************************************************************************************

void Node::setTreeItemId(wxTreeItemId item)
{
	this->mTreeItemId = item;
}
//**********************************************************************************************************
wxTreeItemId Node::getTreeItemId()
{
	return this->mTreeItemId;
}