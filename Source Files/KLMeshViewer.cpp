#include "../Header Files/KLMeshViewer.h"
#include "../Header Files/BinaryFileRead.h"
#include "../Header Files/KLMESH.h"


wxBEGIN_EVENT_TABLE(KLMESHViewer, wxFrame)
	EVT_END_SESSION(KLMESHViewer::OnClose)
	EVT_CLOSE(KLMESHViewer::OnClose)
	EVT_MENU(ID_MENU_EXIT, KLMESHViewer::OnMenuClose)
	EVT_MENU(ID_MENU_SAVE, KLMESHViewer::OnMenuSave)
	EVT_MENU(ID_MENU_ZOOM, KLMESHViewer::OnMenuZoom)
	EVT_MENU(ID_MENU_REIMPORT, KLMESHViewer::OnMenuReimport)

	EVT_MENU(ID_Toolbar_ZOOM, KLMESHViewer::OnMenuZoom)
	EVT_MENU(ID_Toolbar_BBOX, KLMESHViewer::OnMenuBBox)
	EVT_MENU(ID_Toolbar_COLL, KLMESHViewer::OnMenuCollision)
	
	
	
	EVT_PAINT(KLMESHViewer::OnPaint)
	//Info
	EVT_PG_CHANGED(ID_TAB_INFO, KLMESHViewer::OnTabInfoGridPropertyChanged)
	//Material case:
	EVT_LISTBOX(ID_TAB_MATERIAL_ListOfMaterials, KLMESHViewer::OnTabMatSelectedMaterialList)
	EVT_PG_CHANGED(ID_TAB_MATERIAL_GridProperty, KLMESHViewer::OnTabMatGridPropertyChanged)
	//EVT_PG_SELECTED(ID_TAB_MATERIAL_GridProperty, KLMESHViewer::OnTabMatGridPropertyChanging)
	EVT_PG_HIGHLIGHTED(ID_TAB_MATERIAL_GridProperty, KLMESHViewer::OnTabMatGridPropertyChanging)
	//LODs case:
	EVT_LISTBOX(ID_TAB_LODs_ListOfLODs, KLMESHViewer::OnTabLODSelectedList)
	EVT_PG_CHANGED(ID_TAB_LOD_GridProperty, KLMESHViewer::OnTabLODGridPropertyChanged)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT, KLMESHViewer::OnSliderLight)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_COLOR_R, KLMESHViewer::OnSliderLightColorR)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_COLOR_G, KLMESHViewer::OnSliderLightColorG)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_COLOR_B, KLMESHViewer::OnSliderLightColorB)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_POWER, KLMESHViewer::OnSliderLightPower)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_SPEC, KLMESHViewer::OnSliderLightSpec)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_COLOR_SPEC_R, KLMESHViewer::OnSliderLightColorSpecR)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_COLOR_SPEC_G, KLMESHViewer::OnSliderLightColorSpecG)
	EVT_COMMAND_SCROLL(ID_SLIDER_LIGHT_COLOR_SPEC_B, KLMESHViewer::OnSliderLightColorSpecB)
	EVT_COMMAND_SCROLL(ID_SLIDER_ROTATION, KLMESHViewer::OnSliderRotation)
	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NotebookWindow, KLMESHViewer::OnNotebookChanged)
	EVT_CHECKBOX(ID_TAB_MATERIAL_SELECTED, KLMESHViewer::OnCheckBoxClicked)

wxEND_EVENT_TABLE()

/*wxBEGIN_EVENT_TABLE(KLMESHViewer, wxPanel)
	EVT_PAINT(KLMESHViewer::OnPaint)
wxEND_EVENT_TABLE()*/

wxString gLinkToTexture = "";
int gChangeValues = 0;
//0 - null
//1 - diffuse
//2 - normal
//3 - ash
//11 - reset diffuse
//12 - resetnormal
//13 - reset ash


class wxSampleMultiButtonEditor : public wxPGTextCtrlEditor
{
	wxDECLARE_DYNAMIC_CLASS(wxSampleMultiButtonEditor);

public:
	wxSampleMultiButtonEditor() {}
	virtual ~wxSampleMultiButtonEditor() {}
	virtual wxString GetName() const { return "SampleMultiButtonEditor"; }
	virtual wxPGWindowList CreateControls(wxPropertyGrid* propGrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const;
	virtual bool OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const;
};
wxIMPLEMENT_DYNAMIC_CLASS(wxSampleMultiButtonEditor, wxPGTextCtrlEditor);
wxPGWindowList wxSampleMultiButtonEditor::CreateControls(wxPropertyGrid* propGrid, wxPGProperty* property, const wxPoint& pos, const wxSize& sz) const
{
	wxPGMultiButton* buttons = new wxPGMultiButton(propGrid, sz);
	buttons->Add(wxIcon(wxT("KL_ICON_GREEN_ARROW"), wxBITMAP_TYPE_ICO_RESOURCE, 12, 12));
	buttons->Add(wxIcon(wxT("KL_ICON_RED_X"), wxBITMAP_TYPE_ICO_RESOURCE, 12, 12));

	wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls (propGrid, property, pos, buttons->GetPrimarySize());
	buttons->Finalize(propGrid, pos);
	wndList.SetSecondary(buttons);
	return wndList;
}
bool wxSampleMultiButtonEditor::OnEvent(wxPropertyGrid* propGrid, wxPGProperty* property, wxWindow* ctrl, wxEvent& event) const
{
	if (event.GetEventType() == wxEVT_BUTTON)
	{
		wxPGMultiButton* buttons = (wxPGMultiButton*)propGrid->GetEditorControlSecondary();
		if (event.GetId() == buttons->GetButtonId(0))
		{
			//if(property->GetLabel() == "DiffuseTexture:")
				//wxMessageBox(gLinkToTexture + ",", "Please confirm", wxICON_QUESTION);
			if (gLinkToTexture != "")
			{
				wxString directory = wxGetCwd() + "\\"; // +"\\data\\";
				wxString tempLink = "";
				for (int i = directory.size(); i < (int)gLinkToTexture.size(); ++i)
				{
					tempLink << gLinkToTexture.at(i);
				}
			//	gLinkToTexture = tempLink;


				property->SetValueFromString(tempLink);
				if (property->GetLabel() == "DiffuseTexture:")
					gChangeValues = 1;
				else if (property->GetLabel() == "NormalTexture:")
					gChangeValues = 2;
				else if (property->GetLabel() == "ASHTexture:")
					gChangeValues = 3;
				else if (property->GetLabel() == "CubeTexture:")
					gChangeValues = 4;
			}	
			else
				wxMessageBox("No texture selected in the AssetBrowser", "Warning", wxICON_QUESTION);

		}
		if (event.GetId() == buttons->GetButtonId(1))
		{
			property->SetValueFromString("NULL");
			if (property->GetLabel() == "DiffuseTexture:")
				gChangeValues = 11;
			else if (property->GetLabel() == "NormalTexture:")
				gChangeValues = 12;
			else if (property->GetLabel() == "ASHTexture:")
				gChangeValues = 13;
			else if (property->GetLabel() == "CubeTexture:")
				gChangeValues = 14;
		}
	}
	return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
}














//************************************************************************************************************************************************************************************************
//ImageViewer - C-TOR
//************************************************************************************************************************************************************************************************
KLMESHViewer::KLMESHViewer(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{


	mDxAppKLMeshViever = new DxAppKLMeshViever(this, ID_WindowDirectX);
	mDxAppKLMeshViever->Init();

	mLinkToSelectedTexture = "";
	mFileChanges = false;

	wxString tempText = "";
	for (int i = 8; i < (int)title.size(); ++i)
	{
		tempText << title.at(i);
	}
	this->mPathToFile = tempText;


	mMultiButtonEditor = new wxSampleMultiButtonEditor();
	wxPropertyGrid::RegisterEditorClass(mMultiButtonEditor);


	//Read klmesh file
	BinaryFileRead *binaryFileRead = new BinaryFileRead((std::string)mPathToFile); //KalenEngineDX11
	mKlmeshStruct = nullptr;
	mKlmeshStruct = new klmeshFileStruct();
	binaryFileRead->readKLMeshFileStructure(*mKlmeshStruct);


	
	LocalCoordinateSystem* localCoordinate = new LocalCoordinateSystem();




	mArrayOfShaders = new wxArrayString();
	mArrayOfShaders->Add(wxT("Phong"));
	mArrayOfShaders->Add(wxT("Phong Transparency"));
	mArrayOfShaders->Add(wxT("Phong Tessellation"));
	mArrayOfShaders->Add(wxT("Cook-Torrance"));
	mArrayOfShaders->Add(wxT("Cook-Torrance Transparency"));
	mArrayOfShaders->Add(wxT("Cook-Torrance Tessellation"));

	mArrayOfSelectedTextures = new wxArrayString();
	mArrayOfSelectedTextures->push_back("Current");
	mArrayOfSelectedTextures->push_back("From AssetBrowser");
	mArrayOfSelectedTextures->push_back("Set NULL");


	//--------------
	// Materials
	//--------------

	mArrayOfMaterials = new wxArrayString();

	for (int i = 0; i < (int)mKlmeshStruct->materialsBuffer.size(); ++i)
	{
		mArrayOfMaterials->push_back(wxString(mKlmeshStruct->materialsBuffer.at(i).materialName));
	}

	mVectorOfMaterials = new std::vector<MaterialInfo*>(0);
	int numberOfMaterial = (int)mArrayOfMaterials->size();
	int vertice = 0;
	int indices = 0;
	for (int i = 0; i < numberOfMaterial; ++i)
	{
		MaterialInfo* mat = new MaterialInfo();
		mat->SetMatName(mArrayOfMaterials->Item(i));
		mat->SetAmbientColour(wxColor(mKlmeshStruct->materialsBuffer.at(i).ambientColour.x * 255, mKlmeshStruct->materialsBuffer.at(i).ambientColour.y * 255, mKlmeshStruct->materialsBuffer.at(i).ambientColour.z * 255));
		mat->SetDiffuseColour(wxColor(mKlmeshStruct->materialsBuffer.at(i).diffuseColour.x * 255, mKlmeshStruct->materialsBuffer.at(i).diffuseColour.y * 255, mKlmeshStruct->materialsBuffer.at(i).diffuseColour.z * 255));
		mat->SetSpecularColour(wxColor(mKlmeshStruct->materialsBuffer.at(i).specularColour.x * 255, mKlmeshStruct->materialsBuffer.at(i).specularColour.y * 255, mKlmeshStruct->materialsBuffer.at(i).specularColour.z * 255));
		mat->SetReflectColour(wxColor(mKlmeshStruct->materialsBuffer.at(i).reflectColour.x * 255, mKlmeshStruct->materialsBuffer.at(i).reflectColour.y * 255, mKlmeshStruct->materialsBuffer.at(i).reflectColour.z * 255));
		mat->SetFresnelFactor(mKlmeshStruct->materialsBuffer.at(i).fresnelFactor);
		mat->SetTransparency(mKlmeshStruct->materialsBuffer.at(i).transparency);
		mat->SetRoughness(mKlmeshStruct->materialsBuffer.at(i).roughness);
		mat->SetShaderName(mKlmeshStruct->materialsBuffer.at(i).shaderName);
		mat->SetSpecularPower(mKlmeshStruct->materialsBuffer.at(i).specularPower);
		mat->SetTessellationHeightScale(mKlmeshStruct->materialsBuffer.at(i).heightScaleTess);
		mat->SetTessellationMaxDistance(mKlmeshStruct->materialsBuffer.at(i).maxDistanceTess);
		mat->SetTessellationMinDistance(mKlmeshStruct->materialsBuffer.at(i).minDistanceTess);
		mat->SetTessellationMaxFactor(mKlmeshStruct->materialsBuffer.at(i).maxFactorTess);
		mat->SetTessellationMinFactor(mKlmeshStruct->materialsBuffer.at(i).minFactorTess);
		mat->SetTextureDiffuseName(mKlmeshStruct->materialsBuffer.at(i).diffuseMapName);
		mat->SetTextureNormalName(mKlmeshStruct->materialsBuffer.at(i).normalMapName);
		mat->SetTextureASHName(mKlmeshStruct->materialsBuffer.at(i).ashMapName);
		mat->SetTextureCubeName(mKlmeshStruct->materialsBuffer.at(i).cubeMapName);
		mat->SetCubeMapDynamic(mKlmeshStruct->materialsBuffer.at(i).cubeMapDynamic);

		for (int j = 0; j < (int)mKlmeshStruct->mObjectsVector_LOD0.size(); ++j)
		{
			if (mKlmeshStruct->mObjectsVector_LOD0.at(j).materialPosition == i)
			{
				vertice += mKlmeshStruct->AllMeshData.Vertices.size() - mKlmeshStruct->mObjectsVector_LOD0.at(j).startVertex;
				indices += mKlmeshStruct->AllMeshData.Indices.size() - mKlmeshStruct->mObjectsVector_LOD0.at(j).startIndex;
			}
		}
		if(mKlmeshStruct->mObjectsVector_LOD1.size() != 0)
		for (int j = 0; j < (int)mKlmeshStruct->mObjectsVector_LOD1.size(); ++j)
		{
			if (mKlmeshStruct->mObjectsVector_LOD1.at(j).materialPosition == i)
			{
				vertice += mKlmeshStruct->AllMeshData.Vertices.size() - mKlmeshStruct->mObjectsVector_LOD1.at(j).startVertex;
				indices += mKlmeshStruct->AllMeshData.Indices.size() - mKlmeshStruct->mObjectsVector_LOD1.at(j).startIndex;
			}
		}
		if (mKlmeshStruct->mObjectsVector_LOD2.size() != 0)
		for (int j = 0; j < (int)mKlmeshStruct->mObjectsVector_LOD2.size(); ++j)
		{
			if (mKlmeshStruct->mObjectsVector_LOD2.at(j).materialPosition == i)
			{
				vertice += mKlmeshStruct->AllMeshData.Vertices.size() - mKlmeshStruct->mObjectsVector_LOD2.at(j).startVertex;
				indices += mKlmeshStruct->AllMeshData.Indices.size() - mKlmeshStruct->mObjectsVector_LOD2.at(j).startIndex;
			}
		}
		mat->SetNumberOfVertices(vertice);
		mat->SetNumberOfTriangles(indices / 3);
		mat->SetNumberOfIndices(indices);
		mVectorOfMaterials->push_back(mat);
	}


	//--------------
	// LODs
	//--------------


	mVectorOfLods = new std::vector<LodInfo*>(0);
	int numberOfLods = mKlmeshStruct->lodAmount;

	for (int i = 0; i < numberOfLods; ++i)
	{
		//Reset values
		vertice = 0;
		indices = 0;

		LodInfo* lod = new LodInfo();
		if (i == 0)
		{
			lod->SetDistance(mKlmeshStruct->mDistanceLOD0);
			lod->SetNumberOfChunks((int)mKlmeshStruct->mObjectsVector_LOD0.size());
			for (int j = 0; j < (int)mKlmeshStruct->mObjectsVector_LOD0.size(); ++j)
			{
				vertice += mKlmeshStruct->AllMeshData.Vertices.size() - mKlmeshStruct->mObjectsVector_LOD0.at(j).startVertex;
				indices += mKlmeshStruct->AllMeshData.Indices.size() - mKlmeshStruct->mObjectsVector_LOD0.at(j).startIndex;
			}
			lod->SetNumberOfIndices(indices);
			lod->SetNumberOfTriangles(indices / 3);
			lod->SetNumberOfVertices(vertice);
			lod->SetNumberOfMaterials((int)mKlmeshStruct->mObjectsVector_LOD0.size());
			mVectorOfLods->push_back(lod);
		}
			
		if(i == 1)
		{
			lod->SetDistance(mKlmeshStruct->mDistanceLOD1);
			lod->SetNumberOfChunks((int)mKlmeshStruct->mObjectsVector_LOD1.size());
			for (int j = 0; j < (int)mKlmeshStruct->mObjectsVector_LOD1.size(); ++j)
			{
				vertice += mKlmeshStruct->AllMeshData.Vertices.size() - mKlmeshStruct->mObjectsVector_LOD1.at(j).startVertex;
				indices += mKlmeshStruct->AllMeshData.Indices.size() - mKlmeshStruct->mObjectsVector_LOD1.at(j).startIndex;
			}
			lod->SetNumberOfIndices(indices);
			lod->SetNumberOfTriangles(indices / 3);
			lod->SetNumberOfVertices(vertice);
			lod->SetNumberOfMaterials((int)mKlmeshStruct->mObjectsVector_LOD1.size());
			mVectorOfLods->push_back(lod);
		}
		if(i == 2)
		{
			lod->SetDistance(mKlmeshStruct->mDistanceLOD2);
			lod->SetNumberOfChunks((int)mKlmeshStruct->mObjectsVector_LOD2.size());
			for (int j = 0; j < (int)mKlmeshStruct->mObjectsVector_LOD2.size(); ++j)
			{
				vertice += mKlmeshStruct->AllMeshData.Vertices.size() - mKlmeshStruct->mObjectsVector_LOD2.at(j).startVertex;
				indices += mKlmeshStruct->AllMeshData.Indices.size() - mKlmeshStruct->mObjectsVector_LOD2.at(j).startIndex;
			}
			lod->SetNumberOfIndices(indices);
			lod->SetNumberOfTriangles(indices / 3);
			lod->SetNumberOfVertices(vertice);
			lod->SetNumberOfMaterials((int)mKlmeshStruct->mObjectsVector_LOD2.size());
			mVectorOfLods->push_back(lod);
		}


		
	}


	mArrayOfLODs = new wxArrayString();
	wxString LodText = "LOD ";
	for (int i = 0; i < numberOfLods; ++i)
	{
		LodText = "LOD ";
		LodText << i << " [Distance: ";
		LodText << mVectorOfLods->at(i)->GetDistance();
		LodText << "m]";
		mArrayOfLODs->push_back(LodText);
	}





	SetIcon(wxIcon(wxT("KL_ICON_Logo")));


	//--------------------------------------------------------------------------------------------------
	// MENU
	//--------------------------------------------------------------------------------------------------

	wxMenuBar* menuBar = new wxMenuBar;

	//-----------------------
	//MENU - FILES
	//-----------------------
	wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_MENU_SAVE, wxT("Save\tCTRL+S"), wxT(""), wxITEM_NORMAL);
	file_menu->Append(ID_MENU_EXIT, wxT("Quit\tESC"), wxT(""), wxITEM_NORMAL);

	//----------------------- 
	//MENU - VIEW
	//-----------------------
	wxMenu* view_menu = new wxMenu;

	view_menu->Append(ID_MENU_ZOOM, wxT("Zoom extents\tF1"), wxT(""), wxITEM_NORMAL);

	//----------------------- 
	//MENU - TOOLS
	//-----------------------
	wxMenu* tools_menu = new wxMenu;

	tools_menu->Append(ID_MENU_REIMPORT, wxT("Reimport"), wxT(""), wxITEM_NORMAL);



	//-----------------------------------------
	//MENU - ADD MENU
	//-----------------------------------------
	menuBar->Append(file_menu, wxT("&File"));
	menuBar->Append(view_menu, wxT("&View"));
	menuBar->Append(tools_menu, wxT("&Tools"));
	SetMenuBar(menuBar);




	//--------------------------------------------------------------------------------------------------
	//TOOLBAR - TOP
	//--------------------------------------------------------------------------------------------------
	mToolBarTop = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_HORIZONTAL | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND | wxAUI_TB_GRIPPER);




