#include "../Header Files/FiltersWindow.h"

extern bool gFilterDebugGrid;
extern bool gFilterDebugGizmo;
extern bool gFilterRenderMesh;
extern bool gFilterDebugBBoxes;
extern bool gFilterDebugCollision;
extern bool gFilterDebugSelection;
extern bool gFilterDebugWireframe;
extern bool gFilterRenderTessellation;
extern bool gFilterRenderRefraction;
extern bool gFilterRenderRefractionDynamic;
extern bool gFilterDebugBSphereLight;
extern bool gFilterSSAO;
extern bool gFilterRenderShadow;

wxBEGIN_EVENT_TABLE(FiltersWindow, wxFrame)
	EVT_END_SESSION(FiltersWindow::OnClose)
	EVT_CLOSE(FiltersWindow::OnClose)
	EVT_TREE_ITEM_ACTIVATED(ID_TreeFilters, FiltersWindow::OnTreeLeftDoubleClick)
	EVT_CHECKBOX(ID_TreeFiltersDebugGrid, FiltersWindow::OnDebugGrid)
	EVT_CHECKBOX(ID_TreeFiltersDebugGizmo, FiltersWindow::OnDebugGizmo)
	EVT_CHECKBOX(ID_TreeFiltersRenderMesh, FiltersWindow::OnRenderMesh)
	EVT_CHECKBOX(ID_TreeFiltersRenderLighting, FiltersWindow::OnRenderLighting)
	EVT_CHECKBOX(ID_TreeFiltersRenderLights, FiltersWindow::OnRenderLights)
	EVT_CHECKBOX(ID_TreeFiltersRenderRefraction, FiltersWindow::OnRenderRefraction)
	EVT_CHECKBOX(ID_TreeFiltersRenderRefractionDynamic, FiltersWindow::OnRenderRefractionDynamic)
	EVT_CHECKBOX(ID_TreeFiltersRenderShadows, FiltersWindow::OnRenderShadows)
	EVT_CHECKBOX(ID_TreeFiltersRenderTessellation, FiltersWindow::OnRenderTessellation)

	EVT_CHECKBOX(ID_TreeFiltersDebugBBoxes, FiltersWindow::OnDebugBBoxes)
	EVT_CHECKBOX(ID_TreeFiltersDebugBSphereLight, FiltersWindow::OnDebugBSphereLight)
	EVT_CHECKBOX(ID_TreeFiltersDebugCollision, FiltersWindow::OnDebugCollision)
	EVT_CHECKBOX(ID_TreeFiltersDebugSelection, FiltersWindow::OnDebugSelection)
	EVT_CHECKBOX(ID_TreeFiltersDebugWireframe, FiltersWindow::OnDebugWireframe)
	EVT_CHECKBOX(ID_TreeFiltersSSAO, FiltersWindow::OnSSAO)
	

	
	EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, FiltersWindow::OnNotebookDnD)
wxEND_EVENT_TABLE()

//************************************************************************************************************************************************************************************************
//FiltersWindow - C-TOR
//************************************************************************************************************************************************************************************************
FiltersWindow::FiltersWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	
	wxSize client_size = GetClientSize();
	wxAuiNotebook* ctrl = new wxAuiNotebook(this, ID_TreeFilters, wxPoint(0, 0), wxSize(430, 200), wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxNO_BORDER);
	//wxAuiNotebook* ctrl = new wxAuiNotebook(this, ID_TreeFilters, wxPoint(0, 0), wxSize(430, 200), wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxNO_BORDER);
	ctrl->Freeze();
	wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));


	//--------------------------
	//Tab 
	//--------------------------
	ctrl->AddPage(CreateRenderFlagsCtrl(ctrl), wxT("Render"), false, page_bmp);
	ctrl->SetPageToolTip(0, "Render - Tab");
	
	ctrl->AddPage(CreateDebugFlagsCtrl(ctrl), wxT("Debug"), false, page_bmp);
	ctrl->SetPageToolTip(1, "Debug - Tab");

	ctrl->AddPage(CreatePostProcessFlagsCtrl(ctrl), wxT("Post-process"), false, page_bmp);
	ctrl->SetPageToolTip(2, "Post-process - Tab");

	/*ctrl->AddPage(CreateSimulationFlagsCtrl(ctrl), wxT("Simulation"), false, page_bmp);
	ctrl->SetPageToolTip(3, "Simulation - Tab");
	*/

	ctrl->Thaw();




	//CreateStatusBar();
	//GetStatusBar()->SetStatusText(_("Ready"), 0);
	// set frame icon
	SetIcon(wxIcon(wxT("KL_ICON_Logo")));


	wxString time = wxNow();
	wxString buffer = "";
	for (int i = 11; i <= 19; ++i)
	{
		buffer.append(time.GetChar(i));
	}

	//(*mTextOututWindow123) << "\n<<" << buffer << ">> " << wxT("awdawdadadaw");
}

