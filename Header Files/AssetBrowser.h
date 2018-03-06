#ifndef __ASSETBROWSER__
#define  __ASSETBROWSER__

#include "Utility.h"
#include "ImageViewer.h"
#include "KLMeshViewer.h"
#include "KLMESH_Buffer.h"
#include "TexturesResource.h"



class AssetBrowser : public wxFrame
{
	enum
	{
		ID_AssetsNotebook,
		ID_TreeFiles,
		ID_MENU_VIEW_ICON_SMALL,
		ID_MENU_VIEW_ICON_MEDIUM,
		ID_MENU_VIEW_ICON_HUGE,
		ID_MENU_VIEW_REFRESH,
		ID_TreeFilesMenuRefresh,
		ID_TreeFilesMenuNewFolder,
		ID_TreeFilesMenuRemove,
		ID_TreeFilesMenuNewFolderRoot,
		ID_ListCrtItemsNotebook,
		ID_AssetBrowserDirectoryOpen,
		ID_AssetBrowserDirectoryDelete,
		ID_AssetBrowserDirectoryRename,
		ID_AssetBrowserItemOpen,
		ID_AssetBrowserItemLoad,
		ID_AssetBrowserItemReload,
		ID_AssetBrowserItemRefresh,
		ID_AssetBrowserItemCopy,
		ID_AssetBrowserItemCut,
		ID_AssetBrowserItemDelete,
		ID_AssetBrowserItemRename,
		ID_NotebookPageLock,
		ID_NotebookPageDuplicate,
		ID_NotebookPageClose,
		ID_TreeFilesMenuOpen,
		ID_TreeFilesMenuOpenInNewTab,
		ID_MENU_EDIT_COPY,
		ID_MENU_EDIT_CUT,
		ID_MENU_EDIT_PASTE,
		ID_MENU_EDIT_DELETE,
		ID_MENU_IMPORT_MESH,
		ID_MENU_IMPORT_TEXTURE,
		ID_MENU_CREATE_WORLD,
		ID_MENU_CREATE_KLMESH,
		ID_MENU_EDIT_REFRESH,
		ID_AssetBrowserDirectoryOpenInNewTab,
		ID_AssetBrowserDirectoryOpenBack,
		ID_AssetBrowserDirectoryOpenBackInNewTab,
		ID_MENU_EDIT_PASTE_AS,
		ID_MENU_ADD_Directory,
		ID_MENU_EXIT,
	};

public:
	AssetBrowser(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

	wxString GetLinkToSelectedFile();
	void SetKLMESH_Buffer(KLMESH_Buffer* klmeshBuffer);
	void SetTexturesResource(TexturesResource* texturesResource);
	void SetDevice(ID3D11Device* device);
	std::vector<KLMESHViewer*>* GetListOfKLMESHViewer();

	~AssetBrowser();

private:
	KLMESH_Buffer* mKLMESH_Buffer;
	TexturesResource* mTexturesResource;
	ID3D11Device* mDevice;

	wxString mLinkToSelectedFile;
	wxAuiManager m_mgr;
	wxTreeCtrl* mTreeFiles;
	wxAuiNotebook* mNotebookAssets;
	wxBitmap mPage_bmp;
	wxArrayTreeItemIds mFolder;
	wxTreeItemId mRoot;
	wxImageList* mImgList;
	wxImageList* mImgListAssetBrowser;
	wxTreeItemId mSelectedObjectItemTree;
	std::vector<Node*>* mTreeFolders;
	wxString mDirectory; 
	wxString mDirectoryPathAssetBrowser;
	wxString mDirectoryNameAssetBrowser;
	wxString mSelectedFilePath;
	wxString mSelectedFileName;
	wxString mCopyBufferPath;
	wxString mCopyBufferName;
	bool mIsItemSelected;
	bool mIsItemSelectedDirectory;
	bool mIsCopiedFile;
	bool mIsCutFile;
	int mSelectedNotebookPage; 
	wxDECLARE_EVENT_TABLE();
	std::vector<KLMESHViewer*>* mListOfKLMESHViewer;
	std::vector<ImageViewer*>* mListOfImageViewer;

	void OnClose(wxCloseEvent& evt);
	void OnResize(wxSizeEvent& evt);
	wxListCtrl* CreateAssetBrowserNotebook(wxWindow* parent);
	wxListCtrl* CreateAssetBrowserNotebookDirectory(wxWindow* parent, wxString directory);
	wxTreeCtrl* CreateTreeCtrl();
	void OnNotebookPageClose(wxAuiNotebookEvent& evt);
	void OnNotebookPageRightClick(wxAuiNotebookEvent& evt);
	void OnNotebookPageChanged(wxAuiNotebookEvent& evt);
	void OnNotebookPageRightClickLock(wxCommandEvent& evt);
	void OnNotebookPageRightClickDuplicate(wxCommandEvent& evt);
	void OnNotebookPageRightClickClose(wxCommandEvent& evt);
	void OnTreeClick(wxTreeEvent& evt);
	void OnRefreshDictionary();
	void OnTreeFileOpenDirectory(wxCommandEvent& evt);
	void OnTreeFileOpenDirectoryInNewTab(wxCommandEvent& evt);
	void OnTreeRightClick(wxTreeEvent& evt);
	void OnTreeFileRefresh(wxCommandEvent& evt);
	void OnTreeFileCreateFolder(wxCommandEvent& evt);
	void OnTreeFileCreateFolderRoot(wxCommandEvent& evt);
	void OnTreeFileRemoveFolder(wxCommandEvent& evt);
	bool DeleteDirectory(wxString rmDir);

	void OnDragAndDropItem(wxListEvent& evt);
	void OnListCrtDeleteItem(wxListEvent& evt);
	void OnListCrtActivatedItem(wxListEvent& evt);
	void OnListCrtRightClickItem(wxListEvent& evt);
	void OnListCrtFocusedItem(wxListEvent& evt);

	void OnAssetBrowserRefresh(wxCommandEvent& evt);

	void OnMenuViewGridSmall(wxCommandEvent& evt);
	void OnMenuViewGridMedium(wxCommandEvent& evt);
	void OnMenuViewGridHuge(wxCommandEvent& evt);
	void OnMenuClose(wxCommandEvent& evt);

	void OnItemKLMESHOpen(wxCommandEvent& evt);
	void OnItemKLMESHLoadFromDisk(wxCommandEvent& evt);
	void OnItemKLMESHReload(wxCommandEvent& evt);
	void OnItemCopy(wxCommandEvent& evt);
	void OnItemCut(wxCommandEvent& evt);
	void OnItemDelete(wxCommandEvent& evt);
	void OnItemRename(wxCommandEvent& evt);
	void OnAssetBrowserDirectoryOpen(wxCommandEvent& evt);
	void OnAssetBrowserDirectoryOpenInNewTab(wxCommandEvent& evt);
	void OnAssetBrowserDirectoryOpenBack(wxCommandEvent& evt);
	void OnAssetBrowserDirectoryOpenBackInNewTab(wxCommandEvent& evt);
	void OnAssetBrowserDirectoryDelete(wxCommandEvent& evt);
	void OnAssetBrowserDirectoryRename(wxCommandEvent& evt);
	void OnAssetBrowserNewDirectory(wxCommandEvent& evt);

	void OnMenuEditCopy(wxCommandEvent& evt);
	void OnMenuEditCut(wxCommandEvent& evt);
	void OnMenuEditPaste(wxCommandEvent& evt);
	void OnMenuEditPasteAs(wxCommandEvent& evt);
	void OnMenuEditDelete(wxCommandEvent& evt);

	void OnListSelectedItem(wxListEvent& evt);
	void OnListDeselectedItem(wxListEvent& evt);
	
	void OnRightClickBackground(wxContextMenuEvent& evt);

	void RefreshAllNotebooks();

};




#endif // __ASSETBROWSER__