	mToolBarTop->AddTool(ID_Toolbar_REIMPORT, wxT("Reimport"), wxIcon(wxT("KL_ICON_RestoreWorld"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Reimport"));
	mToolBarTop->AddSeparator();
	mToolBarTop->AddTool(ID_Toolbar_BBOX, wxT("Show BoundingBox"), wxIcon(wxT("KL_ICON_BBox"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Show BoundingBox"));
	mToolBarTop->AddSeparator();
	mToolBarTop->AddTool(ID_Toolbar_COLL, wxT("Show Collision"), wxIcon(wxT("KL_ICON_COLL"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Show Collision"));
	mToolBarTop->AddSeparator();
	mToolBarTop->AddTool(ID_Toolbar_ZOOM, wxT("Zoom object"), wxIcon(wxT("KL_ICON_ZOOM"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16), wxT("Zoom object"));
	//mToolBarTop->ToggleTool(ID_Toolbar_1, true);
	mToolBarTop->Realize();
	
	
	//-----------------------------------------
	//StatusBar
	//-----------------------------------------
	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"), 0);

	


	//-----------------------------------------
	//wxAuiManager
	//-----------------------------------------

	//wxAuiNotebook* mRightWindow

	mAuiManager.SetManagedWindow(this);

	mAuiManager.AddPane(mToolBarTop, wxAuiPaneInfo().
		Name(wxT("mToolBarTop")).Caption(wxT("Toolbar 2 (Horizontal)")).
		ToolbarPane().Top().Row(1));


	mRightWindow = CreateRightWindow();

	mAuiManager.AddPane(mRightWindow, wxAuiPaneInfo().
		Name(wxT("panelRight")).Caption(wxT("Right Panel")).
		Right().
		CloseButton(false).MaximizeButton(false));


	//-----------------
	//Render space
	//-----------------
	//mPanelRender = new wxPanel(this, wxID_RenderPanel);
	mAuiManager.AddPane(mDxAppKLMeshViever->GetWindow(), wxAuiPaneInfo().
		Name(wxT("redner")).Caption(wxT("Render")).
		CentrePane().
		CloseButton(false).MaximizeButton(false));


	//-----------------
	//Render space
	//-----------------
	mAuiManager.AddPane(CreatePanelBottom(), wxAuiPaneInfo().
		Name(wxT("panelBottom")).Caption(wxT("PanelBottom")).
		Bottom().
		CloseButton(false).MaximizeButton(false));




		wxString perspective_all = mAuiManager.SavePerspective();

		int i, count;
		wxAuiPaneInfoArray& all_panes = mAuiManager.GetAllPanes();
		for (i = 0, count = all_panes.GetCount(); i < count; ++i)
			if (!all_panes.Item(i).IsToolbar())
				all_panes.Item(i).Hide();
		mAuiManager.GetPane(wxT("panelRight")).Show().Right().Layer(0).Row(0).Position(0);
		mAuiManager.GetPane(wxT("redner")).Show();
		mAuiManager.GetPane(wxT("panelBottom")).Show();
		mAuiManager.GetPane(wxT("mToolBarTop")).Show();


		mAuiManager.Update();



	//-----------------------------------------
	//wxAuiManager - END
	//-----------------------------------------

		/*wxImageHandler* tgaLoader = new wxTGAHandler();
		wxImage::AddHandler(tgaLoader);
		wxImage* image = new wxImage("C:\\wxWidgets-3.1.0\\samples\\aui\\data\\poor_messy_stonewall_n.tga", wxBITMAP_TYPE_TGA);
		//*image = image->Scale(512, h);
		mImage = new wxBitmap(*image, wxBITMAP_SCREEN_DEPTH);

	*/

}

//************************************************************************************************************************************************************************************************
//KLMESHViewer - D-TOR
//************************************************************************************************************************************************************************************************
KLMESHViewer::~KLMESHViewer()
{
	//delete mImage;
	mAuiManager.UnInit();
}
//***********************************************************************************************************************************************************************
wxAuiNotebook* KLMESHViewer::CreateRightWindow()
{
	wxSize client_size = GetClientSize();

	mNotebookWindow = new wxAuiNotebook(this, ID_NotebookWindow, wxPoint(client_size.x, client_size.y), wxSize(430, 100),
		wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_SCROLL_BUTTONS | wxNO_BORDER);//wxAUI_NB_TAB_MOVE
	mNotebookWindow->Freeze();

	mNotebookWindow->SetAutoLayout(true);
	
	
	//--------------------------
	//Tab 1 - Properties
	//--------------------------
	mNotebookWindow->AddPage(CreatePropertyGridProperty(), wxT("Properties"), false, wxIcon(wxT("KL_ICON_Property"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mNotebookWindow->SetPageToolTip(0, "Properties - Tab");
	//--------------------------
	//Tab 2 - Material
	//--------------------------
	mNotebookWindow->AddPage(CreateMaterialsPanel(), wxT("Materials"), false, wxIcon(wxT("KL_ICON_MAT"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mNotebookWindow->SetPageToolTip(1, "Material - Tab");
	//--------------------------
	//Tab 3 - LODs
	//--------------------------
	mNotebookWindow->AddPage(CreateLODsPanel(), wxT("LODs"), false, wxIcon(wxT("KL_ICON_Gears"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	mNotebookWindow->SetPageToolTip(2, "LODs - tab");
	//--------------------------
	//Tab 4 - Informations
	//--------------------------
	//mNotebookWindow->AddPage(CreateHTMLCtrl(this), wxT("INFO"), false, wxIcon(wxT("KL_ICON_INFO"), wxBITMAP_TYPE_ICO_RESOURCE, 16, 16));
	//mNotebookWindow->SetPageToolTip(3, "INFO - tab");

	mNotebookWindow->Thaw();
	return mNotebookWindow;
}
//***********************************************************************************************************************************************************************
wxScrolledWindow* KLMESHViewer::CreatePanelBottom()
{

	//	wxScrolledWindow* panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	mPanelBottom = new wxScrolledWindow(this, wxID_ANY, wxPoint(0, 0), wxSize(1370, 100));

	mPanelBottomText = new wxStaticText(mPanelBottom, -1, wxT("Light position:"), wxPoint(10, 10), wxSize(-1, -1));
	mPanelBottomSlider = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT, 180, 0, 360, wxPoint(90, 5), wxSize(300, -1), wxSL_HORIZONTAL | wxSL_LABELS | wxSL_VALUE_LABEL);
	//mPanelBottomCheckBoxIsCastShadows = new wxCheckBox(mPanelBottom, wxID_ANY, wxT("IsCastShadows?"), wxPoint(10, 30), wxSize(-1, -1));
	mPanelBottomTextRot = new wxStaticText(mPanelBottom, -1, wxT("Rotation:"), wxPoint(10, 50), wxSize(-1, -1));
	mPanelBottomSliderRot = new wxSlider(mPanelBottom, ID_SLIDER_ROTATION, 180, 0, 360, wxPoint(90, 45), wxSize(300, -1), wxSL_HORIZONTAL | wxSL_LABELS | wxSL_VALUE_LABEL);

	mPanelBottomTextLightColour = new wxStaticText(mPanelBottom, -1, wxT("Light colour (RGB):"), wxPoint(400, 10), wxSize(-1, -1));
	mPanelBottomSliderLightR = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_COLOR_R, 255, 0, 255, wxPoint(520, 5), wxSize(100, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	mPanelBottomSliderLightG = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_COLOR_G, 255, 0, 255, wxPoint(640, 5), wxSize(100, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	mPanelBottomSliderLightB = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_COLOR_B, 255, 0, 255, wxPoint(760, 5), wxSize(100, -1), wxSL_HORIZONTAL | wxSL_LABELS);

	mPanelBottomTextLightPower = new wxStaticText(mPanelBottom, -1, wxT("Light power:"), wxPoint(880, 10), wxSize(-1, -1));
	mPanelBottomSliderLightPower = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_POWER, 50, 0, 100, wxPoint(960, 5), wxSize(150, -1), wxSL_HORIZONTAL | wxSL_LABELS | wxSL_VALUE_LABEL);
	mPanelBottomSliderLightPower->SetValue(100);

	mPanelBottomTextLightColour = new wxStaticText(mPanelBottom, -1, wxT("Specular colour (RGB):"), wxPoint(400, 50), wxSize(-1, -1));
	mPanelBottomSliderLightSpecR = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_COLOR_SPEC_R, 255, 0, 255, wxPoint(520, 45), wxSize(100, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	mPanelBottomSliderLightSpecG = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_COLOR_SPEC_G, 255, 0, 255, wxPoint(640, 45), wxSize(100, -1), wxSL_HORIZONTAL | wxSL_LABELS);
	mPanelBottomSliderLightSpecB = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_COLOR_SPEC_B, 255, 0, 255, wxPoint(760, 45), wxSize(100, -1), wxSL_HORIZONTAL | wxSL_LABELS);

	mPanelBottomTextLightSpecular = new wxStaticText(mPanelBottom, -1, wxT("Light specular:"), wxPoint(880, 50), wxSize(-1, -1));
	mPanelBottomSliderLightSpecular = new wxSlider(mPanelBottom, ID_SLIDER_LIGHT_SPEC, 50, 0, 100, wxPoint(960, 45), wxSize(150, -1), wxSL_HORIZONTAL | wxSL_LABELS | wxSL_VALUE_LABEL);
	mPanelBottomSliderLightSpecular->SetValue(100);

	mPanelBottom->FitInside();
	mPanelBottom->SetScrollRate(5, 5);

	return mPanelBottom;
}
//***********************************************************************************************************************************************************************
wxScrolledWindow* KLMESHViewer::CreatePropertyGridProperty()
{
	wxScrolledWindow* panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	wxFlexGridSizer* flex = new wxFlexGridSizer(1, 1, 0, 0);

	int w = -1;
	int h = -1;
	int a = -1;
	GetSize(&a, &h);
	mNotebookWindow->GetSize(&w, &a);

	mPropertyGridProperty = new wxPropertyGrid(panel, ID_TAB_INFO, wxDefaultPosition, wxSize(w - 40, h/2), wxPG_DEFAULT_STYLE, wxT("Node"));

	mPropertyGridProperty->Append(new wxPropertyCategory("Resource"));
		mFileNameProp = mPropertyGridProperty->Append(new wxStringProperty(wxT("FileName"), wxPG_LABEL, wxT("" + mKlmeshStruct->name)));
		mFileNameProp->SetHelpString(wxT("" + mKlmeshStruct->name));
	mPropertyGridProperty->Append(new wxPropertyCategory("Mesh"));

	wxPGProperty* boundingBoxProp = mPropertyGridProperty->Append(new wxStringProperty(wxT("BoundingBox"), wxPG_LABEL, wxT("<composed>")));
	boundingBoxProp->SetExpanded(false);
		wxPGProperty* BBMaxProp = mPropertyGridProperty->AppendIn(boundingBoxProp, new wxStringProperty(wxT("Max"), wxPG_LABEL, wxT("<composed>")));
		BBMaxProp->SetExpanded(false);
			mBBmaxXProp = mPropertyGridProperty->AppendIn(BBMaxProp, new wxFloatProperty(wxT("X:"), wxPG_LABEL, mKlmeshStruct->boundingBoxMax.x));
			mBBmaxYProp = mPropertyGridProperty->AppendIn(BBMaxProp, new wxFloatProperty(wxT("Y:"), wxPG_LABEL, mKlmeshStruct->boundingBoxMax.y));
			mBBmaxZProp = mPropertyGridProperty->AppendIn(BBMaxProp, new wxFloatProperty(wxT("Z:"), wxPG_LABEL, mKlmeshStruct->boundingBoxMax.z));

		wxPGProperty* BBMinProp = mPropertyGridProperty->AppendIn(boundingBoxProp, new wxStringProperty(wxT("Min"), wxPG_LABEL, wxT("<composed>")));
		BBMinProp->SetExpanded(false);
			mBBminXProp = mPropertyGridProperty->AppendIn(BBMinProp, new wxFloatProperty(wxT("X:"), wxPG_LABEL, mKlmeshStruct->boundingBoxMin.x));
			mBBminYProp = mPropertyGridProperty->AppendIn(BBMinProp, new wxFloatProperty(wxT("Y:"), wxPG_LABEL, mKlmeshStruct->boundingBoxMin.y));
			mBBminZProp = mPropertyGridProperty->AppendIn(BBMinProp, new wxFloatProperty(wxT("Z:"), wxPG_LABEL, mKlmeshStruct->boundingBoxMin.z));

			mHideDistanceProp = mPropertyGridProperty->Append(new wxFloatProperty(wxT("AutoHideDistance"), wxPG_LABEL, mKlmeshStruct->mDistanceHide / 100)); //cm -> m
			mShadowDistaceProp = mPropertyGridProperty->Append(new wxFloatProperty(wxT("ShadowRenderingDistace      "), wxPG_LABEL, mKlmeshStruct->shadowDistance / 100));//cm -> m
			mForceNoShadowProp = mPropertyGridProperty->Append(new wxBoolProperty(wxT("CastShadow?"), wxPG_LABEL, mKlmeshStruct->castShadow));

		mPropertyGridProperty->FitColumns();

	flex->Add(mPropertyGridProperty, 0, wxALL | wxALIGN_LEFT, 5);
	panel->SetSizer(flex);

	flex->FitInside(panel);
	panel->FitInside();
	panel->SetScrollRate(5, 5);

	return panel;
}
//***********************************************************************************************************************************************************************
wxScrolledWindow* KLMESHViewer::CreateMaterialsPanel()
{
	wxScrolledWindow* panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	wxFlexGridSizer* flex = new wxFlexGridSizer(4, 1, 0, 0);
	//wxBoxSizer* flex = new wxBoxSizer(wxVERTICAL);
	wxStaticText* listOfMaterialsText = new wxStaticText(panel, -1, wxT("List of materials:"));
	int w = -1;
	int h = -1;
	int a = -1;
	GetSize(&a, &h);
	mNotebookWindow->GetSize(&w, &a);
	//panel->SetAutoLayout(true);
	panel->SetVirtualSize(wxDefaultSize);
	panel->FitInside();

	//Select material --> set to mDxAppKLMeshViever
	mSelectedMaterialPos = 0;
	mDxAppKLMeshViever->SetSelectedMaterialPos(mSelectedMaterialPos);

	mListboxMaterial = new wxListBox(panel, ID_TAB_MATERIAL_ListOfMaterials, wxDefaultPosition, wxSize(w - 60, h/7), *mArrayOfMaterials, wxLB_SINGLE | wxLB_HSCROLL | wxLB_ALWAYS_SB | wxLB_SORT);
	mListboxMaterial->SetSelection(0);
	wxCheckBox* checkBoxHighlight = new wxCheckBox(panel, ID_TAB_MATERIAL_SELECTED,  wxT("Highlight selected material"));


	wxPropertyGrid* propertyGridProperty = new wxPropertyGrid(panel, ID_TAB_MATERIAL_GridProperty, wxDefaultPosition, wxSize(w - 60, (h/2) - 40), wxPG_DEFAULT_STYLE, wxT("Node"));

	propertyGridProperty->Append(new wxPropertyCategory("Shaders"));
	mSelectedMaterialProp = propertyGridProperty->Append(new wxStringProperty(wxT("Selected material:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetMatName()));
	mSelectedMaterialProp->Enable(false);


		mChooseShaderProp = propertyGridProperty->Append(new wxEnumProperty(wxT("Shader"), wxPG_LABEL, *mArrayOfShaders));
	propertyGridProperty->Append(new wxPropertyCategory("Used SHADER"));
		mUsedShaderProp = propertyGridProperty->Append(new wxStringProperty(wxT("Used Shader:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetShaderName()));
		mUsedShaderProp->Enable(false);

		if (mVectorOfMaterials->at(0)->GetShaderName() == "Phong")
			mChooseShaderProp->SetChoiceSelection(0);
		if (mVectorOfMaterials->at(0)->GetShaderName() == "Phong Transparency")
			mChooseShaderProp->SetChoiceSelection(1);
		if (mVectorOfMaterials->at(0)->GetShaderName() == "Phong Tessellation")
			mChooseShaderProp->SetChoiceSelection(2);
		if (mVectorOfMaterials->at(0)->GetShaderName() == "Cook-Torrance")
			mChooseShaderProp->SetChoiceSelection(3);
		if (mVectorOfMaterials->at(0)->GetShaderName() == "Cook-Torrance Transparency")
			mChooseShaderProp->SetChoiceSelection(4);
		if (mVectorOfMaterials->at(0)->GetShaderName() == "Cook-Torrance Tessellation")
			mChooseShaderProp->SetChoiceSelection(5);

	
	propertyGridProperty->Append(new wxPropertyCategory("Material Properties"));
		//DiffuseMap
		mMatTextureDiffuseProp = propertyGridProperty->Append(new wxStringProperty(wxT("DiffuseTexture:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTextureDiffuseName()));
		propertyGridProperty->SetPropertyEditor(mMatTextureDiffuseProp, mMultiButtonEditor);//2 added buttons
																							//NormalMap
		mMatTextureNormalProp = propertyGridProperty->Append(new wxStringProperty(wxT("NormalTexture:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTextureNormalName()));
		propertyGridProperty->SetPropertyEditor(mMatTextureNormalProp, mMultiButtonEditor);//2 added buttons
		//ASHMap
		mMatTextureSpecularProp = propertyGridProperty->Append(new wxStringProperty(wxT("ASHTexture:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTextureASHName()));
		propertyGridProperty->SetPropertyEditor(mMatTextureSpecularProp, mMultiButtonEditor);//2 added buttons
		//CubeMap
		mMatTextureCubeProp = propertyGridProperty->Append(new wxStringProperty(wxT("CubeTexture:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTextureCubeName()));
		propertyGridProperty->SetPropertyEditor(mMatTextureCubeProp, mMultiButtonEditor);//2 added buttons

		mAmbientColourProp = propertyGridProperty->Append(new wxStringProperty(wxT("ambient colour"), wxPG_LABEL, wxT("<composed>")));
			mAmbientColourProp->SetExpanded(false);
		mAmbient_R_ColourProp = propertyGridProperty->AppendIn(mAmbientColourProp, new wxIntProperty(wxT("R:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetAmbientColour().Red()));
		mAmbient_G_ColourProp = propertyGridProperty->AppendIn(mAmbientColourProp, new wxIntProperty(wxT("G:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetAmbientColour().Green()));
		mAmbient_B_ColourProp = propertyGridProperty->AppendIn(mAmbientColourProp, new wxIntProperty(wxT("B:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetAmbientColour().Blue()));

		mDiffuseColourProp = propertyGridProperty->Append(new wxStringProperty(wxT("diffuse colour"), wxPG_LABEL, wxT("<composed>")));
			mDiffuseColourProp->SetExpanded(false);
		mDiffuse_R_ColourProp = propertyGridProperty->AppendIn(mDiffuseColourProp, new wxIntProperty(wxT("R:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetDiffuseColour().Red()));
		mDiffuse_G_ColourProp = propertyGridProperty->AppendIn(mDiffuseColourProp, new wxIntProperty(wxT("G:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetDiffuseColour().Green()));
		mDiffuse_B_ColourProp = propertyGridProperty->AppendIn(mDiffuseColourProp, new wxIntProperty(wxT("B:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetDiffuseColour().Blue()));

		mSpecularColourProp = propertyGridProperty->Append(new wxStringProperty(wxT("specular colour"), wxPG_LABEL, wxT("<composed>")));
			mSpecularColourProp->SetExpanded(false);
		mSpecular_R_ColourProp = propertyGridProperty->AppendIn(mSpecularColourProp, new wxIntProperty(wxT("R:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetSpecularColour().Red()));
		mSpecular_G_ColourProp = propertyGridProperty->AppendIn(mSpecularColourProp, new wxIntProperty(wxT("G:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetSpecularColour().Green()));
		mSpecular_B_ColourProp = propertyGridProperty->AppendIn(mSpecularColourProp, new wxIntProperty(wxT("B:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetSpecularColour().Blue()));

		mReflectColourProp = propertyGridProperty->Append(new wxStringProperty(wxT("reflect colour"), wxPG_LABEL, wxT("<composed>")));
		mReflectColourProp->SetExpanded(false);
		mReflect_R_ColourProp = propertyGridProperty->AppendIn(mReflectColourProp, new wxIntProperty(wxT("R:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetReflectColour().Red()));
		mReflect_G_ColourProp = propertyGridProperty->AppendIn(mReflectColourProp, new wxIntProperty(wxT("G:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetReflectColour().Green()));
		mReflect_B_ColourProp = propertyGridProperty->AppendIn(mReflectColourProp, new wxIntProperty(wxT("B:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetReflectColour().Blue()));



		mSpecularPowerProp = propertyGridProperty->Append(new wxIntProperty(wxT("Specular power"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetSpecularPower()));
		mRoughnessProp = propertyGridProperty->Append(new wxFloatProperty(wxT("Roughness"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetRoughness()));
		mFresnelFactorProp = propertyGridProperty->Append(new wxFloatProperty(wxT("Fresnel Factor"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetFresnelFactor()));
		mTransparencyProp = propertyGridProperty->Append(new wxFloatProperty(wxT("Transparency"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTransparency()));

		mCubeMapDynamicProp = propertyGridProperty->Append(new wxBoolProperty(wxT("CubeMapDynamic"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetCubeMapDynamic()));


		if (mVectorOfMaterials->at(0)->GetShaderName() != "Cook-Torrance" && mVectorOfMaterials->at(0)->GetShaderName() != "Cook-Torrance Transparency" && mVectorOfMaterials->at(0)->GetShaderName() != "Cook-Torrance Tessellation")
		{
			mSpecularPowerProp->Hide(false);
			mRoughnessProp->Hide(true);
			mFresnelFactorProp->Hide(true);
		}
		else
		{
			mSpecularPowerProp->Hide(true);
			mRoughnessProp->Hide(false);
			mFresnelFactorProp->Hide(false);
		}

		if (mVectorOfMaterials->at(0)->GetShaderName() != "Cook-Torrance Transparency" && mVectorOfMaterials->at(0)->GetShaderName() != "Phong Transparency")
		{
			mTransparencyProp->Hide(true);
		}



	mTessellationProp = propertyGridProperty->Append(new wxPropertyCategory("Tessellation"));
		mTessellationHeightScaleProp = propertyGridProperty->Append(new wxIntProperty(wxT("Height Scale"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTessellationHeightScale()));
		mTessellationDistanceProp = propertyGridProperty->Append(new wxStringProperty(wxT("Distance (cm):"), wxPG_LABEL, wxT("<composed>")));
			mTessellationDistanceProp->SetExpanded(false);
			mTessellationDistanceMinProp = propertyGridProperty->AppendIn(mTessellationDistanceProp, new wxIntProperty(wxT("Min"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTessellationMinDistance()));
			mTessellationDistanceMaxProp = propertyGridProperty->AppendIn(mTessellationDistanceProp, new wxIntProperty(wxT("Max"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTessellationMaxDistance()));
		mTessellationFactorProp = propertyGridProperty->Append(new wxStringProperty(wxT("Factor:"), wxPG_LABEL, wxT("<composed>")));
			mTessellationFactorProp->SetExpanded(false);
			mTessellationFactorMinProp = propertyGridProperty->AppendIn(mTessellationFactorProp, new wxIntProperty(wxT("Min"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTessellationMinFactor()));
			mTessellationFactorMaxProp = propertyGridProperty->AppendIn(mTessellationFactorProp, new wxIntProperty(wxT("Max"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetTessellationMaxFactor()));
			


	wxPGProperty* infoProp =  propertyGridProperty->Append(new wxPropertyCategory("INFO:"));
	infoProp->SetExpanded(false);
	mInfoVerticesProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Vertices:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetNumberOfVertices()));
	mInfoVerticesProp->Enable(false);
	mInfoTrianglesProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Triangles:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetNumberOfTriangles()));
	mInfoTrianglesProp->Enable(false);
	mInfoIndicesProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Indices:"), wxPG_LABEL, mVectorOfMaterials->at(0)->GetNumberOfIndices()));
	mInfoIndicesProp->Enable(false);


	wxString text = mChooseShaderProp->GetDisplayedString();
	if (text != "Blinn-Phong Tessellation" && text != "Cook-Torrance Tessellation")
		mTessellationProp->Hide(true);
	else
		mTessellationProp->Hide(false);


	flex->Add(listOfMaterialsText, 0, wxALL | wxALIGN_LEFT, 5);
	flex->Add(mListboxMaterial, 1, wxALL | wxALIGN_LEFT, 5);
	flex->Add(checkBoxHighlight, 2, wxALL | wxALIGN_LEFT, 5);
	flex->Add(propertyGridProperty, 3, wxALL | wxALIGN_LEFT, 5);
	panel->SetSizer(flex);

	flex->FitInside(panel);
	panel->FitInside();
	panel->SetScrollRate(5, 5);

	return panel;
}
//***********************************************************************************************************************************************************************
wxScrolledWindow* KLMESHViewer::CreateLODsPanel()
{
	wxScrolledWindow* panel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	wxFlexGridSizer* flex = new wxFlexGridSizer(4, 1, 0, 0);
	wxStaticText* listOfLODsText = new wxStaticText(panel, -1, wxT("List of LODs:"));
	int w = -1;
	int h = -1;
	int a = -1;
	GetSize(&a, &h);
	mNotebookWindow->GetSize(&w, &a);
	mListboxLODs = new wxListBox(panel, ID_TAB_LODs_ListOfLODs, wxDefaultPosition, wxSize(w - 60, h / 7), *mArrayOfLODs, wxLB_SINGLE | wxLB_HSCROLL | wxLB_ALWAYS_SB | wxLB_SORT);
	mListboxLODs->SetSelection(0);

	wxStaticText* propertiesText = new wxStaticText(panel, -1, wxT("Properties:"));
	wxPropertyGrid* propertyGridProperty = new wxPropertyGrid(panel, ID_TAB_LOD_GridProperty, wxDefaultPosition, wxSize(w - 60, (h / 3) - 40), wxPG_DEFAULT_STYLE, wxT("Node"));

	propertyGridProperty->Append(new wxPropertyCategory("LOD properties:"));
	mLOD_DistanceProp = propertyGridProperty->Append(new wxIntProperty(wxT("Distance:"), wxPG_LABEL, 0));
	mLOD_TrianglesProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Triangles:"), wxPG_LABEL, mVectorOfLods->at(0)->GetNumberOfTriangles()));
	mLOD_VerticesProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Vertices:"), wxPG_LABEL, mVectorOfLods->at(0)->GetNumberOfVertices()));
	mLOD_IndicesProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Indices:"), wxPG_LABEL, mVectorOfLods->at(0)->GetNumberOfIndices()));
	mLOD_ChunksProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Chunks:"), wxPG_LABEL, mVectorOfLods->at(0)->GetNumberOfChunks()));
	mLOD_MaterialProp = propertyGridProperty->Append(new wxIntProperty(wxT("Number of Materials:"), wxPG_LABEL, mVectorOfLods->at(0)->GetNumberOfMaterials()));

	mLOD_TrianglesProp->Enable(false);
	mLOD_VerticesProp->Enable(false);
	mLOD_IndicesProp->Enable(false);
	mLOD_ChunksProp->Enable(false);
	mLOD_MaterialProp->Enable(false);


	flex->Add(listOfLODsText, 0, wxALL | wxALIGN_LEFT, 5);
	flex->Add(mListboxLODs, 1, wxALL | wxALIGN_LEFT, 5);
	flex->Add(propertiesText, 2, wxALL | wxALIGN_LEFT, 5);
	flex->Add(propertyGridProperty, 3, wxALL | wxALIGN_LEFT, 5);
	panel->SetSizer(flex);

	flex->FitInside(panel);

	panel->FitInside();
	panel->SetScrollRate(5, 5);
	return panel;
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnTabInfoGridPropertyChanged(wxPropertyGridEvent& WXUNUSED(evt))
{
	//Show this LOD level
	mForceLODLevel = -1;
	mDxAppKLMeshViever->SetForceLODLevel(mForceLODLevel);

	mFileNameProp->SetValueFromString(wxT("" + mKlmeshStruct->name));
	//	= mPropertyGridProperty->Append(new wxStringProperty(wxT("FileName"), wxPG_LABEL, wxT("" + mKlmeshStruct->name)));
	mKlmeshStruct->boundingBoxMax.x = mBBmaxXProp->GetValue().GetDouble();
	mKlmeshStruct->boundingBoxMax.y = mBBmaxYProp->GetValue().GetDouble();
	mKlmeshStruct->boundingBoxMax.z = mBBmaxZProp->GetValue().GetDouble();

	mKlmeshStruct->boundingBoxMin.x = mBBminXProp->GetValue().GetDouble();
	mKlmeshStruct->boundingBoxMin.y = mBBminYProp->GetValue().GetDouble();
	mKlmeshStruct->boundingBoxMin.z = mBBminZProp->GetValue().GetDouble();

	mKlmeshStruct->castShadow = mForceNoShadowProp->GetValue().GetBool();
	mKlmeshStruct->shadowDistance = mShadowDistaceProp->GetValue().GetDouble() * 100; //cm -> m
	mKlmeshStruct->mDistanceHide = mHideDistanceProp->GetValue().GetDouble() * 100;//cm -> m



	mKLMESH->SetBoundingBoxMax(XMLoadFloat3(&mKlmeshStruct->boundingBoxMax));
	mKLMESH->SetBoundingBoxMin(XMLoadFloat3(&mKlmeshStruct->boundingBoxMin));
	mKLMESH->SetShadowDistance(mKlmeshStruct->shadowDistance);
	mKLMESH->SetDistanceHide(mKlmeshStruct->mDistanceHide); 


	mKLMESH_render->SetBoundingBoxMax(XMLoadFloat3(&mKlmeshStruct->boundingBoxMax));
	mKLMESH_render->SetBoundingBoxMin(XMLoadFloat3(&mKlmeshStruct->boundingBoxMin));
	mKLMESH_render->SetShadowDistance(mKlmeshStruct->shadowDistance);
	mKLMESH_render->SetDistanceHide(mKlmeshStruct->mDistanceHide);


	if (mLinkToSelectedTexture == "")
	{
		mLinkToSelectedTexture = wxT("123123");
	}
	mFileChanges = true;
	mDxAppKLMeshViever->SetRender(true); //Update render
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnTabMatGridPropertyChanging(wxPropertyGridEvent& WXUNUSED(evt))
{
	if (gChangeValues != 0)
	{
		int pos = mListboxMaterial->GetSelection();
		if (gChangeValues == 1)
		{
			mVectorOfMaterials->at(pos)->SetTextureDiffuseName(mMatTextureDiffuseProp->GetValueAsString());
			LoadTexture(pos, (std::string)mMatTextureDiffuseProp->GetValueAsString(), 0);
		}
		else if (gChangeValues == 2)
		{
			mVectorOfMaterials->at(pos)->SetTextureNormalName(mMatTextureNormalProp->GetValueAsString());
			LoadTexture(pos, (std::string)mMatTextureNormalProp->GetValueAsString(), 1);
		}	
		else if (gChangeValues == 3)
		{
			mVectorOfMaterials->at(pos)->SetTextureASHName(mMatTextureSpecularProp->GetValueAsString());
			LoadTexture(pos, (std::string)mMatTextureSpecularProp->GetValueAsString(), 2);
		}
		else if (gChangeValues == 4)
		{
			mVectorOfMaterials->at(pos)->SetTextureCubeName(mMatTextureCubeProp->GetValueAsString());
			LoadTexture(pos, (std::string)mMatTextureCubeProp->GetValueAsString(), 3);
		}
		if (gChangeValues == 11)
			mVectorOfMaterials->at(pos)->SetTextureDiffuseName(mMatTextureDiffuseProp->GetValueAsString());
		else if (gChangeValues == 12)
			mVectorOfMaterials->at(pos)->SetTextureNormalName(mMatTextureNormalProp->GetValueAsString());
		else if (gChangeValues == 13)
			mVectorOfMaterials->at(pos)->SetTextureASHName(mMatTextureSpecularProp->GetValueAsString());
		else if (gChangeValues == 14)
			mVectorOfMaterials->at(pos)->SetTextureCubeName(mMatTextureCubeProp->GetValueAsString());
		gChangeValues = 0;

		mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName = mVectorOfMaterials->at(pos)->GetTextureDiffuseName();
		mKlmeshStruct->materialsBuffer.at(pos).normalMapName = mVectorOfMaterials->at(pos)->GetTextureNormalName();
		mKlmeshStruct->materialsBuffer.at(pos).ashMapName = mVectorOfMaterials->at(pos)->GetTextureASHName();
		mKlmeshStruct->materialsBuffer.at(pos).cubeMapName = mVectorOfMaterials->at(pos)->GetTextureCubeName();

		mKLMESH->SetMaterialDiffuseMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName);
		mKLMESH->SetMaterialNormalMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).normalMapName);
		mKLMESH->SetMaterialASHMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).ashMapName);
		mKLMESH->SetMaterialCubeMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).cubeMapName);

		mKLMESH_render->SetMaterialDiffuseMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName);
		mKLMESH_render->SetMaterialNormalMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).normalMapName);
		mKLMESH_render->SetMaterialASHMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).ashMapName);
		mKLMESH_render->SetMaterialCubeMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).cubeMapName);

		mFileChanges = true;
		mDxAppKLMeshViever->SetRender(true); //Update render
	}
}
void KLMESHViewer::OnTabMatGridPropertyChanged(wxPropertyGridEvent& WXUNUSED(evt))
{
	//Show this LOD level
	mForceLODLevel = -1;
	mDxAppKLMeshViever->SetForceLODLevel(mForceLODLevel);

	mUsedShaderProp->SetValueFromString(mChooseShaderProp->GetDisplayedString());
	int pos = mListboxMaterial->GetSelection();
	mVectorOfMaterials->at(pos)->SetMatName(mSelectedMaterialProp->GetValueAsString());
	mVectorOfMaterials->at(pos)->SetShaderName(mChooseShaderProp->GetDisplayedString());

	//Reset textures name
	mMatTextureDiffuseProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureDiffuseName());
	mMatTextureSpecularProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureASHName());
	mMatTextureNormalProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureNormalName());
	mMatTextureCubeProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureCubeName());


	int valueR = 0;
	int valueG = 0;
	int valueB = 0;

	valueR = mAmbient_R_ColourProp->GetValue().GetInteger();
	if (valueR < 0)
		valueR = 0;
	if (valueR > 255)
		valueR = 255;

	valueG = mAmbient_G_ColourProp->GetValue().GetInteger();
	if (valueG < 0)
		valueG = 0;
	if (valueG > 255)
		valueG = 255;

	valueB = mAmbient_B_ColourProp->GetValue().GetInteger();
	if (valueB < 0)
		valueB = 0;
	if (valueB > 255)
		valueB = 255;


	wxColour ambientColour(valueR, valueG, valueB);


	valueR = mDiffuse_R_ColourProp->GetValue().GetInteger();
	if (valueR < 0)
		valueR = 0;
	if (valueR > 255)
		valueR = 255;

	valueG = mDiffuse_G_ColourProp->GetValue().GetInteger();
	if (valueG < 0)
		valueG = 0;
	if (valueG > 255)
		valueG = 255;

	valueB = mDiffuse_B_ColourProp->GetValue().GetInteger();
	if (valueB < 0)
		valueB = 0;
	if (valueB > 255)
		valueB = 255;

	wxColour diffuseColour(valueR, valueG, valueB);

	valueR = mSpecular_R_ColourProp->GetValue().GetInteger();
	if (valueR < 0)
		valueR = 0;
	if (valueR > 255)
		valueR = 255;

	valueG = mSpecular_G_ColourProp->GetValue().GetInteger();
	if (valueG < 0)
		valueG = 0;
	if (valueG > 255)
		valueG = 255;

	valueB = mSpecular_B_ColourProp->GetValue().GetInteger();
	if (valueB < 0)
		valueB = 0;
	if (valueB > 255)
		valueB = 255;

	wxColour specularColour(valueR, valueG, valueB);

	valueR = mReflect_R_ColourProp->GetValue().GetInteger();
	if (valueR < 0)
		valueR = 0;
	if (valueR > 255)
		valueR = 255;

	valueG = mReflect_G_ColourProp->GetValue().GetInteger();
	if (valueG < 0)
		valueG = 0;
	if (valueG > 255)
		valueG = 255;

	valueB = mReflect_B_ColourProp->GetValue().GetInteger();
	if (valueB < 0)
		valueB = 0;
	if (valueB > 255)
		valueB = 255;
	wxColour reflectColour(valueR, valueG, valueB);

	mVectorOfMaterials->at(pos)->SetAmbientColour(ambientColour);
	mVectorOfMaterials->at(pos)->SetDiffuseColour(diffuseColour);
	mVectorOfMaterials->at(pos)->SetSpecularColour(specularColour);
	mVectorOfMaterials->at(pos)->SetReflectColour(reflectColour);


	mAmbient_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetAmbientColour().Red());
	mAmbient_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetAmbientColour().Green());
	mAmbient_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetAmbientColour().Blue());
	mDiffuse_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetDiffuseColour().Red());
	mDiffuse_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetDiffuseColour().Green());
	mDiffuse_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetDiffuseColour().Blue());
	mSpecular_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularColour().Red());
	mSpecular_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularColour().Green());
	mSpecular_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularColour().Blue());
	mReflect_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetReflectColour().Red());
	mReflect_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetReflectColour().Green());
	mReflect_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetReflectColour().Blue());
	


	float transparency = mTransparencyProp->GetValue().GetDouble();
	if (transparency < 0.0f)
		transparency = 0.0f;
	if (transparency > 1.0f)
		transparency = 1.0f;
	mVectorOfMaterials->at(pos)->SetTransparency(transparency);

	mVectorOfMaterials->at(pos)->SetSpecularPower(mSpecularPowerProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetRoughness(mRoughnessProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetFresnelFactor(mFresnelFactorProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetTessellationHeightScale(mTessellationHeightScaleProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetTessellationMaxDistance(mTessellationDistanceMaxProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetTessellationMinDistance(mTessellationDistanceMinProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetTessellationMaxFactor(mTessellationFactorMaxProp->GetValue().GetDouble());
	mVectorOfMaterials->at(pos)->SetTessellationMinFactor(mTessellationFactorMinProp->GetValue().GetDouble());

	mVectorOfMaterials->at(pos)->SetCubeMapDynamic(mCubeMapDynamicProp->GetValue().GetBool());

	wxString text = mUsedShaderProp->GetValueAsString();
	if (text != "Phong Tessellation" && text != "Cook-Torrance Tessellation")
		mTessellationProp->Hide(true);
	else
		mTessellationProp->Hide(false);

	if (mUsedShaderProp->GetValueAsString() != "Cook-Torrance" && mUsedShaderProp->GetValueAsString() != "Cook-Torrance Transparency" && mUsedShaderProp->GetValueAsString() != "Cook-Torrance Tessellation")
	{
		mSpecularPowerProp->Hide(false);
		mRoughnessProp->Hide(true);
		mFresnelFactorProp->Hide(true);
	}
	else
	{
		mSpecularPowerProp->Hide(true);
		mRoughnessProp->Hide(false);
		mFresnelFactorProp->Hide(false);
	}

	if (mUsedShaderProp->GetValueAsString() != "Cook-Torrance Transparency" && mUsedShaderProp->GetValueAsString() != "Phong Transparency")
	{
		mTransparencyProp->Hide(true);
	}
	else
	{
		mTransparencyProp->Hide(false);
	}


	//Update material in the KLMESH structure 
	mKlmeshStruct->materialsBuffer.at(pos).ambientColour.x = (float)mVectorOfMaterials->at(pos)->GetAmbientColour().Red() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).ambientColour.y = (float)mVectorOfMaterials->at(pos)->GetAmbientColour().Green() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).ambientColour.z = (float)mVectorOfMaterials->at(pos)->GetAmbientColour().Blue() / 255.0;

	mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.x = (float)mVectorOfMaterials->at(pos)->GetDiffuseColour().Red() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.y = (float)mVectorOfMaterials->at(pos)->GetDiffuseColour().Green() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.z = (float)mVectorOfMaterials->at(pos)->GetDiffuseColour().Blue() / 255.0;

	mKlmeshStruct->materialsBuffer.at(pos).specularColour.x = (float)mVectorOfMaterials->at(pos)->GetSpecularColour().Red() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).specularColour.y = (float)mVectorOfMaterials->at(pos)->GetSpecularColour().Green() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).specularColour.z = (float)mVectorOfMaterials->at(pos)->GetSpecularColour().Blue() / 255.0;

	mKlmeshStruct->materialsBuffer.at(pos).reflectColour.x = (float)mVectorOfMaterials->at(pos)->GetReflectColour().Red() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).reflectColour.y = (float)mVectorOfMaterials->at(pos)->GetReflectColour().Green() / 255.0;
	mKlmeshStruct->materialsBuffer.at(pos).reflectColour.z = (float)mVectorOfMaterials->at(pos)->GetReflectColour().Blue() / 255.0;

	mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName = mVectorOfMaterials->at(pos)->GetTextureDiffuseName();
	mKlmeshStruct->materialsBuffer.at(pos).normalMapName = mVectorOfMaterials->at(pos)->GetTextureNormalName();
	mKlmeshStruct->materialsBuffer.at(pos).ashMapName = mVectorOfMaterials->at(pos)->GetTextureASHName();
	mKlmeshStruct->materialsBuffer.at(pos).cubeMapName = mVectorOfMaterials->at(pos)->GetTextureCubeName();
	mKlmeshStruct->materialsBuffer.at(pos).fresnelFactor = mVectorOfMaterials->at(pos)->GetFresnelFactor();
	mKlmeshStruct->materialsBuffer.at(pos).roughness = mVectorOfMaterials->at(pos)->GetRoughness();
	mKlmeshStruct->materialsBuffer.at(pos).transparency = mVectorOfMaterials->at(pos)->GetTransparency();
	mKlmeshStruct->materialsBuffer.at(pos).specularPower = mVectorOfMaterials->at(pos)->GetSpecularPower();
	mKlmeshStruct->materialsBuffer.at(pos).heightScaleTess = mVectorOfMaterials->at(pos)->GetTessellationHeightScale();
	mKlmeshStruct->materialsBuffer.at(pos).maxDistanceTess = mVectorOfMaterials->at(pos)->GetTessellationMaxDistance();
	mKlmeshStruct->materialsBuffer.at(pos).minDistanceTess = mVectorOfMaterials->at(pos)->GetTessellationMinDistance();
	mKlmeshStruct->materialsBuffer.at(pos).maxFactorTess = mVectorOfMaterials->at(pos)->GetTessellationMaxFactor();
	mKlmeshStruct->materialsBuffer.at(pos).minFactorTess = mVectorOfMaterials->at(pos)->GetTessellationMinFactor();
	mKlmeshStruct->materialsBuffer.at(pos).materialName = mVectorOfMaterials->at(pos)->GetMatName();
	mKlmeshStruct->materialsBuffer.at(pos).shaderName = mVectorOfMaterials->at(pos)->GetShaderName();
	mKlmeshStruct->materialsBuffer.at(pos).cubeMapDynamic = mVectorOfMaterials->at(pos)->GetCubeMapDynamic();



	mKLMESH->SetMaterialAmbientColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).ambientColour.x, mKlmeshStruct->materialsBuffer.at(pos).ambientColour.y, mKlmeshStruct->materialsBuffer.at(pos).ambientColour.z, 1.0f));
	mKLMESH->SetMaterialDiffuseColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.x, mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.y, mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.z, 1.0f));
	mKLMESH->SetMaterialSpecularColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).specularColour.x, mKlmeshStruct->materialsBuffer.at(pos).specularColour.y, mKlmeshStruct->materialsBuffer.at(pos).specularColour.z, 1.0f));
	mKLMESH->SetMaterialReflectColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).reflectColour.x, mKlmeshStruct->materialsBuffer.at(pos).reflectColour.y, mKlmeshStruct->materialsBuffer.at(pos).reflectColour.z, 1.0f));
	mKLMESH->SetMaterialFresnelFactor(pos, mKlmeshStruct->materialsBuffer.at(pos).fresnelFactor);
	mKLMESH->SetMaterialRoughness(pos, mKlmeshStruct->materialsBuffer.at(pos).roughness);
	mKLMESH->SetMaterialTransparency(pos, mKlmeshStruct->materialsBuffer.at(pos).transparency);
	mKLMESH->SetMaterialSpecularPower(pos, mKlmeshStruct->materialsBuffer.at(pos).specularPower);
	mKLMESH->SetMaterialShaderName(pos, mKlmeshStruct->materialsBuffer.at(pos).shaderName);
	mKLMESH->SetMaterialHeightScaleTess(pos, mKlmeshStruct->materialsBuffer.at(pos).heightScaleTess);
	mKLMESH->SetMaterialMaxDistanceTess(pos, mKlmeshStruct->materialsBuffer.at(pos).maxDistanceTess);
	mKLMESH->SetMaterialMinDistanceTess(pos, mKlmeshStruct->materialsBuffer.at(pos).minDistanceTess);
	mKLMESH->SetMaterialMaxFactorTess(pos, mKlmeshStruct->materialsBuffer.at(pos).maxFactorTess);
	mKLMESH->SetMaterialMinFactorTess(pos, mKlmeshStruct->materialsBuffer.at(pos).minFactorTess);
	mKLMESH->SetMaterialDiffuseMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName);
	mKLMESH->SetMaterialNormalMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).normalMapName);
	mKLMESH->SetMaterialASHMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).ashMapName);
	mKLMESH->SetMaterialCubeMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).cubeMapName);
	mKLMESH->SetMaterialCubeMapDynamic(pos, mKlmeshStruct->materialsBuffer.at(pos).cubeMapDynamic);
	//mKLMESH->GetMaterialBuffer()->at(pos).diffuseMapName = mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName;
	//mKLMESH->GetMaterialBuffer()->at(pos).ashMapName = mKlmeshStruct->materialsBuffer.at(pos).ashMapName;
	//mKLMESH->GetMaterialBuffer()->at(pos).normalMapName = mKlmeshStruct->materialsBuffer.at(pos).normalMapName;


	mKLMESH_render->SetMaterialAmbientColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).ambientColour.x, mKlmeshStruct->materialsBuffer.at(pos).ambientColour.y, mKlmeshStruct->materialsBuffer.at(pos).ambientColour.z, 1.0f));
	mKLMESH_render->SetMaterialDiffuseColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.x, mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.y, mKlmeshStruct->materialsBuffer.at(pos).diffuseColour.z, 1.0f));
	mKLMESH_render->SetMaterialSpecularColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).specularColour.x, mKlmeshStruct->materialsBuffer.at(pos).specularColour.y, mKlmeshStruct->materialsBuffer.at(pos).specularColour.z, 1.0f));
	mKLMESH_render->SetMaterialReflectColour(pos, XMFLOAT4(mKlmeshStruct->materialsBuffer.at(pos).reflectColour.x, mKlmeshStruct->materialsBuffer.at(pos).reflectColour.y, mKlmeshStruct->materialsBuffer.at(pos).reflectColour.z, 1.0f));
	mKLMESH_render->SetMaterialFresnelFactor(pos, mKlmeshStruct->materialsBuffer.at(pos).fresnelFactor);
	mKLMESH_render->SetMaterialRoughness(pos, mKlmeshStruct->materialsBuffer.at(pos).roughness);
	mKLMESH_render->SetMaterialTransparency(pos, mKlmeshStruct->materialsBuffer.at(pos).transparency);
	mKLMESH_render->SetMaterialSpecularPower(pos, mKlmeshStruct->materialsBuffer.at(pos).specularPower);
	mKLMESH_render->SetMaterialShaderName(pos, mKlmeshStruct->materialsBuffer.at(pos).shaderName);
	mKLMESH_render->SetMaterialHeightScaleTess(pos, mKlmeshStruct->materialsBuffer.at(pos).heightScaleTess);
	mKLMESH_render->SetMaterialMaxDistanceTess(pos, mKlmeshStruct->materialsBuffer.at(pos).maxDistanceTess);
	mKLMESH_render->SetMaterialMinDistanceTess(pos, mKlmeshStruct->materialsBuffer.at(pos).minDistanceTess);
	mKLMESH_render->SetMaterialMaxFactorTess(pos, mKlmeshStruct->materialsBuffer.at(pos).maxFactorTess);
	mKLMESH_render->SetMaterialMinFactorTess(pos, mKlmeshStruct->materialsBuffer.at(pos).minFactorTess);
	mKLMESH_render->SetMaterialDiffuseMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).diffuseMapName);
	mKLMESH_render->SetMaterialNormalMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).normalMapName);
	mKLMESH_render->SetMaterialASHMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).ashMapName);
	mKLMESH_render->SetMaterialCubeMapName(pos, mKlmeshStruct->materialsBuffer.at(pos).cubeMapName);
	mKLMESH_render->SetMaterialCubeMapDynamic(pos, mKlmeshStruct->materialsBuffer.at(pos).cubeMapDynamic);

	mFileChanges = true;
	mDxAppKLMeshViever->SetRender(true); //Update render
	Refresh();
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnTabMatSelectedMaterialList(wxCommandEvent& WXUNUSED(evt))
{
	//Show this LOD level
	mForceLODLevel = -1;
	mDxAppKLMeshViever->SetForceLODLevel(mForceLODLevel);
	int pos = mListboxMaterial->GetSelection();
	wxString textShader = mVectorOfMaterials->at(pos)->GetShaderName();


	//Select material --> set to mDxAppKLMeshViever
	mSelectedMaterialPos = pos;
	mDxAppKLMeshViever->SetSelectedMaterialPos(mSelectedMaterialPos);

	if (textShader == "Phong")
	{
		mChooseShaderProp->SetChoiceSelection(0);
	}
	else if (textShader == "Phong Transparency")
	{
		mChooseShaderProp->SetChoiceSelection(1);
	}
	else if (textShader == "Phong Tessellation")
	{
		mChooseShaderProp->SetChoiceSelection(2);
	}
	else if (textShader == "Cook-Torrance")
	{
		mChooseShaderProp->SetChoiceSelection(3);
	}
	else if (textShader == "Cook-Torrance Transparency")
	{
		mChooseShaderProp->SetChoiceSelection(4);
	}
	else if (textShader == "Cook-Torrance Tessellation")
	{
		mChooseShaderProp->SetChoiceSelection(5);
	}

	mSelectedMaterialProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetMatName());
	mUsedShaderProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetShaderName());
	mMatTextureDiffuseProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureDiffuseName());
	mMatTextureSpecularProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureASHName());
	mMatTextureNormalProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureNormalName());
	mMatTextureCubeProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureCubeName());
	//mMatTextureHeightProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureHeightName());
	//mMatTextureAlphaProp->SetValueFromString(mVectorOfMaterials->at(pos)->GetTextureAlphaName());
	mAmbient_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetAmbientColour().Red());
	mAmbient_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetAmbientColour().Green());
	mAmbient_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetAmbientColour().Blue());
	mDiffuse_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetDiffuseColour().Red());
	mDiffuse_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetDiffuseColour().Green());
	mDiffuse_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetDiffuseColour().Blue());
	mSpecular_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularColour().Red());
	mSpecular_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularColour().Green());
	mSpecular_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularColour().Blue());
	mSpecularPowerProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetSpecularPower());

	mReflect_R_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetReflectColour().Red());
	mReflect_G_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetReflectColour().Green());
	mReflect_B_ColourProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetReflectColour().Blue());