//************************************************************************************************************************************************************************************************
//FiltersWindow - D-TOR
//************************************************************************************************************************************************************************************************
FiltersWindow::~FiltersWindow()
{

}

//************************************************************************************************************************************************************************************************
void FiltersWindow::OnTreeLeftDoubleClick(wxTreeEvent& WXUNUSED(evt))
{
	
}

//****************************************************************************************************************************
wxPanel* FiltersWindow::CreateRenderFlagsCtrl(wxWindow* parent)
{
	mPanelRight = new wxPanel(parent, wxID_ANY);
	wxFlexGridSizer* flexPanelRightRender = new wxFlexGridSizer(20, 1, 0, 0);

	//wxCheckBox* mCheckBoxDecals = new wxCheckBox(mPanelRight, wxID_ANY, wxT("Decals"));
	//wxCheckBox* mCheckBoxEmissive = new wxCheckBox(mPanelRight, wxID_ANY, wxT("Emissive"));
	//wxCheckBox* mCheckBoxFlares = new wxCheckBox(mPanelRight, wxID_ANY, wxT("Flares"));
	//wxCheckBox* mCheckBoxLighting = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderLighting, wxT("Lighting"));
	wxCheckBox* mCheckBoxLights = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderLights, wxT("Lights"));
	wxCheckBox* mCheckBoxMeshes = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderMesh, wxT("Meshes"));
	wxCheckBox* mCheckBoxRefraction = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderRefraction, wxT("Refraction"));
	wxCheckBox* mCheckBoxRefractionDynamic = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderRefractionDynamic, wxT("RefractionDynamicCubeMap"));
	wxCheckBox* mCheckBoxShadows = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderShadows, wxT("Shadows"));
	//wxCheckBox* mCheckBoxTerrain = new wxCheckBox(mPanelRight, wxID_ANY, wxT("Terrain"));
	wxCheckBox* mCheckBoxTesellation = new wxCheckBox(mPanelRight, ID_TreeFiltersRenderTessellation, wxT("Tessellation"));

//	mCheckBoxDecals->SetValue(true);
//	mCheckBoxEmissive->SetValue(true);
//	mCheckBoxFlares->SetValue(true);
//	mCheckBoxLighting->SetValue(true);
	mCheckBoxLights->SetValue(true);
	mCheckBoxMeshes->SetValue(true);
	mCheckBoxRefraction->SetValue(true);
	mCheckBoxRefractionDynamic->SetValue(true);
	mCheckBoxShadows->SetValue(true);
//	mCheckBoxTerrain->SetValue(true);
	mCheckBoxTesellation->SetValue(true);

	flexPanelRightRender->Add(10, 10);
	flexPanelRightRender->Add(10, 10);
//	flexPanelRightRender->Add(mCheckBoxDecals, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightRender->Add(mCheckBoxEmissive, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightRender->Add(mCheckBoxFlares, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightRender->Add(mCheckBoxLighting, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightRender->Add(mCheckBoxLights, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightRender->Add(mCheckBoxMeshes, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightRender->Add(mCheckBoxRefraction, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightRender->Add(mCheckBoxRefractionDynamic, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightRender->Add(mCheckBoxShadows, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightRender->Add(mCheckBoxTerrain, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightRender->Add(mCheckBoxTesellation, 0, wxALL | wxALIGN_LEFT, 10);

	mPanelRight->SetSizer(flexPanelRightRender);


	return mPanelRight;

}
//****************************************************************************************************************************
wxPanel* FiltersWindow::CreateDebugFlagsCtrl(wxWindow* parent)
{
	mPanelRightDebug = new wxPanel(parent, wxID_ANY);
	wxFlexGridSizer* flexPanelRightDebug = new wxFlexGridSizer(20, 1, 0, 0);


	wxCheckBox* mCheckBoxBBoxes = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugBBoxes, wxT("BBoxes"));
