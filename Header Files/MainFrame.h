#ifndef __MAINFRAME__
#define  __MAINFRAME__


#include "Utility.h"
#include "FiltersWindow.h"
#include "AssetBrowser.h"
#include "KalenEngine.h"
#include "Layer.h"
#include "KLMESH_Buffer.h"


//**********************************************************************************************************************************************
class MyApp : public wxApp
{
public:
	bool OnInit() wxOVERRIDE;

};
//**********************************************************************************************************************************************
extern wxTextCtrl        *mTextOututWindow123;
wxWindowID				 mWindowsDirectID;


//**********************************************************************************************************************************************
class MyFrame : public wxFrame
{
	enum
	{
		ID_CustomizeToolbar,
		ID_DropDownToolbarItem,
		ID_NotebookAlignBottom,
		ID_NotebookAlignTop,

		ID_CreateRightWindow,
		ID_ToolItemX,
		ID_ToolItemACTIVE_L,
		ID_ToolItemMULTI_L,
		ID_ToolItemGLOBAL,
		ID_ToolItemLOCAL,
		ID_ToolItemMOVE,
		ID_ToolItemROTATE,
		ID_ToolItemSCALE,
		ID_ToolItemCOPY,
		ID_ToolItemCUT,
		ID_ToolItemPASTE,
		ID_ToolItemDELETE,
		ID_ToolItemPanelSceneVisible,
		ID_ToolItemPanelSceneTabbed,
		ID_ToolItemPanelSceneShow,
		ID_ToolItemPanelToolsVisible,
		ID_ToolItemPanelToolsTabbed,
		ID_ToolItemPanelToolsShow,
		ID_ToolItemPanelPropertiesVisible,
		ID_ToolItemPanelPropertiesTabbed,
		ID_ToolItemPanelPropertiesShow,
		ID_ToolItemPanelShowAll,
		ID_ToolItemPanelHideAll,
		ID_ToolItemPanelAllToWindows,
		ID_ToolItemPanelAllToTab,
		ID_ToolItemToolbarsMain,
		ID_ToolItemToolbarsMode,
		ID_ToolItemViewportFloat,
		ID_ToolItemAssetBrowser,
		ID_ToolItemOutputWindow,
		ID_ToolItemFilters,
		ID_ToolItemMenuUndo,
		ID_ToolItemMenuRedo,
		ID_ToolItemMenuCopy,
		ID_ToolItemMenuCut,
		ID_ToolItemMenuPaste,
		ID_ToolItemMenuDelete,
		ID_ToolItemMenuLookAt,
		ID_ToolItemMenuCancel,
		ID_ToolItemMenuActiveLayer,
		ID_ToolItemMenuMultiLayer,
		ID_ToolItemMenuMove,
		ID_ToolItemMenuRotate,
		ID_ToolItemMenuScale,
		ID_ToolItemMenuSnap,
		ID_ToolItemMenuSnapTerrain,
		ID_ToolItemMenuSnapAll,
		ID_ToolItemMenuLocal,
		ID_ToolItemMenuGlobal,
		ID_ToolSmallNewWorld,
		ID_ToolSmallOpenWorld,
		ID_ToolSmallRestoreWorld,
		ID_ToolSmallSaveWorld,
		ID_ToolSmallCopy,
		ID_ToolSmallPaste,
		ID_ToolSmallDelete,
		ID_ToolSmallAssetBrowser,
		ID_ToolSmallGridMove,
		ID_ToolSmallChoiceMove,
		ID_ToolSmallGridRotate,
		ID_ToolSmallChoiceRotate,
		ID_ToolSmallChoiceSnap,
		ID_ToolSmallChoiceRender,
		ID_TreeScene,
		ID_TreeSceneMenuFolderSaveHier,
		ID_TreeSceneMenuFolderSaveFolder,
		ID_TreeSceneMenuFolderNewLayer,
		ID_TreeSceneMenuFolderNewFolder,
		ID_TreeSceneMenuFolderRemove,
		ID_TreeSceneMenuLayerSave,
		ID_TreeSceneMenuLayerMakeActive,
		ID_TreeSceneMenuLayerRemove,
		ID_TreeSceneMenuLayerHide,
		ID_TreeSceneMenuObjectLook,
		ID_TreeSceneMenuObjectCopy,
		ID_TreeSceneMenuObjectCut,
		ID_TreeSceneMenuObjectDelete,
		ID_SampleItem,
		ID_WindowDirectX,