	//Transparency
	wxString text = "";
	text << mVectorOfMaterials->at(pos)->GetTransparency();
	mTransparencyProp->SetValueFromString(text);

	//oughness
	text = "";
	text << mVectorOfMaterials->at(pos)->GetRoughness();
	mRoughnessProp->SetValueFromString(text);

	//cubeMapDynamic
	if(mVectorOfMaterials->at(pos)->GetCubeMapDynamic() == true)
		mCubeMapDynamicProp->SetValueFromString("True");
	else
		mCubeMapDynamicProp->SetValueFromString("False");

	//FresnelFactor
	text = "";
	text << mVectorOfMaterials->at(pos)->GetFresnelFactor();
	mFresnelFactorProp->SetValueFromString(text);

	if (mUsedShaderProp->GetValueAsString() != "Phong Tessellation" && mUsedShaderProp->GetValueAsString() != "Cook-Torrance Tessellation")
		mTessellationProp->Hide(true);
	else
		mTessellationProp->Hide(false);

	if (mUsedShaderProp->GetValueAsString() != "Cook-Torrance" && mUsedShaderProp->GetValueAsString() != "Cook-Torrance Transparency" && mUsedShaderProp->GetValueAsString() != "Cook-Torrance Tessellation")
	{
		mSpecularPowerProp->Hide(false);
		mRoughnessProp->Hide(true);
		mFresnelFactorProp->Hide(true);
	}
	else
	{
		mSpecularPowerProp->Hide(true);
		mRoughnessProp->Hide(false);
		mFresnelFactorProp->Hide(false);
	}

