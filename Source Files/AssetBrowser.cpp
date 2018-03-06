#include "../Header Files/AssetBrowser.h"


wxBEGIN_EVENT_TABLE(AssetBrowser, wxFrame)
	EVT_END_SESSION(AssetBrowser::OnClose)
	EVT_CLOSE(AssetBrowser::OnClose)
	EVT_SIZE(AssetBrowser::OnResize)
	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_AssetsNotebook, AssetBrowser::OnNotebookPageClose)
	EVT_AUINOTEBOOK_TAB_RIGHT_DOWN(ID_AssetsNotebook, AssetBrowser::OnNotebookPageRightClick)
	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_AssetsNotebook, AssetBrowser::OnNotebookPageChanged)
	EVT_MENU(ID_NotebookPageLock, AssetBrowser::OnNotebookPageRightClickLock)
	EVT_MENU(ID_NotebookPageDuplicate, AssetBrowser::OnNotebookPageRightClickDuplicate)
	EVT_MENU(ID_NotebookPageClose, AssetBrowser::OnNotebookPageRightClickClose)

	EVT_TREE_ITEM_RIGHT_CLICK(ID_TreeFiles, AssetBrowser::OnTreeRightClick)
	EVT_MENU(ID_TreeFilesMenuRefresh, AssetBrowser::OnTreeFileRefresh)
	EVT_MENU(ID_TreeFilesMenuNewFolder, AssetBrowser::OnTreeFileCreateFolder)
	EVT_MENU(ID_TreeFilesMenuNewFolderRoot, AssetBrowser::OnTreeFileCreateFolderRoot)
	EVT_MENU(ID_TreeFilesMenuRemove, AssetBrowser::OnTreeFileRemoveFolder)
	EVT_MENU(ID_MENU_VIEW_REFRESH, AssetBrowser::OnAssetBrowserRefresh)
	EVT_MENU(ID_TreeFilesMenuOpen, AssetBrowser::OnTreeFileOpenDirectory)
	EVT_MENU(ID_TreeFilesMenuOpenInNewTab, AssetBrowser::OnTreeFileOpenDirectoryInNewTab)

	EVT_LIST_BEGIN_DRAG(ID_ListCrtItemsNotebook, AssetBrowser::OnDragAndDropItem)
	EVT_LIST_DELETE_ITEM(ID_ListCrtItemsNotebook, AssetBrowser::OnListCrtDeleteItem)
	EVT_LIST_ITEM_ACTIVATED(ID_ListCrtItemsNotebook, AssetBrowser::OnListCrtActivatedItem)
	EVT_LIST_ITEM_RIGHT_CLICK(ID_ListCrtItemsNotebook, AssetBrowser::OnListCrtRightClickItem)
	EVT_LIST_ITEM_FOCUSED(ID_ListCrtItemsNotebook, AssetBrowser::OnListCrtFocusedItem)

	EVT_MENU(ID_MENU_VIEW_ICON_SMALL, AssetBrowser::OnMenuViewGridSmall)
	EVT_MENU(ID_MENU_VIEW_ICON_MEDIUM, AssetBrowser::OnMenuViewGridMedium)
	EVT_MENU(ID_MENU_VIEW_ICON_HUGE, AssetBrowser::OnMenuViewGridHuge)
	//EVT_MENU(ID_MENU_VIEW_REFRESH, AssetBrowser::OnAssetBrowserRefresh)
	
	EVT_MENU(wxID_EXIT, AssetBrowser::OnMenuClose)
	EVT_MENU(ID_MENU_EXIT, AssetBrowser::OnMenuClose)
	
	
	EVT_MENU(ID_AssetBrowserItemOpen, AssetBrowser::OnItemKLMESHOpen)
	EVT_MENU(ID_AssetBrowserItemLoad, AssetBrowser::OnItemKLMESHLoadFromDisk)
	EVT_MENU(ID_AssetBrowserItemReload, AssetBrowser::OnItemKLMESHReload)
	EVT_MENU(ID_AssetBrowserItemCopy, AssetBrowser::OnMenuEditCopy)
	EVT_MENU(ID_AssetBrowserItemCut, AssetBrowser::OnMenuEditCut)
	EVT_MENU(ID_AssetBrowserItemDelete, AssetBrowser::OnItemDelete)
	EVT_MENU(ID_AssetBrowserItemRename, AssetBrowser::OnItemRename)

	EVT_MENU(ID_AssetBrowserDirectoryOpen, AssetBrowser::OnAssetBrowserDirectoryOpen)
	EVT_MENU(ID_AssetBrowserDirectoryOpenInNewTab, AssetBrowser::OnAssetBrowserDirectoryOpenInNewTab)
	EVT_MENU(ID_AssetBrowserDirectoryOpenBack, AssetBrowser::OnAssetBrowserDirectoryOpenBack)
	EVT_MENU(ID_AssetBrowserDirectoryOpenBackInNewTab, AssetBrowser::OnAssetBrowserDirectoryOpenBackInNewTab)
	EVT_MENU(ID_AssetBrowserDirectoryDelete, AssetBrowser::OnAssetBrowserDirectoryDelete)
	EVT_MENU(ID_AssetBrowserDirectoryRename, AssetBrowser::OnAssetBrowserDirectoryRename)


	EVT_MENU(ID_MENU_EDIT_COPY, AssetBrowser::OnMenuEditCopy)
	EVT_MENU(ID_MENU_EDIT_CUT, AssetBrowser::OnMenuEditCut)
	EVT_MENU(ID_MENU_EDIT_PASTE, AssetBrowser::OnMenuEditPaste)
	EVT_MENU(ID_MENU_EDIT_PASTE_AS, AssetBrowser::OnMenuEditPasteAs)
	EVT_MENU(ID_MENU_EDIT_DELETE, AssetBrowser::OnMenuEditDelete)

	EVT_MENU(ID_MENU_ADD_Directory, AssetBrowser::OnAssetBrowserNewDirectory)
	

	EVT_LIST_ITEM_SELECTED(ID_ListCrtItemsNotebook, AssetBrowser::OnListSelectedItem)
	EVT_LIST_ITEM_DESELECTED(ID_ListCrtItemsNotebook, AssetBrowser::OnListDeselectedItem)

	EVT_CONTEXT_MENU(AssetBrowser::OnRightClickBackground)

	//TreeFiles
	EVT_TREE_ITEM_ACTIVATED(ID_TreeFiles, AssetBrowser::OnTreeClick)
wxEND_EVENT_TABLE()