		ID_PG_PropertyObject,
		ID_NOTEBOOK_RightWindow,
		ID_PG_PropertyDirectionalLight,
		ID_TreeSceneMenuLayerLoad,
		ID_TreeSceneMenuLayerShow,
		ID_TreeSceneMenuFolderLoadAll,
		ID_TreeSceneMenuLayerDeactivation,
	};

public:
	MyFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

	~MyFrame();

	wxAuiDockArt* GetDockArt();
	void DoUpdate();
	void SetDeviceAndTextureBuffer(ID3D11Device* device, TexturesResource* texturesResource);
	std::vector<Layer*>* GetLayerBuffer();
	KLMESH_Buffer* GetKLMESH_Buffer();
	wxTreeItemId AddObjectToLayer(wxTreeItemId* parent, wxString name, int type);
	wxTreeCtrl* GetTree();
	AssetBrowser* GetAssetBrowser();


private:
	wxTextCtrl* CreateTextCtrl(const wxString& text = wxEmptyString);
	wxTreeCtrl* CreateTreeCtrl();
	wxPoint GetStartPosition();
	wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent = NULL);
	wxAuiNotebook* CreateNotebook();
	wxTextCtrl* CreateOutputWindow();
	wxAuiNotebook* CreateRightWindow();
	wxString GetIntroText(wxString text);
	wxListCtrl* CreateListCtrlRightPanel();
	wxPropertyGrid* CreateGrid();
	wxPropertyGrid* CreateTestPropertyGrid();
	void LoadLayer(wxTreeItemId layerTreeID);
	void LoadAllLayersInFolder(wxTreeItemId folderTreeID);
	void LoadAllLayersInFolderHier(wxTreeItemId parentFolderTreeID);

	void DeleteLayer(wxTreeItemId layerTreeID);
	void DeleteEmptyFolder(wxTreeItemId folderTreeID);
	void DeleteFolderHier(wxTreeItemId folderTreeID);

	void SaveLayer(wxTreeItemId layerTreeID);
	void SaveAllLayersInFolder(wxTreeItemId layerTreeID);
	void SaveAllLayersInFolderHier(wxTreeItemId parentFolderTreeID);




	void OnIdle(wxIdleEvent& evt);
	void OnUpdateValues(wxMouseEvent& evt);
	void OnCloseEngine(wxCloseEvent& event);
	void AddTextToOutputWindow(wxString text);
	void OnEraseBackground(wxEraseEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void OnCreateRightWindow(wxCommandEvent& evt);
	// void OnDropDownToolbarItem(wxAuiToolBarEvent& evt);
	void OnCustomizeToolbar(wxCommandEvent& evt);
	void OnAllowNotebookDnD(wxAuiNotebookEvent& evt);
	void OnNotebookPageClose(wxAuiNotebookEvent& evt);
	void OnNotebookPageClosed(wxAuiNotebookEvent& evt);
	void OnExit(wxCommandEvent& evt);
	void OnAbout(wxCommandEvent& evt);
	void OnPaneClose(wxAuiManagerEvent& evt);
	void OnPGObjectPropertyChanged(wxPropertyGridEvent& evt);
	void OnPGSunPropertyChanged(wxPropertyGridEvent& evt);


	void OnXTouch(wxCommandEvent& evt);
	//Big toolbar
	void OnToolXActiveL(wxCommandEvent& evt);
	void OnToolXMultiL(wxCommandEvent& evt);
	void OnToolXGlobal(wxCommandEvent& evt);
	void OnToolXLocal(wxCommandEvent& evt);
	void OnToolXMove(wxCommandEvent& evt);
	void OnToolXRotate(wxCommandEvent& evt);
	void OnToolXScale(wxCommandEvent& evt);
	void OnToolXCopy(wxCommandEvent& evt);
	void OnToolXCut(wxCommandEvent& evt);
	void OnToolXPaste(wxCommandEvent& evt);
	void OnToolXDelete(wxCommandEvent& evt);

	//menu
	void OnToolMenuFiltersWindow(wxCommandEvent& evt);
	void OnToolMenuAssetBrowsersWindow(wxCommandEvent& evt);
	void OnToolMenuOutputWindowShowHide(wxCommandEvent& evt);
	

	//Small toolbar
	void OnToolSNewWorld(wxCommandEvent& evt);
	void OnToolSOpenWorld(wxCommandEvent& evt);
	void OnToolSRestoreWorld(wxCommandEvent& evt);
	void OnToolSSaveWorld(wxCommandEvent& evt);
	void OnToolSCopy(wxCommandEvent& evt);
	void OnToolSPaste(wxCommandEvent& evt);
	void OnToolSDelete(wxCommandEvent& evt);
	void OnToolSAssetBrowser(wxCommandEvent& evt);
	void OnToolSGridMove(wxCommandEvent& evt);
	void OnToolSChoiceMove(wxCommandEvent& evt);
	void OnToolSGridRotate(wxCommandEvent& evt);
	void OnToolSChoiceRotate(wxCommandEvent& evt);
	void OnToolSChoiceSnap(wxCommandEvent& evt);
	void OnToolSChoiceRender(wxCommandEvent& evt);
	void OnToolMenuUndo(wxCommandEvent& evt);
	void OnToolMenuRedo(wxCommandEvent& evt);
	void OnToolMenuLookAt(wxCommandEvent& evt);
	void OnToolMenuCancel(wxCommandEvent& evt);
	void OnToolMenuActiveLayer(wxCommandEvent& evt);
	void OnToolMenuActiveLayerClick(wxCommandEvent& evt);
	void OnToolMenuMultiLayerClick(wxCommandEvent& evt);
	void OnToolMenuMove(wxCommandEvent& evt);
	void OnToolMenuRotate(wxCommandEvent& evt);
	void OnToolMenuScale(wxCommandEvent& evt);
	void OnToolMenuSnap(wxCommandEvent& evt);
	void OnToolMenuSnapTerrain(wxCommandEvent& evt);
	void OnToolMenuSnapAll(wxCommandEvent& evt);
	void OnToolMenuLocal(wxCommandEvent& evt);
	void OnToolMenuGlobal(wxCommandEvent& evt);
	void OnToolMenuCut(wxCommandEvent& evt);
	void OnTreeRightClick(wxTreeEvent& evt);
	void OnTreeLeftDoubleClick(wxTreeEvent& evt);
	void OnTreeSeletionING(wxTreeEvent& evt);
	void OnTreeSeletionED(wxTreeEvent& evt);
	void OnTreeSceneMenuFolderSaveFolder(wxCommandEvent& evt);
	void OnTreeSceneMenuFolderSaveHier(wxCommandEvent& evt);
	void OnTreeSceneMenuFolderNewLayer(wxCommandEvent& evt);
	void OnTreeSceneMenuFolderNewFolder(wxCommandEvent& evt);
	void OnTreeSceneMenuFolderLoadAll(wxCommandEvent& evt);

	
	void OnTreeSceneMenuFolderRemove(wxCommandEvent& evt);
	void OnTreeSceneMenuLayerSave(wxCommandEvent& evt);
	void OnTreeSceneMenuLayerMakeActive(wxCommandEvent& evt);
	void OnTreeSceneMenuLayerDeactivation(wxCommandEvent& evt);
	void OnTreeSceneMenuLayerRemove(wxCommandEvent& evt);
	void OnTreeSceneMenuLayerLoad(wxCommandEvent& evt);
	void OnTreeSceneMenuLayerShow(wxCommandEvent& evt);

	void OnTreeSceneMenuLayerHide(wxCommandEvent& evt);
	void OnTreeSceneMenuObjectLook(wxCommandEvent& evt);
	void OnTreeSceneMenuObjectCopy(wxCommandEvent& evt);
	void OnTreeSceneMenuObjectCut(wxCommandEvent& evt);
	void OnTreeSceneMenuObjectDelete(wxCommandEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	void connectKey(wxWindow* a);
	void OnNotebookChanging(wxAuiNotebookEvent& evt);
	void LoadAllFoldersAndFilesAsTree(wxString path, wxTreeItemId parent);

	wxTextCtrl        *mTextOututWindow;

private:

	KalenEngine* mKalenEngine; //DirectX 11 and the output window 
	ID3D11Device* mDevice;
	TexturesResource* mTexturesResource;
	KLMESH_Buffer* mKLMESH_Buffer;
	wxString mWorldName = "";
	bool mShowProperty = false;

	std::vector<Layer*>* mLayerBuffer;

	wxHtmlWindow* mTestWindowMain;

	wxAuiManager m_mgr;
	wxMenu* m_edit_menu;
	wxMenu* m_mode_menu;
	long m_notebook_style;
	long m_notebook_theme;

	//Windows
	FiltersWindow* mFiltersWindow;
	AssetBrowser* mAssetBrowser;

	//Others
	wxAuiToolBarItem* mToolBarActiveLayerItem;
	wxAuiToolBarItem* mToolBarMultiLayerItem;
	bool mDeletedObject = false;
	bool mCutObject = false;
	int mCopyPasteCutDelete = -1;

	Object* mCopiedObject;
	Object* test1;
	Object* test2;
	wxAuiToolBar* mToolBarTop;
	wxAuiToolBar* mToolBarLeft;
	wxChoice* mChoiceMoveUnit;
	wxChoice* mChoiceRotateUnit;
	wxChoice* mChoiceSnap;
	wxTreeCtrl* mTreeScene;
	wxMenu* mMenuRightClickFolder;
	wxMenu* mMenuRightClickLayer;
	wxString mActiveLayerName;
	wxTreeItemId mActiveLayerItem;
	wxTreeItemId mSelectedObjectItem;
	wxTreeItemId mSelectedLayerItem;
	wxTreeItemId mTreeBuffer;
	wxTreeItemId mRightClickedItem;
	wxTreeItemId mTreeRoot;
	wxString mTreeBufferNameObject;
	wxListCtrl* mListCtrlRightPanel;
	wxTextCtrl* mNodeName;
	wxFlexGridSizer *mFlexPropertiesPanel;
	wxGrid* mNodeGridMesh;
	wxCheckBox* mNodeCheckBoxIsCastShadows;
	wxCheckBox* mNodeCheckBoxIsVisible;
	wxGrid* mNodeGridScale;
	wxGrid* mNodeGridRotation;
	wxGrid* mNodeGridPosition;
	wxStaticText* mNodeNameLabel;
	wxStaticText* mNodeTransformLabel;
	wxStaticText* mNodePositionLabel;
	wxStaticText* mNodeRotationLabel;
	wxStaticText* mNodeScaleLabel;
	wxStaticText* mNodeFlagsLabel;
	wxStaticText* mNodeMeshLabel;
	wxPanel* mDirectXPanel;

	wxPropertyGrid* mPropertyGridNode;
	wxPropertyCategory* mPropertyGridCase_Node;
	wxPropertyCategory* mPropertyGridCase_Position;
	wxPropertyCategory* mPropertyGridCase_Rotation;
	wxPropertyCategory* mPropertyGridCase_Scale;
	wxPropertyCategory* mPropertyGridCase_DrawableFlags;
	wxPropertyCategory* mPropertyGridCase_MeshComponent;

	wxPropertyGrid* mPropertyDirectionalLight;
	wxPropertyCategory* mPropertyDirectionalLightAmbient;
	wxPropertyCategory* mPropertyDirectionalLightDiffuse;
	wxPropertyCategory* mPropertyDirectionalLightSpecular;
	wxPropertyCategory* mPropertyDirectionalLightDirection;
	wxPropertyCategory* mPropertyCubeMapPosition;

	wxPropertyCategory* mPropertyGridCase_LightAmbient;
	wxPropertyCategory* mPropertyGridCase_LightDiffuse;
	wxPropertyCategory* mPropertyGridCase_LightSpecular;
	wxPropertyCategory* mPropertyGridCase_LightAtt;
	wxPropertyCategory* mPropertyGridCase_LightOther;



	wxDECLARE_EVENT_TABLE();
};




#endif // !__MAINFRAME__