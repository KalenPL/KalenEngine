
#include "wx/wxprec.h"
#include "../Header Files/MainFrame.h"
#include "../Header Files/FiltersWindow.h"
#include "../Header Files/AssetBrowser.h"
#include "../Header Files/KalenEngine.h"
#include "../Header Files/DXApp.h"
#include "../Header Files/BinaryFileRead.h"

//MyFrame* mMainFrame; wxFrame
wxFrame* mMainFrame;
wxTextCtrl        *mTextOututWindow123;
extern wxWindow* gOutputWindowDirectX;
extern int gTransformGizmoType;
extern bool gGlobalSpace;
extern bool gMultiLayers;





class DnDText : public wxTextDropTarget
{
public:
	DnDText(wxWindow *pOwner, MyFrame* myFrame, KalenEngine* kalenEngine)
	{
		mTestWindow = pOwner;
		mMyFrame = myFrame;
		mKalenEngine = kalenEngine;
	}

	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text) wxOVERRIDE;

private:
	wxWindow* mTestWindow;
	MyFrame* mMyFrame;
	KalenEngine* mKalenEngine;
};

bool DnDText::OnDropText(wxCoord x, wxCoord y, const wxString& textInput)
{

	wxString text = "Got the file from Asset Browser!\n";
	text << textInput;
	text << "\nX = ";
	text << x;
	text << "\nY = ";
	text << y;
	

	wxString time = wxNow();
	wxString buffer = "";
	for (int i = 11; i <= 19; ++i)
	{
		buffer.append(time.GetChar(i));
	}
	(*mTextOututWindow123) << "\n<<" << buffer << ">> " << text;

	if (textInput.at(textInput.size() - 1) == 'h' || textInput.at(textInput.size() - 1) == 'H')
	{
		bool isOk = false;
		for (int i = 0; i < (int)mMyFrame->GetLayerBuffer()->size(); ++i)//size of layers
		{
			if (mMyFrame->GetLayerBuffer()->at(i)->GetIsActive())
			{
				isOk = true;
				KLMESH* klmesh;

				//wxProgressDialog* dialog = new wxProgressDialog(wxT("Loading..."), wxT("Keep waiting..."), 1, NULL, wxPD_AUTO_HIDE | wxPD_APP_MODAL);
				//dialog->Update(0);
				wxString folder = wxGetCwd();
				wxString tempString = "";

				for (int k = folder.size() + 1; k < textInput.size(); ++k)
					tempString << textInput.at(k);
				//textInput = (string)tempString;

				//---------------------------
				//Load klmesh if it doesn't exist in the KLMESH_Buffer
				if (!mMyFrame->GetKLMESH_Buffer()->CheckMesh((std::string)tempString)) //if klmesh does not exist in the klmeshBuffer
				{
					BinaryFileRead *binaryFileRead = new BinaryFileRead((std::string)tempString);
					klmeshFileStruct klmeshTmp;
					binaryFileRead->readKLMeshFileStructure(klmeshTmp);
					klmesh = mMyFrame->GetKLMESH_Buffer()->AddMesh(new KLMESH(&klmeshTmp, mKalenEngine->getDevice()));
				}
				else
				{
					klmesh = mMyFrame->GetKLMESH_Buffer()->GetMeshByLink((std::string)tempString);
				}

				//---------------------------
				//Create coordinate of the object
				LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
				XMFLOAT3 pos;

				mKalenEngine->CountRay(&pos, x, y);
				localCoordinate->SetLocalCoordinatePosition(pos);

				//---------------------------
				//Load textures
				for (int j = 0; j < (int)klmesh->GetMaterialBuffer()->size(); ++j)//Size of materials
				{
					if (klmesh->GetMaterialBuffer()->at(j).diffuseMapName != "NULL")
						klmesh->SetMaterialDiffuseMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).diffuseMapName), klmesh->GetMaterialBuffer()->at(j).diffuseMapName);

					if (klmesh->GetMaterialBuffer()->at(j).normalMapName != "NULL")
						klmesh->SetMaterialNormalMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).normalMapName), klmesh->GetMaterialBuffer()->at(j).normalMapName);

					if (klmesh->GetMaterialBuffer()->at(j).ashMapName != "NULL")
						klmesh->SetMaterialASHMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).ashMapName), klmesh->GetMaterialBuffer()->at(j).ashMapName);

					if (klmesh->GetMaterialBuffer()->at(j).cubeMapName != "NULL")
						klmesh->SetMaterialCubeMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).cubeMapName), klmesh->GetMaterialBuffer()->at(j).cubeMapName);
				}

				//---------------------------
				//Create object name
				wxString nameObject = "";
				for (int k = (int)textInput.size() - 8; k >= 0; --k)
					if (textInput.at(k) != '\\')
						nameObject << textInput.at(k);
					else
						break;

				wxString nameObjectSwap = "";
				for (int k = (int)nameObject.size() - 1; k >= 0; --k)
					nameObjectSwap << nameObject.at(k);

				nameObject = nameObjectSwap;
				nameObject << " #";
				nameObject << (int)mMyFrame->GetLayerBuffer()->at(i)->GetAllObjects()->size() + 1;

				//---------------------------
				//Add object to seleceted layer
				wxTreeItemId treeID = mMyFrame->AddObjectToLayer(&mMyFrame->GetLayerBuffer()->at(i)->GetRootTreeId(), nameObject, 0);
				mMyFrame->GetLayerBuffer()->at(i)->AddObject(new Object((string)nameObject, "", klmesh, *klmesh->GetAxisAlignedBoxOrigin(), *localCoordinate, false, 0.0f, true, 0, nullptr, -1), treeID);
				//---------------------------
				//Find open object in the klmeshViwer
				for (int j = 0; j < (int)mMyFrame->GetAssetBrowser()->GetListOfKLMESHViewer()->size(); ++j)
				{
					if (mMyFrame->GetAssetBrowser()->GetListOfKLMESHViewer()->at(j)->GetName() == klmesh->GetLinkToFile())
						mMyFrame->GetAssetBrowser()->GetListOfKLMESHViewer()->at(j)->SetKLMESH(klmesh);
				}
				//---------------------------
				//Add this object to the RenderObjectBuffer
				mKalenEngine->RebuildBoundingBox(mMyFrame->GetLayerBuffer()->at(i)->GetObject(treeID));
				mKalenEngine->GetOctree()->AddObject(mMyFrame->GetLayerBuffer()->at(i)->GetObject(treeID));//Add to octree
				mMyFrame->Refresh();

				//dialog->Update(1);
			//	delete dialog;
			}
		}

		if (isOk == false)
			wxMessageBox("Can not add object - no layer is active", "Error", wxICON_QUESTION | wxOK);
	}

	mKalenEngine->AllowRender(true); //start (one frame) rendering
	return true;
}











wxDECLARE_APP(MyApp);
wxIMPLEMENT_APP(MyApp);
//************************************************************************************************************************************
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

	int width = wxSystemSettings::GetMetric(wxSYS_SCREEN_X) + 20;
	int height = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) - 30;

	//wxDefaultPosition
	mMainFrame = new MyFrame(NULL,
                                 wxID_ANY,
                                 wxT("KalenEngine DirectX 11 - Build 1.0.1 (June 2017)"),
                                 wxPoint(-10, -5),
                                 wxSize(width, height));
	

	mMainFrame->Show();
	
    return true;
}
//************************************************************************************************************************************



//--------------------------------------------------
// MyFrame events table
//--------------------------------------------------
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_CLOSE(MyFrame::OnCloseEngine)
    EVT_ERASE_BACKGROUND(MyFrame::OnEraseBackground) 
	EVT_IDLE(MyFrame::OnIdle)
    EVT_SIZE(MyFrame::OnSize)
	EVT_MOTION(MyFrame::OnUpdateValues)
	EVT_MENU(MyFrame::ID_CreateRightWindow, MyFrame::OnCreateRightWindow)
    EVT_MENU(ID_CustomizeToolbar, MyFrame::OnCustomizeToolbar)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    //EVT_AUITOOLBAR_TOOL_DROPDOWN(ID_DropDownToolbarItem, MyFrame::OnDropDownToolbarItem)
    EVT_AUI_PANE_CLOSE(MyFrame::OnPaneClose)
    EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, MyFrame::OnAllowNotebookDnD)
    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MyFrame::OnNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MyFrame::OnNotebookPageClosed)

	EVT_MENU(ID_ToolItemFilters, MyFrame::OnToolMenuFiltersWindow)
	EVT_MENU(ID_ToolItemAssetBrowser, MyFrame::OnToolMenuAssetBrowsersWindow)

	EVT_MENU(ID_ToolItemOutputWindow, MyFrame::OnToolMenuOutputWindowShowHide)
	

	EVT_MENU(ID_ToolItemACTIVE_L, MyFrame::OnToolMenuActiveLayerClick)
	EVT_MENU(ID_ToolItemMULTI_L, MyFrame::OnToolMenuMultiLayerClick)
	//EVT_MENU(ID_ToolItemMULTI_L, MyFrame::OnToolMenuMultiLayer)
	EVT_MENU(ID_ToolItemGLOBAL, MyFrame::OnToolXGlobal)
	EVT_MENU(ID_ToolItemLOCAL, MyFrame::OnToolXLocal)
	EVT_MENU(ID_ToolItemMOVE, MyFrame::OnToolXMove)
	EVT_MENU(ID_ToolItemROTATE, MyFrame::OnToolXRotate)
	EVT_MENU(ID_ToolItemSCALE, MyFrame::OnToolXScale)
	EVT_MENU(ID_ToolItemCOPY, MyFrame::OnToolXCopy)
	EVT_MENU(ID_ToolItemCUT, MyFrame::OnToolXCut)
	EVT_MENU(ID_ToolItemPASTE, MyFrame::OnToolXPaste)
	EVT_MENU(ID_ToolItemDELETE, MyFrame::OnToolXDelete)
	EVT_MENU(ID_ToolSmallNewWorld, MyFrame::OnToolSNewWorld)
	EVT_MENU(ID_ToolSmallOpenWorld, MyFrame::OnToolSOpenWorld)
	EVT_MENU(ID_ToolSmallRestoreWorld, MyFrame::OnToolSRestoreWorld)
	EVT_MENU(ID_ToolSmallSaveWorld, MyFrame::OnToolSSaveWorld)
	EVT_MENU(ID_ToolSmallCopy, MyFrame::OnToolSCopy)
	EVT_MENU(ID_ToolSmallPaste, MyFrame::OnToolSPaste)
	EVT_MENU(ID_ToolSmallDelete, MyFrame::OnToolSDelete)
	EVT_MENU(ID_ToolSmallAssetBrowser, MyFrame::OnToolSAssetBrowser)
	EVT_MENU(ID_ToolSmallGridMove, MyFrame::OnToolSGridMove)
	EVT_CHOICE(ID_ToolSmallChoiceMove, MyFrame::OnToolSChoiceMove)
	EVT_MENU(ID_ToolSmallGridRotate, MyFrame::OnToolSGridRotate)
	EVT_CHOICE(ID_ToolSmallChoiceRotate, MyFrame::OnToolSChoiceRotate)
	EVT_CHOICE(ID_ToolSmallChoiceSnap, MyFrame::OnToolSChoiceSnap)
	EVT_CHOICE(ID_ToolSmallChoiceRender, MyFrame::OnToolSChoiceRender)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_TreeScene, MyFrame::OnTreeRightClick)
	EVT_TREE_ITEM_ACTIVATED(ID_TreeScene, MyFrame::OnTreeLeftDoubleClick)
	EVT_TREE_SEL_CHANGING(ID_TreeScene, MyFrame::OnTreeSeletionING)
	EVT_TREE_SEL_CHANGED(ID_TreeScene, MyFrame::OnTreeSeletionED)
	EVT_MENU(ID_ToolItemMenuUndo, MyFrame::OnToolMenuUndo)
	EVT_MENU(ID_ToolItemMenuRedo, MyFrame::OnToolMenuRedo)
	EVT_MENU(ID_ToolItemMenuCopy, MyFrame::OnToolSCopy)
	EVT_MENU(ID_ToolItemMenuCut, MyFrame::OnToolMenuCut)
	EVT_MENU(ID_ToolItemMenuPaste, MyFrame::OnToolSPaste)
	EVT_MENU(ID_ToolItemMenuDelete, MyFrame::OnToolSDelete)
	EVT_MENU(ID_ToolItemMenuLookAt, MyFrame::OnToolMenuLookAt)
	EVT_MENU(ID_ToolItemMenuCancel, MyFrame::OnToolMenuCancel)
	EVT_MENU(ID_ToolItemMenuActiveLayer, MyFrame::OnToolMenuActiveLayer)
	EVT_MENU(ID_ToolItemMenuMultiLayer, MyFrame::OnToolMenuActiveLayer)
	EVT_MENU(ID_ToolItemMenuMove, MyFrame::OnToolMenuMove)
	EVT_MENU(ID_ToolItemMenuRotate, MyFrame::OnToolMenuRotate)
	EVT_MENU(ID_ToolItemMenuScale, MyFrame::OnToolMenuScale)
	EVT_MENU(ID_ToolItemMenuSnap, MyFrame::OnToolMenuSnap)
	EVT_MENU(ID_ToolItemMenuSnapTerrain, MyFrame::OnToolMenuSnapTerrain)
	EVT_MENU(ID_ToolItemMenuSnapAll, MyFrame::OnToolMenuSnapAll)
	EVT_MENU(ID_ToolItemMenuLocal, MyFrame::OnToolMenuLocal)
	//EVT_MENU(ID_ToolItemMenuGlobal, MyFrame::OnToolMenuGlobal)
	EVT_MENU(ID_ToolItemMenuGlobal, MyFrame::OnToolMenuLocal)
	EVT_MENU(ID_TreeSceneMenuFolderSaveHier, MyFrame::OnTreeSceneMenuFolderSaveHier)
	EVT_MENU(ID_TreeSceneMenuFolderSaveFolder, MyFrame::OnTreeSceneMenuFolderSaveFolder)
	EVT_MENU(ID_TreeSceneMenuFolderNewLayer, MyFrame::OnTreeSceneMenuFolderNewLayer)
	EVT_MENU(ID_TreeSceneMenuFolderNewFolder, MyFrame::OnTreeSceneMenuFolderNewFolder)
	EVT_MENU(ID_TreeSceneMenuFolderRemove, MyFrame::OnTreeSceneMenuFolderRemove)//-----------------------
	EVT_MENU(ID_TreeSceneMenuLayerSave, MyFrame::OnTreeSceneMenuLayerSave)
	EVT_MENU(ID_TreeSceneMenuLayerMakeActive, MyFrame::OnTreeSceneMenuLayerMakeActive)
	EVT_MENU(ID_TreeSceneMenuLayerDeactivation, MyFrame::OnTreeSceneMenuLayerDeactivation)

	EVT_MENU(ID_TreeSceneMenuLayerRemove, MyFrame::OnTreeSceneMenuLayerRemove)
	EVT_MENU(ID_TreeSceneMenuLayerLoad, MyFrame::OnTreeSceneMenuLayerLoad)
	EVT_MENU(ID_TreeSceneMenuLayerShow, MyFrame::OnTreeSceneMenuLayerShow) 
	EVT_MENU(ID_TreeSceneMenuFolderLoadAll, MyFrame::OnTreeSceneMenuFolderLoadAll)
	
	EVT_MENU(ID_TreeSceneMenuLayerHide, MyFrame::OnTreeSceneMenuLayerHide)
	EVT_MENU(ID_TreeSceneMenuObjectLook, MyFrame::OnTreeSceneMenuObjectLook)
	EVT_MENU(ID_TreeSceneMenuObjectCopy, MyFrame::OnToolSCopy)
	EVT_MENU(ID_TreeSceneMenuObjectCut, MyFrame::OnToolMenuCut)
	EVT_MENU(ID_TreeSceneMenuObjectDelete, MyFrame::OnTreeSceneMenuObjectDelete)

	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK_RightWindow, MyFrame::OnNotebookChanging)

	EVT_PG_CHANGED(ID_PG_PropertyObject, MyFrame::OnPGObjectPropertyChanged)
	EVT_PG_CHANGED(ID_PG_PropertyDirectionalLight, MyFrame::OnPGSunPropertyChanged)
wxEND_EVENT_TABLE()