//************************************************************************************************************************************************************************************************
//AssetBrowser - C-TOR
//************************************************************************************************************************************************************************************************
AssetBrowser::AssetBrowser(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	mLinkToSelectedFile = "";
    mSelectedFilePath = "";
	mSelectedFileName = "";
	mIsItemSelected = false;
	mIsItemSelectedDirectory = false;
	mIsCopiedFile = false;
	mIsCutFile = false;
	mListOfKLMESHViewer = new std::vector<KLMESHViewer*>(0);
	mListOfImageViewer = new std::vector<ImageViewer*>(0);
	
	//-----------------------------------------
	//Set Icons - Huge 
	//-----------------------------------------
	mImgListAssetBrowser = new wxImageList(128, 128, true, 2);
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_LargeDirectory"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_PNG"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_DDS"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_TGA"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_JPG"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));


	//-----------------------------------------
	//Set Directory
	//-----------------------------------------
	mDirectory = wxGetCwd() + "\\data";
	mSelectedNotebookPage = 0;

	m_mgr.SetManagedWindow(this);
	mNotebookAssets = new wxAuiNotebook(this, ID_AssetsNotebook, wxPoint(0, 0), wxSize(430, 200), wxAUI_NB_DEFAULT_STYLE | wxNO_BORDER);
	
	mNotebookAssets->Freeze();
	mPage_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));
	mPage_bmp.SetHeight(15);

	//--------------------------
	//Tab 1 - Scene
	//--------------------------
	mNotebookAssets->AddPage(CreateAssetBrowserNotebook(mNotebookAssets), wxT("Depot"), false, mPage_bmp);
	mNotebookAssets->SetPageToolTip(0, mDirectory);


	mNotebookAssets->Thaw();


	m_mgr.AddPane(mNotebookAssets, wxAuiPaneInfo().
		Name(wxT("test1")).Caption(wxT("Assets")).
		Center().
		CloseButton(false).MaximizeButton(false).Movable(false));

	m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
		Name(wxT("test2")).Caption(wxT("Directory structure")).
		Left().
		CloseButton(false).MaximizeButton(false).Movable(false));


	SetIcon(wxIcon(wxT("KL_ICON_Logo")));


	//--------------------------------------------------------------------------------------------------
	// MENU
	//--------------------------------------------------------------------------------------------------

	wxMenuBar* menuBar = new wxMenuBar;

	//-----------------------
	//MENU - FILES
	//-----------------------
	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_MENU_EXIT, wxT("Quit\tESC"), wxT(""), wxITEM_NORMAL);

	//----------------------- //
	//MENU - VIEW
	//-----------------------
	wxMenu* view_menu = new wxMenu;

	view_menu->Append(ID_MENU_VIEW_ICON_SMALL, wxT("Small icons\tF1"), wxT(""), wxITEM_RADIO);
	view_menu->Append(ID_MENU_VIEW_ICON_MEDIUM, wxT("Medium icons\tF2"), wxT(""), wxITEM_RADIO);
	view_menu->Append(ID_MENU_VIEW_ICON_HUGE, wxT("Huge icons\tF3"), wxT(""), wxITEM_RADIO);
	view_menu->Check(ID_MENU_VIEW_ICON_SMALL, false);
	view_menu->Check(ID_MENU_VIEW_ICON_MEDIUM, false);
	view_menu->Check(ID_MENU_VIEW_ICON_HUGE, true);
	view_menu->AppendSeparator();
	view_menu->Append(ID_MENU_VIEW_REFRESH, wxT("Refresh\tF5"), wxT(""), wxITEM_NORMAL);


	//-----------------------
	//MENU - EDIT
	//-----------------------
	wxMenu* edit_menu = new wxMenu;

	edit_menu->Append(ID_MENU_EDIT_COPY, wxT("Copy\tCtrl-C"), wxT(""), wxITEM_NORMAL);
	edit_menu->Append(ID_MENU_EDIT_CUT, wxT("Cut\tCtrl-X"), wxT(""), wxITEM_NORMAL);
	edit_menu->Append(ID_MENU_EDIT_PASTE, wxT("Paste\tCtrl-V"), wxT(""), wxITEM_NORMAL);
	edit_menu->Append(ID_MENU_EDIT_DELETE, wxT("Delete\tDelete"), wxT(""), wxITEM_NORMAL);


	//-----------------------------------------
	//MENU - ADD MENU
	//-----------------------------------------
	menuBar->Append(file_menu, wxT("&File"));
	menuBar->Append(view_menu, wxT("&View"));
	menuBar->Append(edit_menu, wxT("&Edit"));
	SetMenuBar(menuBar);


	//-----------------------------------------
	//StatusBar
	//-----------------------------------------
	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"), 0);





	m_mgr.Update();

}
//************************************************************************************************************************************************************************************************
//AssetBrowser - D-TOR
//************************************************************************************************************************************************************************************************
AssetBrowser::~AssetBrowser()
{
	for (int i = 0; i < (int)mListOfKLMESHViewer->size(); ++i)
	{
		if (!mListOfKLMESHViewer->at(i)->IsBeingDeleted())
		{
			if(mListOfKLMESHViewer->at(i) != nullptr)
				mListOfKLMESHViewer->at(i)->Destroy();
		}
	}
	delete mListOfKLMESHViewer;

	for (int i = 0; i < (int)mListOfImageViewer->size(); ++i)
	{
		if (!mListOfImageViewer->at(i)->IsBeingDeleted())
		{
			mListOfImageViewer->at(i)->Destroy();
		}
	}
	delete mListOfImageViewer;
	delete mTreeFiles;
	delete mNotebookAssets;
	delete mTreeFolders;
	m_mgr.UnInit();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnClose(wxCloseEvent& WXUNUSED(evt))
{
	Hide();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnResize(wxSizeEvent& WXUNUSED(evt))
{
	int selectedPageIndex = mNotebookAssets->GetSelection();
	wxString path = "";
	wxString directoryName = "";

	for (int i = 0; i < (int)mNotebookAssets->GetPageCount(); ++i)
	{
		directoryName = mNotebookAssets->GetPageText(i);
		path = mNotebookAssets->GetPageToolTip(i);
		mNotebookAssets->InsertPage(i, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
		if (mNotebookAssets->GetPageBitmap(i+1).GetHeight() != 15) //Is page lock?
		{
			mNotebookAssets->SetPageBitmap(i, wxIcon(wxT("KL_ICON_Lock"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//locked
		}
		mNotebookAssets->SetPageToolTip(i, path);
		mNotebookAssets->RemovePage(i + 1); //remove the old page
	}
	mNotebookAssets->SetSelection(selectedPageIndex);
}
//*********************************************************************************************************************************************************************
wxListCtrl* AssetBrowser::CreateAssetBrowserNotebook(wxWindow* parent)
{
	wxListCtrl* listCtrl = new wxListCtrl(parent, ID_ListCrtItemsNotebook, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxBORDER_THEME | wxLC_SINGLE_SEL);

	

	listCtrl->SetBackgroundColour(wxColour(128, 128, 128, 255));
	listCtrl->SetTextColour(wxColour(255, 255, 255, 255));
	
	listCtrl->SetImageList(mImgListAssetBrowser, wxIMAGE_LIST_NORMAL);

	int index = 0;
	wxListItem* localItem = new wxListItem();

	//--------------------------------
	//Add to listCtrl a back directory
	//--------------------------------
	localItem->SetText("...");
	localItem->SetImage(1);
	localItem->SetId(index);
	localItem->SetData(1000);
	listCtrl->InsertItem(*localItem);
	++index;
	//------------------------------------------------------------------------------------------------------------------------------
	wxString dirAddress = wxGetCwd();
	

	Node* tempParentNode = NULL;
	if(mSelectedObjectItemTree != NULL)
		for (int i = 0; i < (int)mTreeFolders->size(); ++i)
		{
			if (mTreeFolders->at(i)->getTreeItemId() == mSelectedObjectItemTree)
			{
				tempParentNode = mTreeFolders->at(i);
				break;
			}
		}
	if (tempParentNode == NULL) //Selected the root
	{
		dirAddress << "\\data";
		mDirectory = dirAddress;
	}
	else
	{
		dirAddress = tempParentNode->getPathNameNode();
		mDirectory = dirAddress;
	}
	wxDir* dir = new wxDir(dirAddress);
	wxString nameFile = "";
	const wxString emptyString = "";
	wxArrayString* dirListDirectories = new wxArrayString(0);
	wxArrayString* dirListFiles = new wxArrayString(0);
	//----------------------------------------
	//Get directories
	//----------------------------------------
	bool isNextFile = dir->GetFirst(&nameFile, emptyString, wxDIR_DIRS);
	do
	{
		if (nameFile != "")
			dirListDirectories->Add(nameFile);
		isNextFile = dir->GetNext(&nameFile);
	} while (isNextFile);


	wxString formatFile = "";
	if(dirListDirectories->size() != 0)
		for (int i = 0; i < (int)dirListDirectories->size(); ++i)
		{
			//--------------------------------
			//Add to listCtrl directories
			//--------------------------------
			localItem->SetText(dirListDirectories->Item(i));
			localItem->SetImage(1);
			localItem->SetId(index);
			localItem->SetData(1001); //Flag of directory
			listCtrl->InsertItem(*localItem);
			++index;
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
	} while (isNextFile);

	if (dirListFiles->size() != 0)
		for (int i = 0; i < (int)dirListFiles->size(); ++i)
		{
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
			//Add to listCtrl files
			//--------------------------------
			if (formatFile == ".KLMESH" || formatFile == ".klmesh")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(0);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".PNG" || formatFile == ".png")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(2);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".DDS" || formatFile == ".dds")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(3);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".TGA" || formatFile == ".tga")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(4);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".JPG" || formatFile == ".jpg")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(5);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			++index;
		}

	//------------------------------------------------------------------------------------------------------------------------------


	return listCtrl;

}
//*********************************************************************************************************************************************************************
wxListCtrl* AssetBrowser::CreateAssetBrowserNotebookDirectory(wxWindow* parent, wxString directory)
{
	wxListCtrl* listCtrl = new wxListCtrl(parent, ID_ListCrtItemsNotebook, wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_SINGLE_SEL);

	listCtrl->SetBackgroundColour(wxColour(128, 128, 128, 255));
	listCtrl->SetTextColour(wxColour(255, 255, 255, 255));
	//wxImageList* imglist = new wxImageList(128, 128, true, 2);

	//imglist->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	//imglist->Add(wxIcon(wxT("KL_ICON_LargeDirectory"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	listCtrl->SetImageList(mImgListAssetBrowser, wxIMAGE_LIST_NORMAL);



	int index = 0;
	wxListItem* localItem = new wxListItem();



	//--------------------------------
	//Add to listCtrl a back directory
	//--------------------------------
	localItem->SetText("...");
	localItem->SetImage(1);
	localItem->SetId(index);
	localItem->SetData(1000);
	localItem->SetColumn(0);
	listCtrl->InsertItem(*localItem);
	++index;

	//------------------------------------------------------------------------------------------------------------------------------
	wxString dirAddress = directory;



	wxDir* dir = new wxDir(dirAddress);
	wxString nameFile = "";
	const wxString emptyString = "";
	wxArrayString* dirListDirectories = new wxArrayString(0);
	wxArrayString* dirListFiles = new wxArrayString(0);


	//----------------------------------------
	//Get directories
	//----------------------------------------
	bool isNextFile = dir->GetFirst(&nameFile, emptyString, wxDIR_DIRS);
	do
	{
		if (nameFile != "")
			dirListDirectories->Add(nameFile);
		isNextFile = dir->GetNext(&nameFile);
	} while (isNextFile);

	if (dirListDirectories->size() != 0)
		for (int i = 0; i < (int)dirListDirectories->size(); ++i)
		{
			//--------------------------------
			//Add to listCtrl directories
			//--------------------------------
			localItem->SetText(dirListDirectories->Item(i));
			localItem->SetImage(1);
			localItem->SetId(index);
			localItem->SetData(1001); //Flag of directory
			listCtrl->InsertItem(*localItem);
			++index;
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
	} while (isNextFile);

	wxString formatFile = "";

	if (dirListFiles->size() != 0)
		for (int i = 0; i < (int)dirListFiles->size(); ++i)
		{
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
			//Add to listCtrl files
			//--------------------------------
			if (formatFile == ".KLMESH" || formatFile == ".klmesh")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(0);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".PNG" || formatFile == ".png")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(2);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".DDS" || formatFile == ".dds")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(3);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".TGA" || formatFile == ".tga")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(4);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			else if (formatFile == ".JPG" || formatFile == ".jpg")
			{
				localItem->SetText(dirListFiles->Item(i));
				localItem->SetImage(5);
				localItem->SetId(index);
				localItem->SetData(1002); //Flag of KLMESH file format
				listCtrl->InsertItem(*localItem);
			}
			++index;
		}

	//------------------------------------------------------------------------------------------------------------------------------





	return listCtrl;

}
//*********************************************************************************************************************************************************************
wxTreeCtrl* AssetBrowser::CreateTreeCtrl()
{
	mTreeFiles = new wxTreeCtrl(this, ID_TreeFiles, wxPoint(0, 0), wxSize(160, 250), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTR_MULTIPLE);
	//----------
	//List of images
	//----------
	mImgList = new wxImageList(16, 16, true, 2);
	mImgList->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mImgList->Add(wxIcon(wxT("KL_ICON_Folder"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mTreeFiles->AssignImageList(mImgList); //Assign images to tree
	
	mRoot = mTreeFiles->AddRoot(wxT("Depot"), 0, 0);

	//---------------------------------------------------------------------
	//Scan directories
	//---------------------------------------------------------------------
	OnRefreshDictionary();

	//-----------
	// Expand
	//-----------

	mTreeFiles->Expand(mRoot);

	return mTreeFiles;
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{
	if (mNotebookAssets->GetPageBitmap(evt.GetSelection()).GetHeight() == 15) //Can we close this page?
	{
		if (mNotebookAssets->GetPageCount() == 1)
		{
			wxMessageBox("Cannot close a last page!", "Warning", wxICON_QUESTION | wxOK);
			mNotebookAssets->AddPage(CreateAssetBrowserNotebook(mNotebookAssets), wxT("Depot"), true, mPage_bmp);
			mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, wxGetCwd() + "\\data");
			mNotebookAssets->RemovePage(0);
			RefreshAllNotebooks();
			evt.Veto();
		}
	}
	else
	{
		evt.Veto();
	}	
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeClick(wxTreeEvent& evt)
{
	mSelectedObjectItemTree = evt.GetItem();

	Node* tempParentNode = NULL;
	int pagePosition = -1;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		if (mTreeFolders->at(i)->getTreeItemId() == mSelectedObjectItemTree)
		{
			tempParentNode = mTreeFolders->at(i);
			break;
		}
	}

	if (tempParentNode == NULL) //Root
	{
	}
	wxString namePage = mTreeFiles->GetItemText(evt.GetItem());
	if (mDirectory == (wxGetCwd() + "\\data") && namePage == "data")
	{
		namePage = "Depot";
	}
	pagePosition = mNotebookAssets->GetSelection();
	if (mNotebookAssets->GetPageBitmap(pagePosition).GetHeight() == 15)//Insert and remove unlocked page
	{
		mNotebookAssets->InsertPage(pagePosition, CreateAssetBrowserNotebook(mNotebookAssets), namePage, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(pagePosition, mDirectory);
		mNotebookAssets->RemovePage(pagePosition + 1);
	}
	else //Add page without delete locked page
	{
		mNotebookAssets->AddPage(CreateAssetBrowserNotebook(mNotebookAssets), namePage, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectory);
	}
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnRefreshDictionary()
{
	//Clean all entities in the 'Folder' array
	mFolder.Clear();
	mTreeFiles->DeleteAllItems();
	mRoot = mTreeFiles->AddRoot(wxT("Depot"), 0, 0);

	//---------------------------------------------------------------------
	//Scan directories
	//---------------------------------------------------------------------

	//-----------
	// Create DirectonTree
	//-----------
	mTreeFolders = new std::vector<Node*>();
	Node* rootNode = new Node(NULL, "data", mTreeFolders, 0);

	
	//-----------
	// Set levels of nodes
	//-----------
	int level = 0;
	Node* iterNode = NULL;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		level = 0;
		iterNode = mTreeFolders->at(i);
		while (iterNode->getParent() != NULL)
		{
			iterNode = iterNode->getParent();
			++level;
		}
		mTreeFolders->at(i)->setLevel(level);
	}

	//-----------
	//Create tree - ROOT
	//-----------

	//StateItemList
	// -1 - Root
	// x e N - mFolder
	mTreeFiles->SetItemState(mRoot, -1); //set ROOT state (-1)

	//-----------
	// Set nodes level
	//-----------
	int maxLevel = 0;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
		if (maxLevel < mTreeFolders->at(i)->getLevel())
			maxLevel = mTreeFolders->at(i)->getLevel();

	//------------------------------------
	// Detect and create a dictionary tree
	//------------------------------------
	std::vector<std::vector<Node*>*>* vectorOfVectorsFolders = new std::vector<std::vector<Node*>*>();

	//-----------
	// Add empty nodes to 'vectorOfVectorsFolders'
	//-----------
	for (int i = 0; i <= maxLevel; ++i)
	{
		std::vector<Node*>* tempVector = new std::vector<Node*>();
		vectorOfVectorsFolders->push_back(tempVector); //Lenght == maxLevel
	}

	//-----------
	// Set fields of 'vectorOfVectorsFolders'
	//-----------
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		vectorOfVectorsFolders->at(mTreeFolders->at(i)->getLevel())->push_back(mTreeFolders->at(i));
	}

	//-----------
	//Set and add first level (level 1)
	//-----------
	wxTreeItemId treeItemIdTemp = NULL;
	for (int k = 0; k < (int)vectorOfVectorsFolders->at(1)->size(); ++k)
	{
		treeItemIdTemp = mTreeFiles->AppendItem(mRoot, vectorOfVectorsFolders->at(1)->at(k)->getNameNode(), 1);
		vectorOfVectorsFolders->at(1)->at(k)->setTreeItemId(treeItemIdTemp);
		mFolder.Add(treeItemIdTemp);
		vectorOfVectorsFolders->at(1)->at(k)->setArrayFolderPosition((int)mFolder.size() - 1); //set ArrayFolderPosition for node
	}

	//-----------
	//Set ArrayFoldersPosition - another levels
	//-----------
	for (int i = 2; i < (int)vectorOfVectorsFolders->size(); ++i)//Lenght == maxLevel but without 2 first levels (root and level 1) 
	{
		for (int k = 0; k < (int)vectorOfVectorsFolders->at(i)->size(); ++k)
		{
			vectorOfVectorsFolders->at(i)->at(k)->setArrayFolderPosition((int)mFolder.size()); //set ArrayFolderPosition for node
			treeItemIdTemp = mTreeFiles->AppendItem(mFolder[vectorOfVectorsFolders->at(i)->at(k)->getParent()->getArrayFolderPosition()], vectorOfVectorsFolders->at(i)->at(k)->getNameNode(), 1);
			vectorOfVectorsFolders->at(i)->at(k)->setTreeItemId(treeItemIdTemp);
			mFolder.Add(treeItemIdTemp);	
		}
	}

	//-----------
	// Set States
	//-----------
	for (int i = 0; i < (int)mFolder.size(); ++i)
	{
		mTreeFiles->SetItemState(mFolder[i], 1); //set ids
	}

	//-----------
	// Expand
	//-----------
	mTreeFiles->Expand(mRoot);
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeFileOpenDirectory(wxCommandEvent& WXUNUSED(evt))
{
	//mSelectedObjectItemTree -- can not be NULL
	Node* tempParentNode = NULL;
	int pagePosition = -1;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		if (mTreeFolders->at(i)->getTreeItemId() == mSelectedObjectItemTree)
		{
			tempParentNode = mTreeFolders->at(i);
			break;
		}
	}

	if (tempParentNode == NULL) //Root
	{
	}
	wxString namePage = mTreeFiles->GetItemText(mSelectedObjectItemTree);
	if (mDirectory == (wxGetCwd() + "\\data") && namePage == "data")
	{
		namePage = "Depot";
	}
	pagePosition = mNotebookAssets->GetSelection();
	if (mNotebookAssets->GetPageBitmap(pagePosition).GetHeight() == 15)//Insert and remove unlocked page
	{
		mNotebookAssets->InsertPage(pagePosition, CreateAssetBrowserNotebook(mNotebookAssets), namePage, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(pagePosition, mDirectory);
		mNotebookAssets->RemovePage(pagePosition + 1);
	}
	else //Add page without delete locked page
	{
		mNotebookAssets->AddPage(CreateAssetBrowserNotebook(mNotebookAssets), namePage, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectory);
	}
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeFileOpenDirectoryInNewTab(wxCommandEvent& WXUNUSED(evt))
{
	//mSelectedObjectItemTree -- can not be NULL
	Node* tempParentNode = NULL;
	int pagePosition = -1;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		if (mTreeFolders->at(i)->getTreeItemId() == mSelectedObjectItemTree)
		{
			tempParentNode = mTreeFolders->at(i);
			break;
		}
	}
	if (tempParentNode == NULL) //Root
	{
	}
	wxString namePage = mTreeFiles->GetItemText(mSelectedObjectItemTree);
	if (mDirectory == (wxGetCwd() + "\\data") && namePage == "data")
	{
		namePage = "Depot";
	}
	pagePosition = mNotebookAssets->GetSelection();
	mNotebookAssets->AddPage(CreateAssetBrowserNotebook(mNotebookAssets), namePage, true, mPage_bmp);
	mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectory);
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeRightClick(wxTreeEvent& evt)
{
	wxMenu menu(wxT(""));
	wxPoint point = evt.GetPoint();
	point.y = point.y + 20;

	mSelectedObjectItemTree = evt.GetItem();

	if (mTreeFiles->GetItemState(evt.GetItem()) == -1)// Root
	{
		menu.Append(ID_TreeFilesMenuRefresh, wxT("Refresh"));
		menu.AppendSeparator();
		menu.Append(ID_TreeFilesMenuOpen, wxT("Open"));
		menu.Append(ID_TreeFilesMenuOpenInNewTab, wxT("Open in new tab"));
		menu.AppendSeparator();
		menu.Append(ID_TreeFilesMenuNewFolderRoot, wxT("Add new directory"));
		PopupMenu(&menu, point);
	}
	else //if (mTreeFiles->GetItemState(evt.GetItem()) == 1) //folder
	{
		menu.Append(ID_TreeFilesMenuOpen, wxT("Open"));
		menu.Append(ID_TreeFilesMenuOpenInNewTab, wxT("Open in new tab"));
		menu.AppendSeparator();
		menu.Append(ID_TreeFilesMenuNewFolder, wxT("Add new directory"));
		menu.AppendSeparator();
		menu.Append(ID_TreeFilesMenuRemove, wxT("Remove"));
		PopupMenu(&menu, point);
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeFileRefresh(wxCommandEvent& WXUNUSED(evt))
{
	OnRefreshDictionary();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeFileCreateFolder(wxCommandEvent& WXUNUSED(evt))
{
	Node* tempParentNode = NULL;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		if (mTreeFolders->at(i)->getTreeItemId() == mSelectedObjectItemTree)
		{
			tempParentNode = mTreeFolders->at(i);
			break;
		}
	}
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxTextEntryDialog dialog(this, wxT("Enter name of new directory:"), wxT("Add directory"), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

	if (dialog.ShowModal() == wxID_OK) //Press ok button
	{
		if (dialog.GetValue().size() != 0) //Name is not empty
		{
			wxDir* dir = new wxDir("data");
			wxString folderName = tempParentNode->getPathNameNode();
			folderName << "\\" << dialog.GetValue();

			if (dir->Make(folderName, wxS_DIR_DEFAULT, 0))
			{
				OnRefreshDictionary();
			}
			else
			{
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("This folder exists!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}
			dir->Close();
		}
		else  //Name IS empty
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
		}
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeFileCreateFolderRoot(wxCommandEvent& WXUNUSED(evt))
{
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxTextEntryDialog dialog(this, wxT("Enter name of new directory:"), wxT("Add directory"), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

	if (dialog.ShowModal() == wxID_OK) //Press ok button
	{
		if (dialog.GetValue().size() != 0) //Name is not empty
		{
			wxDir* dir = new wxDir("data");
			wxString folderName = "data";
			folderName << "\\" << dialog.GetValue();

			if (dir->Make(folderName, wxS_DIR_DEFAULT, 0))
			{
				OnRefreshDictionary();
			}
			else
			{
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("This folder exists!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}
			dir->Close();
		}
		else  //Name IS empty
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
		}
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnTreeFileRemoveFolder(wxCommandEvent& WXUNUSED(evt))
{
	Node* tempParentNode = NULL;
	for (int i = 0; i < (int)mTreeFolders->size(); ++i)
	{
		if (mTreeFolders->at(i)->getTreeItemId() == mSelectedObjectItemTree)
		{
			tempParentNode = mTreeFolders->at(i);
			break;
		}
	}
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;

	wxMessageDialog *dialog = new wxMessageDialog(NULL, wxT("Are you sure you want to delete this directory and delete all its contents file?"), wxT("Delete?"), wxOK | wxCANCEL | wxICON_QUESTION, wxPoint(width, height));
	if (dialog->ShowModal() == wxID_OK) //Press ok button
	{
		//wxDir* dir = new wxDir("data");

	//	if (dir->Remove(tempParentNode->getPathNameNode(), wxS_DIR_DEFAULT))
		if (DeleteDirectory(tempParentNode->getPathNameNode()))
		{
			OnRefreshDictionary();
		}
		else
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Cannot delete this directory!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
		}
		//dir->Close();
	}
	dialog->Destroy();
}
//*********************************************************************************************************************************************************************
bool AssetBrowser::DeleteDirectory(wxString directoryToRemoveName) 
{
	//directoryToRemoveName = "C:\\Users\\Kamil\\Desktop\\kk - kopia";
	/*
	wxDir* dir = new wxDir(directoryToRemoveName);
	if (dir == NULL) 
		return false;

	wxString filename;
	bool isNextFolder = dir->GetFirst(&filename);

	// if there are files to process
	if (isNextFolder)
	{
		do 
		{
			// if the next filename is actually a directory
			if (wxDirExists(directoryToRemoveName + "\\" + filename))
			{
				// delete this directory
				DeleteDirectory(directoryToRemoveName + "\\" + filename);
			}
			else 
			{
				// delete files
				if (!wxRemoveFile(directoryToRemoveName + "\\" + filename))
				{
					//wxLogError("Could not remove file \"" + rmDir + filename + "\"");
				}
			}
		}
		while (dir->GetNext(&filename));
	}

	

	// now actually try to delete it
	//if (!wxFileName::Rmdir(directoryToRemoveName, wxPATH_RMDIR_FULL))
	if(!dir->Remove(directoryToRemoveName, wxS_DIR_DEFAULT))
	{
		//wxLogError("Could not remove directory " + rmDir);
		return false;
	}
	else 
	{
		dir->Close();
		delete dir;
		return true;
	}
	*/
	return false;
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserRefresh(wxCommandEvent& WXUNUSED(evt))
{
	OnRefreshDictionary();
	RefreshAllNotebooks();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnDragAndDropItem(wxListEvent& evt)
{
	wxListItem item = evt.GetItem();
	if (item.GetData() == 1002)//items KLMESH
	{
		wxString message = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		//if (evt.GetText().at(evt.GetText().size() - 1) == 'h' || evt.GetText().at(evt.GetText().size() - 1) == 'H')
		//{
			message << "\\" << evt.GetText();
			wxTextDataObject my_data(message);
			wxDropSource dragSource(my_data, this, wxDROP_ICON(dnd_copy));
			//dragSource.SetData(my_data);

			wxDragResult result = dragSource.DoDragDrop(true);
	//	}

		evt.Allow();
	}
	else
	{
		evt.Skip();
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnListCrtDeleteItem(wxListEvent& WXUNUSED(evt))
{
	wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Delete item"), wxT("INFO"), wxOK | wxICON_INFORMATION);
	errorMsg->ShowModal();
	errorMsg->Destroy();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnListCrtActivatedItem(wxListEvent& evt)  
{
	wxString directoryName = "";
	wxString path = "";
	int pagePosition = -1;
	
	//Go back directory
	if (evt.GetData() == 1000)
	{
		path = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		if (path != wxGetCwd() + "\\data")
		{
			short position = 0;
			wxString pathTemp = "";
			for (int i = path.size(); i != 0; --i)
			{
				if (path.at(i - 1) == '\\') //previous directory
				{
					position = i;
					break;
				}
			}
			for (int i = 0; i != position - 1; ++i)
			{
				pathTemp << path.at(i);
			}

			//Detect a back directory
			for (int i = position - 1; i != 0; --i)
			{
				if (path.at(i - 1) == '\\') //previous directory
				{
					position = i;
					break;
				}
			}
			for (int i = position; i <= (int)path.size() - 1; ++i)
			{
				if (path.at(i) != '\\')
					directoryName << path.at(i);
				else
					break;
			}

			//Set values
			path = pathTemp;
			pagePosition = mNotebookAssets->GetSelection();

			//Rename home directory name
			if (path == (wxGetCwd() + "\\data") && directoryName == "data")
			{
				directoryName = "Depot";
			}

			if (mNotebookAssets->GetPageBitmap(pagePosition).GetHeight() == 15)//Insert and remove unlocked page
			{
				mNotebookAssets->InsertPage(pagePosition, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
				mNotebookAssets->SetPageToolTip(pagePosition, path);
				mNotebookAssets->RemovePage(pagePosition + 1);
			}
			else //Add page without delete locked page
			{
				mNotebookAssets->AddPage(CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
				mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, path);
			}
			Refresh();
		}
	}
	//Directory
	if (evt.GetData() == 1001)
	{
		directoryName = evt.GetText();
		pagePosition = mNotebookAssets->GetSelection();
		path = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		path << "\\" << directoryName;
		if (mNotebookAssets->GetPageBitmap(pagePosition).GetHeight() == 15)//Insert and remove unlocked page
		{
			mNotebookAssets->InsertPage(pagePosition, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
			mNotebookAssets->SetPageToolTip(pagePosition, path);
			mNotebookAssets->RemovePage(pagePosition + 1);
		}
		else //Add page without delete locked page
		{
			mNotebookAssets->AddPage(CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
			mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, path);
		}
		Refresh();
	}
	//KLMESH
	if (evt.GetData() == 1002)
	{
		directoryName = evt.GetText();
		/*errorMsg = new wxMessageDialog(NULL, wxT("Activated item - KLMESH: " + directoryName), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
		*/


		wxString formatFile = "";
		for (int k = 0; k < (int)directoryName.size(); ++k)
		{
			if (directoryName.at(k) == '.')
			{
				formatFile = "";
				for (int j = k; j < (int)directoryName.size(); ++j)
				{
					formatFile << directoryName.at(j);
				}
				break;
			}
		}
		if (formatFile == ".KLMESH" || formatFile == ".klmesh")
		{
			int width = wxSystemSettings::GetMetric(wxSYS_SCREEN_X) + 20;
			int height = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) - 30;
			path = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
			path << "\\" << directoryName;


			wxString folder = wxGetCwd();
			wxString tempString = "";

			for (int k = folder.size() + 1; k < path.size(); ++k)
				tempString << path.at(k);


			path = tempString;

			bool existFrame = false;
			for (int k = 0; k < (int)mListOfKLMESHViewer->size(); ++k)
			{
				if (mListOfKLMESHViewer->at(k)->GetTitle() == ("KLMESH: " + path))
					if (mListOfKLMESHViewer->at(k)->IsShown())
					{
						existFrame = true;
						mListOfKLMESHViewer->at(k)->SetFocus();
						if (mListOfKLMESHViewer->at(k)->IsIconized())
							mListOfKLMESHViewer->at(k)->Iconize(false);
					}
			}

			if (existFrame == false)
			{
				KLMESHViewer* KLMeshViewer = new KLMESHViewer(NULL, wxID_ANY, wxT("KLMESH: " + path), wxDefaultPosition, wxSize(width, height));
				mListOfKLMESHViewer->push_back(KLMeshViewer);
				bool exist = mKLMESH_Buffer->CheckMesh((std::string)path);

				//Set values 
				KLMeshViewer->SetTexturesResource(mTexturesResource);
				KLMeshViewer->SetDevice(mDevice);
				KLMeshViewer->CreateKLMESH();
				//Show frame
				KLMeshViewer->Show();

				//Set exist klmesh 
				if (exist == true)
				{
					mKLMESH_Buffer->GetMeshByLink((std::string)path)->SetIsAllowRender(false); //render stop
					KLMeshViewer->SetKLMESH(mKLMESH_Buffer->GetMeshByLink((std::string)path)); //Set KLMESH (as pointer) to KLMESHviewer
					mKLMESH_Buffer->GetMeshByLink((std::string)path)->SetIsAllowRender(true); //start render
				}
			}

				
		//	KLMeshViewer->SetLinkToTexture()
		}
		else if (formatFile == ".PNG" || formatFile == ".png" || formatFile == ".TGA" || formatFile == ".tga" || formatFile == ".JPG" || formatFile == ".jpg" || formatFile == ".DDS" || formatFile == ".dds")
		{
			path = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
			path << "\\" << directoryName;
			ImageViewer* imageViewer = new ImageViewer(this, wxID_ANY, wxT("ImageViewer: " + path), wxDefaultPosition, wxSize(512, 512), wxMINIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN);
			mListOfImageViewer->push_back(imageViewer);
			imageViewer->Show();
		}
	/*	else if (formatFile == ".DDS" || formatFile == ".dds")
		{

		}*/
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnListCrtRightClickItem(wxListEvent& evt)
{
	
	wxMenu menu(wxT(""));
	wxPoint point = evt.GetPoint();
	point.x = point.x + 180;
	point.y = point.y + 50;

	wxListItem item = evt.GetItem();

	mDirectoryPathAssetBrowser = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
	mDirectoryNameAssetBrowser = evt.GetText();
	
	if (item.GetData() == 1002)//items KLMESH
	{
		menu.Append(ID_AssetBrowserItemOpen, wxT("Open"));
		menu.AppendSeparator();
		menu.Append(ID_AssetBrowserItemLoad, wxT("Load from disk"));
		menu.Append(ID_AssetBrowserItemReload, wxT("Reload"));
		//menu.Append(ID_AssetBrowserItemRefresh, wxT("Refresh"));
		menu.AppendSeparator();
		menu.Append(ID_AssetBrowserItemCopy, wxT("Copy"));
		menu.Append(ID_AssetBrowserItemCut, wxT("Cut"));
		menu.Append(ID_AssetBrowserItemDelete, wxT("Delete"));
		menu.AppendSeparator();
		menu.Append(ID_AssetBrowserItemRename, wxT("Rename"));
		PopupMenu(&menu, point);
	}
	else if (item.GetData() == 1001) //directory
	{
		menu.Append(ID_AssetBrowserDirectoryOpen, wxT("Open directory"));
		menu.Append(ID_AssetBrowserDirectoryOpenInNewTab, wxT("Open directory in new tab"));
		menu.AppendSeparator();
		menu.Append(ID_AssetBrowserDirectoryDelete, wxT("Delete directory"));
		menu.Append(ID_AssetBrowserDirectoryRename, wxT("Rename directory"));
		PopupMenu(&menu, point);
	}
	else if (item.GetData() == 1000) //directory
	{
		wxString directoryName = "";
		wxString path = "";
		int pagePosition = -1;

		path = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		if (path != wxGetCwd() + "\\data")
		{
			short position = 0;
			wxString pathTemp = "";
			for (int i = path.size(); i != 0; --i)
			{
				if (path.at(i - 1) == '\\') //previous directory
				{
					position = i;
					break;
				}
			}
			for (int i = 0; i != position - 1; ++i)
			{
				pathTemp << path.at(i);
			}

			//Detect a back directory
			for (int i = position - 1; i != 0; --i)
			{
				if (path.at(i - 1) == '\\') //previous directory
				{
					position = i;
					break;
				}
			}
			for (int i = position; i <= (int)path.size() - 1; ++i)
			{
				if (path.at(i) != '\\')
					directoryName << path.at(i);
				else
					break;
			}

			//Set values
			path = pathTemp;
			pagePosition = mNotebookAssets->GetSelection();

			//Rename home directory name
			if (path == (wxGetCwd() + "\\data") && directoryName == "data")
			{
				directoryName = "Depot";
			}

			mDirectoryPathAssetBrowser = path;
			mDirectoryNameAssetBrowser = directoryName;

			menu.Append(ID_AssetBrowserDirectoryOpenBack, wxT("Open directory"));
			menu.Append(ID_AssetBrowserDirectoryOpenBackInNewTab, wxT("Open directory in new tab"));
			PopupMenu(&menu, point);
		}

	}
	mIsItemSelectedDirectory = true;
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnListCrtFocusedItem(wxListEvent& evt)
{
	mIsItemSelected = false;
	mIsItemSelectedDirectory = false;
	evt.Allow();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnNotebookPageRightClick(wxAuiNotebookEvent& evt)
{
	mSelectedNotebookPage = evt.GetSelection();
	
	wxMenu menu(wxT(""));
	wxPoint point = wxGetMousePosition();

	const wxPoint pt = wxGetMousePosition();
	point.x = pt.x - this->GetScreenPosition().x;
	point.y = pt.y - this->GetScreenPosition().y - 50;

	if (mNotebookAssets->GetPageBitmap(evt.GetSelection()).GetHeight() == 15) //Is page lock?
	{
		menu.Append(ID_NotebookPageLock, wxT("Lock"));
		menu.AppendSeparator();
		menu.Append(ID_NotebookPageDuplicate, wxT("Duplicate this tab"));
		menu.AppendSeparator();
		menu.Append(ID_NotebookPageClose, wxT("Close this tab"));
	}
	else
	{
		menu.Append(ID_NotebookPageLock, wxT("Unlock"));
		menu.AppendSeparator();
		menu.Append(ID_NotebookPageDuplicate, wxT("Duplicate this tab"));
	}
		
	PopupMenu(&menu, point);

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnNotebookPageRightClickLock(wxCommandEvent& WXUNUSED(evt))
{
	if (mNotebookAssets->GetPageBitmap(mSelectedNotebookPage).GetHeight() == 15) //Is page lock?
	{
		mNotebookAssets->SetPageBitmap(mSelectedNotebookPage, wxIcon(wxT("KL_ICON_Lock"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//locked
	}
	else
	{
		mNotebookAssets->SetPageBitmap(mSelectedNotebookPage, mPage_bmp);//unlocked
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnNotebookPageRightClickDuplicate(wxCommandEvent& WXUNUSED(evt))
{
	int selectedPageIndex = mNotebookAssets->GetSelection();
	wxString path = mNotebookAssets->GetPageToolTip(mSelectedNotebookPage);
	wxString directoryName = mNotebookAssets->GetPageText(mSelectedNotebookPage);
	mNotebookAssets->InsertPage(mSelectedNotebookPage + 1, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
	mNotebookAssets->SetPageToolTip(mSelectedNotebookPage + 1, path);
	mNotebookAssets->SetSelection(selectedPageIndex);
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnNotebookPageRightClickClose(wxCommandEvent& WXUNUSED(evt))
{
	//mSelectedNotebookPage = evt.GetSelection();
	if (mNotebookAssets->GetPageBitmap(mSelectedNotebookPage).GetHeight() == 15) //Is page lock?
	{
		if (mNotebookAssets->GetPageCount() == 1)
		{
			wxMessageBox("Cannot close a last page!", "Warning", wxICON_QUESTION | wxOK);
			
			mNotebookAssets->AddPage(CreateAssetBrowserNotebook(mNotebookAssets), wxT("Depot"), true, mPage_bmp);
			mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, wxGetCwd() + "\\data");
			//mNotebookAssets->SetSelection(1);
			mNotebookAssets->RemovePage(mSelectedNotebookPage);
			RefreshAllNotebooks();
		}
		else
		{
			if(mSelectedNotebookPage > 0)
				mNotebookAssets->SetSelection(mSelectedNotebookPage - 1);
			else
				mNotebookAssets->SetSelection(1);
			mNotebookAssets->RemovePage(mSelectedNotebookPage);
		}
	}

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuViewGridSmall(wxCommandEvent& WXUNUSED(evt))
{
	mImgListAssetBrowser = new wxImageList(16, 16, true, 2);
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_LargeDirectory"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_PNG"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_DDS"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_TGA"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_JPG"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));

	int selectedPageIndex = mNotebookAssets->GetSelection();
	wxString path = "";
	wxString directoryName = "";

	for (int i = 0; i < (int)mNotebookAssets->GetPageCount(); ++i)
	{
		directoryName = mNotebookAssets->GetPageText(i);
		path = mNotebookAssets->GetPageToolTip(i);
		mNotebookAssets->InsertPage(i, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
		if (mNotebookAssets->GetPageBitmap(i + 1).GetHeight() != 15) //Is page lock?
		{
			mNotebookAssets->SetPageBitmap(i, wxIcon(wxT("KL_ICON_Lock"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//locked
		}
		mNotebookAssets->SetPageToolTip(i, path);
		mNotebookAssets->RemovePage(i + 1); //remove the old page
	}
	mNotebookAssets->SetSelection(selectedPageIndex);
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuViewGridMedium(wxCommandEvent& WXUNUSED(evt))
{
	mImgListAssetBrowser = new wxImageList(64, 64, true, 2);
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_LargeDirectory"), wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_PNG"), wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_DDS"), wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_TGA"), wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_JPG"), wxBITMAP_TYPE_ICO_RESOURCE, 64, 64));

	int selectedPageIndex = mNotebookAssets->GetSelection();
	wxString path = "";
	wxString directoryName = "";

	for (int i = 0; i < (int)mNotebookAssets->GetPageCount(); ++i)
	{
		directoryName = mNotebookAssets->GetPageText(i);
		path = mNotebookAssets->GetPageToolTip(i);
		mNotebookAssets->InsertPage(i, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
		if (mNotebookAssets->GetPageBitmap(i + 1).GetHeight() != 15) //Is page lock?
		{
			mNotebookAssets->SetPageBitmap(i, wxIcon(wxT("KL_ICON_Lock"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//locked
		}
		mNotebookAssets->SetPageToolTip(i, path);
		mNotebookAssets->RemovePage(i + 1); //remove the old page
	}
	mNotebookAssets->SetSelection(selectedPageIndex);
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuViewGridHuge(wxCommandEvent& WXUNUSED(evt))
{
	mImgListAssetBrowser = new wxImageList(128, 128, true, 2);
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_Logo"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_LargeDirectory"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_PNG"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_DDS"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_TGA"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));
	mImgListAssetBrowser->Add(wxIcon(wxT("KL_ICON_JPG"), wxBITMAP_TYPE_ICO_RESOURCE, 128, 128));

	int selectedPageIndex = mNotebookAssets->GetSelection();
	wxString path = "";
	wxString directoryName = "";

	for (int i = 0; i < (int)mNotebookAssets->GetPageCount(); ++i)
	{
		directoryName = mNotebookAssets->GetPageText(i);
		path = mNotebookAssets->GetPageToolTip(i);
		mNotebookAssets->InsertPage(i, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
		if (mNotebookAssets->GetPageBitmap(i + 1).GetHeight() != 15) //Is page lock?
		{
			mNotebookAssets->SetPageBitmap(i, wxIcon(wxT("KL_ICON_Lock"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//locked
		}
		mNotebookAssets->SetPageToolTip(i, path);
		mNotebookAssets->RemovePage(i + 1); //remove the old page
	}
	mNotebookAssets->SetSelection(selectedPageIndex);
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuClose(wxCommandEvent& WXUNUSED(evt))
{
	Hide();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemKLMESHOpen(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemKLMESHLoadFromDisk(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemKLMESHReload(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemCopy(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemCut(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemDelete(wxCommandEvent& WXUNUSED(evt))
{
	mDirectoryPathAssetBrowser << "\\" << mDirectoryNameAssetBrowser;
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;

	wxMessageDialog *dialog = new wxMessageDialog(NULL, wxT("Are you sure you want to delete this file?"), wxT("Delete?"), wxOK | wxCANCEL | wxICON_QUESTION, wxPoint(width, height));
	if (dialog->ShowModal() == wxID_OK) //Press ok button
	{
		if (wxRemoveFile(mDirectoryPathAssetBrowser))
		{
			OnRefreshDictionary();
			RefreshAllNotebooks();
		}
		else
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Cannot delete this file!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
		}
	}
	dialog->Destroy();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnItemRename(wxCommandEvent& WXUNUSED(evt))
{
	wxString newFilePath = "";
	wxString oldFilePath = "";
	wxString formatFile = "";
	oldFilePath = mDirectoryPathAssetBrowser << "\\" << mDirectoryNameAssetBrowser;
	
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;

	wxTextEntryDialog dialog(this, wxT("Enter new name:"), wxT("Rename..."), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

	newFilePath = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
	oldFilePath = mSelectedFilePath;
	oldFilePath << "\\" << mSelectedFileName;

	wxString fileName = "";
	for (int i = 0; i < (int)mSelectedFileName.size(); ++i)
	{
		if (mSelectedFileName.at(i) == '.')
		{
			formatFile = "";
			for (int j = i; j < (int)mSelectedFileName.size(); ++j)
			{
				formatFile << mSelectedFileName.at(j);
			}
			break;
		}
		else
		{
			fileName << mSelectedFileName.at(i);
		}
	}

	dialog.SetValue(fileName);
	if (dialog.ShowModal() == wxID_OK) //Press ok button
	{
		if (dialog.GetValue().size() != 0) //Name is not empty
		{
			if (wxFileExists(oldFilePath)) //if file still exists.
			{
				newFilePath << "\\" << dialog.GetValue();
				newFilePath << formatFile;
				if (wxRenameFile(oldFilePath, newFilePath, false))
				{
					//Refresh all notebooks
					RefreshAllNotebooks();
				}
				else
				{
					wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
					errorMsg->ShowModal();
					errorMsg->Destroy();
				}
			}
		}
		else  //Name IS empty
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
		}
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserDirectoryOpen(wxCommandEvent& WXUNUSED(evt))
{

	int pagePosition = mNotebookAssets->GetSelection();


	 mDirectoryPathAssetBrowser << "\\" << mDirectoryNameAssetBrowser;
	if (mNotebookAssets->GetPageBitmap(pagePosition).GetHeight() == 15)//Insert and remove unlocked page
	{
		mNotebookAssets->InsertPage(pagePosition, CreateAssetBrowserNotebookDirectory(mNotebookAssets, mDirectoryPathAssetBrowser), mDirectoryNameAssetBrowser, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(pagePosition, mDirectoryPathAssetBrowser);
		mNotebookAssets->RemovePage(pagePosition + 1);
	}
	else //Add page without delete locked page
	{
		mNotebookAssets->AddPage(CreateAssetBrowserNotebookDirectory(mNotebookAssets, mDirectoryPathAssetBrowser), mDirectoryNameAssetBrowser, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectoryPathAssetBrowser);
	}
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserDirectoryOpenInNewTab(wxCommandEvent&  WXUNUSED(evt))
{
	mDirectoryPathAssetBrowser << "\\" << mDirectoryNameAssetBrowser;
	mNotebookAssets->AddPage(CreateAssetBrowserNotebookDirectory(mNotebookAssets, mDirectoryPathAssetBrowser), mDirectoryNameAssetBrowser, true, mPage_bmp);
	mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectoryPathAssetBrowser);
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserDirectoryDelete(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserDirectoryRename(wxCommandEvent& WXUNUSED(evt))
{

}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuEditCopy(wxCommandEvent& WXUNUSED(evt))
{
	if (mIsItemSelected)
	{
		mCopyBufferPath = mSelectedFilePath;
		mCopyBufferName = mSelectedFileName;
		mIsCopiedFile = true;
		mIsCutFile = false;
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuEditCut(wxCommandEvent& WXUNUSED(evt))
{
	if (mIsItemSelected)
	{
		mCopyBufferPath = mSelectedFilePath;
		mCopyBufferName = mSelectedFileName;
		mIsCopiedFile = true;
		mIsCutFile = true;
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuEditPaste(wxCommandEvent& WXUNUSED(evt))
{
	wxString newFilePath = "";
	wxString oldFilePath = "";
	int iter = 0;
	wxString temp = "";
	wxString formatFile = "";
	if (mIsCopiedFile)
	{
		newFilePath = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		newFilePath << "\\" << mCopyBufferName;
		oldFilePath << mCopyBufferPath;
		oldFilePath  << "\\" << mCopyBufferName;
		//oldFilePath = mCopyBufferPath;//new way of copy file
		if (wxFileExists(oldFilePath)) //if file to copy still exists.
		{
			while (wxFileExists(newFilePath)) //While file not exists in this directory	
			{
				temp = "";
				for (int i = 0; i < (int)mCopyBufferName.size(); ++i)
				{
					if (mCopyBufferName.at(i) == '.')
					{
						formatFile = "";
						for (int j = i; j < (int)mCopyBufferName.size(); ++j)
						{
							formatFile << mCopyBufferName.at(j);
						}
						break;
					}
					else
					{
						temp << mCopyBufferName.at(i);
					}
				}
				newFilePath = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
				newFilePath << "\\" << temp;
				newFilePath << "_" << iter;
				newFilePath << formatFile;
				++iter;
			}
			wxCopyFile(oldFilePath, newFilePath, false); //Copy
			if (mIsCutFile) //Has file been cut?
			{
				wxRemoveFile(oldFilePath);//remove cut file
			}
			mIsCutFile = false;
			//Refresh all notebooks
			RefreshAllNotebooks();
		}
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Cannot paste this file!"), wxT("ERROR"), wxOK | wxICON_ERROR);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuEditPasteAs(wxCommandEvent& WXUNUSED(evt))
{
	wxString newFilePath = "";
	wxString oldFilePath = "";
	wxString formatFile = "";
	wxString fileName = "";
	if (mIsCopiedFile)
	{
		int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
		int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
		wxTextEntryDialog dialog(this, wxT("Enter name:"), wxT("Paste as..."), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

		newFilePath = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		oldFilePath << mCopyBufferPath;
		oldFilePath << "\\" << mCopyBufferName;
	//	oldFilePath = mCopyBufferPath;//new way of copy file


		fileName = "";
		for (int i = 0; i < (int)mCopyBufferName.size(); ++i)
		{
			if (mCopyBufferName.at(i) == '.')
			{
				formatFile = "";
				for (int j = i; j < (int)mCopyBufferName.size(); ++j)
				{
					formatFile << mCopyBufferName.at(j);
				}
				break;
			}
			else
			{
				fileName << mCopyBufferName.at(i);
			}
		}

		dialog.SetValue(fileName);
		if (dialog.ShowModal() == wxID_OK) //Press ok button
		{
			
			if (dialog.GetValue().size() != 0) //Name is not empty
			{
				if (wxFileExists(oldFilePath)) //if file to copy still exists.
				{
					newFilePath << "\\" << dialog.GetValue();
					newFilePath << formatFile;
					if (!wxFileExists(newFilePath))
					{
						wxCopyFile(oldFilePath, newFilePath, false);
						if (mIsCutFile) //Has file been cut?
						{
							wxRemoveFile(oldFilePath);//remove cut file
						}

						mIsCutFile = false;
						
						//Refresh all notebooks
						RefreshAllNotebooks();
					}
					else
					{
						wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("This file exists!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
						errorMsg->ShowModal();
						errorMsg->Destroy();
					}
				}
			}
			else  //Name IS empty
			{
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}
		}
	}
	else
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Cannot paste this file!"), wxT("ERROR"), wxOK | wxICON_ERROR);
		errorMsg->ShowModal();
		errorMsg->Destroy();
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnMenuEditDelete(wxCommandEvent& WXUNUSED(evt))
{
	wxString deleteFilePath = "";
	if (mIsItemSelected)
	{
		int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
		int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
		wxMessageDialog *dialog = new wxMessageDialog(NULL, wxT("Are you sure you want to delete this file?"), wxT("Delete?"), wxOK | wxCANCEL | wxICON_QUESTION, wxPoint(width, height));
		if (dialog->ShowModal() == wxID_OK) //Press ok button
		{
			deleteFilePath << mSelectedFilePath;
			deleteFilePath << "\\" << mSelectedFileName;
			if (wxRemoveFile(deleteFilePath))
			{
				//Refresh all notebooks
				RefreshAllNotebooks();
			}
			else
			{
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Cannot delete this file!"), wxT("ERROR"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}
		}
		dialog->Destroy();
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnListSelectedItem(wxListEvent& evt)
{

	
	if (evt.GetData() == 1000) //a back directory
	{
		mIsItemSelected = false;
		mIsItemSelectedDirectory = true;
	}
	else if (evt.GetData() == 1001) //a simple directory
	{
		mIsItemSelected = false;
		mIsItemSelectedDirectory = true;
	}
	else if (evt.GetData() == 1002) //a simple directory
	{
		mIsItemSelected = true;
		mIsItemSelectedDirectory = false;
		mLinkToSelectedFile = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		mSelectedFilePath = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
		mSelectedFileName = evt.GetText();
		mLinkToSelectedFile << "\\";
		mLinkToSelectedFile << mSelectedFileName;


		wxString formatFile = "";
		for (int k = 0; k < (int)mSelectedFileName.size(); ++k)
			if (mSelectedFileName.at(k) == '.')
			{
				formatFile = "";
				for (int j = k; j < (int)mSelectedFileName.size(); ++j)
					formatFile << mSelectedFileName.at(j);
				break;
			}
		if (formatFile == ".PNG" || formatFile == ".png" || formatFile == ".TGA" || formatFile == ".tga" || formatFile == ".JPG" || formatFile == ".jpg" || formatFile == ".DDS" || formatFile == ".dds")
		{
			for (int i = 0; i < (int)mListOfKLMESHViewer->size(); ++i)
				mListOfKLMESHViewer->at(i)->SetLinkToTexture(mLinkToSelectedFile, mSelectedFileName);
		}
		else
			for (int i = 0; i < (int)mListOfKLMESHViewer->size(); ++i)
				mListOfKLMESHViewer->at(i)->SetLinkToTexture(wxT(""), wxT(""));



		
		//mLinkToSelectedFile = mLinkToSelectedFile;
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnListDeselectedItem(wxListEvent& WXUNUSED(evt))
{
	mIsItemSelected = false;
	mLinkToSelectedFile = "";
	mIsItemSelectedDirectory = false;
	for (int i = 0; i < (int)mListOfKLMESHViewer->size(); ++i)
	{
		mListOfKLMESHViewer->at(i)->SetLinkToTexture(mLinkToSelectedFile, wxT(""));
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnRightClickBackground(wxContextMenuEvent& evt)
{
	wxMenu* menu = new wxMenu;
	wxMenu* menu_import = new wxMenu;
	wxMenu* menu_create = new wxMenu;

	wxPoint point = wxGetMousePosition();
	wxString oldFilePath = "";
	oldFilePath << mCopyBufferPath;
	oldFilePath << "\\" << mCopyBufferName;
	wxListCtrl* listTemp = NULL;
	wxWindow* windowTemp = mNotebookAssets->GetPage(mNotebookAssets->GetSelection());
	listTemp = (wxListCtrl*)windowTemp;
	if (listTemp->GetEditControl() == NULL && mIsItemSelected == false && mIsItemSelectedDirectory == false)
	{

		point = evt.GetPosition();
		if ((point.x == -1) && (point.y == -1))
		{
			wxSize size = GetSize();
			point.x = size.x / 2;
			point.y = size.y / 2;
		}
		else
		{
			point = ScreenToClient(point);
		}
		if (wxFileExists(oldFilePath)) //if file to copy still exists.
		{
			menu->Append(ID_MENU_EDIT_PASTE, wxT("Paste"), wxT(""), wxITEM_NORMAL);
			menu->Append(ID_MENU_EDIT_PASTE_AS, wxT("Paste as..."), wxT(""), wxITEM_NORMAL);
			menu->AppendSeparator();
		}
		menu_import->Append(ID_MENU_IMPORT_MESH, wxT("Mesh"), wxT(""), wxITEM_NORMAL);
		menu_import->Append(ID_MENU_IMPORT_TEXTURE, wxT("Texture"), wxT(""), wxITEM_NORMAL);
		menu_create->Append(ID_MENU_CREATE_WORLD, wxT("World"), wxT(""), wxITEM_NORMAL);
		menu_create->Append(ID_MENU_CREATE_KLMESH, wxT("KLMESH"), wxT(""), wxITEM_NORMAL);
		menu->Append(ID_MENU_VIEW_REFRESH, wxT("Refresh"), wxT(""), wxITEM_NORMAL);
		menu->AppendSeparator();
		menu->Append(ID_MENU_ADD_Directory, wxT("Add new directory"), wxT(""), wxITEM_NORMAL);
		menu->AppendSeparator();
		menu->AppendSubMenu(menu_import, wxT("Import"));
		menu->AppendSubMenu(menu_create, wxT("Create"));

		PopupMenu(menu, point);
	}
	
	else
	{
		evt.Skip();
	}
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnNotebookPageChanged(wxAuiNotebookEvent& WXUNUSED(evt))
{
	mIsItemSelected = false;
	mIsItemSelectedDirectory = false;
}
//*********************************************************************************************************************************************************************
void AssetBrowser::RefreshAllNotebooks()
{
	int selectedPageIndex = mNotebookAssets->GetSelection();
	wxString path = "";
	wxString directoryName = "";

	for (int i = 0; i < (int)mNotebookAssets->GetPageCount(); ++i)
	{
		directoryName = mNotebookAssets->GetPageText(i);
		path = mNotebookAssets->GetPageToolTip(i);
		mNotebookAssets->InsertPage(i, CreateAssetBrowserNotebookDirectory(mNotebookAssets, path), directoryName, true, mPage_bmp);
		if (mNotebookAssets->GetPageBitmap(i + 1).GetHeight() != 15) //Is page lock?
		{
			mNotebookAssets->SetPageBitmap(i, wxIcon(wxT("KL_ICON_Lock"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));//locked
		}
		mNotebookAssets->SetPageToolTip(i, path);
		mNotebookAssets->RemovePage(i + 1); //remove the old page
	}
	mNotebookAssets->SetSelection(selectedPageIndex);
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserDirectoryOpenBack(wxCommandEvent& WXUNUSED(evt))
{

	int pagePosition = mNotebookAssets->GetSelection();


	//mDirectoryPathAssetBrowser << "\\" << mDirectoryNameAssetBrowser;
	if (mNotebookAssets->GetPageBitmap(pagePosition).GetHeight() == 15)//Insert and remove unlocked page
	{
		mNotebookAssets->InsertPage(pagePosition, CreateAssetBrowserNotebookDirectory(mNotebookAssets, mDirectoryPathAssetBrowser), mDirectoryNameAssetBrowser, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(pagePosition, mDirectoryPathAssetBrowser);
		mNotebookAssets->RemovePage(pagePosition + 1);
	}
	else //Add page without delete locked page
	{
		mNotebookAssets->AddPage(CreateAssetBrowserNotebookDirectory(mNotebookAssets, mDirectoryPathAssetBrowser), mDirectoryNameAssetBrowser, true, mPage_bmp);
		mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectoryPathAssetBrowser);
	}
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserDirectoryOpenBackInNewTab(wxCommandEvent&  WXUNUSED(evt))
{
	//mDirectoryPathAssetBrowser << "\\" << mDirectoryNameAssetBrowser;
	mNotebookAssets->AddPage(CreateAssetBrowserNotebookDirectory(mNotebookAssets, mDirectoryPathAssetBrowser), mDirectoryNameAssetBrowser, true, mPage_bmp);
	mNotebookAssets->SetPageToolTip(mNotebookAssets->GetPageCount() - 1, mDirectoryPathAssetBrowser);
	Refresh();
}
//*********************************************************************************************************************************************************************
void AssetBrowser::OnAssetBrowserNewDirectory(wxCommandEvent& WXUNUSED(evt))
{
	int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
	int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
	wxTextEntryDialog dialog(this, wxT("Enter name of new directory:"), wxT("Add directory"), wxT(""), wxOK | wxCANCEL, wxPoint(width, height));

	if (dialog.ShowModal() == wxID_OK) //Press ok button
	{
		if (dialog.GetValue().size() != 0) //Name is not empty
		{
			wxDir* dir = new wxDir("data");
			wxString folderName = mNotebookAssets->GetPageToolTip(mNotebookAssets->GetSelection());
			folderName << "\\" << dialog.GetValue();

			if (dir->Make(folderName, wxS_DIR_DEFAULT, 0))
			{
				OnRefreshDictionary();
				RefreshAllNotebooks();
			}
			else
			{
				wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("This folder exists!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
				errorMsg->ShowModal();
				errorMsg->Destroy();
			}
			dir->Close();
		}
		else  //Name IS empty
		{
			wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Incorrect name!"), wxT("Error!!!"), wxOK | wxICON_ERROR);
			errorMsg->ShowModal();
			errorMsg->Destroy();
		}
	}
}
//*********************************************************************************************************************************************************************
wxString AssetBrowser::GetLinkToSelectedFile()
{
	if (mIsItemSelected)
		return mLinkToSelectedFile;
	else
		return wxString("");
}
void AssetBrowser::SetKLMESH_Buffer(KLMESH_Buffer* klmeshBuffer)
{
	this->mKLMESH_Buffer = klmeshBuffer;
}
void  AssetBrowser::SetTexturesResource(TexturesResource* texturesResource)
{
	this->mTexturesResource = texturesResource;
}
void AssetBrowser::SetDevice(ID3D11Device* device)
{
	this->mDevice = device;
}
std::vector<KLMESHViewer*>* AssetBrowser::GetListOfKLMESHViewer()
{
	return this->mListOfKLMESHViewer;
}