	if (mUsedShaderProp->GetValueAsString() != "Cook-Torrance Transparency" && mUsedShaderProp->GetValueAsString() != "Phong Transparency")
	{
		mTransparencyProp->Hide(true);
	}
	else
	{
		mTransparencyProp->Hide(false);
	}


	//TessellationHeightScale
	text = "";
	text << mVectorOfMaterials->at(pos)->GetTessellationHeightScale();
	mTessellationHeightScaleProp->SetValueFromString(text);

	//TessellationMaxDistance
	text = "";
	text << mVectorOfMaterials->at(pos)->GetTessellationMaxDistance();
	mTessellationDistanceMaxProp->SetValueFromString(text);

	//TessellationMinDistance
	text = "";
	text << mVectorOfMaterials->at(pos)->GetTessellationMinDistance();
	mTessellationDistanceMinProp->SetValueFromString(text);

	//TessellationMaxFactor
	text = "";
	text << mVectorOfMaterials->at(pos)->GetTessellationMaxFactor();
	mTessellationFactorMaxProp->SetValueFromString(text);

	//TessellationMinFactor
	text = "";
	text << mVectorOfMaterials->at(pos)->GetTessellationMinFactor();
	mTessellationFactorMinProp->SetValueFromString(text);

	mInfoVerticesProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetNumberOfVertices());
	mInfoTrianglesProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetNumberOfTriangles());
	mInfoIndicesProp->SetValueFromInt(mVectorOfMaterials->at(pos)->GetNumberOfIndices());

	mFileChanges = true;
	mDxAppKLMeshViever->SetRender(true); //Update render
	Refresh();
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnTabLODGridPropertyChanged(wxPropertyGridEvent& WXUNUSED(evt))
{
	int pos = mListboxLODs->GetSelection();
	int maxPos = mListboxLODs->GetCount();
	int distance = mLOD_DistanceProp->GetValue().GetInteger();

	//Show this LOD level
	mForceLODLevel = pos;
	mSelectedLOD = pos;
	mDxAppKLMeshViever->SetForceLODLevel(mForceLODLevel);

	if (pos == 0)
	{
		if (distance != 0)
		{
			distance = 0;
		}
	}
	else if (pos == 1 && maxPos == 3) //exists LOD1 & LOD2
	{
		if (distance < 0 || distance > (int)mVectorOfLods->at(2)->GetDistance())
		{
			distance = mVectorOfLods->at(pos)->GetDistance();
		}
	}
	else if (pos == 2 && maxPos == 3) //exists LOD2
	{
		if (distance < 0 || distance <= (int)mVectorOfLods->at(1)->GetDistance())
		{
			distance = mVectorOfLods->at(pos)->GetDistance();
		}
	}

	wxString LodText = "LOD ";
	LodText = "LOD ";
	LodText << pos << " [Distance: ";
	LodText << distance;
	LodText << "m]";


	mListboxLODs->SetString(pos, LodText);
		
	mLOD_DistanceProp->SetValueFromInt(distance);
	mVectorOfLods->at(pos)->SetDistance(distance);
	mVectorOfLods->at(pos)->SetNumberOfTriangles(mLOD_TrianglesProp->GetValue().GetInteger());
	mVectorOfLods->at(pos)->SetNumberOfVertices(mLOD_VerticesProp->GetValue().GetInteger());
	mVectorOfLods->at(pos)->SetNumberOfIndices(mLOD_IndicesProp->GetValue().GetInteger());
	mVectorOfLods->at(pos)->SetNumberOfChunks(mLOD_ChunksProp->GetValue().GetInteger());
	mVectorOfLods->at(pos)->SetNumberOfMaterials(mLOD_MaterialProp->GetValue().GetInteger());


	//Update lod distance in the KLMESH structure
	if (pos == 0)
		mKlmeshStruct->mDistanceLOD0 = distance;
	if (pos == 1)
		mKlmeshStruct->mDistanceLOD1 = distance;
	if (pos == 2)
		mKlmeshStruct->mDistanceLOD2 = distance;


	mKLMESH->SetDistanceLOD0(mKlmeshStruct->mDistanceLOD0);
	mKLMESH->SetDistanceLOD1(mKlmeshStruct->mDistanceLOD1);
	mKLMESH->SetDistanceLOD1(mKlmeshStruct->mDistanceLOD2);

	mKLMESH_render->SetDistanceLOD0(mKlmeshStruct->mDistanceLOD0);
	mKLMESH_render->SetDistanceLOD1(mKlmeshStruct->mDistanceLOD1);
	mKLMESH_render->SetDistanceLOD1(mKlmeshStruct->mDistanceLOD2);

	mDxAppKLMeshViever->SetRender(true); //Update render
	mFileChanges = true;
	Refresh();
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnTabLODSelectedList(wxCommandEvent& WXUNUSED(evt))
{
	int pos = mListboxLODs->GetSelection();

	//Show this LOD level
	mForceLODLevel = pos;
	mSelectedLOD = pos;
	mDxAppKLMeshViever->SetForceLODLevel(mForceLODLevel);

	mLOD_DistanceProp->SetValueFromInt(mVectorOfLods->at(pos)->GetDistance());
	mLOD_TrianglesProp->SetValueFromInt(mVectorOfLods->at(pos)->GetNumberOfTriangles());
	mLOD_VerticesProp->SetValueFromInt(mVectorOfLods->at(pos)->GetNumberOfVertices());
	mLOD_IndicesProp->SetValueFromInt(mVectorOfLods->at(pos)->GetNumberOfIndices());
	mLOD_ChunksProp->SetValueFromInt(mVectorOfLods->at(pos)->GetNumberOfChunks());
	mLOD_MaterialProp->SetValueFromInt(mVectorOfLods->at(pos)->GetNumberOfMaterials());
	mDxAppKLMeshViever->SetRender(true); //Update render
	Refresh();
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnPaint(wxPaintEvent & WXUNUSED(evt))
{
	wxPaintDC dc(mPanelRender);
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnClose(wxCloseEvent& WXUNUSED(evt))
{
	Close();
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnMenuClose(wxCommandEvent& WXUNUSED(evt))
{
	Close();
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnMenuSave(wxCommandEvent& evt)
{
	BinaryFileWrite* binaryFileWrite = new BinaryFileWrite(mKlmeshStruct->name);
	binaryFileWrite->writeKLMeshFileStructure(*mKlmeshStruct);
	binaryFileWrite->closeFile();
	delete binaryFileWrite;
	wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("SAVED"), wxT("INFO"), wxOK | wxICON_INFORMATION);
	errorMsg->ShowModal();
	errorMsg->Destroy();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mFileChanges = false;
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnMenuZoom(wxCommandEvent& WXUNUSED(evt))
{
	mDxAppKLMeshViever->SetCameraView();
	mDxAppKLMeshViever->SetRender(true); //Update render
}
void KLMESHViewer::OnMenuBBox(wxCommandEvent& WXUNUSED(evt))
{
	if (mShowBBox == true)
	{
		mDxAppKLMeshViever->ShowBoundingBox(false);
		mDxAppKLMeshViever->SetRender(true); //Update render
		mShowBBox = false;
	}
	else
	{
		mDxAppKLMeshViever->ShowBoundingBox(true);
		mDxAppKLMeshViever->SetRender(true); //Update render
		mShowBBox = true;
	}
		
}
void KLMESHViewer::OnMenuCollision(wxCommandEvent& WXUNUSED(evt))
{
	if (mShowCollision == true)
	{
		mDxAppKLMeshViever->ShowCollision(false);
		mDxAppKLMeshViever->SetRender(true); //Update render
		mShowCollision = false;
	}
	else
	{
		mDxAppKLMeshViever->ShowCollision(true);
		mDxAppKLMeshViever->SetRender(true); //Update render
		mShowCollision = true;
	}

}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnMenuReimport(wxCommandEvent& evt)
{
	wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("Reimport"), wxT("INFO"), wxOK | wxICON_INFORMATION);
	errorMsg->ShowModal();
	errorMsg->Destroy();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mFileChanges = true;
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnSliderLight(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	value = value - 180;

	float radius = max(max(abs(mKlmeshStruct->boundingBoxMax.x) * 4.0f, abs(mKlmeshStruct->boundingBoxMin.x) * 4.0f), max(abs(mKlmeshStruct->boundingBoxMax.z) * 4.0f, abs(mKlmeshStruct->boundingBoxMin.z) * 4.0f));

	float x = radius * cos(XMConvertToRadians(value));

	float y = mKlmeshStruct->boundingBoxMax.y + ((mKlmeshStruct->boundingBoxMax.y + mKlmeshStruct->boundingBoxMin.y) * 2.0f);
	//float y = mKlmeshStruct->boundingBoxMax.y;
	float z = radius * sin(XMConvertToRadians(value));

	mDxAppKLMeshViever->SetRender(true); //Update render

	//XMFLOAT3 dir;

	//XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(-x, -y, -z))));

	mDxAppKLMeshViever->SetLightPosition(XMFLOAT3(x, y, z));
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnSliderLightColorR(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightColorR(value);
}
void KLMESHViewer::OnSliderLightColorG(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightColorG(value);
}
void KLMESHViewer::OnSliderLightColorB(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightColorB(value);
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnSliderLightColorSpecR(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightColorSpecR(value);
}
void KLMESHViewer::OnSliderLightColorSpecG(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightColorSpecG(value);
}
void KLMESHViewer::OnSliderLightColorSpecB(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightColorSpecB(value);
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnSliderLightPower(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightPower(value);
}
void KLMESHViewer::OnSliderLightSpec(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetLightSpec(value);
}
void KLMESHViewer::OnSliderRotation(wxScrollEvent& evt)
{
	int value = evt.GetPosition();
	value -= 180;
	mDxAppKLMeshViever->SetRender(true); //Update render
	mDxAppKLMeshViever->SetRotation(value);
}





//***********************************************************************************************************************************************************************
wxHtmlWindow* KLMESHViewer::CreateHTMLCtrl(wxWindow* parent)
{
	if (!parent)
		parent = this;

	wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize);
	ctrl->SetPage(GetIntroText());
	return ctrl;
}
//***********************************************************************************************************************************************************************
wxString KLMESHViewer::GetIntroText()
{
	wxString textMain = "<html><body>"
		"<h3>Welcome to KalenEngine</h3>"

		"<br/><br/>"
		"<h3><br/><b>     KalenEngine - Build 1.0.1 (January 2017)</b><br/></h3>"
		"<h3><br/><b>          DirectX 11</b><br/></h3>"
		"<h3><br/><b>          wxWidgets v3.0</b><br/></h3>"
		"<br/><br/>"
		"<h3><br/><b>Created by: Kamil Leniart</b><br/></h3>"
		"</body></html>";
	const char* text = textMain;
	return wxString::FromAscii(text);
}
/*void KLMESHViewer::SetAssetBrowser(AssetBrowser* assetBrowser)
{
	//this->mAssetBrowser = assetBrowser;
}*/
//***********************************************************************************************************************************************************************
void KLMESHViewer::SetLinkToTexture(wxString link, wxString name)
{
	mLinkToSelectedTexture = link;
	mSelectedTextureName = name;
	gLinkToTexture = link;
	mDxAppKLMeshViever->SetRender(true); //Update render
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::SetKLMESH(KLMESH* klmesh)
{
	mDxAppKLMeshViever->SetAllowRender(false); //clock render



	//Temporary object to swap data.
	
	mKLMESH_TMP = new KLMESH(mKlmeshStruct, mDevice);
	for (int i = 0; i < (int)klmesh->GetMaterialBuffer()->size(); ++i)
	{
		mKLMESH_TMP->GetMaterialBuffer()->at(i).alphaColour = mKLMESH->GetMaterialBuffer()->at(i).alphaColour;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).ambientColour = mKLMESH->GetMaterialBuffer()->at(i).ambientColour;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).ashMap = mKLMESH->GetMaterialBuffer()->at(i).ashMap;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).ashMapName = mKLMESH->GetMaterialBuffer()->at(i).ashMapName;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).cubeMap = mKLMESH->GetMaterialBuffer()->at(i).cubeMap;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).cubeMapName = mKLMESH->GetMaterialBuffer()->at(i).cubeMapName;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).diffuseColour = mKLMESH->GetMaterialBuffer()->at(i).diffuseColour;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).diffuseMap = mKLMESH->GetMaterialBuffer()->at(i).diffuseMap;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).fresnelFactor = mKLMESH->GetMaterialBuffer()->at(i).fresnelFactor;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).roughness = mKLMESH->GetMaterialBuffer()->at(i).roughness;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).transparency = mKLMESH->GetMaterialBuffer()->at(i).transparency;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).hasTessellation = mKLMESH->GetMaterialBuffer()->at(i).hasTessellation;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).heightScaleTess = mKLMESH->GetMaterialBuffer()->at(i).heightScaleTess;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).materialHlsl = mKLMESH->GetMaterialBuffer()->at(i).materialHlsl;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).materialName = mKLMESH->GetMaterialBuffer()->at(i).materialName;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).maxDistanceTess = mKLMESH->GetMaterialBuffer()->at(i).maxDistanceTess;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).maxFactorTess = mKLMESH->GetMaterialBuffer()->at(i).maxFactorTess;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).minDistanceTess = mKLMESH->GetMaterialBuffer()->at(i).minDistanceTess;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).minFactorTess = mKLMESH->GetMaterialBuffer()->at(i).minFactorTess;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).normalMap = mKLMESH->GetMaterialBuffer()->at(i).normalMap;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).normalMapName = mKLMESH->GetMaterialBuffer()->at(i).normalMapName;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).shaderName = mKLMESH->GetMaterialBuffer()->at(i).shaderName;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).specularColour = mKLMESH->GetMaterialBuffer()->at(i).specularColour;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).specularPower = mKLMESH->GetMaterialBuffer()->at(i).specularPower;
		mKLMESH_TMP->GetMaterialBuffer()->at(i).cubeMapDynamic = mKLMESH->GetMaterialBuffer()->at(i).cubeMapDynamic;
	}
	mKLMESH_TMP->SetBoundingBoxMax(mKLMESH->GetBoundingBoxMax());
	mKLMESH_TMP->SetBoundingBoxMin(mKLMESH->GetBoundingBoxMin());
	mKLMESH_TMP->SetDistanceHide(mKLMESH->GetDistanceHide());
	mKLMESH_TMP->SetDistanceLOD0(mKLMESH->GetDistanceLOD0());
	mKLMESH_TMP->SetDistanceLOD1(mKLMESH->GetDistanceLOD1());
	mKLMESH_TMP->SetDistanceLOD2(mKLMESH->GetDistanceLOD2());
	mKLMESH_TMP->SetShadowDistance(mKLMESH->GetShadowDistance());

	//klmesh - input object from main window
	//mKLMESH - rendering object in the klmeshViewer.
	//Swap pointer. 
	mKLMESH = klmesh;
	//Set changed values
	klmesh->SetIsAllowRender(false);
	for (int i = 0; i < (int)klmesh->GetMaterialBuffer()->size(); ++i)
	{
		klmesh->GetMaterialBuffer()->at(i).alphaColour = mKLMESH_TMP->GetMaterialBuffer()->at(i).alphaColour;
		klmesh->GetMaterialBuffer()->at(i).ambientColour = mKLMESH_TMP->GetMaterialBuffer()->at(i).ambientColour;
		klmesh->GetMaterialBuffer()->at(i).ashMap = mKLMESH_TMP->GetMaterialBuffer()->at(i).ashMap;
		klmesh->GetMaterialBuffer()->at(i).ashMapName = mKLMESH_TMP->GetMaterialBuffer()->at(i).ashMapName;
		klmesh->GetMaterialBuffer()->at(i).cubeMap = mKLMESH_TMP->GetMaterialBuffer()->at(i).cubeMap;
		klmesh->GetMaterialBuffer()->at(i).cubeMapName = mKLMESH_TMP->GetMaterialBuffer()->at(i).cubeMapName;
		klmesh->GetMaterialBuffer()->at(i).diffuseColour = mKLMESH_TMP->GetMaterialBuffer()->at(i).diffuseColour;
		klmesh->GetMaterialBuffer()->at(i).diffuseMap = mKLMESH_TMP->GetMaterialBuffer()->at(i).diffuseMap;
		klmesh->GetMaterialBuffer()->at(i).fresnelFactor = mKLMESH_TMP->GetMaterialBuffer()->at(i).fresnelFactor;
		klmesh->GetMaterialBuffer()->at(i).roughness = mKLMESH_TMP->GetMaterialBuffer()->at(i).roughness;
		klmesh->GetMaterialBuffer()->at(i).transparency = mKLMESH_TMP->GetMaterialBuffer()->at(i).transparency;
		klmesh->GetMaterialBuffer()->at(i).hasTessellation = mKLMESH_TMP->GetMaterialBuffer()->at(i).hasTessellation;
		klmesh->GetMaterialBuffer()->at(i).heightScaleTess = mKLMESH_TMP->GetMaterialBuffer()->at(i).heightScaleTess;
		klmesh->GetMaterialBuffer()->at(i).materialHlsl = mKLMESH_TMP->GetMaterialBuffer()->at(i).materialHlsl;
		klmesh->GetMaterialBuffer()->at(i).materialName = mKLMESH_TMP->GetMaterialBuffer()->at(i).materialName;
		klmesh->GetMaterialBuffer()->at(i).maxDistanceTess = mKLMESH_TMP->GetMaterialBuffer()->at(i).maxDistanceTess;
		klmesh->GetMaterialBuffer()->at(i).maxFactorTess = mKLMESH_TMP->GetMaterialBuffer()->at(i).maxFactorTess;
		klmesh->GetMaterialBuffer()->at(i).minDistanceTess = mKLMESH_TMP->GetMaterialBuffer()->at(i).minDistanceTess;
		klmesh->GetMaterialBuffer()->at(i).minFactorTess = mKLMESH_TMP->GetMaterialBuffer()->at(i).minFactorTess;
		klmesh->GetMaterialBuffer()->at(i).normalMap = mKLMESH_TMP->GetMaterialBuffer()->at(i).normalMap;
		klmesh->GetMaterialBuffer()->at(i).normalMapName = mKLMESH_TMP->GetMaterialBuffer()->at(i).normalMapName;
		klmesh->GetMaterialBuffer()->at(i).shaderName = mKLMESH_TMP->GetMaterialBuffer()->at(i).shaderName;
		klmesh->GetMaterialBuffer()->at(i).specularColour = mKLMESH_TMP->GetMaterialBuffer()->at(i).specularColour;
		klmesh->GetMaterialBuffer()->at(i).specularPower = mKLMESH_TMP->GetMaterialBuffer()->at(i).specularPower;
		klmesh->GetMaterialBuffer()->at(i).cubeMapDynamic = mKLMESH_TMP->GetMaterialBuffer()->at(i).cubeMapDynamic;
	}
	klmesh->SetBoundingBoxMax(mKLMESH_TMP->GetBoundingBoxMax());
	klmesh->SetBoundingBoxMin(mKLMESH_TMP->GetBoundingBoxMin());
	klmesh->SetDistanceHide(mKLMESH_TMP->GetDistanceHide());
	klmesh->SetDistanceLOD0(mKLMESH_TMP->GetDistanceLOD0());
	klmesh->SetDistanceLOD1(mKLMESH_TMP->GetDistanceLOD1());
	klmesh->SetDistanceLOD2(mKLMESH_TMP->GetDistanceLOD2());
	klmesh->SetShadowDistance(mKLMESH_TMP->GetShadowDistance());

	klmesh->SetIsAllowRender(true);

	//Update renderer object in the klmeshviewer!
	for (int i = 0; i < (int)mKLMESH_render->GetMaterialBuffer()->size(); ++i)
	{
		mKLMESH_render->GetMaterialBuffer()->at(i).alphaColour = klmesh->GetMaterialBuffer()->at(i).alphaColour;
		mKLMESH_render->GetMaterialBuffer()->at(i).ambientColour = klmesh->GetMaterialBuffer()->at(i).ambientColour;
		mKLMESH_render->GetMaterialBuffer()->at(i).diffuseMapName = klmesh->GetMaterialBuffer()->at(i).diffuseMapName;
		//mKLMESH_render->GetMaterialBuffer()->at(i).diffuseMap = klmesh->GetMaterialBuffer()->at(i).diffuseMap;
		mKLMESH_render->GetMaterialBuffer()->at(i).ashMapName = klmesh->GetMaterialBuffer()->at(i).ashMapName;
	//	mKLMESH_render->GetMaterialBuffer()->at(i).ashMap = klmesh->GetMaterialBuffer()->at(i).ashMap;
		mKLMESH_render->GetMaterialBuffer()->at(i).normalMapName = klmesh->GetMaterialBuffer()->at(i).normalMapName;
		//mKLMESH_render->GetMaterialBuffer()->at(i).normalMap = klmesh->GetMaterialBuffer()->at(i).normalMap;
		mKLMESH_render->GetMaterialBuffer()->at(i).cubeMapName = klmesh->GetMaterialBuffer()->at(i).cubeMapName;
		//mKLMESH_render->GetMaterialBuffer()->at(i).cubeMap = klmesh->GetMaterialBuffer()->at(i).cubeMap;
		mKLMESH_render->GetMaterialBuffer()->at(i).diffuseColour = klmesh->GetMaterialBuffer()->at(i).diffuseColour;
		mKLMESH_render->GetMaterialBuffer()->at(i).fresnelFactor = klmesh->GetMaterialBuffer()->at(i).fresnelFactor;
		mKLMESH_render->GetMaterialBuffer()->at(i).roughness = klmesh->GetMaterialBuffer()->at(i).roughness;
		mKLMESH_render->GetMaterialBuffer()->at(i).transparency = klmesh->GetMaterialBuffer()->at(i).transparency;
		mKLMESH_render->GetMaterialBuffer()->at(i).hasTessellation = klmesh->GetMaterialBuffer()->at(i).hasTessellation;
		mKLMESH_render->GetMaterialBuffer()->at(i).heightScaleTess = klmesh->GetMaterialBuffer()->at(i).heightScaleTess;
		mKLMESH_render->GetMaterialBuffer()->at(i).materialHlsl = klmesh->GetMaterialBuffer()->at(i).materialHlsl;
		mKLMESH_render->GetMaterialBuffer()->at(i).materialName = klmesh->GetMaterialBuffer()->at(i).materialName;
		mKLMESH_render->GetMaterialBuffer()->at(i).maxDistanceTess = klmesh->GetMaterialBuffer()->at(i).maxDistanceTess;
		mKLMESH_render->GetMaterialBuffer()->at(i).maxFactorTess = klmesh->GetMaterialBuffer()->at(i).maxFactorTess;
		mKLMESH_render->GetMaterialBuffer()->at(i).minDistanceTess = klmesh->GetMaterialBuffer()->at(i).minDistanceTess;
		mKLMESH_render->GetMaterialBuffer()->at(i).minFactorTess = klmesh->GetMaterialBuffer()->at(i).minFactorTess;
		mKLMESH_render->GetMaterialBuffer()->at(i).shaderName = klmesh->GetMaterialBuffer()->at(i).shaderName;
		mKLMESH_render->GetMaterialBuffer()->at(i).specularColour = klmesh->GetMaterialBuffer()->at(i).specularColour;
		mKLMESH_render->GetMaterialBuffer()->at(i).specularPower = klmesh->GetMaterialBuffer()->at(i).specularPower;
		mKLMESH_render->GetMaterialBuffer()->at(i).cubeMapDynamic = klmesh->GetMaterialBuffer()->at(i).cubeMapDynamic;
	}
	mKLMESH_render->SetBoundingBoxMax(klmesh->GetBoundingBoxMax());
	mKLMESH_render->SetBoundingBoxMin(klmesh->GetBoundingBoxMin());
	mKLMESH_render->SetDistanceHide(klmesh->GetDistanceHide());
	mKLMESH_render->SetDistanceLOD0(klmesh->GetDistanceLOD0());
	mKLMESH_render->SetDistanceLOD1(klmesh->GetDistanceLOD1());
	mKLMESH_render->SetDistanceLOD2(klmesh->GetDistanceLOD2());
	mKLMESH_render->SetShadowDistance(klmesh->GetShadowDistance());
	
	mDxAppKLMeshViever->SetAllowRender(true); //unclock render
	mFileChanges = true;
	mDxAppKLMeshViever->SetRender(true); //Update render

}
//***********************************************************************************************************************************************************************
void KLMESHViewer::SetTexturesResource(TexturesResource* texturesResource)
{
	this->mTexturesResource = texturesResource;
	mDxAppKLMeshViever->SetRender(true); //Update render
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::LoadTexture(int matPosition, std::string textureName, int textureType)
{
	//textureType ==> 0 -> DiffuseMap; 1 -> NormalMap; 2 -> ASH_Map  mTexturesResourceRender
	if (textureType == 0)
	{
		mKLMESH_render->SetMaterialDiffuseMap(matPosition, mTexturesResourceRender->AddTextureToBuffor(textureName), textureName);
		mKLMESH->SetMaterialDiffuseMap(matPosition, mTexturesResource->AddTextureToBuffor(textureName), textureName);
	}
	else if (textureType == 1)
	{
		mKLMESH_render->SetMaterialNormalMap(matPosition, mTexturesResourceRender->AddTextureToBuffor(textureName), textureName);
		mKLMESH->SetMaterialNormalMap(matPosition, mTexturesResource->AddTextureToBuffor(textureName), textureName);
	}
	else if (textureType == 2)
	{
		mKLMESH_render->SetMaterialASHMap(matPosition, mTexturesResourceRender->AddTextureToBuffor(textureName), textureName);
		mKLMESH->SetMaterialASHMap(matPosition, mTexturesResource->AddTextureToBuffor(textureName), textureName);
	}
	else if (textureType == 3)
	{
		mKLMESH_render->SetMaterialCubeMap(matPosition, mTexturesResourceRender->AddTextureToBuffor(textureName), textureName);
		mKLMESH->SetMaterialCubeMap(matPosition, mTexturesResource->AddTextureToBuffor(textureName), textureName);
	}
		
	mDxAppKLMeshViever->SetRender(true); //Update render
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::SetDevice(ID3D11Device* device)
{
	this->mDevice = device;
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::CreateKLMESH()
{
	//KLMESH - render object
	mTexturesResourceRender = mDxAppKLMeshViever->CreateTexturesResourceToRender();
	mKLMESH_render = mDxAppKLMeshViever->CreateKLMESH_ToRender(mKlmeshStruct);

	//KLMESH - shared with main window
	mKLMESH = new KLMESH(mKlmeshStruct, mDevice);
	mKLMESH->CreateBufferVertexAndIndex(mDevice);
	mKLMESH->CreateBufferVertexAndIndexCollision(mDevice);

	for (int i = 0; i < (int)mKLMESH->GetMaterialBuffer()->size(); ++i)
	{
		if (mKLMESH->GetMaterialBuffer()->at(i).diffuseMapName != "NULL")
			LoadTexture(i, mKLMESH->GetMaterialBuffer()->at(i).diffuseMapName, 0);
		if (mKLMESH->GetMaterialBuffer()->at(i).normalMapName != "NULL")
			LoadTexture(i, mKLMESH->GetMaterialBuffer()->at(i).normalMapName, 1);
		if (mKLMESH->GetMaterialBuffer()->at(i).ashMapName != "NULL")
			LoadTexture(i, mKLMESH->GetMaterialBuffer()->at(i).ashMapName, 2);
		if (mKLMESH->GetMaterialBuffer()->at(i).cubeMapName != "NULL")
			LoadTexture(i, mKLMESH->GetMaterialBuffer()->at(i).cubeMapName, 3);
	}
	//mDxAppKLMeshViever->SetKLMESH(mKLMESH);

	mDxAppKLMeshViever->SetRender(true); //Update render
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::OnNotebookChanged(wxAuiNotebookEvent& evt)
{
	int value = evt.GetSelection();

	if (value == 0 || value == 1 || value == 3)
		mForceLODLevel = -1;
	else
	{
		mForceLODLevel = mSelectedLOD;
	}


	mDxAppKLMeshViever->SetForceLODLevel(mForceLODLevel);
	mDxAppKLMeshViever->SetRender(true); //Update render
}
void KLMESHViewer::OnCheckBoxClicked(wxCommandEvent& evt)
{
	if (evt.IsChecked())
	{
		this->mIsCheckBoxSelected = true;
		mDxAppKLMeshViever->SetIsCheckBoxSelected(mIsCheckBoxSelected);
	}
	else
	{
		this->mIsCheckBoxSelected = false;
		mDxAppKLMeshViever->SetIsCheckBoxSelected(mIsCheckBoxSelected);
	}
	mDxAppKLMeshViever->SetRender(true); //Update render
}
//***********************************************************************************************************************************************************************
std::string KLMESHViewer::GetName()
{
	return this->mKlmeshStruct->name;
}
//***********************************************************************************************************************************************************************
void KLMESHViewer::RestoreNoSavedKLMESH()
{
	CreateKLMESH();

}
//***********************************************************************************************************************************************************************
void KLMESHViewer::Close()
{
	if (mFileChanges == true)
	{
		int width = (wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2) - 150;
		int height = (wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 2) - 100;
		wxMessageDialog *dialog = new wxMessageDialog(NULL, wxT("Do you want to save changes before close the KLMESH-Viewer?\nClick 'Yes' to save and close, 'No' to close without saving, or 'Cancel' to not close"), wxT("Confirm close."), wxYES_NO | wxCANCEL | wxICON_WARNING, wxPoint(width, height));
		int value = dialog->ShowModal();

		if (value == wxID_YES) //Press 'yes' button
		{
			BinaryFileWrite* binaryFileWrite = new BinaryFileWrite(mKlmeshStruct->name);
			binaryFileWrite->writeKLMeshFileStructure(*mKlmeshStruct);
			binaryFileWrite->closeFile();
			delete binaryFileWrite;
			mFileChanges = false;
			delete mDxAppKLMeshViever;
			Hide();
		}  
		else
			if (value == wxID_NO) //Press 'no' button
			{
				RestoreNoSavedKLMESH();
				delete mDxAppKLMeshViever;
				Hide();
			}
			else
				if (value == wxID_CANCEL) //Press 'cancel' button
				{
				}
		dialog->Destroy();
	}
	else
	{
		delete mDxAppKLMeshViever;
		Hide();
	}
}
//***********************************************************************************************************************************************************************