//	wxCheckBox* mCheckBoxBBoxesParticles = new wxCheckBox(mPanelRightDebug, wxID_ANY, wxT("BBoxesParticles"));
//	wxCheckBox* mCheckBoxBBoxesTerrain = new wxCheckBox(mPanelRightDebug, wxID_ANY, wxT("BBoxesTerrain"));
	wxCheckBox* mCheckBoxBBoxesLights = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugBSphereLight, wxT("BSphereLights"));
	wxCheckBox* mCheckBoxCollision = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugCollision, wxT("Collision"));
	wxCheckBox* mCheckBoxGizmo = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugGizmo, wxT("Gizmo"));
	wxCheckBox* mCheckBoxGrid = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugGrid, wxT("Grid"));
	wxCheckBox* mCheckBoxSelection = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugSelection, wxT("Selection"));
//	wxCheckBox* mCheckBoxSprites = new wxCheckBox(mPanelRightDebug, wxID_ANY, wxT("Sprites"));
	wxCheckBox* mCheckBoxWireframe = new wxCheckBox(mPanelRightDebug, ID_TreeFiltersDebugWireframe, wxT("Wireframe"));

	mCheckBoxGizmo->SetValue(true);
	mCheckBoxGrid->SetValue(true);

	flexPanelRightDebug->Add(10, 10);
	flexPanelRightDebug->Add(10, 10);
	flexPanelRightDebug->Add(mCheckBoxBBoxes, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightDebug->Add(mCheckBoxBBoxesParticles, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightDebug->Add(mCheckBoxBBoxesTerrain, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightDebug->Add(mCheckBoxBBoxesLights, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightDebug->Add(mCheckBoxCollision, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightDebug->Add(mCheckBoxGizmo, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightDebug->Add(mCheckBoxGrid, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightDebug->Add(mCheckBoxSelection, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightDebug->Add(mCheckBoxSprites, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightDebug->Add(mCheckBoxWireframe, 0, wxALL | wxALIGN_LEFT, 10);


	mPanelRightDebug->SetSizer(flexPanelRightDebug);

	return mPanelRightDebug;
}
//****************************************************************************************************************************
wxPanel* FiltersWindow::CreatePostProcessFlagsCtrl(wxWindow* parent)
{
	mPanelRightPostProcess = new wxPanel(parent, wxID_ANY);
	wxFlexGridSizer* flexPanelRightPostProcess = new wxFlexGridSizer(20, 1, 0, 0);



	/*wxCheckBox* mCheckBoxAntialias = new wxCheckBox(mPanelRightPostProcess, wxID_ANY, wxT("Antialias"));
	wxCheckBox* mCheckBoxBloom = new wxCheckBox(mPanelRightPostProcess, wxID_ANY, wxT("Bloom"));
	wxCheckBox* mCheckBoxBlur = new wxCheckBox(mPanelRightPostProcess, wxID_ANY, wxT("Blur"));
	wxCheckBox* mCheckBoxDOF = new wxCheckBox(mPanelRightPostProcess, wxID_ANY, wxT("DOF"));
	wxCheckBox* mCheckBoxFog = new wxCheckBox(mPanelRightPostProcess, wxID_ANY, wxT("Fog"));*/
	wxCheckBox* mCheckBoxSSAO = new wxCheckBox(mPanelRightPostProcess, ID_TreeFiltersSSAO, wxT("SSAO"));

	/*mCheckBoxAntialias->SetValue(true);
	mCheckBoxBloom->SetValue(true);
	mCheckBoxBlur->SetValue(true);
	mCheckBoxDOF->SetValue(true);
	mCheckBoxFog->SetValue(true);*/
	mCheckBoxSSAO->SetValue(true);

	flexPanelRightPostProcess->Add(10, 10);
	flexPanelRightPostProcess->Add(10, 10);
//	flexPanelRightPostProcess->Add(mCheckBoxAntialias, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightPostProcess->Add(mCheckBoxBloom, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightPostProcess->Add(mCheckBoxBlur, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightPostProcess->Add(mCheckBoxDOF, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightPostProcess->Add(mCheckBoxFog, 0, wxALL | wxALIGN_LEFT, 10);
	flexPanelRightPostProcess->Add(mCheckBoxSSAO, 0, wxALL | wxALIGN_LEFT, 10);

	mPanelRightPostProcess->SetSizer(flexPanelRightPostProcess);

	return mPanelRightPostProcess;
}
//****************************************************************************************************************************
wxPanel* FiltersWindow::CreateSimulationFlagsCtrl(wxWindow* parent)
{
	mPanelRightSimulation = new wxPanel(parent, wxID_ANY);
	wxFlexGridSizer* flexPanelRightSimulation = new wxFlexGridSizer(20, 1, 0, 0);


	wxCheckBox* mCheckBoxRendering = new wxCheckBox(mPanelRightSimulation, wxID_ANY, wxT("Rendering"));
	//wxCheckBox* mCheckBoxParticles = new wxCheckBox(mPanelRightSimulation, wxID_ANY, wxT("Particles"));
	//wxCheckBox* mCheckBoxPostProcess = new wxCheckBox(mPanelRightSimulation, wxID_ANY, wxT("PostProcess"));

	mCheckBoxRendering->SetValue(true);
	//mCheckBoxParticles->SetValue(true);
	//mCheckBoxPostProcess->SetValue(true);

	flexPanelRightSimulation->Add(10, 10);
	flexPanelRightSimulation->Add(10, 10);
	flexPanelRightSimulation->Add(mCheckBoxRendering, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightSimulation->Add(mCheckBoxParticles, 0, wxALL | wxALIGN_LEFT, 10);
//	flexPanelRightSimulation->Add(mCheckBoxPostProcess, 0, wxALL | wxALIGN_LEFT, 10);

	mPanelRightSimulation->SetSizer(flexPanelRightSimulation);

	return mPanelRightSimulation;
}


void FiltersWindow::OnClose(wxCloseEvent& WXUNUSED(evt))
{
	Hide();
}

void FiltersWindow::OnNotebookDnD(wxAuiNotebookEvent& WXUNUSED(evt))
{
	//evt.Allow();
}

void FiltersWindow::OnDebugGrid(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugGrid)
		gFilterDebugGrid = false;
	else
		gFilterDebugGrid = true;
}

void FiltersWindow::OnDebugGizmo(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugGizmo)
		gFilterDebugGizmo = false;
	else
		gFilterDebugGizmo = true;
}

void FiltersWindow::OnRenderMesh(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterRenderMesh)
		gFilterRenderMesh = false;
	else
		gFilterRenderMesh = true;
}

void FiltersWindow::OnDebugBBoxes(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugBBoxes)
		gFilterDebugBBoxes = false;
	else
		gFilterDebugBBoxes = true;
}
void FiltersWindow::OnDebugBSphereLight(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugBSphereLight)
		gFilterDebugBSphereLight = false;
	else
		gFilterDebugBSphereLight = true;
}
void FiltersWindow::OnDebugCollision(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugCollision)
		gFilterDebugCollision = false;
	else
		gFilterDebugCollision = true;
}
void FiltersWindow::OnDebugSelection(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugSelection)
		gFilterDebugSelection = false;
	else
		gFilterDebugSelection = true;
}
void FiltersWindow::OnDebugWireframe(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterDebugWireframe)
		gFilterDebugWireframe = false;
	else
		gFilterDebugWireframe = true;
}


void FiltersWindow::OnRenderLighting(wxCommandEvent& WXUNUSED(evt))
{

}
void FiltersWindow::OnRenderLights(wxCommandEvent& WXUNUSED(evt))
{

}
void FiltersWindow::OnRenderRefraction(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterRenderRefraction)
		gFilterRenderRefraction = false;
	else
		gFilterRenderRefraction = true;
}
void FiltersWindow::OnRenderRefractionDynamic(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterRenderRefractionDynamic)
		gFilterRenderRefractionDynamic = false;
	else
		gFilterRenderRefractionDynamic = true;
}
void FiltersWindow::OnRenderShadows(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterRenderShadow)
		gFilterRenderShadow = false;
	else
		gFilterRenderShadow = true;
}
void FiltersWindow::OnRenderTessellation(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterRenderTessellation)
		gFilterRenderTessellation = false;
	else
		gFilterRenderTessellation = true;
}

void FiltersWindow::OnSSAO(wxCommandEvent& WXUNUSED(evt))
{
	if (gFilterSSAO)
		gFilterSSAO = false;
	else
		gFilterSSAO = true;
}