//************************************************************************************************************************************************************************************************
//MyFrame - C-TOR
//************************************************************************************************************************************************************************************************
MyFrame::MyFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style) 
        : wxFrame(parent, id, title, pos, size, style)
{



	//std::string filenameKlmesh = "cylinder.klmesh";

	//BinaryFileRead *binaryFileRead = new BinaryFileRead(filenameKlmesh);
	klmeshFileStruct klmeshTmp;
	//binaryFileRead->readKLMeshFileStructure(klmeshTmp);
	//delete binaryFileRead;


	mLayerBuffer = new std::vector<Layer*>();
	mKLMESH_Buffer = new KLMESH_Buffer();

	mCopiedObject = nullptr;
	//-------------------------------
	// Create DirectX11 
	//-------------------------------
	mKalenEngine = new KalenEngine(this, ID_WindowDirectX, klmeshTmp);
	if (mKalenEngine->Init() == false)
	{
		wxMessageBox("Cannot open application!", "Sorry :(", wxICON_INFORMATION | wxOK);
		delete mKalenEngine;
		wxExit();
		Destroy();
	}
	//mKalenEngine->SetMyFrame(this);
	//-------------------------------
	SetDeviceAndTextureBuffer(mKalenEngine->getDevice(), mKalenEngine->GetTexturesResource());


	mActiveLayerName = "";
	mTreeBufferNameObject = "";
	//mTreeBufferCopyPaste = new std::vector<wxTreeItemId*>(0, 0);
	mSelectedObjectItem = NULL;

    // tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);


    // set frame icon
	SetIcon(wxIcon(wxT("KL_ICON_Logo")));
    // set up default notebook style
    //m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
	m_notebook_style = wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxNO_BORDER;
    m_notebook_theme = 0;

    // create menu
    wxMenuBar* mb = new wxMenuBar;





	//**********************************************************************************************************
	//MENU
	//**********************************************************************************************************

	//--------------------------------------------------------------------------------------------------
	//MENU - File
	//--------------------------------------------------------------------------------------------------


	wxMenu* file_menu = new wxMenu;
	wxMenuItem *menuNewWorld = new wxMenuItem(file_menu, ID_ToolSmallNewWorld, wxT("New World\tCtrl-N"));
	menuNewWorld->SetBitmap(wxIcon(wxT("KL_ICON_NewWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	file_menu->Append(menuNewWorld);
	file_menu->AppendSeparator();


	wxMenuItem *menuOpenWorld = new wxMenuItem(file_menu, ID_ToolSmallOpenWorld, wxT("Open World\tCtrl-O"));
	menuOpenWorld->SetBitmap(wxIcon(wxT("KL_ICON_Open"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	file_menu->Append(menuOpenWorld);
	//file_menu->Append()
	//, wxIcon(wxT("KL_ICON_RestoreWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16)

	wxMenuItem *menuSaveWorld = new wxMenuItem(file_menu, ID_ToolSmallSaveWorld, wxT("Save World\tCtrl-S"));
	menuSaveWorld->SetBitmap(wxIcon(wxT("KL_ICON_Save"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	file_menu->Append(menuSaveWorld);

	/*wxMenuItem *menuCloseWorld = new wxMenuItem(file_menu, wxID_ANY, wxT("Close World\tCtrl-Z"));
	menuCloseWorld->SetBitmap(wxIcon(wxT("KL_ICON_CloseWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	file_menu->Append(menuCloseWorld);

	file_menu->AppendSeparator();


	wxMenuItem *menuRestoreWorld = new wxMenuItem(file_menu, wxID_ANY, wxT("Restore World"));
	menuRestoreWorld->SetBitmap(wxIcon(wxT("KL_ICON_RestoreWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	file_menu->Append(menuRestoreWorld);*/

	file_menu->AppendSeparator();

    file_menu->Append(wxID_EXIT);



	//--------------------------------------------------------------------------------------------------
	//MENU - VIEW
	//--------------------------------------------------------------------------------------------------
    wxMenu* view_menu = new wxMenu;

	wxMenu* menuPanelScene = new wxMenu;
	menuPanelScene->Append(ID_ToolItemPanelSceneVisible, wxT("Visible"), wxT(""), wxITEM_CHECK);
	menuPanelScene->Append(ID_ToolItemPanelSceneTabbed, wxT("Tabbed"), wxT(""), wxITEM_CHECK);
	menuPanelScene->Append(ID_ToolItemPanelSceneShow, wxT("Show"), wxT(""), wxITEM_CHECK);
	//Set isClickedTools
	menuPanelScene->Check(ID_ToolItemPanelSceneVisible, true);
	menuPanelScene->Check(ID_ToolItemPanelSceneTabbed, true);
	menuPanelScene->Check(ID_ToolItemPanelSceneShow, false);

	
	wxMenu* menuPanel = new wxMenu; //Panel - subMenu
	//Add to subMenu
	menuPanel->AppendSubMenu(menuPanelScene, wxT("Scene"));

	//----------------
	wxMenu* menuPanelTools = new wxMenu;
	menuPanelTools->Append(ID_ToolItemPanelToolsVisible, wxT("Visible"), wxT(""), wxITEM_CHECK);
	menuPanelTools->Append(ID_ToolItemPanelToolsTabbed, wxT("Tabbed"), wxT(""), wxITEM_CHECK);
	menuPanelTools->Append(ID_ToolItemPanelToolsShow, wxT("Show"), wxT(""), wxITEM_CHECK);
	//Set isClickedTools
	menuPanelTools->Check(ID_ToolItemPanelToolsVisible, true);
	menuPanelTools->Check(ID_ToolItemPanelToolsTabbed, true);
	menuPanelTools->Check(ID_ToolItemPanelToolsShow, false);

	//Add to submenu
	menuPanel->AppendSubMenu(menuPanelTools, wxT("Tools"));


	//----------------
	wxMenu* menuPanelProperties = new wxMenu;
	menuPanelProperties->Append(ID_ToolItemPanelPropertiesVisible, wxT("Visible"), wxT(""), wxITEM_CHECK);
	menuPanelProperties->Append(ID_ToolItemPanelPropertiesTabbed, wxT("Tabbed"), wxT(""), wxITEM_CHECK);
	menuPanelProperties->Append(ID_ToolItemPanelPropertiesShow, wxT("Show"), wxT(""), wxITEM_CHECK);
	//Set isClickedTools
	menuPanelProperties->Check(ID_ToolItemPanelPropertiesVisible, true);
	menuPanelProperties->Check(ID_ToolItemPanelPropertiesTabbed, true);
	menuPanelProperties->Check(ID_ToolItemPanelPropertiesShow, false);

	//Add to submenu
	menuPanel->AppendSubMenu(menuPanelProperties, wxT("Properties"));


	//----------------
	menuPanel->Append(ID_ToolItemPanelShowAll, wxT("Show all"), wxT(""), wxITEM_CHECK);
	menuPanel->Append(ID_ToolItemPanelHideAll, wxT("Hide all"), wxT(""), wxITEM_CHECK);
	menuPanel->Append(ID_ToolItemPanelAllToWindows, wxT("All to windows"), wxT(""), wxITEM_CHECK);
	menuPanel->Append(ID_ToolItemPanelAllToTab, wxT("All to tab"), wxT(""), wxITEM_CHECK);

	menuPanel->Check(ID_ToolItemPanelShowAll, true);
	menuPanel->Check(ID_ToolItemPanelHideAll, false);
	menuPanel->Check(ID_ToolItemPanelAllToWindows, false);
	menuPanel->Check(ID_ToolItemPanelAllToTab, true);


	//--------------------------------
	//Toolbars - create
	wxMenu* menuToolbars = new wxMenu; 
	menuToolbars->Append(ID_ToolItemToolbarsMain, wxT("Main"), wxT(""), wxITEM_CHECK);
	menuToolbars->Append(ID_ToolItemToolbarsMode, wxT("Mode"), wxT(""), wxITEM_CHECK);
	menuToolbars->Check(ID_ToolItemToolbarsMain, true);
	menuToolbars->Check(ID_ToolItemToolbarsMode, true);

	//--------------------------------
	//Viewport - create
	wxMenu* menuViewport = new wxMenu; 
	menuViewport->Append(ID_ToolItemViewportFloat, wxT("Float"), wxT(""), wxITEM_CHECK);
	menuViewport->Check(ID_ToolItemViewportFloat, false);



   /* view_menu->AppendSubMenu(menuPanel, wxT("Panel"));
	view_menu->AppendSubMenu(menuToolbars, wxT("Toolbars"));
	view_menu->AppendSubMenu(menuViewport, wxT("Viewport"));
	view_menu->AppendSeparator();*/
	view_menu->Append(ID_ToolItemAssetBrowser, wxT("Asset Browser\tCtrl-A"), wxT(""), wxITEM_NORMAL);
	view_menu->Append(ID_ToolItemOutputWindow, wxT("LOG - Output window"), wxT(""), wxITEM_NORMAL);
	view_menu->Append(ID_ToolItemFilters,      wxT("Filters\tCtrl-F"), wxT(""), wxITEM_NORMAL);

	//--------------------------------------------------------------------------------------------------
	//MENU - Edit
	//--------------------------------------------------------------------------------------------------
	m_edit_menu = new wxMenu;
	m_edit_menu->Append(ID_ToolItemMenuUndo, wxT("Undo\tCtrl-Z"));
	m_edit_menu->Append(ID_ToolItemMenuRedo, wxT("Redo\tCtrl-Y"));
	m_edit_menu->AppendSeparator();
	m_edit_menu->Append(ID_ToolItemMenuCopy, wxT("Copy\tCtrl-C"));
	m_edit_menu->Append(ID_ToolItemMenuCut, wxT("Cut\tCtrl-X"));
	m_edit_menu->Append(ID_ToolItemMenuPaste, wxT("Paste\tCtrl-V"));
	m_edit_menu->Append(ID_ToolItemMenuDelete, wxT("Delete\tDelete"));
	m_edit_menu->AppendSeparator();
	m_edit_menu->Append(ID_ToolItemMenuLookAt, wxT("Look at selected"));
	m_edit_menu->Append(ID_ToolItemMenuCancel, wxT("Cancel selection"));
	m_edit_menu->AppendSeparator();
	m_edit_menu->Append(ID_ToolItemMenuActiveLayer, wxT("Active layer\tShift-Space"), wxT(""), wxITEM_CHECK);
	m_edit_menu->Append(ID_ToolItemMenuMultiLayer, wxT("Multi layer\tShift-Space"), wxT(""), wxITEM_CHECK);
	m_edit_menu->Check(ID_ToolItemMenuActiveLayer, true);


	//--------------------------------------------------------------------------------------------------
	//MENU - Mode
	//--------------------------------------------------------------------------------------------------
	m_mode_menu = new wxMenu;

	m_mode_menu->Append(ID_ToolItemMenuMove, wxT("Move\tSHIFT+Z"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Append(ID_ToolItemMenuRotate, wxT("Rotate\tSHIFT+X"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Append(ID_ToolItemMenuScale, wxT("Scale\tSHIFT+C"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Check(ID_ToolItemMenuMove, true);
	m_mode_menu->AppendSeparator();
	/*m_mode_menu->Append(ID_ToolItemMenuSnap, wxT("No Snap"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Append(ID_ToolItemMenuSnapTerrain, wxT("No to terrain"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Append(ID_ToolItemMenuSnapAll, wxT("Snap to all"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Check(ID_ToolItemMenuSnap, true);
	m_mode_menu->AppendSeparator();*/
	m_mode_menu->Append(ID_ToolItemMenuGlobal, wxT("Global space\tCtrl-Space"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Append(ID_ToolItemMenuLocal, wxT("Local space\tCtrl-Space"), wxT(""), wxITEM_CHECK);
	m_mode_menu->Check(ID_ToolItemMenuGlobal, true);




	//--------------------------------------------------------------------------------------------------
	//MENU - Tools
	//--------------------------------------------------------------------------------------------------
    /*wxMenu* tools_menu = new wxMenu;
	tools_menu->Append(wxID_ANY, wxT("EmptySlot..."));
	tools_menu->Append(wxID_ANY, wxT("EmptySlot..."));
	tools_menu->Append(wxID_ANY, wxT("EmptySlot..."));
	tools_menu->AppendSeparator();
	tools_menu->Append(wxID_ANY, wxT("EmptySlot..."));
	tools_menu->Append(wxID_ANY, wxT("EmptySlot..."));
	tools_menu->Append(wxID_ANY, wxT("EmptySlot..."));
	*/





	//--------------------------------------------------------------------------------------------------
	//MENU - HELP
	//--------------------------------------------------------------------------------------------------
    wxMenu* help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);


	//-----------------------------------------
	//MENU - ADD MENU
	//-----------------------------------------
    mb->Append(file_menu, wxT("&File"));
    mb->Append(view_menu, wxT("&View"));
    mb->Append(m_edit_menu, wxT("&Edit"));
    mb->Append(m_mode_menu, wxT("&Mode"));
   // mb->Append(tools_menu, wxT("&Tools"));
    mb->Append(help_menu, wxT("&Help"));

    SetMenuBar(mb);

    CreateStatusBar(4);
	//SetStatusBarPane(3);
	int w, h;
	GetSize(&w, &h);
	int arr[4];
	arr[0] = (w/2 + w/5);
	arr[1] = 50;
	arr[2] = 50;
	arr[3] = 50;
	SetStatusWidths(4, arr);
    GetStatusBar()->SetStatusText(_("Ready"), 0);
	GetStatusBar()->SetStatusText(_("X: 0.0"), 1);
	GetStatusBar()->SetStatusText(_("Y: 0.0"), 2);
	GetStatusBar()->SetStatusText(_("Z: 0.0"), 3);


    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    SetMinSize(wxSize(400,300));


	//**********************************************************************************************************
	//TOOLBAR
	//**********************************************************************************************************


	//--------------------------------------------------------------------------------------------------
	//TOOLBAR - TOP
	//--------------------------------------------------------------------------------------------------
	mToolBarTop = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORIZONTAL);

    //wxBitmap mToolBarTop_bmp1 = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16));

    mToolBarTop->AddTool(ID_ToolSmallNewWorld, wxT("Test"), wxIcon(wxT("KL_ICON_NewWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("New Wolrd"));//Disabled
	mToolBarTop->AddSeparator();
	//mToolBarTop->AddTool(ID_ToolSmallRestoreWorld, wxT("Test"), wxIcon(wxT("KL_ICON_RestoreWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Restore Wolrd"));
	//mToolBarTop->AddSeparator();
    mToolBarTop->AddTool(ID_ToolSmallOpenWorld, wxT("Test"), wxIcon(wxT("KL_ICON_Open"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Open Wolrd"));

    mToolBarTop->AddTool(ID_ToolSmallSaveWorld, wxT("Test"), wxIcon(wxT("KL_ICON_Save"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Save Wolrd"));
    mToolBarTop->AddSeparator();
    mToolBarTop->AddTool(ID_ToolSmallCopy, wxT("Test"), wxIcon(wxT("KL_ICON_Copy"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Copy selected object"));
    mToolBarTop->AddTool(ID_ToolSmallPaste, wxT("Test"), wxIcon(wxT("KL_ICON_Paste"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Paste object"));
	mToolBarTop->AddTool(ID_ToolSmallDelete, wxT("Test"), wxIcon(wxT("KL_ICON_Delete"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Delete selected object"));
    mToolBarTop->AddSeparator();
    mToolBarTop->AddTool(ID_ToolSmallAssetBrowser, wxT("Test"), wxIcon(wxT("KL_ICON_AssetBrowser"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Open Asset Browser"));
	mToolBarTop->AddSeparator();

    mToolBarTop->AddTool(ID_ToolSmallGridMove, wxT("Test"), wxIcon(wxT("KL_ICON_Grid"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Grid on/off"), wxITEM_CHECK);
	mToolBarTop->ToggleTool(ID_ToolSmallGridMove, true);



	mChoiceMoveUnit = new wxChoice(mToolBarTop, ID_ToolSmallChoiceMove);
	mChoiceMoveUnit->AppendString(wxT("0.25 cm"));
	mChoiceMoveUnit->AppendString(wxT("0.5 cm"));
	mChoiceMoveUnit->AppendString(wxT("1 cm"));
	mChoiceMoveUnit->AppendString(wxT("3 cm"));
	mChoiceMoveUnit->AppendString(wxT("5 cm"));
	mChoiceMoveUnit->AppendString(wxT("10 cm"));
	mChoiceMoveUnit->AppendString(wxT("25 cm"));
	mChoiceMoveUnit->AppendString(wxT("50 cm"));
	mChoiceMoveUnit->AppendString(wxT("1 m"));
	mChoiceMoveUnit->AppendString(wxT("3 m"));
	mChoiceMoveUnit->AppendString(wxT("5 m"));
	mChoiceMoveUnit->AppendString(wxT("10 m"));
	mChoiceMoveUnit->AppendString(wxT("25 m"));
	mChoiceMoveUnit->AppendString(wxT("50 m"));
	mChoiceMoveUnit->AppendString(wxT("100 m"));
	mToolBarTop->AddControl(mChoiceMoveUnit);
	mChoiceMoveUnit->SetSelection(2);
	//mChoiceMoveUnit->Enable(false);

	mToolBarTop->AddSeparator();

	mToolBarTop->AddTool(ID_ToolSmallGridRotate, wxT("Test"), wxIcon(wxT("KL_ICON_GridRotate"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Rotate on/off"), wxITEM_CHECK);
	mChoiceRotateUnit = new wxChoice(mToolBarTop, ID_ToolSmallChoiceRotate);
	mToolBarTop->ToggleTool(ID_ToolSmallGridRotate, true);

	mChoiceRotateUnit->AppendString(wxT("1\xB0"));
	mChoiceRotateUnit->AppendString(wxT("3\xB0"));
	mChoiceRotateUnit->AppendString(wxT("5\xB0"));
	mChoiceRotateUnit->AppendString(wxT("10\xB0"));
	mChoiceRotateUnit->AppendString(wxT("15\xB0"));
	mChoiceRotateUnit->AppendString(wxT("30\xB0"));
	mChoiceRotateUnit->AppendString(wxT("45\xB0"));
	mChoiceRotateUnit->AppendString(wxT("90\xB0"));
	mToolBarTop->AddControl(mChoiceRotateUnit);
	mChoiceRotateUnit->SetSelection(6);
	
	mToolBarTop->AddSeparator();
	
	//----------------
	//Snap case
	//----------------
	mChoiceSnap = new wxChoice(mToolBarTop, ID_ToolSmallChoiceSnap);
	mChoiceSnap->AppendString(wxT("No snap"));
	mChoiceSnap->AppendString(wxT("Snap to terrain"));
	mChoiceSnap->AppendString(wxT("Snap to all"));
	mChoiceSnap->SetSelection(0);
	mChoiceSnap->Enable(false);


	mToolBarTop->AddControl(mChoiceSnap);

	mToolBarTop->AddSeparator();
	
	//mToolBarTop->AddTool(ID_SampleItem + 13, wxT("Test"), mToolBarTop_bmp1);
	//wxChoice* choice4 = new wxChoice(mToolBarTop, ID_SampleItem + 35, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxDefaultValidator, wxT("awd"));
	wxChoice* choice4 = new wxChoice(mToolBarTop, ID_ToolSmallChoiceRender);
	//choice4->SetBackgroundColour(wxColor(100, 100, 100, 255));
	choice4->AppendString(wxT("Normal Rendering"));
	choice4->AppendString(wxT("Diffuse Only"));
	choice4->AppendString(wxT("Simple Lighting"));
	choice4->AppendString(wxT("Specular Only"));
	choice4->AppendString(wxT("Specular Glossiness"));
	choice4->AppendString(wxT("SSAO Only"));
	choice4->AppendString(wxT("Bloom"));
	mToolBarTop->AddControl(choice4);
	choice4->SetSelection(0);
	choice4->Enable(false);

	//choice4->Set

	mToolBarTop->AddSeparator();
	//*/




	////////////////////////////////////////////////////////////////////////

	//***********************************************************************************
	// prepare a few custom overflow elements for the toolbars' overflow buttons
	wxAuiToolBarItemArray prepend_items;
	wxAuiToolBarItemArray append_items;
	wxAuiToolBarItem item;
	item.SetKind(wxITEM_SEPARATOR);
	append_items.Add(item);
	item.SetKind(wxITEM_NORMAL);
	item.SetId(ID_CustomizeToolbar);
	item.SetLabel(_("Customize..."));
	append_items.Add(item);
	//***********************************************************************************
    mToolBarTop->SetCustomOverflowItems(prepend_items, append_items);
    mToolBarTop->EnableTool(ID_SampleItem+6, false);
    mToolBarTop->Realize();



	//--------------------------------------------------------------------------------------------------
	//TOOLBAR - LEFT!!! IMAGES!!!
	//--------------------------------------------------------------------------------------------------
    mToolBarLeft = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_TEXT | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
    mToolBarLeft->SetToolBitmapSize(wxSize(48,48));
	mToolBarActiveLayerItem = mToolBarLeft->AddTool(ID_ToolItemACTIVE_L, wxT("Active Layer"), wxIcon(wxT("KL_ICON_ActiveLayer"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Active layer"), wxITEM_RADIO);
	mToolBarMultiLayerItem = mToolBarLeft->AddTool(ID_ToolItemMULTI_L, wxT(" Multi Layer "), wxIcon(wxT("KL_ICON_MultiLayers"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Multi layer"), wxITEM_RADIO);
	mToolBarLeft->ToggleTool(ID_ToolItemMULTI_L, true);
	mToolBarLeft->AddSeparator();
    mToolBarLeft->AddTool(ID_ToolItemGLOBAL, wxT("    Global     "), wxIcon(wxT("KL_ICON_Global"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Global GIZMO"), wxITEM_RADIO);
	mToolBarLeft->ToggleTool(ID_ToolItemGLOBAL, true);
	mToolBarLeft->AddTool(ID_ToolItemLOCAL, wxT("     Local      "), wxIcon(wxT("KL_ICON_Local"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Local GIZMO"), wxITEM_RADIO);
    mToolBarLeft->AddSeparator();
    mToolBarLeft->AddTool(ID_ToolItemMOVE, wxT("     Move      "), wxIcon(wxT("KL_ICON_Move"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Move"), wxITEM_RADIO);
	mToolBarLeft->ToggleTool(ID_ToolItemMOVE, true);
    mToolBarLeft->AddTool(ID_ToolItemROTATE, wxT("     Rotate     "), wxIcon(wxT("KL_ICON_Rotate"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Rotate"), wxITEM_RADIO);
    mToolBarLeft->AddTool(ID_ToolItemSCALE, wxT("      Scale      "), wxIcon(wxT("KL_ICON_Scale"), wxBITMAP_TYPE_ICO_RESOURCE, 32, 32), wxT("Scale"), wxITEM_RADIO);
	mToolBarLeft->AddSeparator();
	mToolBarLeft->AddTool(ID_ToolItemCOPY, wxT("      Copy      "), wxIcon(wxT("KL_ICON_Copy"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Copy selected object"));
	//mToolBarLeft->AddTool(ID_ToolItemCUT, wxT("    Cut    "), wxArtProvider::GetBitmap(wxART_INFORMATION), wxT("Cut selected object"));
	mToolBarLeft->AddTool(ID_ToolItemPASTE, wxT("      Paste      "), wxIcon(wxT("KL_ICON_Paste"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Paste object on the active layer"));
	//mToolBarLeft->AddSeparator();
	mToolBarLeft->AddTool(ID_ToolItemDELETE, wxT("     Delete    "), wxIcon(wxT("KL_ICON_Delete"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Delete selected object"));
	//mToolBarLeft->AddSeparator();
	//***********************************************************************************
	// prepare a few custom overflow elements for the toolbars' overflow buttons
	wxAuiToolBarItemArray prepend_items2;
	wxAuiToolBarItemArray append_items2;
	wxAuiToolBarItem item2;
	item2.SetKind(wxITEM_SEPARATOR);
	append_items2.Add(item2);
	item2.SetKind(wxITEM_NORMAL);
	item2.SetId(ID_CustomizeToolbar);
	item2.SetLabel(_("Customize123..."));
	append_items2.Add(item2);
	//***********************************************************************************
  //  mToolBarLeft->SetCustomOverflowItems(prepend_items2, append_items2);
    mToolBarLeft->Realize();



	//**************************************************************************************************************************
	//ADD ITEM
	//**************************************************************************************************************************


	m_mgr.AddPane(CreateRightWindow(), wxAuiPaneInfo().
		Name(wxT("test8")).Caption(wxT("Right Panel")).
		Right().
		CloseButton(false).MaximizeButton(false));


    // Give this pane an icon, too, just for testing.
    int iconSize = m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE);
    // Make it even to use 16 pixel icons with default 17 caption height.
    iconSize &= ~1;



	m_mgr.AddPane(CreateOutputWindow(), wxAuiPaneInfo().
		//m_mgr.AddPane(m_textctrl, wxAuiPaneInfo().
                  Name(wxT("outputWindow")).Caption(wxT("Output Window")).
                  Bottom().Layer(1).Position(1).
                  Icon(wxArtProvider::GetBitmap(wxART_INFORMATION,
                                                wxART_OTHER,
                                                wxSize(iconSize, iconSize))));
	mTextOututWindow123 = mTextOututWindow;

    // create some center panes

  //  m_mgr.AddPane(CreateGrid(), wxAuiPaneInfo().Name(wxT("grid_content")).
    //              CenterPane().Hide());

  //  m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().Name(wxT("tree_content1234")).
    //              CenterPane().Hide());

    m_mgr.AddPane(CreateTextCtrl(), wxAuiPaneInfo().Name(wxT("text_content")).
                  CenterPane().Hide());

    m_mgr.AddPane(CreateHTMLCtrl(), wxAuiPaneInfo().Name(wxT("html_content")).
                  CenterPane().Hide());

	//-----------------------------------------------------------------------------------------------------------------------
	//DirectX - Render place!!!
	//-----------------------------------------------------------------------------------------------------------------------
  //  m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().Name(wxT("notebook_content")).
   //               CenterPane().PaneBorder(true));

	//m_mgr.AddPane(CreatePanelDirectX(), wxAuiPaneInfo().Name(wxT("notebook_content")).
	//	CenterPane().PaneBorder(true));
	 
	gOutputWindowDirectX->SetDropTarget(new DnDText(gOutputWindowDirectX, this, mKalenEngine));

	m_mgr.AddPane(gOutputWindowDirectX, wxAuiPaneInfo().Name(wxT("notebook_content")).
					CenterPane().PaneBorder(true));


	

	//m_mgr.AddPane(NULL, wxAuiPaneInfo().Name(wxT("notebook_content")).
	//	CenterPane().PaneBorder(true));


	


	
	//-----------------------------------------------------------------------------------------------------------------------



    m_mgr.AddPane(mToolBarTop, wxAuiPaneInfo().
                  Name(wxT("mToolBarTop")).Caption(wxT("Toolbar 2 (Horizontal)")).
                  ToolbarPane().Top().Row(1));

    m_mgr.AddPane(mToolBarLeft, wxAuiPaneInfo().
                  Name(wxT("mToolBarLeft")).Caption(wxT("Sample Vertical Toolbar")).
                  ToolbarPane().Left().CloseButton(false).
                  GripperTop());

    m_mgr.AddPane(new wxButton(this, wxID_ANY, _("Test Button")),
                  wxAuiPaneInfo().Name(wxT("tb6")).
                  ToolbarPane().Top().Row(1).Position(1).
                  LeftDockable(false).RightDockable(false));

    // make some default perspectives

    wxString perspective_all = m_mgr.SavePerspective();

    int i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
        if (!all_panes.Item(i).IsToolbar())
            all_panes.Item(i).Hide();
    m_mgr.GetPane(wxT("tb1")).Hide();
    m_mgr.GetPane(wxT("tb6")).Hide();
    m_mgr.GetPane(wxT("test8")).Show().Right().Layer(0).Row(0).Position(0);
    m_mgr.GetPane(wxT("outputWindow")).Show().Bottom().Layer(0).Row(0).Position(0);
    m_mgr.GetPane(wxT("notebook_content")).Show();


	   

   // wxString perspective_default = m_mgr.SavePerspective();



  //  m_perspectives.Add(perspective_default);
   // m_perspectives.Add(perspective_all);



	

   //------------------------------------------------------
   //Create additional windows
   //------------------------------------------------------
	mFiltersWindow = new FiltersWindow(parent, wxID_ANY, wxT("Filters"), wxDefaultPosition, wxSize(350, 480),  wxMINIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN);
	mFiltersWindow->Hide();


	mAssetBrowser = new AssetBrowser(parent, wxID_ANY, wxT("Asset Browser"), wxDefaultPosition, wxSize(880, 500));
	mAssetBrowser->Hide();
	mAssetBrowser->SetKLMESH_Buffer(mKLMESH_Buffer);//Send pointer to assetBrowser (mKLMESH_Buffer is defined before)
	mAssetBrowser->SetTexturesResource(mTexturesResource);//Send pointer to assetBrowser (mTexturesResource is defined before)
	mAssetBrowser->SetDevice(mKalenEngine->getDevice());


	//------------------------------------------------------
	//Conenct - keyborad!
	//------------------------------------------------------
	//Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(MyFrame::OnKeyDown), NULL, this);
	connectKey(this);

	mRightClickedItem = nullptr;


    // "commit" all changes made to wxAuiManager
    m_mgr.Update();
}





wxTextCtrl* MyFrame::CreateOutputWindow()
{
	mTextOututWindow = CreateTextCtrl(wxT("KalenEngine DX11 -- Output window:\n"));
	mTextOututWindow->SetEditable(false);
	mTextOututWindow->SetSize(wxSize(500, 100));
	mTextOututWindow123 = mTextOututWindow;
	return 	mTextOututWindow;
}





//************************************************************************************************************************************************************************************************
//MyFrame - D-TOR
//************************************************************************************************************************************************************************************************
MyFrame::~MyFrame()
{
    m_mgr.UnInit();
	//delete mTreeBufferCopyPaste;
}




//************************************************************************************************************************************************************************************************
//WINDOWS--TREE
//************************************************************************************************************************************************************************************************

wxTreeCtrl* MyFrame::CreateTreeCtrl()
{
	//wxDefaultSize
	mTreeScene = new wxTreeCtrl(this, ID_TreeScene, wxPoint(0, 0), wxSize(160, 250), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTR_MULTIPLE);
	//mTreeScene = new wxTreeCtrl(this, ID_TreeScene, wxDefaultPosition, wxSize(560, 550), wxTR_EDIT_LABELS | wxTR_HAS_BUTTONS | wxTR_FULL_ROW_HIGHLIGHT | wxTR_LINES_AT_ROOT | wxTR_ROW_LINES | wxTR_HAS_VARIABLE_ROW_HEIGHT | wxTR_MULTIPLE);

	wxImageList* imglist = new wxImageList(16, 16, true, 2);

	
	imglist->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	imglist->Add(wxIcon(wxT("KL_ICON_Folder"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	imglist->Add(wxIcon(wxT("KL_ICON_TreeLayer"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	imglist->Add(wxIcon(wxT("KL_ICON_Object3D"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	imglist->Add(wxIcon(wxT("KL_ICON_TreeLayerActive"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//ActiveLayer
	imglist->Add(wxIcon(wxT("KL_ICON_TreeLayerActiveNoSave"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//ActiveLayerNoSave
	imglist->Add(wxIcon(wxT("KL_ICON_TreeLayerNoSave"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//LayerNoSave
	imglist->Add(wxIcon(wxT("KL_ICON_TreeLayerHide"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//LayerHide
	imglist->Add(wxIcon(wxT("KL_ICON_Object3DSelected"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//SelectedObject3D -- 8
	imglist->Add(wxIcon(wxT("KL_ICON_Light"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//MenuLight -- 9
	imglist->Add(wxIcon(wxT("KL_ICON_LightSelected"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//MenuLightSelected -- 10
	
	mTreeScene->AssignImageList(imglist);

	wxTreeItemId root = mTreeScene->AddRoot(wxT("World                                                                        "), 0, 0);
	mTreeRoot = root;
	//StateItemList
	// 0 - Root
	// 1 - Folder
	// 2 - Layer
	// 3 - Object3D
	// -1 - NULL
	mTreeScene->SetItemState(root, 0);
	wxArrayTreeItemIds folder;
	wxArrayTreeItemIds layer;
	wxArrayTreeItemIds object3D;
	

	mTreeScene->Expand(root);







	return mTreeScene;
}

//******************************************************************************************


wxAuiNotebook* MyFrame::CreateRightWindow()
{
	// create the notebook off-window to avoid flicker
	wxSize client_size = GetClientSize();

	wxAuiNotebook* ctrl = new wxAuiNotebook(this, ID_NOTEBOOK_RightWindow, wxPoint(client_size.x, client_size.y), wxSize(430, 200), m_notebook_style);
	ctrl->Freeze();

	wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));





	//--------------------------
	//Tab 1 - Scene
	//--------------------------
	ctrl->AddPage(CreateTreeCtrl(), wxT("Scene"), false, page_bmp);
	ctrl->SetPageToolTip(0, "Scene - Tab");
	

	//--------------------------
	//Tab 2 - Tools
	//--------------------------
	ctrl->AddPage(CreateGrid(), wxT("Tools"), false, page_bmp);
	ctrl->SetPageToolTip(1, "Tools - Tab");

	//--------------------------
	//Tab 3 - Properties
	//--------------------------
	ctrl->AddPage(CreateTestPropertyGrid(), wxT("Properties"), false, page_bmp);
	ctrl->SetPageToolTip(2, "Properties - tab");

	//ctrl->SetBackgroundColour(wxColour(128, 128, 128, 255));

	ctrl->Thaw();
	return ctrl;
}



wxAuiNotebook* MyFrame::CreateNotebook()
{
	// create the notebook off-window to avoid flicker
	wxSize client_size = GetClientSize();

	wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY, wxPoint(client_size.x, client_size.y), wxSize(430, 200), m_notebook_style);
	ctrl->Freeze();

	wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));


	mTestWindowMain = CreateHTMLCtrl(ctrl);
	mTestWindowMain->SetDropTarget(new DnDText(mTestWindowMain, this, mKalenEngine));
	//gOutputWindowDirectX->SetDropTarget(new DnDText(gOutputWindowDirectX));


	ctrl->AddPage(mTestWindowMain, wxT("Welcome to wxAUI"), false, page_bmp);
	//ctrl->GetH
	//ctrl->AddPage(CreateListCtrlRightPanel(), wxT("Welcome to wxAUI"), false, page_bmp);
	
	ctrl->SetPageToolTip(0, "Welcome to wxAUI (this is a page tooltip)");

	wxPanel *panel = new wxPanel(ctrl, wxID_ANY);
	wxFlexGridSizer *flex = new wxFlexGridSizer(4, 2, 0, 0);
	flex->AddGrowableRow(0);
	flex->AddGrowableRow(3);
	flex->AddGrowableCol(1);
	flex->Add(5, 5);   
	flex->Add(5, 5);
	flex->Add(new wxStaticText(panel, -1, wxT("wxTextCtrl:")), 0, wxALL | wxALIGN_CENTRE, 5);
	flex->Add(new wxTextCtrl(panel, -1, wxT(""), wxDefaultPosition, wxSize(100, -1)), 1, wxALL | wxALIGN_CENTRE, 5);
	flex->Add(new wxStaticText(panel, -1, wxT("wxSpinCtrl:")), 0, wxALL | wxALIGN_CENTRE, 5);
	flex->Add(new wxSpinCtrl(panel, -1, wxT("5"), wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 5, 50, 5), 0, wxALL | wxALIGN_CENTRE, 5);
	flex->Add(5, 5);   flex->Add(5, 5);
	panel->SetSizer(flex);




	ctrl->AddPage(panel, wxT("wxPanel"), false, page_bmp);


	ctrl->Thaw();
	return ctrl;
}




wxAuiDockArt* MyFrame::GetDockArt()
{
    return m_mgr.GetArtProvider();
}

void MyFrame::DoUpdate()
{
    m_mgr.Update();
}

void MyFrame::OnEraseBackground(wxEraseEvent& evt)
{
	evt.Skip();
}

void MyFrame::OnSize(wxSizeEvent& evt)
{
	int w, h;
	GetSize(&w, &h);
	int arr[4];
	arr[0] = (w / 2 + w / 5);
	arr[1] = 50;
	arr[2] = 50;
	arr[3] = 50;
	SetStatusWidths(4, arr);
	evt.Skip();
}

//************************************************************************************************************************************************************************************************
//TOOLBARS - 
//************************************************************************************************************************************************************************************************
void MyFrame::OnCustomizeToolbar(wxCommandEvent& WXUNUSED(evt))
{
    wxMessageBox(_("Customize Toolbar clicked123"));
	AddTextToOutputWindow("Customize Toolbar clicked123");
}
//************************************************************************************************************************************************************************
void MyFrame::OnXTouch(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("X touched");
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXActiveL(wxCommandEvent& WXUNUSED(evt))
{
	gMultiLayers = false;
	AddTextToOutputWindow("Tool X - ActiveLayer");
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//*****************************************************************************************************************************************
void MyFrame::OnToolXMultiL(wxCommandEvent& WXUNUSED(evt))
{
	gMultiLayers = true;
	AddTextToOutputWindow("Tool X - MultiLayer");
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXGlobal(wxCommandEvent& WXUNUSED(evt))
{
	gGlobalSpace = true;
	AddTextToOutputWindow("Tool X - Global");
	m_mode_menu->Check(ID_ToolItemMenuGlobal, true);
	m_mode_menu->Check(ID_ToolItemMenuLocal, false);
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXLocal(wxCommandEvent& WXUNUSED(evt))
{
	gGlobalSpace = false;
	AddTextToOutputWindow("Tool X - Local");
	m_mode_menu->Check(ID_ToolItemMenuGlobal, false);
	m_mode_menu->Check(ID_ToolItemMenuLocal, true);
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXMove(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("Tool X - Move");
	m_mode_menu->Check(ID_ToolItemMenuMove, true);
	m_mode_menu->Check(ID_ToolItemMenuRotate, false);
	m_mode_menu->Check(ID_ToolItemMenuScale, false);
	//mKalenEngine->SetTransformType(1);
	gTransformGizmoType = 1;
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXRotate(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("Tool X - Rotate");
	m_mode_menu->Check(ID_ToolItemMenuMove, false);
	m_mode_menu->Check(ID_ToolItemMenuRotate, true);
	m_mode_menu->Check(ID_ToolItemMenuScale, false);
	//mKalenEngine->SetTransformType(4);
	gTransformGizmoType = 2;
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXScale(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("Tool X - Scale");
	m_mode_menu->Check(ID_ToolItemMenuMove, false);
	m_mode_menu->Check(ID_ToolItemMenuRotate, false);
	m_mode_menu->Check(ID_ToolItemMenuScale, true);
	//mKalenEngine->SetTransformType(6);
	gTransformGizmoType = 3;
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXCopy(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		mCopiedObject = mKalenEngine->GetObject();
		AddTextToOutputWindow("Copied object");
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to copy!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXCut(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		AddTextToOutputWindow("Cut object");

		//Delete
		wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

		for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
		{
			if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
			{
				mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
				mKalenEngine->SetSelectedObject(nullptr);
				mDeletedObject = true;
				mTreeScene->Delete(mSelectedObjectItem);
				mSelectedObjectItem = NULL;
			}
		}
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to cut!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXPaste(wxCommandEvent& WXUNUSED(evt))
{
	wxMessageDialog *errorMsg = NULL;
	if (mTreeBuffer != NULL)
	{
		if (mActiveLayerItem != NULL)
		{
			if (mCopiedObject->GetType() == 0)//KLMESH
			{
				for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
				{
					if (GetLayerBuffer()->at(i)->GetIsActive())
					{
						LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
						localCoordinate = mCopiedObject->GetCoordinate();
					
						//localCoordinate->SetScale(XMFLOAT3(1.0f / localCoordinate->GetScale().x, 1.0f / localCoordinate->GetScale().y, 1.0f / localCoordinate->GetScale().z));
						wxTreeItemId treeID = AddObjectToLayer(&GetLayerBuffer()->at(i)->GetRootTreeId(), mTreeBufferNameObject, 0);
						GetLayerBuffer()->at(i)->AddObject(
							new Object
							(
								(string)mTreeBufferNameObject, mCopiedObject->GetTAG(), mCopiedObject->GetKLMESH(), *mCopiedObject->GetKLMESH()->GetAxisAlignedBoxOrigin(),
								*localCoordinate, mCopiedObject->GetIsCastShadow(), mCopiedObject->GetShadowDistance(), mCopiedObject->GetIsVisible(),
								0, mCopiedObject->GetPointLight(), -1
								),
							treeID);
						mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));

						//Select copied object
						if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
							mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

						if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
							mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

						mSelectedObjectItem = treeID;
						mTreeScene->SetItemImage(treeID, 8);
						mTreeScene->SetFocusedItem(treeID);//Focused item 

						wxTreeItemId idParent = mTreeScene->GetItemParent(mSelectedObjectItem);

						//Select copied object
						for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
						{
							if (mLayerBuffer->at(j)->GetRootTreeId() == idParent)
							{
								if (mLayerBuffer->at(j)->GetObject(mSelectedObjectItem) != nullptr)
								{
									mKalenEngine->SetSelectedObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));
									mKalenEngine->GetOctree()->AddObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));//Add to octree
								}
							}
						}
					}
				}
			}
			else
				if (mCopiedObject->GetType() == 1)//Light
				{
					for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
					{
						if (GetLayerBuffer()->at(i)->GetIsActive())
						{
							LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
							localCoordinate = mCopiedObject->GetCoordinate();
							wxTreeItemId treeID = AddObjectToLayer(&GetLayerBuffer()->at(i)->GetRootTreeId(), mTreeBufferNameObject, 1);

							AxisAlignedBox aabb;
							aabb.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
							aabb.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

							GetLayerBuffer()->at(i)->AddObject(
								new Object
								(
									(string)mTreeBufferNameObject, mCopiedObject->GetTAG(), nullptr, aabb,
									*localCoordinate, mCopiedObject->GetIsCastShadow(), mCopiedObject->GetShadowDistance(), mCopiedObject->GetIsVisible(),
									1, mCopiedObject->GetPointLight(), -1
									),
								treeID);
							mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));

							//Select copied object
							if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
								mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

							if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
								mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

							mSelectedObjectItem = treeID;
							mTreeScene->SetItemImage(treeID, 10);
							mTreeScene->SetFocusedItem(treeID);//Focused item 

							wxTreeItemId idParent = mTreeScene->GetItemParent(mSelectedObjectItem);

							//Select copied object
							for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
							{
								if (mLayerBuffer->at(j)->GetRootTreeId() == idParent)
								{
									if (mLayerBuffer->at(j)->GetObject(mSelectedObjectItem) != nullptr)
									{
										mKalenEngine->GetOctree()->AddObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));//Add to octree
										mKalenEngine->SetSelectedObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));
									}
								}
							}
						}
					}
				}
		}
		else
		{
			errorMsg = new wxMessageDialog(NULL, wxT("There is no active layer!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
			errorMsg->ShowModal();
		}
	}
	else
	{
		errorMsg = new wxMessageDialog(NULL, wxT("There is no object to paste!\nBuffer is empty."), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolXDelete(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{

		int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
		int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
		wxMessageDialog dialog(this, wxT("Delete this object?"), wxT("Delete"), wxOK | wxCANCEL, wxPoint(width, height));

		if (dialog.ShowModal() == wxID_OK)
		{
			wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

			for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
				{
					mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
					mKalenEngine->SetSelectedObject(nullptr);
					mDeletedObject = true;
					mTreeScene->Delete(mSelectedObjectItem);
					mSelectedObjectItem = NULL;
					AddTextToOutputWindow("Delete object");
				}
			}
		}
		else
		{
			AddTextToOutputWindow("Delete object -- cancel");
		}
		dialog.Destroy();
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to delete!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSNewWorld(wxCommandEvent& WXUNUSED(evt))
{
	wxString path = wxGetCwd();
	path << "\\data\\levels";
	wxFileDialog saveFileDialog(this, wxT("New World"), path, "", "KLWORLD (*.klw)|*.klw", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	//saveFileDialog.ShowModal();

	wxString pathToFile = "";
	wxString file = "";
	wxString temp = "";
	if (saveFileDialog.ShowModal() != wxID_CANCEL)
	{
		pathToFile = saveFileDialog.GetPath();
		file = saveFileDialog.GetFilename();



		
		path = wxGetCwd();

		for (int i = path.size() + 1; i < (int)(pathToFile.size() - file.size() - 1); ++i)
			temp << pathToFile.at(i);

		file = temp;

		BinaryFileWrite *binaryFileWrite = new BinaryFileWrite((std::string)pathToFile);
		klwFileStruct klworld;
		klworld.mName = (std::string)file;
		binaryFileWrite->writeKLWFileStructure(klworld);
		binaryFileWrite->closeFile();

		mWorldName = "";
		mWorldName << wxGetCwd();
		mWorldName << "\\";
		mWorldName = klworld.mName;

		//-----------------------------
		//Reset old layers
		//-----------------------------
		mLayerBuffer = new std::vector<Layer*>();

		mKalenEngine->SetSelectedObject(nullptr);
		mSelectedObjectItem = nullptr;
		mSelectedLayerItem = nullptr;

		mTreeScene->DeleteAllItems();
		mTreeRoot = mTreeScene->AddRoot(wxT("World                                                                        "), 0, 0);
		mTreeScene->SetItemState(mTreeRoot, 0);

		LoadAllFoldersAndFilesAsTree(mWorldName, mTreeRoot);
		Refresh();
		mKalenEngine->GetCamera()->LookAt(XMFLOAT3(50.0f, 50.0f, 50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.f));
		//-----------------------------
	}


}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSOpenWorld(wxCommandEvent& WXUNUSED(evt))
{
	wxString path = wxGetCwd();
	path << "\\data\\levels";
	wxFileDialog openFileDialog(this, wxT("Open World"), path, "", "KLWORLD (*.klw)|*.klw", wxFD_OPEN | wxFD_FILE_MUST_EXIST);


	wxString pathToFile = "";
	wxString file = "";

	if (openFileDialog.ShowModal() != wxID_CANCEL)
	{
		pathToFile = openFileDialog.GetPath();

		file = "";
		path = wxGetCwd();

		for (int i = path.size() + 1; i < (int)pathToFile.size(); ++i)
			file << pathToFile.at(i);

		BinaryFileRead *binaryFileRead = new BinaryFileRead((std::string)pathToFile);
		klwFileStruct klworld;
		//klworld.mName = (std::string)file;
		binaryFileRead->readKLWFileStructure(klworld);
		binaryFileRead->closeFile();

		mWorldName = "";
		mWorldName << wxGetCwd();
		mWorldName << "\\";

		mWorldName << klworld.mName;


		//-----------------------------
		//Reset old layers
		//-----------------------------
		mLayerBuffer = new std::vector<Layer*>();

		mKalenEngine->SetSelectedObject(nullptr);
		mSelectedObjectItem = nullptr;
		mSelectedLayerItem = nullptr;

		mTreeScene->DeleteAllItems();
		mTreeRoot = mTreeScene->AddRoot(wxT("World                                                                        "), 0, 0);
		mTreeScene->SetItemState(mTreeRoot, 0);

		Refresh();

		LoadAllFoldersAndFilesAsTree(mWorldName, mTreeRoot);

		mKalenEngine->GetCamera()->LookAt(XMFLOAT3(50.0f, 50.0f, 50.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.f));
		//-----------------------------
	}
}
//************************************************************************************************************************************************************************
void MyFrame::LoadAllFoldersAndFilesAsTree(wxString path, wxTreeItemId parent)
{


	wxDir* dir = new wxDir(path);

	wxString nameFile = "";
	const wxString emptyString = "";
	wxArrayString* dirListDirectories = new wxArrayString(0);
	wxArrayString* dirListFiles = new wxArrayString(0);
	wxString formatFile = "";
	bool isNextFile = false;
	wxTreeItemId parentFolder;
	wxTreeItemId temp;

	//----------------------------------------
	//Get directories
	//----------------------------------------
	isNextFile = dir->GetFirst(&nameFile, emptyString, wxDIR_DIRS);
	do
	{
		if (nameFile != "")
			dirListDirectories->Add(nameFile);
		isNextFile = dir->GetNext(&nameFile);
	} 
	while (isNextFile);

	if (dirListDirectories->size() != 0)
		for (int i = 0; i < (int)dirListDirectories->size(); ++i)
		{
			mTreeScene->SetItemState(parentFolder = mTreeScene->AppendItem(parent, dirListDirectories->Item(i), 1), 1);
			wxString parentPath = path;
			parentPath << "\\";
			parentPath << dirListDirectories->Item(i);
			LoadAllFoldersAndFilesAsTree(parentPath, parentFolder);
		}

	//----------------------------------------
	//Get all files
	//----------------------------------------
	isNextFile = dir->GetFirst(&nameFile, emptyString, wxDIR_FILES);
	do
	{
		if (nameFile != "")
			dirListFiles->Add(nameFile);
		isNextFile = dir->GetNext(&nameFile);
	} 
	while (isNextFile);

	if (dirListFiles->size() != 0)
		for (int i = 0; i < (int)dirListFiles->size(); ++i)
		{
			nameFile = "";
			for (int k = 0; k < (int)dirListFiles->Item(i).size(); ++k)
			{
				if (dirListFiles->Item(i).at(k) == '.')
				{
					formatFile = "";
					for (int j = k; j < (int)dirListFiles->Item(i).size(); ++j)
					{
						formatFile << dirListFiles->Item(i).at(j);
					}
					break;
				}
			}
			//--------------------------------
			//Add to layer to tree 
			//--------------------------------
			if (formatFile == ".kll" || formatFile == ".KLL")
			{
				formatFile = "";
				for (int k = 0; k < (int)dirListFiles->Item(i).size(); ++k)
					if (dirListFiles->Item(i).at(k) != '.')
						formatFile << dirListFiles->Item(i).at(k);
					else
						break;
				nameFile = formatFile;

				mTreeScene->SetItemState(temp = mTreeScene->AppendItem(parent, nameFile, 7), 2);
				mLayerBuffer->push_back(new Layer(nameFile, temp));
				Refresh();
			}
		}
	dir->Close();

	mKalenEngine->AllowRender(true); //start (one frame) rendering
}




//************************************************************************************************************************************************************************
void MyFrame::OnToolSRestoreWorld(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("Tool S - RestoreWorld");
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSSaveWorld(wxCommandEvent& WXUNUSED(evt))
{
	SaveAllLayersInFolderHier(mTreeRoot);
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSCopy(wxCommandEvent& WXUNUSED(evt)) 
{
	if (mSelectedObjectItem != NULL)
	{
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		mCopiedObject = mKalenEngine->GetObject();
		//mCopiedObject->GetCoordinate()->SetScale(XMFLOAT3((1.0f / mCopiedObject->GetCoordinate()->GetScale().x), (1.0f / mCopiedObject->GetCoordinate()->GetScale().y), (1.0f / mCopiedObject->GetCoordinate()->GetScale().z)));
		AddTextToOutputWindow("Copied object");
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to copy!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolMenuCut(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		mCopiedObject = mKalenEngine->GetObject();
		mCutObject = true;

		//Delete
		wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

		for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
		{
			if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
			{
				mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
				mKalenEngine->SetSelectedObject(nullptr);
				mDeletedObject = true;
				mTreeScene->Delete(mSelectedObjectItem);
				mSelectedObjectItem = NULL;
				AddTextToOutputWindow("Cut object");
			}
		}
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to cut!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSPaste(wxCommandEvent& WXUNUSED(evt)) 
{
	wxMessageDialog *errorMsg = NULL;
	if (mTreeBuffer != NULL)
	{
		if (mActiveLayerItem != NULL)
		{
			if (mCopiedObject->GetType() == 0)//KLMESH
			{
				for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
				{
					if (GetLayerBuffer()->at(i)->GetIsActive())
					{
						LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
						localCoordinate = mCopiedObject->GetCoordinate();
						//localCoordinate->SetScale(XMFLOAT3((1.0f / localCoordinate->GetScale().x), (1.0f / localCoordinate->GetScale().y), (1.0f / localCoordinate->GetScale().z)));

						wxTreeItemId treeID = AddObjectToLayer(&GetLayerBuffer()->at(i)->GetRootTreeId(), mTreeBufferNameObject, 0);
						GetLayerBuffer()->at(i)->AddObject(
							new Object
							(
								(string)mTreeBufferNameObject, mCopiedObject->GetTAG(), mCopiedObject->GetKLMESH(), *mCopiedObject->GetKLMESH()->GetAxisAlignedBoxOrigin(),
								*localCoordinate, mCopiedObject->GetIsCastShadow(), mCopiedObject->GetShadowDistance(), mCopiedObject->GetIsVisible(),
								0, mCopiedObject->GetPointLight(), -1
								),
							treeID);
						mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));

						//Select copied object
						if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
							mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

						if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
							mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

						mSelectedObjectItem = treeID;
						mTreeScene->SetItemImage(treeID, 8);
						mTreeScene->SetFocusedItem(treeID);//Focused item 

						wxTreeItemId idParent = mTreeScene->GetItemParent(mSelectedObjectItem);

						//Select copied object
						for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
						{
							if (mLayerBuffer->at(j)->GetRootTreeId() == idParent)
							{
								if (mLayerBuffer->at(j)->GetObject(mSelectedObjectItem) != nullptr)
								{
									mKalenEngine->SetSelectedObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));
									mKalenEngine->GetOctree()->AddObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));//Add to octree
								}
							}
						}
					}
				}
			}
			else
				if (mCopiedObject->GetType() == 1)//Light
				{
					for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
					{
						if (GetLayerBuffer()->at(i)->GetIsActive())
						{
							LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
							localCoordinate = mCopiedObject->GetCoordinate();
							wxTreeItemId treeID = AddObjectToLayer(&GetLayerBuffer()->at(i)->GetRootTreeId(), mTreeBufferNameObject, 1);

							AxisAlignedBox aabb;
							aabb.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
							aabb.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

							GetLayerBuffer()->at(i)->AddObject(
								new Object
								(
									(string)mTreeBufferNameObject, mCopiedObject->GetTAG(), nullptr, aabb,
									*localCoordinate, mCopiedObject->GetIsCastShadow(), mCopiedObject->GetShadowDistance(), mCopiedObject->GetIsVisible(), 
									1, mCopiedObject->GetPointLight(), -1
								), 
								treeID);
							mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));
							
							//Select copied object
							if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
								mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

							if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
								mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

							mSelectedObjectItem = treeID;
							mTreeScene->SetItemImage(treeID, 10);
							mTreeScene->SetFocusedItem(treeID);//Focused item 

							wxTreeItemId idParent = mTreeScene->GetItemParent(mSelectedObjectItem);

							//Select copied object
							for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
							{
								if (mLayerBuffer->at(j)->GetRootTreeId() == idParent)
								{
									if (mLayerBuffer->at(j)->GetObject(mSelectedObjectItem) != nullptr)
										mKalenEngine->SetSelectedObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));
								}
							}
						}
					}
				}
			AddTextToOutputWindow("Pasted object: " + mTreeBufferNameObject);

			if (mCutObject == true)
			{
				mTreeBuffer = nullptr;
				mTreeBufferNameObject = "";
				mCopiedObject = nullptr;
				mCutObject = false;
			}

			Refresh();
		}
		else 
		{
			errorMsg = new wxMessageDialog(NULL, wxT("There is no active layer!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
			errorMsg->ShowModal();
		}
	}
	else
	{
		errorMsg = new wxMessageDialog(NULL, wxT("There is no object to paste!\nBuffer is empty."), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSDelete(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{


		int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
		int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
		wxMessageDialog dialog(this, wxT("Delete this object?"), wxT("Delete"), wxOK | wxCANCEL, wxPoint(width, height));

		if (dialog.ShowModal() == wxID_OK)
		{
			wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

			for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
				{
					mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
					mKalenEngine->SetSelectedObject(nullptr);
					mDeletedObject = true;
					mTreeScene->Delete(mSelectedObjectItem);
					mSelectedObjectItem = NULL;
					AddTextToOutputWindow("Delete object");
				}

			}
		}
		else
		{
			AddTextToOutputWindow("Delete object -- cancel");
		}
		dialog.Destroy();
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to delete!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSAssetBrowser(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("Tool S - AssetBrowser");

	if (!mAssetBrowser->IsShown())
	{
		mAssetBrowser->Show(true);
	}
	else
	{
		mAssetBrowser->Hide();
		mAssetBrowser->Show(true);
	}
}

void  MyFrame::OnToolMenuOutputWindowShowHide(wxCommandEvent& WXUNUSED(evt))
{
	if (m_mgr.GetPane(wxT("outputWindow")).IsShown() == false)
	{
		m_mgr.GetPane(wxT("outputWindow")).Show();
	}
	else
	{
		m_mgr.GetPane(wxT("outputWindow")).Hide();
	}

	m_mgr.Update();
	Refresh();
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSGridMove(wxCommandEvent& evt)
{
	if (evt.IsChecked())
	{
		mChoiceMoveUnit->Enable(true);
		
		switch (mChoiceMoveUnit->GetSelection())
		{
		case 0:
			mKalenEngine->SetMoveUnit(0.25f);
			break;
		case 1:
			mKalenEngine->SetMoveUnit(0.5f);
			break;
		case 2:
			mKalenEngine->SetMoveUnit(1.0f);
			break;
		case 3:
			mKalenEngine->SetMoveUnit(3.0f);
			break;
		case 4:
			mKalenEngine->SetMoveUnit(5.0f);
			break;
		case 5:
			mKalenEngine->SetMoveUnit(10.0f);
			break;
		case 6:
			mKalenEngine->SetMoveUnit(25.0f);
			break;
		case 7:
			mKalenEngine->SetMoveUnit(50.0f);
			break;
		case 8:
			mKalenEngine->SetMoveUnit(100.0f);
			break;
		case 9:
			mKalenEngine->SetMoveUnit(300.0f);
			break;
		case 10:
			mKalenEngine->SetMoveUnit(500.0f);
			break;
		case 11:
			mKalenEngine->SetMoveUnit(1000.0f);
			break;
		case 12:
			mKalenEngine->SetMoveUnit(2500.0f);
			break;
		case 13:
			mKalenEngine->SetMoveUnit(5000.0f);
			break;
		case 14:
			mKalenEngine->SetMoveUnit(10000.0f);
			break;
		default:
			break;
		}
	}  
	else
	{
		mChoiceMoveUnit->Enable(false);
		mKalenEngine->SetMoveUnit(0.01f);
	}
	AddTextToOutputWindow("Tool S - GridMove");
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSChoiceMove(wxCommandEvent& evt)
{
	switch (evt.GetSelection())
	{
	case 0:
		AddTextToOutputWindow("Tool S - ChoiceMove #0 - 0.25cm");
		mKalenEngine->SetMoveUnit(0.25f);
		break;
	case 1:
		AddTextToOutputWindow("Tool S - ChoiceMove #1 - 0.5cm");
		mKalenEngine->SetMoveUnit(0.5f);
		break;
	case 2:
		AddTextToOutputWindow("Tool S - ChoiceMove #2 - 1.0cm");
		mKalenEngine->SetMoveUnit(1.0f);
		break;
	case 3:
		AddTextToOutputWindow("Tool S - ChoiceMove #3 - 3.0cm");
		mKalenEngine->SetMoveUnit(3.0f);
		break;
	case 4:
		AddTextToOutputWindow("Tool S - ChoiceMove #4 - 5.0cm");
		mKalenEngine->SetMoveUnit(5.0f);
		break;
	case 5:
		AddTextToOutputWindow("Tool S - ChoiceMove #5 - 10.0cm");
		mKalenEngine->SetMoveUnit(10.0f);
		break;
	case 6:
		AddTextToOutputWindow("Tool S - ChoiceMove #6 - 25.0cm");
		mKalenEngine->SetMoveUnit(25.0f);
		break;
	case 7:
		AddTextToOutputWindow("Tool S - ChoiceMove #7 - 50.0cm");
		mKalenEngine->SetMoveUnit(50.0f);
		break;
	case 8:
		AddTextToOutputWindow("Tool S - ChoiceMove #8 - 100.0cm");
		mKalenEngine->SetMoveUnit(100.0f);
		break;
	case 9:
		AddTextToOutputWindow("Tool S - ChoiceMove #9 - 300.0cm");
		mKalenEngine->SetMoveUnit(300.0f);
		break;
	case 10:
		AddTextToOutputWindow("Tool S - ChoiceMove #10 - 500.0cm");
		mKalenEngine->SetMoveUnit(500.0f);
		break;
	case 11:
		AddTextToOutputWindow("Tool S - ChoiceMove #11 - 1000.0cm");
		mKalenEngine->SetMoveUnit(1000.0f);
		break;
	case 12:
		AddTextToOutputWindow("Tool S - ChoiceMove #12 - 2500.0cm");
		mKalenEngine->SetMoveUnit(2500.0f);
		break;
	case 13:
		AddTextToOutputWindow("Tool S - ChoiceMove #13 - 5000.0cm");
		mKalenEngine->SetMoveUnit(5000.0f);
		break;
	case 14:
		AddTextToOutputWindow("Tool S - ChoiceMove #14 - 10000.0cm");
		mKalenEngine->SetMoveUnit(10000.0f);
		break;
	default:
		AddTextToOutputWindow("Tool S - ChoiceMove #NONE");
		break;
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSGridRotate(wxCommandEvent& evt)
{
	if (evt.IsChecked())
	{
		mChoiceRotateUnit->Enable(true);

		switch (mChoiceRotateUnit->GetSelection())
		{
			case 0:
				mKalenEngine->SetRotateDegree(1.0f);
				break;
			case 1:
				mKalenEngine->SetRotateDegree(3.0f);
				break;
			case 2:
				mKalenEngine->SetRotateDegree(5.0f);
				break;
			case 3:
				mKalenEngine->SetRotateDegree(10.0f);
				break;
			case 4:
				mKalenEngine->SetRotateDegree(15.0f);
				break;
			case 5:
				mKalenEngine->SetRotateDegree(30.0f);
				break;
			case 6:
				mKalenEngine->SetRotateDegree(45.0f);
				break;
			case 7:
				mKalenEngine->SetRotateDegree(90.0f);
				break;
		}
	}
	else
	{
		mChoiceRotateUnit->Enable(false);
		mKalenEngine->SetRotateDegree(0.01f);
	}
	AddTextToOutputWindow("Tool S - GridRotate");
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSChoiceRotate(wxCommandEvent& evt)
{
	switch (evt.GetSelection())
	{
	case 0:
		AddTextToOutputWindow("Tool S - ChoiceRotate #0 - 1\xB0");
		mKalenEngine->SetRotateDegree(1.0f);
		break;
	case 1:
		AddTextToOutputWindow("Tool S - ChoiceRotate #1 - 3\xB0");
		mKalenEngine->SetRotateDegree(3.0f);
		break;
	case 2:
		AddTextToOutputWindow("Tool S - ChoiceRotate #2 - 5\xB0");
		mKalenEngine->SetRotateDegree(5.0f);
		break;
	case 3:
		AddTextToOutputWindow("Tool S - ChoiceRotate #3 - 10\xB0");
		mKalenEngine->SetRotateDegree(10.0f);
		break;
	case 4:
		AddTextToOutputWindow("Tool S - ChoiceRotate #4 - 15\xB0");
		mKalenEngine->SetRotateDegree(15.0f);
		break;
	case 5:
		AddTextToOutputWindow("Tool S - ChoiceRotate #5 - 30\xB0");
		mKalenEngine->SetRotateDegree(30.0f);
		break;
	case 6:
		AddTextToOutputWindow("Tool S - ChoiceRotate #6 - 45\xB0");
		mKalenEngine->SetRotateDegree(45.0f);
		break;
	case 7:
		AddTextToOutputWindow("Tool S - ChoiceRotate #7 - 90\xB0");
		mKalenEngine->SetRotateDegree(90.0f);
		break;
	default:
		AddTextToOutputWindow("Tool S - ChoiceRotate #NONE");
		break;
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSChoiceSnap(wxCommandEvent& evt)
{
	switch (evt.GetSelection())
	{
	case 0:
		AddTextToOutputWindow("Tool S - ChoiceSnap #0 - No snap");
		m_mode_menu->Check(ID_ToolItemMenuSnap, true);
		m_mode_menu->Check(ID_ToolItemMenuSnapTerrain, false);
		m_mode_menu->Check(ID_ToolItemMenuSnapAll, false);
		break;
	case 1:
		AddTextToOutputWindow("Tool S - ChoiceSnap #1 - Snap to terrain");
		m_mode_menu->Check(ID_ToolItemMenuSnap, false);
		m_mode_menu->Check(ID_ToolItemMenuSnapTerrain, true);
		m_mode_menu->Check(ID_ToolItemMenuSnapAll, false);
		break;
	case 2:
		AddTextToOutputWindow("Tool S - ChoiceSnap #2 - Snap to all");
		m_mode_menu->Check(ID_ToolItemMenuSnap, false);
		m_mode_menu->Check(ID_ToolItemMenuSnapTerrain, false);
		m_mode_menu->Check(ID_ToolItemMenuSnapAll, true);
		break;
	default:
		AddTextToOutputWindow("Tool S - ChoiceSnap #NONE");
		break;
	}

}
//************************************************************************************************************************************************************************
void MyFrame::OnToolSChoiceRender(wxCommandEvent& evt)
{
	switch (evt.GetSelection())
	{
	case 0:
		AddTextToOutputWindow("Tool S - ChoiceRender #0 - Normal Rendering");
		break;
	case 1:
		AddTextToOutputWindow("Tool S - ChoiceRender #1 - Diffuse Only");
		break;
	case 2:
		AddTextToOutputWindow("Tool S - ChoiceRender #2 - Simple Lighting");
		break;
	case 3:
		AddTextToOutputWindow("Tool S - ChoiceRender #3 - Specular Only");
		break;
	case 4:
		AddTextToOutputWindow("Tool S - ChoiceRender #4 - Specular Glossiness");
		break;
	case 5:
		AddTextToOutputWindow("Tool S - ChoiceRender #5 - SSAO Only");
		break;
	case 6:
		AddTextToOutputWindow("Tool S - ChoiceRender #6 - Bloom");
		break;
	default:
		AddTextToOutputWindow("Tool S - ChoiceRender #NONE");
		break;
	}
}

//************************************************************************************************************************************************************************
void MyFrame::OnTreeRightClick(wxTreeEvent& evt)
{
	mRightClickedItem = evt.GetItem();
	wxString text = "";
	wxMenu menu(wxT(""));

	wxPoint point = evt.GetPoint();
	point.y = point.y + 20;

	if (mWorldName != "")
	{

		if (mTreeScene->GetItemState(evt.GetItem()) == 0)
		{
			mSelectedLayerItem = evt.GetItem();
			text << "ROOT SELECTED";
			AddTextToOutputWindow(text);
			//menu.Append(ID_TreeSceneMenuFolderSaveHier, wxT("Save hierarchy"));
			//menu.Append(ID_TreeSceneMenuFolderSaveFolder, wxT("Save All"));
			menu.Append(ID_TreeSceneMenuFolderSaveHier, wxT("Save All"));
			menu.AppendSeparator();
			menu.Append(ID_TreeSceneMenuFolderLoadAll, wxT("Load All"));
			menu.AppendSeparator();
			menu.Append(ID_TreeSceneMenuFolderNewLayer, wxT("Add new layer"));
			menu.Append(ID_TreeSceneMenuFolderNewFolder, wxT("Add new folder"));
			PopupMenu(&menu, wxPoint(wxGetMousePosition().x, (wxGetMousePosition().y - 40)));
		}
		else if (mTreeScene->GetItemState(evt.GetItem()) == 1) //folder
		{
			text << "FOLDER: '" << text << mTreeScene->GetItemText(evt.GetItem()) << "' SELECTED";
			AddTextToOutputWindow(text);

			mSelectedLayerItem = evt.GetItem();
			menu.Append(ID_TreeSceneMenuFolderSaveHier, wxT("Save hierarchy"));
			menu.Append(ID_TreeSceneMenuFolderSaveFolder, wxT("Save this folder"));
			menu.AppendSeparator();
			menu.Append(ID_TreeSceneMenuFolderLoadAll, wxT("Load All"));
			menu.AppendSeparator();
			menu.Append(ID_TreeSceneMenuFolderNewLayer, wxT("Add new layer"));
			menu.Append(ID_TreeSceneMenuFolderNewFolder, wxT("Add new folder"));
			//menu.AppendSeparator();
			//menu.Append(ID_TreeSceneMenuFolderRemove, wxT("Remove")); //Doesn't work :(
			PopupMenu(&menu, wxPoint(wxGetMousePosition().x, (wxGetMousePosition().y - 40)));

		}
		else if (mTreeScene->GetItemState(evt.GetItem()) == 2) //layer
		{
			mSelectedLayerItem = evt.GetItem();
			text << "LAYER: '" << text << mTreeScene->GetItemText(evt.GetItem()) << "' SELECTED";
			AddTextToOutputWindow(text);

			//layer
			if (mTreeScene->GetItemImage(evt.GetItem()) == 2)
			{
				menu.Append(ID_TreeSceneMenuLayerSave, wxT("Save"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerMakeActive, wxT("Make active"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerRemove, wxT("Remove"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerHide, wxT("Hide"));
			}
			//Active layer
			if (mTreeScene->GetItemImage(evt.GetItem()) == 4)
			{
				menu.Append(ID_TreeSceneMenuLayerSave, wxT("Save"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerDeactivation, wxT("Deactivation"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerRemove, wxT("Remove"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerHide, wxT("Hide"));
			}
			//no load layer
			if (mTreeScene->GetItemImage(evt.GetItem()) == 7)
			{
				menu.Append(ID_TreeSceneMenuLayerLoad, wxT("Load"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerRemove, wxT("Remove"));
			}
			//Hidden layer
			if (mTreeScene->GetItemImage(evt.GetItem()) == 6)
			{
				menu.Append(ID_TreeSceneMenuLayerSave, wxT("Save"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerMakeActive, wxT("Make active"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerRemove, wxT("Remove"));
				menu.AppendSeparator();
				menu.Append(ID_TreeSceneMenuLayerShow, wxT("Show"));
			}

			PopupMenu(&menu, wxPoint(wxGetMousePosition().x, (wxGetMousePosition().y - 30)));
		}
		else if (mTreeScene->GetItemState(evt.GetItem()) == 3)//object
		{
			mSelectedLayerItem = evt.GetItem();
			text << "OBJECT: '" << text << mTreeScene->GetItemText(evt.GetItem()) << "' SELECTED";
			AddTextToOutputWindow(text);
			menu.Append(ID_TreeSceneMenuObjectLook, wxT("Look At Object"));
			menu.AppendSeparator();
			menu.Append(ID_TreeSceneMenuObjectCopy, wxT("Copy"));
			menu.Append(ID_TreeSceneMenuObjectCut, wxT("Cut"));
			menu.Append(ID_TreeSceneMenuObjectDelete, wxT("Delete"));

			PopupMenu(&menu, wxPoint(wxGetMousePosition().x, (wxGetMousePosition().y - 30)));
		}
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeLeftDoubleClick(wxTreeEvent& evt)
{
	
	if (mTreeScene->GetItemState(evt.GetItem()) == 2 && mTreeScene->GetItemImage(evt.GetItem()) != 7)
	{
		if (mActiveLayerName.size() != 0) //first use
		{
			mTreeScene->SetItemText(mActiveLayerItem, mActiveLayerName);
			mTreeScene->SetItemImage(mActiveLayerItem, 2);//restore label
		}

		mActiveLayerItem = evt.GetItem();
		mActiveLayerName = mTreeScene->GetItemText(evt.GetItem());

		wxString text = "--ACTIVE-- ";
		text << mActiveLayerName;
		mTreeScene->SetItemText(evt.GetItem(), text);
		mTreeScene->SetItemImage(evt.GetItem(), 4);//set active label


		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
		{
			mLayerBuffer->at(i)->SetIsActive(false);
			if (mLayerBuffer->at(i)->GetRootTreeId() == evt.GetItem())
			{
				mLayerBuffer->at(i)->SetIsActive(true);
			}
		}
	}
	Refresh();
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerMakeActive(wxCommandEvent& WXUNUSED(evt))
{
	if (mTreeScene->GetItemState(mSelectedLayerItem) == 2 && mTreeScene->GetItemImage(mSelectedLayerItem) != 7)
	{
		if (mActiveLayerName.size() != 0) //first use
		{
			mTreeScene->SetItemText(mActiveLayerItem, mActiveLayerName);
			mTreeScene->SetItemImage(mActiveLayerItem, 2);//restore label
		}
		mActiveLayerItem = mSelectedLayerItem;
		mActiveLayerName = mTreeScene->GetItemText(mSelectedLayerItem);

		wxString text = "--ACTIVE-- ";
		text << mActiveLayerName;
		mTreeScene->SetItemText(mSelectedLayerItem, text);
		mTreeScene->SetItemImage(mActiveLayerItem, 4);//set active label


		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
		{
			mLayerBuffer->at(i)->SetIsActive(false);
			if (mLayerBuffer->at(i)->GetRootTreeId() == mSelectedLayerItem)
			{
				//text += mLayerBuffer->at(i)->GetName();
				mLayerBuffer->at(i)->SetIsActive(true);
				//AddTextToOutputWindow(text);
			}
		}
	}

	Refresh();
	AddTextToOutputWindow("Tree - click - OnTreeSceneMenuLayerMakeActive");
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerDeactivation(wxCommandEvent& evt)
{
	if (mTreeScene->GetItemState(mSelectedLayerItem) == 2 && mTreeScene->GetItemImage(mSelectedLayerItem) == 4)
	{
		if (mActiveLayerName.size() != 0) //first use
		{
			mTreeScene->SetItemText(mActiveLayerItem, mActiveLayerName);
			mTreeScene->SetItemImage(mActiveLayerItem, 2);//restore label

			//Deactivation all layers
			for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
			{
				mLayerBuffer->at(i)->SetIsActive(false);
			}
		}
	}
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuFolderSaveFolder(wxCommandEvent& WXUNUSED(evt))
{
	wxTreeItemId parent = mTreeScene->GetItemParent(mSelectedLayerItem);
	for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
	{
		if (mTreeScene->GetItemParent(mLayerBuffer->at(i)->GetRootTreeId()) == mSelectedLayerItem)
		{
			SaveLayer(mLayerBuffer->at(i)->GetRootTreeId());
		}
	}
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuFolderSaveHier(wxCommandEvent& WXUNUSED(evt))
{
	SaveAllLayersInFolderHier(mSelectedLayerItem);
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuFolderNewLayer(wxCommandEvent& WXUNUSED(evt))
{
	//Get layer name -- TextDialog
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxTextEntryDialog dialog(this, wxT("Enter layer name"), wxT("New layer"), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

	//values
	wxTreeItemIdValue cookie;
	bool errorName = false;
	wxTreeItemId child = mTreeScene->GetFirstChild(mSelectedLayerItem, cookie);
	wxTreeItemId lastChild = mTreeScene->GetLastChild(mSelectedLayerItem);
	bool isNextLayer = true;
	wxTreeItemId temp;

	if (dialog.ShowModal() == wxID_OK) //Press ok button
	{
		if (dialog.GetValue().size() != 0) //Name is not empty
		{
			if (child != NULL && lastChild != NULL)//has layer child? When is it true? Layer has no any children.
			{
				if ((mTreeScene->GetItemState(child) == 2) && mTreeScene->GetItemText(child) == dialog.GetValue()) //Test first child
				{
					errorName = true;
					isNextLayer = false;
				}
				else //Test next children 
				{
					if (lastChild != child) //Are there any layer? 
						isNextLayer = true;//Yes
					else
						isNextLayer = false;//No

					while (isNextLayer) //While next layer exist.
					{
						child = mTreeScene->GetNextChild(mSelectedLayerItem, cookie); //get next child from parent (selected node) 
						if (mTreeScene->GetItemState(child) == 2 && mTreeScene->GetItemText(child) == dialog.GetValue()) //check only layers and compare strings
							errorName = true;
						if (lastChild != child) //Are there any layers? 
							isNextLayer = true;//Yes
						else
							isNextLayer = false;//No
					}
				}
			}
			else
				errorName = false;//Layer has no children

			if (errorName == true) //Layer EXIST!
			{
				AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewLayer - ERROR!");
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("This layer exist!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}

			else //Layer can be create
			{
				mTreeScene->SetItemState(temp = mTreeScene->AppendItem(mSelectedLayerItem, dialog.GetValue(), 2), 2);
				AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewLayer -- Added new layer...");
				mLayerBuffer->push_back(new Layer(dialog.GetValue(), temp));
				SaveLayer(temp);
				Refresh();
			}
		}
		else  //Name IS empty
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect layer name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
			AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewLayer - ERROR!");
		}
	}
	else //Cancel set name
		AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewLayer - CANCEL");

	dialog.Destroy();
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuFolderNewFolder(wxCommandEvent& WXUNUSED(evt))
{

	//Get folder name -- TextDialog
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxTextEntryDialog dialog(this, wxT("Enter folder name"), wxT("New folder"), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

	//values
	wxTreeItemIdValue cookie;
	bool errorName = false;
	wxTreeItemId child = mTreeScene->GetFirstChild(mSelectedLayerItem, cookie);
	wxTreeItemId lastChild = mTreeScene->GetLastChild(mSelectedLayerItem);
	bool isNextFolder = true;


	if (dialog.ShowModal() == wxID_OK) //Press ok button
	{
		if (dialog.GetValue().size() != 0) //Name is not empty
		{
			if (child != NULL && lastChild != NULL)//has folder child? When is it true? Folder has no any children.
			{
				if ((mTreeScene->GetItemState(child) == 1) && mTreeScene->GetItemText(child) == dialog.GetValue()) //Test first child
				{
					errorName = true;
					isNextFolder = false;
				}
				else //Test next children 
				{
					if (lastChild != child) //Are there any folder? 
						isNextFolder = true;//Yes
					else
						isNextFolder = false;//No

					while(isNextFolder) //While next folder exist.
					{
						child = mTreeScene->GetNextChild(mSelectedLayerItem, cookie); //get next child from parent (selected node) 
						if (mTreeScene->GetItemState(child) == 1 && mTreeScene->GetItemText(child) == dialog.GetValue()) //check only folders and compare strings
							errorName = true;
						if (lastChild != child) //Are there any folder? 
							isNextFolder = true;//Yes
						else
							isNextFolder = false;//No
					}
				}
			}
			else
				errorName = false;//Folder has no children

			if (errorName == true) //Folder EXIST!
			{
				AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewFolder - ERROR!");
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("This folder exist!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}
				
			else //Folder can be create
			{
				wxTreeItemId folder;
				mTreeScene->SetItemState(folder = mTreeScene->AppendItem(mSelectedLayerItem, dialog.GetValue(), 1), 1);
				wxTreeItemId parent = folder;
				wxString folderName = "";
				wxString path = "";

				std::vector<wxString>* folders = new std::vector<wxString>();
				while (parent != mTreeRoot)
				{
					parent = mTreeScene->GetItemParent(parent);
					folderName = mTreeScene->GetItemText(parent);
					folders->push_back(folderName);
				}

				path = "";
				for (int i = (int)folders->size() - 2; i >= 0; --i)
				{
					path << folders->at(i);
					path << "\\";
				}

				folderName = mTreeScene->GetItemText(folder);
				path << folderName;

				AddTextToOutputWindow("\n" + path + "\n");

				wxString pathHome = wxGetCwd();
				//pathHome << "\\data\\levels";
				pathHome << "\\";
				pathHome << mWorldName;

				wxDir* dir = new wxDir(pathHome);


				pathHome << "\\";
				pathHome << path;

				//Create folder
				dir->Make(pathHome, wxS_DIR_DEFAULT, 0);
				dir->Close();
				Refresh();
			}
		}
		else  //Name IS empty
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect folder name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
			AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewFolder - ERROR!");
		}
	}
	else //Cancel set name
		AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderNewFolder - CANCEL");

	dialog.Destroy();
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuFolderRemove(wxCommandEvent& WXUNUSED(evt))
{
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxMessageDialog dialog(this, wxT("Remove this folder and all files that are inside?"), wxT("Remove folder?"), wxOK | wxCANCEL, wxPoint(width, height));

	if (dialog.ShowModal() == wxID_OK)
	{

		DeleteFolderHier(mSelectedLayerItem);


		AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderRemove - Remove");
	}
	else
	{
		AddTextToOutputWindow("Tree - click - OnTreeSceneMenuFolderRemove - Cancel");
	}
	dialog.Destroy();
	Refresh();
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuFolderLoadAll(wxCommandEvent& evt)
{
	//wxProgressDialog* dialog = new wxProgressDialog(wxT("Loading..."), wxT("Keep waiting..."), 1, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);
	//dialog->Update(0);
	LoadAllLayersInFolderHier(mRightClickedItem);
	//dialog->Update(1);
	//delete dialog;
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerSave(wxCommandEvent& evt)
{
	SaveLayer(mRightClickedItem);
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerLoad(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedLayerItem != nullptr)
	{
		if (mTreeScene->GetItemState(mSelectedLayerItem) == 2 && mTreeScene->GetItemImage(mSelectedLayerItem) == 7)
		{
			mTreeScene->SetItemImage(mSelectedLayerItem, 2);
			LoadLayer(mSelectedLayerItem);
		}
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerShow(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedLayerItem != nullptr)
	{
		if (mTreeScene->GetItemState(mSelectedLayerItem) == 2 && mTreeScene->GetItemImage(mSelectedLayerItem) == 6)
		{
			mTreeScene->SetItemImage(mSelectedLayerItem, 2);
			for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == mSelectedLayerItem)
				{
					for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)
					{
						mLayerBuffer->at(i)->GetAllObjects()->at(j)->SetIsVisible(true);
					}
				}
			}
		}
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerHide(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedLayerItem != nullptr)
	{
		if (mTreeScene->GetItemState(mSelectedLayerItem) == 2)
		{
			mTreeScene->SetItemImage(mSelectedLayerItem, 6);//set hide label
			for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == mSelectedLayerItem)
				{
					for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)
					{
						mLayerBuffer->at(i)->GetAllObjects()->at(j)->SetIsVisible(false);
					}
				}
			}
		}
	}
	//AddTextToOutputWindow("Tree - click - OnTreeSceneMenuLayerHide");
	Refresh();
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuLayerRemove(wxCommandEvent& WXUNUSED(evt))
{
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxMessageDialog dialog(this, wxT("Remove this layer and all objects that are inside?"), wxT("Remove layer?"), wxOK | wxCANCEL, wxPoint(width, height));

	if (dialog.ShowModal() == wxID_OK)
	{
		DeleteLayer(mSelectedLayerItem);
	}
	else
	{
		AddTextToOutputWindow("Tree - click - OnTreeSceneMenuLayerRemove - Cancel");
	}
	dialog.Destroy();
	Refresh();
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuObjectLook(wxCommandEvent& WXUNUSED(evt))
{
	wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
	{
		if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
		{
			mKalenEngine->RebuildBoundingBox(mLayerBuffer->at(i)->GetObject(mSelectedObjectItem));

			pos.x = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Center.x;
			pos.x += mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Extents.x * 2;
			pos.y = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Center.y;
			pos.y += mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Extents.y * 2;
			pos.z = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Center.z;
			pos.z += mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Extents.z * 2;

			target = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetCoordinate()->GetLocalCoordinatePosition();
			//Set camera
			mKalenEngine->GetCamera()->LookAt(pos, target, up);
		}
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuObjectCopy(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{
		mTreeBuffer = mSelectedObjectItem;
		AddTextToOutputWindow("Copied object");
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to copy!\nSelect object3D!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuObjectCut(wxCommandEvent& WXUNUSED(evt))
{
	if (mSelectedObjectItem != NULL)
	{
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		mCopiedObject = mKalenEngine->GetObject();
		mCutObject = true;

		int position = -1;
		for (int i = 0; i < (int)mKalenEngine->GetRenderObjectsBuffer()->size(); ++i)
		{
			if (mKalenEngine->GetRenderObjectsBuffer()->at(i)->GetTreeItemId() == mSelectedObjectItem)
				position = i;
		}
		if (position != -1)
		{
			mKalenEngine->GetRenderObjectsBuffer()->erase(mKalenEngine->GetRenderObjectsBuffer()->begin() + position);

			mKalenEngine->SetSelectedObject(nullptr);
			mDeletedObject = true;
			mTreeScene->Delete(mSelectedObjectItem);
			mSelectedObjectItem = NULL;
			AddTextToOutputWindow("Cut object");
		}
		Refresh();
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("There is no object to cut!\nSelect object3D!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************
void MyFrame::OnTreeSceneMenuObjectDelete(wxCommandEvent& WXUNUSED(evt))
{
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxMessageDialog dialog(this, wxT("Delete this object?"), wxT("Delete"), wxOK | wxCANCEL, wxPoint(width, height));

	if (dialog.ShowModal() == wxID_OK)
	{
		wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

		for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
		{
			if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
			{
				mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
				mKalenEngine->SetSelectedObject(nullptr);
				mDeletedObject = true;
				mTreeScene->Delete(mSelectedObjectItem);
				mSelectedObjectItem = NULL;
				AddTextToOutputWindow("Delete object");
			}

		}
		AddTextToOutputWindow("Delete object");
	}
	else
	{
		AddTextToOutputWindow("Delete object -- cancel");
	}
	dialog.Destroy();
	Refresh();
	mKalenEngine->AllowRender(true); //start (one frame) rendering
}
//************************************************************************************************************************************************************************



void MyFrame::OnTreeSeletionING(wxTreeEvent& evt)
{
	mKalenEngine->AllowRender(true); //start (one frame) rendering
//	wxTreeItemId abc = evt.GetItem();


	//AddTextToOutputWindow("SELECTING");
}

void MyFrame::OnTreeSeletionED(wxTreeEvent& evt)
{
	mKalenEngine->AllowRender(true); //start (one frame) rendering
	wxString text = "";

	if (mDeletedObject == false)
	{
		if (mTreeScene->GetItemImage(evt.GetItem()) == 3) // -- If the object3D is not selected - Make Selected
		{
			if (mSelectedObjectItem != NULL) //first use
			{
				if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
					mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

				if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
					mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label
			}
			else
			{
				if (mTreeScene->GetItemImage(evt.GetItem()) == 8)
					mTreeScene->SetItemImage(evt.GetItem(), 3);//restore object3D label

				if (mTreeScene->GetItemImage(evt.GetItem()) == 10)
					mTreeScene->SetItemImage(evt.GetItem(), 9);//restore light label
			}
			mSelectedObjectItem = evt.GetItem();
			mTreeScene->SetItemImage(evt.GetItem(), 8);

			wxTreeItemId idParent = mTreeScene->GetItemParent(evt.GetItem());


			//Find object by TreeItemID
			for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == idParent)
				{
					if (mLayerBuffer->at(i)->GetObject(evt.GetItem()) != nullptr)
						mKalenEngine->SetSelectedObject(mLayerBuffer->at(i)->GetObject(evt.GetItem()));
				}
			}
		}
		/*else if (mTreeScene->GetItemImage(evt.GetItem()) == 8)//deselect
		{
			mTreeScene->SetItemImage(evt.GetItem(), 3);//restore label
			//mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore label
			mSelectedObjectItem = NULL;
		}*/
		//Light case - 
		else if (mTreeScene->GetItemImage(evt.GetItem()) == 9)//select
		{
			if (mSelectedObjectItem != NULL) //first use
			{
				if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
					mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

				if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
					mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label
			}
			mSelectedObjectItem = evt.GetItem();
			mTreeScene->SetItemImage(evt.GetItem(), 10);

			wxTreeItemId idParent = mTreeScene->GetItemParent(evt.GetItem());

			for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == idParent)
				{
					if (mLayerBuffer->at(i)->GetObject(evt.GetItem()) != nullptr)
						mKalenEngine->SetSelectedObject(mLayerBuffer->at(i)->GetObject(evt.GetItem()));
				}
			}
		}
		/*else if (mTreeScene->GetItemImage(evt.GetItem()) == 10)//deselect
		{
			mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore label
			mSelectedObjectItem = NULL;
		}*/


		//Selected a layer
		else
			if (mTreeScene->GetItemState(evt.GetItem()) == 2)
			{
				wxTreeItemId id = evt.GetItem();

				if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
					mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

				if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
					mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

				mSelectedObjectItem = NULL;

				for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
				{
					if (mLayerBuffer->at(i)->GetRootTreeId() == id)
					{
						text += mLayerBuffer->at(i)->GetName();
						AddTextToOutputWindow(text);
					}

				}
			}
	}
	mDeletedObject = false;
	Refresh();
}


void MyFrame::OnToolMenuUndo(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuUndo");
}
void MyFrame::OnToolMenuRedo(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuRedo");
}
void MyFrame::OnToolMenuLookAt(wxCommandEvent& WXUNUSED(evt))
{
	wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);
	XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
	{
		if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
		{
			mKalenEngine->RebuildBoundingBox(mLayerBuffer->at(i)->GetObject(mSelectedObjectItem));

			pos.x = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Center.x;
			pos.x += mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Extents.x * 2;
			pos.y = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Center.y;
			pos.y += mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Extents.y * 2;
			pos.z = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Center.z;
			pos.z += mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetAxisAlignedBoundingBox().Extents.z * 2;

			target = mLayerBuffer->at(i)->GetObject(mSelectedObjectItem)->GetCoordinate()->GetLocalCoordinatePosition();
			//Set camera
			mKalenEngine->GetCamera()->LookAt(pos, target, up);
		}
	}

	AddTextToOutputWindow("OnToolMenuLookAt");
}
void MyFrame::OnToolMenuCancel(wxCommandEvent& WXUNUSED(evt))
{
	mSelectedObjectItem = nullptr;
	mKalenEngine->SetSelectedObject(nullptr);
}
//-------------------------------------------------------------------------------------------------------------------
void MyFrame::OnToolMenuActiveLayer(wxCommandEvent& WXUNUSED(evt))
{
	if (mToolBarLeft->GetToolToggled(ID_ToolItemACTIVE_L) == true)
	{
		mToolBarLeft->ToggleTool(ID_ToolItemMULTI_L, true);
		AddTextToOutputWindow("OnToolMenuMultiLayer - Hotkey");
		m_edit_menu->Check(ID_ToolItemMenuActiveLayer, false);
		m_edit_menu->Check(ID_ToolItemMenuMultiLayer, true);
	}
	else
	{
		mToolBarLeft->ToggleTool(ID_ToolItemACTIVE_L, true);
		AddTextToOutputWindow("OnToolMenuActiveLayer - Hotkey");
		m_edit_menu->Check(ID_ToolItemMenuActiveLayer, true);
		m_edit_menu->Check(ID_ToolItemMenuMultiLayer, false);
	}
	Refresh();
}
//-------------------------------------------------------------------------------------------------------------------
void MyFrame::OnToolMenuActiveLayerClick(wxCommandEvent& WXUNUSED(evt))
{
	gMultiLayers = false;
	mToolBarLeft->ToggleTool(ID_ToolItemACTIVE_L, true);
	AddTextToOutputWindow("OnToolMenuActiveLayer - Click");
	m_edit_menu->Check(ID_ToolItemMenuActiveLayer, true);
	m_edit_menu->Check(ID_ToolItemMenuMultiLayer, false);
	Refresh();
}
//-------------------------------------------------------------------------------------------------------------------
void MyFrame::OnToolMenuMultiLayerClick(wxCommandEvent& WXUNUSED(evt))
{
	gMultiLayers = true;
	mToolBarLeft->ToggleTool(ID_ToolItemMULTI_L, true);
	AddTextToOutputWindow("OnToolMenuMultiLayer - Click");
	m_edit_menu->Check(ID_ToolItemMenuActiveLayer, false);
	m_edit_menu->Check(ID_ToolItemMenuMultiLayer, true);
	Refresh();
}
//-------------------------------------------------------------------------------------------------------------------

void MyFrame::OnToolMenuMove(wxCommandEvent& WXUNUSED(evt))
{
	mToolBarLeft->ToggleTool(ID_ToolItemMOVE, true);
	AddTextToOutputWindow("OnToolMenuMove");
	m_mode_menu->Check(ID_ToolItemMenuMove, true);
	m_mode_menu->Check(ID_ToolItemMenuRotate, false);
	m_mode_menu->Check(ID_ToolItemMenuScale, false);
	//mKalenEngine->SetTransformType(1);
	gTransformGizmoType = 1;
	Refresh();
}
void MyFrame::OnToolMenuRotate(wxCommandEvent& WXUNUSED(evt))
{
	mToolBarLeft->ToggleTool(ID_ToolItemROTATE, true);
	AddTextToOutputWindow("OnToolMenuRotate");
	m_mode_menu->Check(ID_ToolItemMenuMove, false);
	m_mode_menu->Check(ID_ToolItemMenuRotate, true);
	m_mode_menu->Check(ID_ToolItemMenuScale, false);
	//mKalenEngine->SetTransformType(4);
	gTransformGizmoType = 2;
	Refresh();
}
void MyFrame::OnToolMenuScale(wxCommandEvent& WXUNUSED(evt))
{
	mToolBarLeft->ToggleTool(ID_ToolItemSCALE, true);
	AddTextToOutputWindow("OnToolMenuScale");
	m_mode_menu->Check(ID_ToolItemMenuMove, false);
	m_mode_menu->Check(ID_ToolItemMenuRotate, false);
	m_mode_menu->Check(ID_ToolItemMenuScale, true);
	//mKalenEngine->SetTransformType(6);
	gTransformGizmoType = 3;
	Refresh();
}
void MyFrame::OnToolMenuSnap(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuSnap");
	mChoiceSnap->SetSelection(0);
	m_mode_menu->Check(ID_ToolItemMenuSnap, true);
	m_mode_menu->Check(ID_ToolItemMenuSnapTerrain, false);
	m_mode_menu->Check(ID_ToolItemMenuSnapAll, false);
}
void MyFrame::OnToolMenuSnapTerrain(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuSnapTerrain");
	mChoiceSnap->SetSelection(1);
	m_mode_menu->Check(ID_ToolItemMenuSnap, false);
	m_mode_menu->Check(ID_ToolItemMenuSnapTerrain, true);
	m_mode_menu->Check(ID_ToolItemMenuSnapAll, false);
}
void MyFrame::OnToolMenuSnapAll(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuSnapAll");
	mChoiceSnap->SetSelection(2);
	m_mode_menu->Check(ID_ToolItemMenuSnap, false);
	m_mode_menu->Check(ID_ToolItemMenuSnapTerrain, false);
	m_mode_menu->Check(ID_ToolItemMenuSnapAll, true);
}
void MyFrame::OnToolMenuLocal(wxCommandEvent& WXUNUSED(evt))
{
	if (mToolBarLeft->GetToolToggled(ID_ToolItemGLOBAL) == true)
	{
		AddTextToOutputWindow("OnToolMenuLocal");
		gGlobalSpace = false;
		mToolBarLeft->ToggleTool(ID_ToolItemLOCAL, true);
		m_mode_menu->Check(ID_ToolItemMenuGlobal, false);
		m_mode_menu->Check(ID_ToolItemMenuLocal, true);
	}
	else
	{
		AddTextToOutputWindow("OnToolMenuGlobal");
		gGlobalSpace = true;
		mToolBarLeft->ToggleTool(ID_ToolItemGLOBAL, true);
		m_mode_menu->Check(ID_ToolItemMenuGlobal, true);
		m_mode_menu->Check(ID_ToolItemMenuLocal, false);
	}
	Refresh();

}
void MyFrame::OnToolMenuGlobal(wxCommandEvent& WXUNUSED(evt))//no use
{
	AddTextToOutputWindow("OnToolMenuGlobal");
	gGlobalSpace = true;
	mToolBarLeft->ToggleTool(ID_ToolItemGLOBAL, true);
	m_mode_menu->Check(ID_ToolItemMenuGlobal, true);
	m_mode_menu->Check(ID_ToolItemMenuLocal, false);
	Refresh();
}

//************************************************************************************************************************************************************************
void MyFrame::OnToolMenuFiltersWindow(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuFiltersWindow");
	if(!mFiltersWindow->IsShown())
		mFiltersWindow->Show(true);
	else
	{
		mFiltersWindow->Hide();
		mFiltersWindow->Show(true);
	}
}
//************************************************************************************************************************************************************************
void MyFrame::OnToolMenuAssetBrowsersWindow(wxCommandEvent& WXUNUSED(evt))
{
	AddTextToOutputWindow("OnToolMenuAssetBrowsersWindow");
	if (!mAssetBrowser->IsShown())
		mAssetBrowser->Show(true);
	else
	{
		mAssetBrowser->Hide();
		mAssetBrowser->Show(true);
	}
}









//************************************************************************************************************************************************************************
void MyFrame::OnPaneClose(wxAuiManagerEvent& evt)
{
	
    if (evt.pane->name == wxT("outputWindow"))
    {
        int res = wxMessageBox(wxT("Are you sure you want to close this pane?"),
                               wxT("Close?"),
                               wxYES_NO,
                               this);
        if (res != wxYES)
            evt.Veto();
		//else
		//{
			//mTextOututWindow123->Hide();
			//mTextOututWindow123->Show(false);
			//evt.Veto();
		//}
    }
}




void MyFrame::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{
   /* wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(wxHtmlWindow)))
    {
        int res = wxMessageBox(wxT("Are you sure you want to close/hide this notebook page?"),
                       wxT("wxAUI"),
                       wxYES_NO,
                       this);
        if (res != wxYES)
            evt.Veto();
    }
	*/
	if (m_mgr.GetPane(wxT("outputWindow")).IsShown() == false)
	{
		m_mgr.GetPane(wxT("outputWindow")).Show();
	}
	else
	{
		m_mgr.GetPane(wxT("outputWindow")).Hide();
	}

	m_mgr.Update();
	Refresh();
	evt.Veto();
}

void MyFrame::OnNotebookPageClosed(wxAuiNotebookEvent& evt)
{
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();

    // selection should always be a valid index
    wxASSERT_MSG( ctrl->GetSelection() < (int)ctrl->GetPageCount(),
                  wxString::Format("Invalid selection %d, only %d pages left",
                                   ctrl->GetSelection(),
                                   (int)ctrl->GetPageCount()) );

    evt.Skip();
}

void MyFrame::OnAllowNotebookDnD(wxAuiNotebookEvent& WXUNUSED(evt))
{
    // for the purpose of this test application, explicitly
    // allow all noteboko drag and drop events
  
	//evt.Allow(); //Denied - NOW

}

wxPoint MyFrame::GetStartPosition()
{
    static int x = 0;
    x += 20;
    wxPoint pt = ClientToScreen(wxPoint(0,0));
    return wxPoint(pt.x + x, pt.y + x);
}





void MyFrame::OnCreateRightWindow(wxCommandEvent& WXUNUSED(evt))
{
	m_mgr.AddPane(CreateRightWindow(), wxAuiPaneInfo().
		Caption(wxT("Notebook123")).
		Float().FloatingPosition(GetStartPosition()).
		//FloatingSize(300,200).
		CloseButton(true).MaximizeButton(true));


	

	
	m_mgr.Update();
}

//*****************************************************************************************************************************


void MyFrame::OnExit(wxCommandEvent& WXUNUSED(evt))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(evt))
{
	wxMessageBox(wxString::Format
		(
			"KalenEngine DirectX 11 - Build 1.0.1 (June 2017), 32-bit edition.\n"
			"\n"
			"API - C++11 & DirectX 11.\nGUI - %s\n"
			"SYSTEM - %s.\n"
			"Copyright © 2017 KAMIL LENIART.\nAll rights reserved.",
			wxVERSION_STRING,
			wxGetOsDescription()
			),
		"KalenEngine - About.",
		wxOK | wxICON_INFORMATION,
		this);
}


wxTextCtrl* MyFrame::CreateTextCtrl(const wxString& ctrl_text)
{
    static int n = 0;

    wxString text;
    if ( !ctrl_text.empty() )
        text = ctrl_text;
    else
        text.Printf(wxT("This is text box %d"), ++n);

    return new wxTextCtrl(this,wxID_ANY, text,  
                          wxPoint(0,0), wxSize(150,90),
                          wxNO_BORDER | wxTE_MULTILINE);
}


wxHtmlWindow* MyFrame::CreateHTMLCtrl(wxWindow* parent) //To create text - KalenEngine - Debug
{
    if (!parent)
        parent = this;

    wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(400,300));
    ctrl->SetPage(GetIntroText(""));
    return ctrl;
}



wxString MyFrame::GetIntroText(wxString text123)
{
	wxString textMain =  "<html><body>"
        "<h3>Welcome to KalenEngine</h3>"

		"<br/><br/>"
		"<h3><br/><b>     KalenEngine - Build 1.0.1 (January 2017)</b><br/></h3>"
		"<h3><br/><b>          DirectX 11</b><br/></h3>"
		"<h3><br/><b>          wxWidgets v3.0</b><br/></h3>"
		"<br/><br/>"
		"<h3><br/><b>Created by: Kamil Leniart</b><br/></h3>"
        "</body></html>";


	textMain << "\n" << text123;

	const char* text = textMain;
      

    return wxString::FromAscii(text);
}

void MyFrame::AddTextToOutputWindow(wxString text)
{
	wxString time = wxNow();
	wxString buffer = "";
	for (int i = 11; i <= 19; ++i)
	{
		buffer.append(time.GetChar(i));
	}
	
	(*mTextOututWindow) << "\n<<" << buffer << ">> " << text;
}



//*******************************************************************************************************************************************
void MyFrame::connectKey(wxWindow* a)
{
	if (a)
	{
		a->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(MyFrame::OnKeyDown), (wxObject*)NULL, this);
		wxWindowListNode* nodeB = a->GetChildren().GetFirst();
		while (nodeB)
		{
			wxWindow* childNode = nodeB->GetData();
			this->connectKey(childNode);
			nodeB = nodeB->GetNext();
		}
	}
}
//*******************************************************************************************************************************************
void MyFrame::OnKeyDown(wxKeyEvent& evt)
{
	if ((evt.GetEventType() == wxEVT_KEY_DOWN) && ((wxKeyEvent&)evt).GetKeyCode() == WXK_CONTROL_C)
	{
		AddTextToOutputWindow("Ctrl + C");
	}
	//AddTextToOutputWindow("BBBBBBBBBBBBBBbb");
	evt.Skip();


}

//*******************************************************************************************************************************************
wxListCtrl* MyFrame::CreateListCtrlRightPanel()
{
	mListCtrlRightPanel = new wxListCtrl(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 100), wxLC_REPORT | wxLC_HRULES | wxLC_VRULES);



	// Add first column       
	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Foo"));
	col0.SetWidth(450);
	mListCtrlRightPanel->InsertColumn(0, col0);

	// Add second column
	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("Name"));
	mListCtrlRightPanel->InsertColumn(1, col1);


	wxListItem item;
	item.SetId(0);
	item.SetText(wxT("123"));
	mListCtrlRightPanel->InsertItem(item);
	mListCtrlRightPanel->SetItem(0, 0, wxT("Foo213"));

	return mListCtrlRightPanel;

	

}

//*******************************************************************************************************************************************
void MyFrame::OnNotebookChanging(wxAuiNotebookEvent& evt)
{
	mKalenEngine->AllowRender(true); //start (one frame) rendering
	//AddTextToOutputWindow("BBBBB -- SHOW");
	if (evt.GetSelection() == 2)
	{
		if (mSelectedObjectItem != NULL)
		{
			//Hide all - first step
			mPropertyGridCase_Node->Hide(true);
			mPropertyGridCase_Position->Hide(true);
			mPropertyGridCase_Rotation->Hide(true);
			mPropertyGridCase_Scale->Hide(true);
			mPropertyGridCase_DrawableFlags->Hide(true);
			mPropertyGridCase_MeshComponent->Hide(true);
			mPropertyGridCase_LightAmbient->Hide(true);
			mPropertyGridCase_LightDiffuse->Hide(true);
			mPropertyGridCase_LightSpecular->Hide(true);
			mPropertyGridCase_LightAtt->Hide(true);
			mPropertyGridCase_LightOther->Hide(true);

			//Show object properties
			if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)//Object3D
			{
				mPropertyGridCase_Node->Hide(false);
				mPropertyGridCase_Position->Hide(false);
				mPropertyGridCase_Rotation->Hide(false);
				mPropertyGridCase_Scale->Hide(false);
				mPropertyGridCase_DrawableFlags->Hide(false);
				mPropertyGridCase_MeshComponent->Hide(false);
			}
			//Show light properties
			if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)//Light
			{
				mPropertyGridCase_Node->Hide(false);
				mPropertyGridCase_Position->Hide(false);
				mPropertyGridCase_LightAmbient->Hide(false);
				mPropertyGridCase_LightDiffuse->Hide(false);
				mPropertyGridCase_LightSpecular->Hide(false);
				mPropertyGridCase_LightAtt->Hide(false);
				mPropertyGridCase_LightOther->Hide(false);
			}

		}	
		else
		{
			mPropertyGridCase_Node->Hide(true);
			mPropertyGridCase_Position->Hide(true);
			mPropertyGridCase_Rotation->Hide(true);
			mPropertyGridCase_Scale->Hide(true);
			mPropertyGridCase_DrawableFlags->Hide(true);
			mPropertyGridCase_MeshComponent->Hide(true);

			//Light case
			mPropertyGridCase_LightAmbient->Hide(true);
			mPropertyGridCase_LightDiffuse->Hide(true);
			mPropertyGridCase_LightSpecular->Hide(true);
			mPropertyGridCase_LightAtt->Hide(true);
			mPropertyGridCase_LightOther->Hide(true);
		}
		
		Refresh();
	}
}
//*******************************************************************************************************************************************

wxPropertyGrid* MyFrame::CreateGrid()
{
	mPropertyDirectionalLight = new wxPropertyGrid(this, ID_PG_PropertyDirectionalLight, wxDefaultPosition, wxSize(400, 100), wxPG_DEFAULT_STYLE, wxT("Directional Light"));
	mPropertyDirectionalLightAmbient = new wxPropertyCategory("Sun Properties - Ambient:");
	mPropertyDirectionalLight->Append(mPropertyDirectionalLightAmbient);
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Ambient R"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Ambient.x * 255.0f)));
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Ambient G"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Ambient.y * 255.0f)));
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Ambient B"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Ambient.z * 255.0f)));

	mPropertyDirectionalLightDiffuse = new wxPropertyCategory("Sun Properties - Diffuse:");
	mPropertyDirectionalLight->Append(mPropertyDirectionalLightDiffuse);
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Diffuse R"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Diffuse.x * 255.0f)));
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Diffuse G"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Diffuse.y * 255.0f)));
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Diffuse B"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Diffuse.z * 255.0f)));

	mPropertyDirectionalLightSpecular = new wxPropertyCategory("Sun Properties - Specular:");
	mPropertyDirectionalLight->Append(mPropertyDirectionalLightSpecular);
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Specular R"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Specular.x * 255.0f)));
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Specular G"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Specular.y * 255.0f)));
	mPropertyDirectionalLight->Append(new wxIntProperty(wxT("Specular B"), wxPG_LABEL, (int)(mKalenEngine->GetSun()->Specular.z * 255.0f)));

	mPropertyDirectionalLightDirection = new wxPropertyCategory("Sun Properties - Direction:");
	mPropertyDirectionalLight->Append(mPropertyDirectionalLightDirection);

	mPropertyDirectionalLight->Append(new wxFloatProperty(wxT("Position X"), wxPG_LABEL, mKalenEngine->GetSun()->Direction.x * -100.0f));
	mPropertyDirectionalLight->Append(new wxFloatProperty(wxT("Position Y"), wxPG_LABEL, mKalenEngine->GetSun()->Direction.z * -100.0f));
	mPropertyDirectionalLight->Append(new wxFloatProperty(wxT("Position Z"), wxPG_LABEL, mKalenEngine->GetSun()->Direction.y * -100.0f));


	mPropertyCubeMapPosition = new wxPropertyCategory("CubeMap Position:");
	mPropertyDirectionalLight->Append(mPropertyCubeMapPosition);

	mPropertyDirectionalLight->Append(new wxFloatProperty(wxT("CubeMap Position X"), wxPG_LABEL, mKalenEngine->GetCubeMapPosition().x));
	mPropertyDirectionalLight->Append(new wxFloatProperty(wxT("CubeMap Position Y"), wxPG_LABEL, mKalenEngine->GetCubeMapPosition().z));
	mPropertyDirectionalLight->Append(new wxFloatProperty(wxT("CubeMap Position Z"), wxPG_LABEL, mKalenEngine->GetCubeMapPosition().y));

	mPropertyDirectionalLight->FitColumns();
	
	return mPropertyDirectionalLight;
}
//*******************************************************************************************************************************************
wxPropertyGrid* MyFrame::CreateTestPropertyGrid()
{
	mPropertyGridNode = new wxPropertyGrid(this, ID_PG_PropertyObject, wxDefaultPosition, wxSize(400, 100), wxPG_DEFAULT_STYLE, wxT("Node"));

	mPropertyGridCase_Node = new wxPropertyCategory("Node");
	mPropertyGridNode->Append(mPropertyGridCase_Node);
	mPropertyGridNode->Append(new wxStringProperty(wxT("Name"), wxT("Name"), wxT("...")));
	mPropertyGridNode->Append(new wxStringProperty(wxT("TAGs"), wxT("Tag"), wxT("[NoTags]")));

	mPropertyGridCase_Position = new wxPropertyCategory("Position");
	mPropertyGridNode->Append(mPropertyGridCase_Position);
	//mPropertyGridNode->Append(new wxPropertyCategory("Position"));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Position X"), wxPG_LABEL, 0.0));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Position Y"), wxPG_LABEL, 0.0));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Position Z"), wxPG_LABEL, 0.0));


	mPropertyGridCase_Rotation = new wxPropertyCategory("Rotation");
	mPropertyGridNode->Append(mPropertyGridCase_Rotation);
	//mPropertyGridNode->Append(new wxPropertyCategory("Rotation"));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Pitch"), wxPG_LABEL, 0.0));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Roll"), wxPG_LABEL, 0.0));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Yaw"), wxPG_LABEL, 0.0));



	mPropertyGridCase_Scale = new wxPropertyCategory("Scale");
	mPropertyGridNode->Append(mPropertyGridCase_Scale);
	//mPropertyGridNode->Append(new wxPropertyCategory("Scale"));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Scale X"), wxPG_LABEL, 1.0));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Scale Y"), wxPG_LABEL, 1.0));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Scale Z"), wxPG_LABEL, 1.0));

	mPropertyGridCase_DrawableFlags = new wxPropertyCategory("DrawableFlags");
	mPropertyGridNode->Append(mPropertyGridCase_DrawableFlags);
	//mPropertyGridNode->Append(new wxPropertyCategory("DrawableFlags"));
	mPropertyGridNode->Append(new wxBoolProperty(wxT("IsVisible?"), wxPG_LABEL, true));
	mPropertyGridNode->Append(new wxBoolProperty(wxT("IsCastShadow?"), wxPG_LABEL, false));

	mPropertyGridCase_MeshComponent = new wxPropertyCategory("MeshComponent");
	mPropertyGridNode->Append(mPropertyGridCase_MeshComponent);
	//mPropertyGridNode->Append(new wxPropertyCategory("MeshComponent"));
	mPropertyGridNode->Append(new wxIntProperty(wxT("ForceLODLevel"), wxPG_LABEL, -1));
	mPropertyGridNode->Append(new wxIntProperty(wxT("ShadowRenderingDistance"), wxPG_LABEL, 600));
	mPropertyGridNode->Append(new wxFileProperty("Mesh", wxPG_LABEL, wxT("kamil/mesh.klmesh")));

	//Light:
	mPropertyGridCase_LightAmbient = new wxPropertyCategory("Ambient");
	mPropertyGridNode->Append(mPropertyGridCase_LightAmbient);
	mPropertyGridNode->Append(new wxIntProperty(wxT("Red Ambient:"), wxPG_LABEL, 255));
	mPropertyGridNode->Append(new wxIntProperty(wxT("Green Ambient:"), wxPG_LABEL, 255));
	mPropertyGridNode->Append(new wxIntProperty(wxT("Blue Ambient:"), wxPG_LABEL, 255));

	mPropertyGridCase_LightDiffuse = new wxPropertyCategory("Diffuse");
	mPropertyGridNode->Append(mPropertyGridCase_LightDiffuse);
	mPropertyGridNode->Append(new wxIntProperty(wxT("Red Diffuse:"), wxPG_LABEL, 255));
	mPropertyGridNode->Append(new wxIntProperty(wxT("Green Diffuse:"), wxPG_LABEL, 255));
	mPropertyGridNode->Append(new wxIntProperty(wxT("Blue Diffuse:"), wxPG_LABEL, 255));

	mPropertyGridCase_LightSpecular = new wxPropertyCategory("Specular");
	mPropertyGridNode->Append(mPropertyGridCase_LightSpecular);
	mPropertyGridNode->Append(new wxIntProperty(wxT("Red Specular:"), wxPG_LABEL, 255));
	mPropertyGridNode->Append(new wxIntProperty(wxT("Green Specular:"), wxPG_LABEL, 255));
	mPropertyGridNode->Append(new wxIntProperty(wxT("Blue Specular:"), wxPG_LABEL, 255));

	mPropertyGridCase_LightAtt = new wxPropertyCategory("Attenuation");
	mPropertyGridNode->Append(mPropertyGridCase_LightAtt);
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Distance 1:"), wxPG_LABEL, 0.1));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Distance 2:"), wxPG_LABEL, 0.1));
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Distance 3:"), wxPG_LABEL, 0.1));


	mPropertyGridCase_LightOther = new wxPropertyCategory("Other");
	mPropertyGridNode->Append(mPropertyGridCase_LightOther);
	mPropertyGridNode->Append(new wxFloatProperty(wxT("Range:"), wxPG_LABEL, 5000.0));


	mPropertyGridNode->FitColumns();

//	mPropertyGridNode->Freeze();

	
	    
	return mPropertyGridNode;
}
//*******************************************************************************************************************************************
void MyFrame::OnIdle(wxIdleEvent& evt)
{
	wxString textOutput = "KalenEngine DirectX 11 - Build 1.0.1 (June 2017)   FPS: ";


	//Deselected all items
	if (mKalenEngine->GetObject() == nullptr)
	{
		mSelectedObjectItem = NULL;
		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)//layers
			for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)//objects
			{
				if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetType() == 0)
				{
					if(mTreeScene->GetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId()) == 8)
						mTreeScene->SetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId(), 3);
				}
				else
					if (mTreeScene->GetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId()) == 10)
						mTreeScene->SetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId(), 9);
			}		
	}
	//Selected
	else if (mKalenEngine->GetObject() != nullptr)
	{
		mSelectedObjectItem = mKalenEngine->GetObject()->GetTreeItemId();

		//Deselected items
		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)//layers
			for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)//objects
			{
				if(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId() != mSelectedObjectItem)
					if (mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetType() == 0)
					{
						if (mTreeScene->GetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId()) == 8)
							mTreeScene->SetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId(), 3);
					}
					else
						if (mTreeScene->GetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId()) == 10)
							mTreeScene->SetItemImage(mLayerBuffer->at(i)->GetAllObjects()->at(j)->GetTreeItemId(), 9);
			}

		//Select item
		if (mKalenEngine->GetObject()->GetType() == 0)
		{
			if (mTreeScene->GetItemImage(mSelectedObjectItem) == 3)
				mTreeScene->SetItemImage(mSelectedObjectItem, 8);
		}
		else
			if (mTreeScene->GetItemImage(mSelectedObjectItem) == 9)
				mTreeScene->SetItemImage(mSelectedObjectItem, 10);
	}
		


	//Show object properties
	if(mSelectedObjectItem != NULL)
		if(mShowProperty == true)
			if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)//Object3D
			{
				mShowProperty = false;
				mPropertyGridCase_Node->Hide(false);
				mPropertyGridCase_Position->Hide(false);
				mPropertyGridCase_Rotation->Hide(false);
				mPropertyGridCase_Scale->Hide(false);
				mPropertyGridCase_DrawableFlags->Hide(false);
				mPropertyGridCase_MeshComponent->Hide(false);
			}

	if (mSelectedObjectItem == NULL)
	{
		if (mShowProperty == false)
		{
			mShowProperty = true;
			mPropertyGridCase_Node->Hide(true);
			mPropertyGridCase_Position->Hide(true);
			mPropertyGridCase_Rotation->Hide(true);
			mPropertyGridCase_Scale->Hide(true);
			mPropertyGridCase_DrawableFlags->Hide(true);
			mPropertyGridCase_MeshComponent->Hide(true);
			mPropertyGridCase_LightAmbient->Hide(true);
			mPropertyGridCase_LightDiffuse->Hide(true);
			mPropertyGridCase_LightSpecular->Hide(true);
			mPropertyGridCase_LightAtt->Hide(true);
			mPropertyGridCase_LightOther->Hide(true);
		}

	}

	//------------------------------------------------------------------------------------------------------------------------------
	if (mKalenEngine->GetIsAddedLight() == true)
	{
		bool isOk = false;
		for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)
		{
			if (GetLayerBuffer()->at(i)->GetIsActive())
			{
				//wxMessageBox(text, "Dropped File - INFO", wxICON_QUESTION | wxOK);
				isOk = true;
				wxTreeItemId treeID = AddObjectToLayer(nullptr, wxT("Point Light"), 1);

				LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
				XMFLOAT3 pos = XMFLOAT3(mKalenEngine->GetCamera()->GetLook().x * 100, mKalenEngine->GetCamera()->GetLook().y * 100, mKalenEngine->GetCamera()->GetLook().z * 100);
				pos.x += mKalenEngine->GetCamera()->GetPosition().x;
				pos.y += mKalenEngine->GetCamera()->GetPosition().y;
				pos.z += mKalenEngine->GetCamera()->GetPosition().z;
				localCoordinate->SetLocalCoordinatePosition(pos);
				AxisAlignedBox aabb;
				aabb.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
				aabb.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

				PointLight* pointLight = new PointLight();

				pointLight->Range = 100.0f;
				GetLayerBuffer()->at(i)->AddObject(new Object("Point Light", "", nullptr, aabb, *localCoordinate, false, 0.0f, true, 1, pointLight, -1), treeID); //add light

				//---------------------------
				//Add this object to the RenderObjectBuffer
				mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));


			}
		}
		if (isOk == false)
			wxMessageBox("Can not add point light - no layer is active", "Error", wxICON_QUESTION | wxOK);

		mKalenEngine->SetIsAddedLight(false);
	}

	//------------------------------------------------------------------------------------------------------------------------------
	if (mKalenEngine->GetCopyPasteCutDelete() == 1)//copy
	{
		mKalenEngine->AllowRender(true); //start (one frame) rendering
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		mCopiedObject = mKalenEngine->GetObject();
		AddTextToOutputWindow("Copied object");
		Refresh();

		mKalenEngine->SetCopyPasteCutDelete(-1);
	}
	if (mKalenEngine->GetCopyPasteCutDelete() == 2)//cut
	{
		mKalenEngine->AllowRender(true); //start (one frame) rendering
		mTreeBuffer = mSelectedObjectItem;
		mTreeBufferNameObject = mTreeScene->GetItemText(mSelectedObjectItem);
		AddTextToOutputWindow("Cut object");

		//Delete
		wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

		for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
		{
			if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
			{
				mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
				mKalenEngine->SetSelectedObject(nullptr);
				mDeletedObject = true;
				mTreeScene->Delete(mSelectedObjectItem);
				mSelectedObjectItem = NULL;
			}
		}
		Refresh();

		mKalenEngine->SetCopyPasteCutDelete(-1);
	}
	//------------------------------------------------------------------------------------------------------------------------------
	if (mKalenEngine->GetCopyPasteCutDelete() == 3)//paste
	{
		wxMessageDialog *errorMsg = NULL;
		if (mTreeBuffer != NULL)
		{
			if (mActiveLayerItem != NULL)
			{
				if (mCopiedObject->GetType() == 0)//KLMESH
				{
					for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
					{
						if (GetLayerBuffer()->at(i)->GetIsActive())
						{
							LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
							localCoordinate = mCopiedObject->GetCoordinate();

							//localCoordinate->SetScale(XMFLOAT3(1.0f / localCoordinate->GetScale().x, 1.0f / localCoordinate->GetScale().y, 1.0f / localCoordinate->GetScale().z));
							wxTreeItemId treeID = AddObjectToLayer(&GetLayerBuffer()->at(i)->GetRootTreeId(), mTreeBufferNameObject, 0);
							GetLayerBuffer()->at(i)->AddObject(
								new Object
								(
									(string)mTreeBufferNameObject, mCopiedObject->GetTAG(), mCopiedObject->GetKLMESH(), *mCopiedObject->GetKLMESH()->GetAxisAlignedBoxOrigin(),
									*localCoordinate, mCopiedObject->GetIsCastShadow(), mCopiedObject->GetShadowDistance(), mCopiedObject->GetIsVisible(),
									0, mCopiedObject->GetPointLight(), -1
									),
								treeID);
							mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));

							//Select copied object
							if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
								mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

							if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
								mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

							mSelectedObjectItem = treeID;
							mTreeScene->SetItemImage(treeID, 8);
							mTreeScene->SetFocusedItem(treeID);//Focused item 

							wxTreeItemId idParent = mTreeScene->GetItemParent(mSelectedObjectItem);

							//Select copied object
							for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
							{
								if (mLayerBuffer->at(j)->GetRootTreeId() == idParent)
								{
									if (mLayerBuffer->at(j)->GetObject(mSelectedObjectItem) != nullptr)
										mKalenEngine->SetSelectedObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));
								}
							}
						}
					}
				}
				else
					if (mCopiedObject->GetType() == 1)//Light
					{
						for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
						{
							if (GetLayerBuffer()->at(i)->GetIsActive())
							{
								LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();
								localCoordinate = mCopiedObject->GetCoordinate();
								wxTreeItemId treeID = AddObjectToLayer(&GetLayerBuffer()->at(i)->GetRootTreeId(), mTreeBufferNameObject, 1);

								AxisAlignedBox aabb;
								aabb.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
								aabb.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);

								GetLayerBuffer()->at(i)->AddObject(
									new Object
									(
										(string)mTreeBufferNameObject, mCopiedObject->GetTAG(), nullptr, aabb,
										*localCoordinate, mCopiedObject->GetIsCastShadow(), mCopiedObject->GetShadowDistance(), mCopiedObject->GetIsVisible(),
										1, mCopiedObject->GetPointLight(), -1
										),
									treeID);
								mKalenEngine->GetRenderObjectsBuffer()->push_back(GetLayerBuffer()->at(i)->GetObject(treeID));

								//Select copied object
								if (mTreeScene->GetItemImage(mSelectedObjectItem) == 8)
									mTreeScene->SetItemImage(mSelectedObjectItem, 3);//restore object3D label

								if (mTreeScene->GetItemImage(mSelectedObjectItem) == 10)
									mTreeScene->SetItemImage(mSelectedObjectItem, 9);//restore light label

								mSelectedObjectItem = treeID;
								mTreeScene->SetItemImage(treeID, 10);
								mTreeScene->SetFocusedItem(treeID);//Focused item 

								wxTreeItemId idParent = mTreeScene->GetItemParent(mSelectedObjectItem);

								//Select copied object
								for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
								{
									if (mLayerBuffer->at(j)->GetRootTreeId() == idParent)
									{
										if (mLayerBuffer->at(j)->GetObject(mSelectedObjectItem) != nullptr)
											mKalenEngine->SetSelectedObject(mLayerBuffer->at(j)->GetObject(mSelectedObjectItem));
									}
								}
							}
						}
					}
				mKalenEngine->AllowRender(true); //start (one frame) rendering
			}
			else
			{
				errorMsg = new wxMessageDialog(NULL, wxT("There is no active layer!"), wxT("INFO"), wxOK | wxICON_INFORMATION);
				errorMsg->ShowModal();
			}
		}
		else
		{
			errorMsg = new wxMessageDialog(NULL, wxT("There is no object to paste!\nBuffer is empty."), wxT("INFO"), wxOK | wxICON_INFORMATION);
			errorMsg->ShowModal();
		}

		mKalenEngine->SetCopyPasteCutDelete(-1);
	}
	//------------------------------------------------------------------------------------------------------------------------------
	if (mKalenEngine->GetCopyPasteCutDelete() == 4)//delete
	{
		int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
		int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
		wxMessageDialog dialog(this, wxT("Delete this object?"), wxT("Delete"), wxOK | wxCANCEL, wxPoint(width, height));

		if (dialog.ShowModal() == wxID_OK)
		{
			wxTreeItemId parentLayer = mTreeScene->GetItemParent(mSelectedObjectItem);

			for (int i = 0; i < (int)GetLayerBuffer()->size(); ++i)//size of layers
			{
				if (mLayerBuffer->at(i)->GetRootTreeId() == parentLayer)
				{
					mLayerBuffer->at(i)->DeleteObject(mSelectedObjectItem);
					mKalenEngine->SetSelectedObject(nullptr);
					mDeletedObject = true;
					mTreeScene->Delete(mSelectedObjectItem);
					mSelectedObjectItem = NULL;
					AddTextToOutputWindow("Delete object");
				}
			}
		}
		else
		{
			AddTextToOutputWindow("Delete object -- cancel");
		}
		dialog.Destroy();
		Refresh();

		mKalenEngine->SetCopyPasteCutDelete(-1);
	}
	//------------------------------------------------------------------------------------------------------------------------------


	if (mKalenEngine != NULL)
	{
		textOutput << (int)mKalenEngine->GetFPS();
		mKalenEngine->SetLayerBuffer(mLayerBuffer);
	}

	SetTitle(textOutput);

	wxString text = "X: "; 
	text << (int)mKalenEngine->GetMousePosition().x;
	GetStatusBar()->SetStatusText(text, 1);
	text = "Y: ";
	text << (int)mKalenEngine->GetMousePosition().y;
	GetStatusBar()->SetStatusText(text, 2);
	GetStatusBar()->SetStatusText(_("Z: 0"), 3);

	if (mKalenEngine->GetObject() != nullptr)
	{
		text = "";
		text = mKalenEngine->GetObject()->GetFileName(); 
		mPropertyGridNode->GetProperty("Name")->SetValueFromString(text);

		text = "";
		text << mKalenEngine->GetObject()->GetCoordinate()->GetLocalCoordinatePosition().x;
		mPropertyGridNode->GetProperty("Position X")->SetValueFromString(text);
		text = "";
		text << mKalenEngine->GetObject()->GetCoordinate()->GetLocalCoordinatePosition().z;
		mPropertyGridNode->GetProperty("Position Y")->SetValueFromString(text);
		text = "";
		text << mKalenEngine->GetObject()->GetCoordinate()->GetLocalCoordinatePosition().y;
		mPropertyGridNode->GetProperty("Position Z")->SetValueFromString(text);

		text = "";
		text << 1.0f / mKalenEngine->GetObject()->GetCoordinate()->GetScale().x;
		mPropertyGridNode->GetProperty("Scale X")->SetValueFromString(text);

		text = "";
		text << 1.0f / mKalenEngine->GetObject()->GetCoordinate()->GetScale().z;
		mPropertyGridNode->GetProperty("Scale Y")->SetValueFromString(text);

		text = "";
		text << 1.0f / mKalenEngine->GetObject()->GetCoordinate()->GetScale().y;
		mPropertyGridNode->GetProperty("Scale Z")->SetValueFromString(text);

		text = "";
		text << mKalenEngine->GetObject()->GetCoordinate()->GetPitch();
		mPropertyGridNode->GetProperty("Pitch")->SetValueFromString(text);

		text = "";
		text << mKalenEngine->GetObject()->GetCoordinate()->GetYaw();
		mPropertyGridNode->GetProperty("Yaw")->SetValueFromString(text);

		text = "";
		text << mKalenEngine->GetObject()->GetCoordinate()->GetRoll();
		mPropertyGridNode->GetProperty("Roll")->SetValueFromString(text);

		if(mKalenEngine->GetObject()->GetIsVisible() == true)
			mPropertyGridNode->GetProperty("IsVisible?")->SetValueFromString("True");
		else
			mPropertyGridNode->GetProperty("IsVisible?")->SetValueFromString("False");

		if(mKalenEngine->GetObject()->GetIsCastShadow() == true)
			mPropertyGridNode->GetProperty("IsCastShadow?")->SetValueFromString("True");
		else
			mPropertyGridNode->GetProperty("IsCastShadow?")->SetValueFromString("False");

		text = "";
		text << mKalenEngine->GetObject()->GetForceLODLevel();
		mPropertyGridNode->GetProperty("ForceLODLevel")->SetValueFromString(text);

		text = "";
		text << (int)mKalenEngine->GetObject()->GetShadowDistance();
		mPropertyGridNode->GetProperty("ShadowRenderingDistance")->SetValueFromString(text);

		if (mKalenEngine->GetObject()->GetKLMESH() != nullptr)
		{
			text = "";
			text << mKalenEngine->GetObject()->GetKLMESH()->GetLinkToFile();
			mPropertyGridNode->GetProperty("Mesh")->SetValueFromString(text);
		}

		if (mKalenEngine->GetObject()->GetPointLight() != nullptr)//If selected object is a light
		{
			//Ambient light
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Ambient.x * 255.0f);
			mPropertyGridNode->GetProperty("Red Ambient:")->SetValueFromString(text);

			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Ambient.y * 255.0f);
			mPropertyGridNode->GetProperty("Green Ambient:")->SetValueFromString(text);
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Ambient.z * 255.0f);
			mPropertyGridNode->GetProperty("Blue Ambient:")->SetValueFromString(text);
			//Diffuse light
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Diffuse.x * 255.0f);
			mPropertyGridNode->GetProperty("Red Diffuse:")->SetValueFromString(text);
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Diffuse.y * 255.0f);
			mPropertyGridNode->GetProperty("Green Diffuse:")->SetValueFromString(text);
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Diffuse.z * 255.0f);
			mPropertyGridNode->GetProperty("Blue Diffuse:")->SetValueFromString(text);
			//Specular light
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Specular.x * 255.0f);
			mPropertyGridNode->GetProperty("Red Specular:")->SetValueFromString(text);
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Specular.y * 255.0f);
			mPropertyGridNode->GetProperty("Green Specular:")->SetValueFromString(text);
			text = "";
			text << (int)(mKalenEngine->GetObject()->GetPointLight()->Specular.z * 255.0f);
			mPropertyGridNode->GetProperty("Blue Specular:")->SetValueFromString(text);

			//Attenuation light
			text = "";
			text << mKalenEngine->GetObject()->GetPointLight()->Att.x;
			mPropertyGridNode->GetProperty("Distance 1:")->SetValueFromString(text);
			text = "";
			text << mKalenEngine->GetObject()->GetPointLight()->Att.y;
			mPropertyGridNode->GetProperty("Distance 2:")->SetValueFromString(text);
			text = "";
			text << mKalenEngine->GetObject()->GetPointLight()->Att.z;
			mPropertyGridNode->GetProperty("Distance 3:")->SetValueFromString(text);
			//Range light
			text = "";
			text << mKalenEngine->GetObject()->GetPointLight()->Range;
			mPropertyGridNode->GetProperty("Range:")->SetValueFromString(text);

			//Set light position
			mKalenEngine->GetObject()->GetPointLight()->Position = mKalenEngine->GetObject()->GetCoordinate()->GetLocalCoordinatePosition();




		}
	}
}
void MyFrame::OnUpdateValues(wxMouseEvent& evt)
{
	evt.Skip();
}
//*****************************************************************************************************************************************
void MyFrame::OnPGSunPropertyChanged(wxPropertyGridEvent& evt)
{
	mKalenEngine->AllowRender(true); //start (one frame) rendering
	wxString textValue = "";
	double valueX = -1.0;
	double valueY = -1.0;
	double valueZ = -1.0;
	//------------------
	//Light - Ambient
	//------------------
	textValue = mPropertyDirectionalLight->GetProperty("Ambient R")->GetValueAsString();
	textValue.ToDouble(&valueX);
	if (valueX < 0)
		valueX = 0;
	if (valueX > 255)
		valueX = 255;
	//Set new value
	textValue = "";
	textValue << valueX;
	mPropertyDirectionalLight->GetProperty("Ambient R")->SetValueFromString(textValue);

	textValue = mPropertyDirectionalLight->GetProperty("Ambient G")->GetValueAsString();
	textValue.ToDouble(&valueY);
	if (valueY < 0)
		valueY = 0;
	if (valueY > 255)
		valueY = 255;
	//Set new value
	textValue = "";
	textValue << valueY;
	mPropertyDirectionalLight->GetProperty("Ambient G")->SetValueFromString(textValue);

	textValue = mPropertyDirectionalLight->GetProperty("Ambient B")->GetValueAsString();
	textValue.ToDouble(&valueZ);
	if (valueZ < 0)
		valueZ = 0;
	if (valueZ > 255)
		valueZ = 255;
	//Set new value
	textValue = "";
	textValue << valueZ;
	mPropertyDirectionalLight->GetProperty("Ambient B")->SetValueFromString(textValue);

	mKalenEngine->GetSun()->Ambient = XMFLOAT4(valueX / 255.0f, valueY / 255.0f, valueZ / 255.0f, 1.0f);
	//------------------
	//Light - Diffuse
	//------------------
	textValue = mPropertyDirectionalLight->GetProperty("Diffuse R")->GetValueAsString();
	textValue.ToDouble(&valueX);
	if (valueX < 0)
		valueX = 0;
	if (valueX > 255)
		valueX = 255;
	//Set new value
	textValue = "";
	textValue << valueX;
	mPropertyDirectionalLight->GetProperty("Diffuse R")->SetValueFromString(textValue);

	textValue = mPropertyDirectionalLight->GetProperty("Diffuse G")->GetValueAsString();
	textValue.ToDouble(&valueY);
	if (valueY < 0)
		valueY = 0;
	if (valueY > 255)
		valueY = 255;
	//Set new value
	textValue = "";
	textValue << valueY;
	mPropertyDirectionalLight->GetProperty("Diffuse G")->SetValueFromString(textValue);

	textValue = mPropertyDirectionalLight->GetProperty("Diffuse B")->GetValueAsString();
	textValue.ToDouble(&valueZ);
	if (valueZ < 0)
		valueZ = 0;
	if (valueZ > 255)
		valueZ = 255;
	//Set new value
	textValue = "";
	textValue << valueZ;
	mPropertyDirectionalLight->GetProperty("Diffuse B")->SetValueFromString(textValue);

	mKalenEngine->GetSun()->Diffuse = XMFLOAT4(valueX / 255.0f, valueY / 255.0f, valueZ / 255.0f, 1.0f);
	//------------------
	//Light - Specular
	//------------------
	textValue = mPropertyDirectionalLight->GetProperty("Specular R")->GetValueAsString();
	textValue.ToDouble(&valueX);
	if (valueX < 0)
		valueX = 0;
	if (valueX > 255)
		valueX = 255;
	//Set new value
	textValue = "";
	textValue << valueX;
	mPropertyDirectionalLight->GetProperty("Specular R")->SetValueFromString(textValue);

	textValue = mPropertyDirectionalLight->GetProperty("Specular G")->GetValueAsString();
	textValue.ToDouble(&valueY);
	if (valueY < 0)
		valueY = 0;
	if (valueY > 255)
		valueY = 255;
	//Set new value
	textValue = "";
	textValue << valueY;
	mPropertyDirectionalLight->GetProperty("Specular G")->SetValueFromString(textValue);

	textValue = mPropertyDirectionalLight->GetProperty("Specular B")->GetValueAsString();
	textValue.ToDouble(&valueZ);
	if (valueZ < 0)
		valueZ = 0;
	if (valueZ > 255)
		valueZ = 255;
	//Set new value
	textValue = "";
	textValue << valueZ;
	mPropertyDirectionalLight->GetProperty("Specular B")->SetValueFromString(textValue);

	mKalenEngine->GetSun()->Specular = XMFLOAT4(valueX / 255.0f, valueY / 255.0f, valueZ / 255.0f, 1.0f);

	textValue = mPropertyDirectionalLight->GetProperty("Position X")->GetValueAsString();
	textValue.ToDouble(&valueX);
	textValue = mPropertyDirectionalLight->GetProperty("Position Y")->GetValueAsString();
	textValue.ToDouble(&valueY);
	textValue = mPropertyDirectionalLight->GetProperty("Position Z")->GetValueAsString();
	textValue.ToDouble(&valueZ);

	XMFLOAT3 dir = XMFLOAT3(-valueX, -valueZ, -valueY);

	//Normalize position to direction
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&dir)));
	mKalenEngine->GetSun()->Direction = dir;

	//CubeMap position
	textValue = mPropertyDirectionalLight->GetProperty("CubeMap Position X")->GetValueAsString();
	textValue.ToDouble(&valueX);
	textValue = mPropertyDirectionalLight->GetProperty("CubeMap Position Y")->GetValueAsString();
	textValue.ToDouble(&valueY);
	textValue = mPropertyDirectionalLight->GetProperty("CubeMap Position Z")->GetValueAsString();
	textValue.ToDouble(&valueZ);

	mKalenEngine->SetCubeMapPosition(XMFLOAT3(valueX, valueZ, valueY));
}
//*****************************************************************************************************************************************
void MyFrame::OnPGObjectPropertyChanged(wxPropertyGridEvent& evt)
{
	mKalenEngine->AllowRender(true); //start (one frame) rendering
	wxString textValue = "";
	double valueX = -1.0;
	double valueY = -1.0;
	double valueZ = -1.0;

	//**************
	//Name
	//**************
	textValue = mPropertyGridNode->GetProperty("Name")->GetValueAsString();


	if(mKalenEngine->GetObject() != nullptr)
	{
		mKalenEngine->GetObject()->SetFileName((std::string)textValue);
		if (mSelectedObjectItem != NULL)
			mTreeScene->SetItemText(mSelectedObjectItem, textValue);
		//**************
		//Move
		//**************
		textValue = mPropertyGridNode->GetProperty("Position X")->GetValueAsString();
		textValue.ToDouble(&valueX);

		textValue = mPropertyGridNode->GetProperty("Position Y")->GetValueAsString();
		textValue.ToDouble(&valueY);

		textValue = mPropertyGridNode->GetProperty("Position Z")->GetValueAsString();
		textValue.ToDouble(&valueZ);

		mKalenEngine->GetObject()->GetCoordinate()->SetLocalCoordinatePosition(XMFLOAT3((float)valueX, (float)valueZ, (float)valueY));

		//**************
		//Scale
		//**************
		textValue = mPropertyGridNode->GetProperty("Scale X")->GetValueAsString();
		textValue.ToDouble(&valueX);

		textValue = mPropertyGridNode->GetProperty("Scale Y")->GetValueAsString();
		textValue.ToDouble(&valueY);

		textValue = mPropertyGridNode->GetProperty("Scale Z")->GetValueAsString();
		textValue.ToDouble(&valueZ);

		if (valueX == 0)
			valueX = 0.001f;
		if (valueY == 0)
			valueY = 0.001f;
		if (valueZ == 0)
			valueZ = 0.001f;

		//Display new values
		textValue = "";
		textValue << 1.0f / valueX;
		mPropertyGridNode->GetProperty("Scale X")->SetValueFromString(textValue);

		textValue = "";
		textValue << 1.0f / valueY;
		mPropertyGridNode->GetProperty("Scale Y")->SetValueFromString(textValue);

		textValue = "";
		textValue << 1.0f / valueZ;
		mPropertyGridNode->GetProperty("Scale Z")->SetValueFromString(textValue);

		mKalenEngine->GetObject()->GetCoordinate()->SetScaleInverse(XMFLOAT3((float)valueX, (float)valueZ, (float)valueY));

		//**************
		//Rotation
		//**************
		textValue = mPropertyGridNode->GetProperty("Pitch")->GetValueAsString();
		textValue.ToDouble(&valueX);

		textValue = mPropertyGridNode->GetProperty("Yaw")->GetValueAsString();
		textValue.ToDouble(&valueY);

		textValue = mPropertyGridNode->GetProperty("Roll")->GetValueAsString();
		textValue.ToDouble(&valueZ);

		int scalar = 0;
		if (valueX >= 360 || valueX <= -360)
		{
			scalar = (int)(valueX / 360);
			valueX = valueX - (scalar * 360);
		}
		if (valueY >= 360 || valueY <= -360)
		{
			scalar = (int)(valueY / 360);
			valueY = valueY - (scalar * 360);
		}
		if (valueZ >= 360 || valueZ <= -360)
		{
			scalar = (int)(valueZ / 360);
			valueZ = valueZ - (scalar * 360);
		}

		//Display new values
		textValue = "";
		textValue << valueX;
		mPropertyGridNode->GetProperty("Pitch")->SetValueFromString(textValue);

		textValue = "";
		textValue << valueY;
		mPropertyGridNode->GetProperty("Yaw")->SetValueFromString(textValue);

		textValue = "";
		textValue << valueZ;
		mPropertyGridNode->GetProperty("Roll")->SetValueFromString(textValue);

		//Set values
		mKalenEngine->GetObject()->GetCoordinate()->SetMatrixRotationRollPitchYaw((float)valueX, (float)valueY, (float)valueZ);

		textValue = "";
		textValue = mPropertyGridNode->GetProperty("IsVisible?")->GetValueAsString();
		if (textValue == "True")
			mKalenEngine->GetObject()->SetIsVisible(true);
		else
			mKalenEngine->GetObject()->SetIsVisible(false);


		textValue = "";
		textValue = mPropertyGridNode->GetProperty("IsCastShadow?")->GetValueAsString();
		if (textValue == "True")
			mKalenEngine->GetObject()->SetIsCastShadow(true);
		else
			mKalenEngine->GetObject()->SetIsCastShadow(false);


		textValue = "";
		textValue = mPropertyGridNode->GetProperty("ForceLODLevel")->GetValueAsString();
		textValue.ToDouble(&valueX);
		mKalenEngine->GetObject()->SetForceLODLevel((int)valueX);

		textValue = "";
		textValue = mPropertyGridNode->GetProperty("ShadowRenderingDistance")->GetValueAsString();
		//valueX *= 10;
		textValue.ToDouble(&valueX);
		mKalenEngine->GetObject()->SetShadowDistance((float)valueX);

		if (mKalenEngine->GetObject()->GetPointLight() != nullptr)
		{
			//------------------
			//Light - Ambient
			//------------------
			textValue = mPropertyGridNode->GetProperty("Red Ambient:")->GetValueAsString();
			textValue.ToDouble(&valueX);
			if (valueX < 0)
				valueX = 0;
			if (valueX > 255)
				valueX = 255;
			//Set new value
			textValue = "";
			textValue << valueX;
			mPropertyGridNode->GetProperty("Red Ambient:")->SetValueFromString(textValue);

			textValue = mPropertyGridNode->GetProperty("Green Ambient:")->GetValueAsString();
			textValue.ToDouble(&valueY);
			if (valueY < 0)
				valueY = 0;
			if (valueY > 255)
				valueY = 255;
			//Set new value
			textValue = "";
			textValue << valueY;
			mPropertyGridNode->GetProperty("Green Ambient:")->SetValueFromString(textValue);

			textValue = mPropertyGridNode->GetProperty("Blue Ambient:")->GetValueAsString();
			textValue.ToDouble(&valueZ);
			if (valueZ < 0)
				valueZ = 0;
			if (valueZ > 255)
				valueZ = 255;
			//Set new value
			textValue = "";
			textValue << valueZ;
			mPropertyGridNode->GetProperty("Blue Ambient:")->SetValueFromString(textValue);

			mKalenEngine->GetObject()->GetPointLight()->Ambient = XMFLOAT4(valueX / 255.0f, valueY / 255.0f, valueZ / 255.0f, 1.0f);


			//------------------
			//Light - Diffuse
			//------------------
			textValue = mPropertyGridNode->GetProperty("Red Diffuse:")->GetValueAsString();
			textValue.ToDouble(&valueX);
			if (valueX < 0)
				valueX = 0;
			if (valueX > 255)
				valueX = 255;
			//Set new value
			textValue = "";
			textValue << valueX;
			mPropertyGridNode->GetProperty("Red Diffuse:")->SetValueFromString(textValue);

			textValue = mPropertyGridNode->GetProperty("Green Diffuse:")->GetValueAsString();
			textValue.ToDouble(&valueY);
			if (valueY < 0)
				valueY = 0;
			if (valueY > 255)
				valueY = 255;
			//Set new value
			textValue = "";
			textValue << valueY;
			mPropertyGridNode->GetProperty("Green Diffuse:")->SetValueFromString(textValue);

			textValue = mPropertyGridNode->GetProperty("Blue Diffuse:")->GetValueAsString();
			textValue.ToDouble(&valueZ);
			if (valueZ < 0)
				valueZ = 0;
			if (valueZ > 255)
				valueZ = 255;
			//Set new value
			textValue = "";
			textValue << valueZ;
			mPropertyGridNode->GetProperty("Blue Diffuse:")->SetValueFromString(textValue);
			mKalenEngine->GetObject()->GetPointLight()->Diffuse = XMFLOAT4(valueX / 255.0f, valueY / 255.0f, valueZ / 255.0f, 1.0f);
			//------------------
			//Light - Specular
			//------------------
			textValue = mPropertyGridNode->GetProperty("Red Specular:")->GetValueAsString();
			textValue.ToDouble(&valueX);
			if (valueX < 0)
				valueX = 0;
			if (valueX > 255)
				valueX = 255;
			//Set new value
			textValue = "";
			textValue << valueX;
			mPropertyGridNode->GetProperty("Red Specular:")->SetValueFromString(textValue);

			textValue = mPropertyGridNode->GetProperty("Green Specular:")->GetValueAsString();
			textValue.ToDouble(&valueY);
			if (valueY < 0)
				valueY = 0;
			if (valueY > 255)
				valueY = 255;
			//Set new value
			textValue = "";
			textValue << valueY;
			mPropertyGridNode->GetProperty("Green Specular:")->SetValueFromString(textValue);

			textValue = mPropertyGridNode->GetProperty("Blue Specular:")->GetValueAsString();
			textValue.ToDouble(&valueZ);
			if (valueZ < 0)
				valueZ = 0;
			if (valueZ > 255)
				valueZ = 255;
			//Set new value
			textValue = "";
			textValue << valueZ;
			mPropertyGridNode->GetProperty("Blue Specular:")->SetValueFromString(textValue);
			mKalenEngine->GetObject()->GetPointLight()->Specular = XMFLOAT4(valueX / 255.0f, valueY / 255.0f, valueZ / 255.0f, 1.0f);
			//------------------
			//Light - Attenuation
			//------------------
			textValue = mPropertyGridNode->GetProperty("Distance 1:")->GetValueAsString();
			textValue.ToDouble(&valueX);
			textValue = mPropertyGridNode->GetProperty("Distance 2:")->GetValueAsString();
			textValue.ToDouble(&valueY);
			textValue = mPropertyGridNode->GetProperty("Distance 3:")->GetValueAsString();
			textValue.ToDouble(&valueZ);
			mKalenEngine->GetObject()->GetPointLight()->Att = XMFLOAT3(valueX, valueY, valueZ);
			//------------------
			//Light - Other
			//------------------
			textValue = mPropertyGridNode->GetProperty("Range:")->GetValueAsString();
			textValue.ToDouble(&valueX);
			mKalenEngine->GetObject()->GetPointLight()->Range = valueX;
		}
	}
}


//*******************************************************************************************************************************************
void MyFrame::OnCloseEngine(wxCloseEvent& evt)
{
	if (wxMessageBox("Do you want to exit?", "Please confirm", wxICON_QUESTION | wxYES_NO) != wxYES)
	{
		evt.Veto();
	}
	else
	{
		//Destroy all frame:
		mFiltersWindow->Destroy();
		mAssetBrowser->Destroy();
		Destroy();
	}
}

//*******************************************************************************************************************************************
void MyFrame::SetDeviceAndTextureBuffer(ID3D11Device* device, TexturesResource* texturesResource)
{
	this->mDevice = device;
	this->mTexturesResource = texturesResource;
}
//*******************************************************************************************************************************************
std::vector<Layer*>* MyFrame::GetLayerBuffer()
{
	return this->mLayerBuffer;
}
//*******************************************************************************************************************************************
KLMESH_Buffer* MyFrame::GetKLMESH_Buffer()
{
	return this->mKLMESH_Buffer;
}
//*******************************************************************************************************************************************
wxTreeItemId MyFrame::AddObjectToLayer(wxTreeItemId* parent, wxString name, int type)
{
	wxTreeItemId ID;
	if(type == 0)//Object3D
		ID= mTreeScene->AppendItem(mActiveLayerItem, name, 3);
	else
		if(type == 1)//Light
			ID = mTreeScene->AppendItem(mActiveLayerItem, name, 9);
		else
			ID = mTreeScene->AppendItem(mActiveLayerItem, name, 9);
	mTreeScene->SetItemState(ID, 3); 
	return ID;
}
//*******************************************************************************************************************************************
wxTreeCtrl* MyFrame::GetTree()
{
	return mTreeScene;
}
//*******************************************************************************************************************************************
AssetBrowser* MyFrame::GetAssetBrowser()
{
	return this->mAssetBrowser;
}
//*******************************************************************************************************************************************
void MyFrame::LoadLayer(wxTreeItemId layerTreeID)
{
	mKalenEngine->AllowRender(false); //Stop rendering
	wxString layerName = "";
	wxString parent = "";
	wxString path = "";
	wxString temp = "";
	wxTreeItemId tempTreeId = layerTreeID;
	wxTreeItemId tempTreeIdParent = layerTreeID;
	if (tempTreeId != nullptr)
	{
		layerName = mTreeScene->GetItemText(tempTreeId);
		if (layerName.at(0) == '-')
			if (layerName.at(1) == '-')
				if (layerName.at(2) == 'A')
					if (layerName.at(3) == 'C')
					{
						//--ACTIVE-- 
						for (int i = 11; i <  (int)layerName.size(); ++i)
							path << layerName.at(i);
						layerName = path;
					}
		while (parent != "World                                                                        ")
		{
			parent = mTreeScene->GetItemText(mTreeScene->GetItemParent(tempTreeIdParent));
			tempTreeIdParent = mTreeScene->GetItemParent(tempTreeIdParent);
			path = temp;
			temp = "";
			if (parent != "World                                                                        ")
			{
				temp << parent;
				temp << "\\";
			}
			temp << path;
			path = temp;
			path << "\\";
		}

		if (path == "\\")
			path = "";



		layerName << ".kll";
		path << layerName;


		temp = mWorldName;
		temp << "\\";

	//	temp = "data\\levels\\";
		temp << path;
		path = temp;


		//-------------
		//Load layer
		//-------------
		kllFileStruct kllFileRead;
		BinaryFileRead *binaryFileRead = new BinaryFileRead((std::string)path);
		binaryFileRead->readKLLFileStructure(kllFileRead);
		binaryFileRead->closeFile();

		//wxTreeItemId itemTreeID = nullptr;
		Object* tempObject = new Object();
		for (int i = 0; i < (int)kllFileRead.objectBuffer.size(); ++i)
		{
			wxTreeItemId itemTreeID = nullptr;
			if(kllFileRead.objectBuffer.at(i).mType == 0)
				itemTreeID = mTreeScene->AppendItem(layerTreeID, kllFileRead.objectBuffer.at(i).name, 3);
			else
				if (kllFileRead.objectBuffer.at(i).mType == 1)//Light
					itemTreeID = mTreeScene->AppendItem(layerTreeID, kllFileRead.objectBuffer.at(i).name, 9);
				else
					itemTreeID = mTreeScene->AppendItem(layerTreeID, kllFileRead.objectBuffer.at(i).name, 9);

			mTreeScene->SetItemState(itemTreeID, 3);

			KLMESH* klmesh = nullptr;
			//---------------------------
			//Load klmesh if it doesn't exist in the KLMESH_Buffer
			if (kllFileRead.objectBuffer.at(i).linkToFile != "")
			{
				if (!GetKLMESH_Buffer()->CheckMesh((std::string)kllFileRead.objectBuffer.at(i).linkToFile)) //if klmesh does not exist in the klmeshBuffer
				{
					BinaryFileRead *binaryFileRead = new BinaryFileRead((std::string)kllFileRead.objectBuffer.at(i).linkToFile);
					klmeshFileStruct klmeshTmp;
					binaryFileRead->readKLMeshFileStructure(klmeshTmp);
					binaryFileRead->closeFile();
					klmesh = GetKLMESH_Buffer()->AddMesh(new KLMESH(&klmeshTmp, mKalenEngine->getDevice()));
				}
				else
				{
					klmesh = GetKLMESH_Buffer()->GetMeshByLink((std::string)kllFileRead.objectBuffer.at(i).linkToFile);
				}
				//-------------
				//Load all texture of this klmesh
				//-------------
				for (int j = 0; j < (int)klmesh->GetMaterialBuffer()->size(); ++j)//Size of materials
				{
					if (klmesh->GetMaterialBuffer()->at(j).diffuseMapName != "NULL")
						klmesh->SetMaterialDiffuseMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).diffuseMapName), klmesh->GetMaterialBuffer()->at(j).diffuseMapName);

					if (klmesh->GetMaterialBuffer()->at(j).normalMapName != "NULL")
						klmesh->SetMaterialNormalMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).normalMapName), klmesh->GetMaterialBuffer()->at(j).normalMapName);

					if (klmesh->GetMaterialBuffer()->at(j).ashMapName != "NULL")
						klmesh->SetMaterialASHMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).ashMapName), klmesh->GetMaterialBuffer()->at(j).ashMapName);

					if (klmesh->GetMaterialBuffer()->at(j).cubeMapName != "NULL")
						klmesh->SetMaterialCubeMap(j, mKalenEngine->GetTexturesResource()->AddTextureToBuffor(klmesh->GetMaterialBuffer()->at(j).cubeMapName), klmesh->GetMaterialBuffer()->at(j).cubeMapName);
				}
				//---------------------------
				//Find open object in the klmeshViwer
				for (int j = 0; j < (int)GetAssetBrowser()->GetListOfKLMESHViewer()->size(); ++j)
				{
					if (GetAssetBrowser()->GetListOfKLMESHViewer()->at(j)->GetName() == klmesh->GetLinkToFile())
						GetAssetBrowser()->GetListOfKLMESHViewer()->at(j)->SetKLMESH(klmesh);
				}
			}
			//-------------
			//Add object to layer
			//-------------
			for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
			{
				if (mLayerBuffer->at(j)->GetRootTreeId() == layerTreeID)
				{
					mLayerBuffer->at(j)->AddObject(new Object(), itemTreeID);
					//Get pointer to the object
					tempObject = mLayerBuffer->at(j)->GetObject(itemTreeID);
				}
			}
			//-------------
			//Convert 'ObjectStruct' to the 'Object'
			//-------------
			mKalenEngine->ConvertObjectStructToObject(tempObject, kllFileRead.objectBuffer.at(i));
			//Set klmesh

			
			if (klmesh != nullptr)
			{
				tempObject->SetKLMESH(klmesh);
				tempObject->SetAxisAlignedBoundingBox(*klmesh->GetAxisAlignedBoxOrigin());
			}	
			else
			{
				AxisAlignedBox aabb;
				aabb.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
				aabb.Extents = XMFLOAT3(0.0f, 0.0f, 0.0f);
				tempObject->SetAxisAlignedBoundingBox(aabb);
			}

			//Rebuild AABB
			for (int j = 0; j < (int)mLayerBuffer->size(); ++j)
			{
				if (mLayerBuffer->at(j)->GetRootTreeId() == layerTreeID)
				{
					if (klmesh != nullptr)
					{
						mKalenEngine->RebuildBoundingBox(mLayerBuffer->at(j)->GetObject(itemTreeID));
						mKalenEngine->GetOctree()->AddObject(mLayerBuffer->at(j)->GetObject(itemTreeID));//Add to octree
					}
						
				}
			}
			Refresh();
		}
	}
	mKalenEngine->AllowRender(true); //Start rendering
}
//**********************************************************************************************************************************************
void MyFrame::LoadAllLayersInFolder(wxTreeItemId folderTreeID)
{
	wxTreeItemId parent = mTreeScene->GetItemParent(folderTreeID);
	for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
	{
		if (mTreeScene->GetItemParent(mLayerBuffer->at(i)->GetRootTreeId()) == folderTreeID)
		{
			if (mTreeScene->GetItemState(mLayerBuffer->at(i)->GetRootTreeId()) == 2 && mTreeScene->GetItemImage(mLayerBuffer->at(i)->GetRootTreeId()) == 7)
			{
				mTreeScene->SetItemImage(mLayerBuffer->at(i)->GetRootTreeId(), 2);
				LoadLayer(mLayerBuffer->at(i)->GetRootTreeId());
			}
		}
	}
}
//**********************************************************************************************************************************************
void MyFrame::LoadAllLayersInFolderHier(wxTreeItemId parentFolderTreeID)
{
	//values
	wxTreeItemIdValue cookie;
	wxTreeItemId child = mTreeScene->GetFirstChild(parentFolderTreeID, cookie);
	wxTreeItemId lastChild = mTreeScene->GetLastChild(parentFolderTreeID);
	bool isNextFolder = true;

	//Save all layers
	LoadAllLayersInFolder(parentFolderTreeID);

	if (child != NULL && lastChild != NULL)//has folder child?
	{
		if (lastChild != child) //Are there any folder? 
		{
			isNextFolder = true;//Yes
			if (mTreeScene->GetItemState(child) == 1)//folder
				LoadAllLayersInFolderHier(child);
		}
		else
			isNextFolder = false;//No

		while (isNextFolder) //While next folder exist.
		{
			child = mTreeScene->GetNextChild(parentFolderTreeID, cookie); //get next child from parent
			if (mTreeScene->GetItemState(child) == 1)//folder
			{
				LoadAllLayersInFolderHier(child);
				if (lastChild != child) //Are there any folders? 
					isNextFolder = true;//Yes
				else
					isNextFolder = false;//No
			}
			else
			{
				if (lastChild != child) //Are there any folders? 
					isNextFolder = true;//Yes
				else
					isNextFolder = false;//No
			}

		}
	}
}
//**********************************************************************************************************************************************
void MyFrame::SaveLayer(wxTreeItemId layerTreeID)
{
	wxString layerName = "";
	wxString parent = "";
	wxString path = "";
	wxString temp = "";
	wxTreeItemId tempTreeId = layerTreeID;
	if (layerTreeID != nullptr)
	{
		layerName = mTreeScene->GetItemText(layerTreeID);
		if (layerName.at(0) == '-')
			if (layerName.at(1) == '-')
				if (layerName.at(2) == 'A')
					if (layerName.at(3) == 'C')
					{
						//--ACTIVE-- 
						for (int i = 11; i < (int)layerName.size(); ++i)
							path << layerName.at(i);

						layerName = path;
					}
		while (parent != "World                                                                        ")
		{
			parent = mTreeScene->GetItemText(mTreeScene->GetItemParent(layerTreeID));
			layerTreeID = mTreeScene->GetItemParent(layerTreeID);
			path = temp;
			temp = "";
			if (parent != "World                                                                        ")
			{
				temp << parent;
				temp << "\\";
			}
			temp << path;
			path = temp;
			path << "\\";
		}
		if (path == "\\")
			path = "";
		layerName << ".kll";
		path << layerName;

		temp = mWorldName;
		temp << "\\";
		//temp = "data\\levels\\";
		temp << path;
		path = temp;

		//wxString folder = wxGetCwd();
		//wxString tempString = "";


		kllFileStruct kllFile;
		for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
		{
			if (mLayerBuffer->at(i)->GetRootTreeId() == tempTreeId)
				for (int j = 0; j < (int)mLayerBuffer->at(i)->GetAllObjects()->size(); ++j)
				{
					objectStruct obj;
					mKalenEngine->ConvertObjectToObjectStruct(obj, mLayerBuffer->at(i)->GetAllObjects()->at(j));
					kllFile.objectBuffer.push_back(obj);
				}
		}

		BinaryFileWrite *binaryFileWrite = new BinaryFileWrite((std::string)path);
		binaryFileWrite->writeKLLFileStructure(kllFile);
		binaryFileWrite->closeFile();

		AddTextToOutputWindow("'" + path + "' --> SAVED");
	}

}
//**************************************************************************************************************************
void MyFrame::SaveAllLayersInFolder(wxTreeItemId folderTreeID)
{
	wxTreeItemId parent = mTreeScene->GetItemParent(folderTreeID);
	for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
	{
		if (mTreeScene->GetItemParent(mLayerBuffer->at(i)->GetRootTreeId()) == folderTreeID)
		{
			SaveLayer(mLayerBuffer->at(i)->GetRootTreeId());
		}
	}
}
//**************************************************************************************************************************
void MyFrame::SaveAllLayersInFolderHier(wxTreeItemId parentFolderTreeID)
{
	//values
	wxTreeItemIdValue cookie;
	wxTreeItemId child = mTreeScene->GetFirstChild(parentFolderTreeID, cookie);
	wxTreeItemId lastChild = mTreeScene->GetLastChild(parentFolderTreeID);
	bool isNextFolder = true;

	//Save all layers
	SaveAllLayersInFolder(parentFolderTreeID);

	if (child != NULL && lastChild != NULL)//has folder child?
	{
		if (lastChild != child) //Are there any folder? 
		{
			isNextFolder = true;//Yes
			if (mTreeScene->GetItemState(child) == 1)//folder
				SaveAllLayersInFolderHier(child);
		}
		else
			isNextFolder = false;//No

		while (isNextFolder) //While next folder exist.
		{
			child = mTreeScene->GetNextChild(parentFolderTreeID, cookie); //get next child from parent
			if (mTreeScene->GetItemState(child) == 1)//folder
			{
				SaveAllLayersInFolderHier(child);
				if (lastChild != child) //Are there any folders? 
					isNextFolder = true;//Yes
				else
					isNextFolder = false;//No
			}
			else
			{
				if (lastChild != child) //Are there any folders? 
					isNextFolder = true;//Yes
				else
					isNextFolder = false;//No
			}
		}
	}
}
//**************************************************************************************************************************
void MyFrame::DeleteLayer(wxTreeItemId layerTreeID)
{
	for (int i = 0; i < (int)mLayerBuffer->size(); ++i)
	{
		if (mLayerBuffer->at(i)->GetRootTreeId() == layerTreeID)
		{
			mLayerBuffer->at(i)->SetIsActive(false);
			mLayerBuffer->erase(mLayerBuffer->begin() + i);
		}
	}

	wxTreeItemId folder = layerTreeID;
	wxTreeItemId parent = folder;
	wxString folderName = "";
	wxString path = "";

	std::vector<wxString>* folders = new std::vector<wxString>();
	while (parent != mTreeRoot)
	{
		parent = mTreeScene->GetItemParent(parent);
		folderName = mTreeScene->GetItemText(parent);
		folders->push_back(folderName);
	}

	path = "";
	for (int i = (int)folders->size() - 2; i >= 0; --i)
	{
		path << folders->at(i);
		path << "\\";
	}

	folderName = mTreeScene->GetItemText(folder);
	path << folderName;

	wxString pathHome = wxGetCwd();
	
	
	//pathHome << "\\data\\levels";
	pathHome << "\\";
	pathHome << mWorldName;
	//temp << "\\";

	wxDir* dir = new wxDir(pathHome);
	pathHome << "\\";
	pathHome << path;
	pathHome << ".kll";

	//delete the file
	wxRemoveFile(pathHome);
	//Delete from tree
	mTreeScene->Delete(layerTreeID);

}
//**************************************************************************************************************************
void MyFrame::DeleteEmptyFolder(wxTreeItemId folderTreeID)
{
	wxTreeItemId folder = folderTreeID;
	wxTreeItemId parent = folder;
	wxString folderName = "";
	wxString path = "";

	std::vector<wxString>* folders = new std::vector<wxString>();
	while (parent != mTreeRoot)
	{
		parent = mTreeScene->GetItemParent(parent);
		folderName = mTreeScene->GetItemText(parent);
		folders->push_back(folderName);
	}

	path = "";
	for (int i = (int)folders->size() - 2; i >= 0; --i)
	{
		path << folders->at(i);
		path << "\\";
	}

	folderName = mTreeScene->GetItemText(folder);
	path << folderName;

	AddTextToOutputWindow("\n" + path + "\n");

	wxString pathHome = wxGetCwd();
	//pathHome << "\\data\\levels";
	pathHome << "\\";
	pathHome << mWorldName;
	//temp << "\\";


	pathHome << "\\";
	pathHome << path;
	wxDir* dir = new wxDir(pathHome);


	//Delete all files 
	wxString file = "";
	wxString pathToFile = "";
	bool isNextFile = true;
	if (dir->GetFirst(&file, "", wxDIR_DEFAULT) == true)
	{
		pathToFile = pathHome;
		pathToFile << "\\";
		pathToFile << file;
		wxRemoveFile(pathToFile);
		file = "";
		//isNextFile = dir->GetNext(&file);
		while (isNextFile)
		{
			file = "";
			isNextFile = dir->GetNext(&file);
			pathToFile = pathHome;
			pathToFile << "\\";
			pathToFile << file;
			if(isNextFile)
				wxRemoveFile(pathToFile);
		}
	}



	//Create folder
	dir->Remove(pathHome, wxPATH_RMDIR_FULL);
	mTreeScene->Delete(folderTreeID);
	dir->Close();
	delete dir;
}
//**************************************************************************************************************************
void MyFrame::DeleteFolderHier(wxTreeItemId folderTreeID)
{
	//values
	wxTreeItemIdValue cookie;
	wxTreeItemId child = mTreeScene->GetFirstChild(folderTreeID, cookie);
	wxTreeItemId lastChild = mTreeScene->GetLastChild(folderTreeID);
	bool isNextFolder = true;
	std::vector<wxTreeItemId>* treeVector = new std::vector<wxTreeItemId>();


	if (child != NULL && lastChild != NULL)//has item child?
	{
		if (lastChild != child)
		{
			isNextFolder = true;
			//Add to vector
			treeVector->push_back(child);
		}
		else //only 1 element
		{
			isNextFolder = false;
			if (mTreeScene->GetItemState(child) == 1)//folder
				DeleteFolderHier(child);
			else if (mTreeScene->GetItemState(child) == 2)//kll
				DeleteLayer(child);

			DeleteEmptyFolder(folderTreeID);
		}

		while (isNextFolder)
		{
			child = mTreeScene->GetNextChild(folderTreeID, cookie); //get next child from parent
			if (child != nullptr)
			{
				treeVector->push_back(child);
				isNextFolder = true;//Yes
			}
			else
				isNextFolder = false;//No
		}

		for (int i = 0; i < (int)treeVector->size(); ++i)
		{
			if(mTreeScene->GetItemState(treeVector->at(i)) == 1)//folder
				DeleteFolderHier(treeVector->at(i));
			if(mTreeScene->GetItemState(treeVector->at(i)) == 2)//kll
				DeleteLayer(treeVector->at(i));
		}
		//folder is empty -- delete
		DeleteEmptyFolder(folderTreeID);
	}
	else
	{
		DeleteEmptyFolder(folderTreeID);
	}
}

