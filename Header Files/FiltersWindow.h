
#ifndef __FILTER__
#define  __FILTER__

#include "Utility.h"



class FiltersWindow : public wxFrame
{
	enum
	{
		ID_TreeFilters,
		ID_TreeFiltersDebugGrid,
		ID_TreeFiltersDebugGizmo,
		ID_TreeFiltersRenderMesh,
		ID_TreeFiltersDebugBBoxes,
		ID_TreeFiltersDebugBSphereLight,
		ID_TreeFiltersDebugCollision,
		ID_TreeFiltersDebugSelection,
		ID_TreeFiltersDebugWireframe,
		ID_TreeFiltersRenderLighting,
		ID_TreeFiltersRenderLights,
		ID_TreeFiltersRenderRefraction,
		ID_TreeFiltersRenderShadows,
		ID_TreeFiltersRenderTessellation,
		ID_TreeFiltersSSAO,
		ID_TreeFiltersRenderRefractionDynamic,
	};

public:
	FiltersWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

	~FiltersWindow();

private:

	wxTreeCtrl* mTreeFilters;
	wxFlexGridSizer* mFlexPropertiesPanel;
	wxPanel *mPanelMain;
	wxPanel *mPanelLeft;
	wxPanel *mPanelRight;
	wxPanel *mPanelRightDebug;
	wxPanel *mPanelRightPostProcess;
	wxPanel *mPanelRightSimulation;
	wxPanel *CreateRenderFlagsCtrl(wxWindow* parent = NULL);
	wxPanel *CreateDebugFlagsCtrl(wxWindow* parent = NULL);
	wxPanel *CreatePostProcessFlagsCtrl(wxWindow* parent = NULL);
	wxPanel *CreateSimulationFlagsCtrl(wxWindow* parent = NULL);

	wxDECLARE_EVENT_TABLE();

	void OnTreeLeftDoubleClick(wxTreeEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void OnNotebookDnD(wxAuiNotebookEvent& evt);

	void OnDebugGrid(wxCommandEvent& evt);
	void OnDebugGizmo(wxCommandEvent& evt);
	void OnDebugBBoxes(wxCommandEvent& evt);
	void OnDebugBSphereLight(wxCommandEvent& evt);
	void OnDebugCollision(wxCommandEvent& evt);
	void OnDebugSelection(wxCommandEvent& evt);
	void OnDebugWireframe(wxCommandEvent& evt);
	void OnRenderMesh(wxCommandEvent& evt);

	void OnRenderLighting(wxCommandEvent& evt);
	void OnRenderLights(wxCommandEvent& evt);
	void OnRenderRefraction(wxCommandEvent& evt);
	void OnRenderRefractionDynamic(wxCommandEvent& evt);
	void OnRenderShadows(wxCommandEvent& evt);
	void OnRenderTessellation(wxCommandEvent& evt);

	void OnSSAO(wxCommandEvent& evt);

};


#endif // !__FILTER__