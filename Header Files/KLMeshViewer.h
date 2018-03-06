#ifndef __KLMESH_VIEWER__
#define  __KLMESH_VIEWER__

#include "Utility.h"
#include "MaterialInfo.h"
#include "LodInfo.h"
#include "File.h"
#include "BinaryFileWrite.h"
#include "KLMESH.h"
#include "TexturesResource.h"
#include "DxAppKLMeshViever.h"
//#include "AssetBrowser.h"



class KLMESHViewer : public wxFrame
{
	enum
	{
		ID_NotebookWindow,
		ID_MENU_EXIT,
		ID_MENU_SAVE,
		ID_MENU_ZOOM,
		ID_MENU_REIMPORT,
		ID_Toolbar_REIMPORT,
		ID_Toolbar_BBOX,
		ID_Toolbar_COLL,
		ID_Toolbar_ZOOM,
		ID_TAB_MATERIAL_GridProperty,
		ID_TAB_MATERIAL_ListOfMaterials,
		ID_TAB_LODs_ListOfLODs,
		ID_TAB_LOD_GridProperty,
		ID_TAB_INFO,
		wxID_RenderPanel,
		ID_WindowDirectX,
		ID_SLIDER_LIGHT,
		ID_SLIDER_LIGHT_COLOR_R,
		ID_SLIDER_LIGHT_COLOR_G,
		ID_SLIDER_LIGHT_COLOR_B,
		ID_SLIDER_LIGHT_POWER,
		ID_SLIDER_LIGHT_SPEC,
		ID_SLIDER_ROTATION,
		ID_SLIDER_LIGHT_COLOR_SPEC_R,
		ID_SLIDER_LIGHT_COLOR_SPEC_G,
		ID_SLIDER_LIGHT_COLOR_SPEC_B,
		ID_TAB_MATERIAL_SELECTED,

	};

public:
	KLMESHViewer(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
	~KLMESHViewer();
//	void SetAssetBrowser(AssetBrowser* assetBrowser);
	void SetLinkToTexture(wxString link, wxString name);
	void SetKLMESH(KLMESH* klmesh);
	void SetTexturesResource(TexturesResource* texturesResource);
	void SetDevice(ID3D11Device* device);
	void CreateKLMESH();
	std::string GetName();

private:
	DxAppKLMeshViever* mDxAppKLMeshViever;
	bool mFileChanges;
	bool mShowBBox = false;
	bool mShowCollision = false;
	int mForceLODLevel = -1;
	int mSelectedLOD = 0;
	int mSelectedMaterialPos = -1;
	bool mIsCheckBoxSelected = false;

	wxPGEditor* mMultiButtonEditor;
	wxString mLinkToSelectedTexture;
	wxString mSelectedTextureName;
	wxAuiManager mAuiManager;
	wxAuiNotebook* mNotebookWindow;
	wxString mPathToFile;
	wxAuiToolBar* mToolBarTop;
	wxBitmap* mImage;

	wxPanel* mPanelRender;
	wxScrolledWindow* mPanelBottom;


	klmeshFileStruct* mKlmeshStruct;
	KLMESH* mKLMESH; //Pointer to an external object! -> render in the main frame
	KLMESH* mKLMESH_render; //Pointer to the local klmesh -> render in the klmesh-viewer
	KLMESH* mKLMESH_TMP; 
	TexturesResource* mTexturesResource; //Pointer to an external TexturesResource! -> use in the main frame
	TexturesResource* mTexturesResourceRender; //Pointer to the local TexturesResource -> use in the klmesh-viewer
	ID3D11Device* mDevice;


	//Bottom panel:

	wxStaticText* mPanelBottomText;
	wxSlider* mPanelBottomSlider;
	wxStaticText* mPanelBottomTextRot;
	wxSlider* mPanelBottomSliderRot;
	wxCheckBox* mPanelBottomCheckBoxIsCastShadows;
	wxStaticText* mPanelBottomTextLightColour;
	wxSlider* mPanelBottomSliderLightR;
	wxSlider* mPanelBottomSliderLightG;
	wxSlider* mPanelBottomSliderLightB;
	wxSlider* mPanelBottomSliderLightSpecR;
	wxSlider* mPanelBottomSliderLightSpecG;
	wxSlider* mPanelBottomSliderLightSpecB;
	wxStaticText* mPanelBottomTextLightPower;
	wxSlider* mPanelBottomSliderLightPower;
	wxStaticText* mPanelBottomTextLightSpecular;
	wxSlider* mPanelBottomSliderLightSpecular;


	wxListBox *mListboxMaterial;
	wxListBox *mListboxLODs;
	std::vector<MaterialInfo*>* mVectorOfMaterials;
	std::vector<LodInfo*>* mVectorOfLods;
	wxArrayString* mArrayOfMaterials;
	wxArrayString* mArrayOfLODs;
	
	wxArrayString* mArrayOfShaders;
	wxArrayString* mArrayOfSelectedTextures;

	wxPropertyGrid* mPropertyGridProperty;
	wxPGProperty* mSelectedMaterialProp;
	wxPGProperty* mChooseShaderProp;
	wxPGProperty* mUsedShaderProp;
	wxPGProperty* mMatTextureDiffuseProp;
	wxPGProperty* mMatTextureSpecularProp;
	wxPGProperty* mMatTextureNormalProp;
	wxPGProperty* mMatTextureCubeProp;
	
	//wxPGProperty* mMatTextureHeightProp;
	//wxPGProperty* mMatTextureAlphaProp;
	wxPGProperty* mAmbientColourProp;
	wxPGProperty* mAmbient_R_ColourProp;
	wxPGProperty* mAmbient_G_ColourProp;
	wxPGProperty* mAmbient_B_ColourProp;
	wxPGProperty* mDiffuseColourProp;
	wxPGProperty* mDiffuse_R_ColourProp;
	wxPGProperty* mDiffuse_G_ColourProp;
	wxPGProperty* mDiffuse_B_ColourProp;
	wxPGProperty* mSpecularColourProp;
	wxPGProperty* mSpecular_R_ColourProp;
	wxPGProperty* mSpecular_G_ColourProp;
	wxPGProperty* mSpecular_B_ColourProp;

	wxPGProperty* mReflectColourProp;
	wxPGProperty* mReflect_R_ColourProp;
	wxPGProperty* mReflect_G_ColourProp;
	wxPGProperty* mReflect_B_ColourProp;
	wxPGProperty* mTransparencyProp;

	wxPGProperty* mCubeMapDynamicProp;

	wxPGProperty* mSpecularPowerProp;
	wxPGProperty* mRoughnessProp;
	wxPGProperty* mFresnelFactorProp;

	wxPGProperty* mTessellationProp;
	wxPGProperty* mTessellationHeightScaleProp;
	wxPGProperty* mTessellationDistanceProp;
	wxPGProperty* mTessellationDistanceMaxProp;
	wxPGProperty* mTessellationDistanceMinProp;
	wxPGProperty* mTessellationFactorProp;
	wxPGProperty* mTessellationFactorMaxProp;
	wxPGProperty* mTessellationFactorMinProp;

	wxPGProperty* mInfoVerticesProp;
	wxPGProperty* mInfoTrianglesProp;
	wxPGProperty* mInfoIndicesProp;



	//LOD
	wxPGProperty* mLOD_DistanceProp;
	wxPGProperty* mLOD_TrianglesProp;
	wxPGProperty* mLOD_VerticesProp;
	wxPGProperty* mLOD_IndicesProp;
	wxPGProperty* mLOD_ChunksProp;
	wxPGProperty* mLOD_MaterialProp;

	wxPGProperty* mFileNameProp;
	wxPGProperty* mBBmaxXProp;
	wxPGProperty* mBBmaxYProp;
	wxPGProperty* mBBmaxZProp;
	wxPGProperty* mBBminXProp;
	wxPGProperty* mBBminYProp;
	wxPGProperty* mBBminZProp;
	wxPGProperty* mHideDistanceProp;
	wxPGProperty* mShadowDistaceProp;
	wxPGProperty* mForceNoShadowProp;

	


	wxDECLARE_EVENT_TABLE();

private:
	void OnClose(wxCloseEvent& evt);
	void OnMenuSave(wxCommandEvent& evt);
	void OnMenuZoom(wxCommandEvent& evt);
	void OnMenuBBox(wxCommandEvent& evt);
	void OnMenuCollision(wxCommandEvent& evt);
	void OnMenuReimport(wxCommandEvent& evt);
	void OnMenuClose(wxCommandEvent& evt);
	void OnPaint(wxPaintEvent & evt);
	void OnTabMatGridPropertyChanged(wxPropertyGridEvent& evt);
	void OnTabMatGridPropertyChanging(wxPropertyGridEvent& evt);
	void OnTabMatSelectedMaterialList(wxCommandEvent& evt);
	void OnTabLODGridPropertyChanged(wxPropertyGridEvent& evt);
	void OnTabLODSelectedList(wxCommandEvent& evt);
	void OnTabInfoGridPropertyChanged(wxPropertyGridEvent& evt);
	void RestoreNoSavedKLMESH();
	void OnSliderLight(wxScrollEvent& evt);
	void OnSliderLightColorR(wxScrollEvent& evt);
	void OnSliderLightColorG(wxScrollEvent& evt);
	void OnSliderLightColorB(wxScrollEvent& evt);
	void OnSliderLightPower(wxScrollEvent& evt);
	void OnSliderLightSpec(wxScrollEvent& evt);
	void OnSliderLightColorSpecR(wxScrollEvent& evt);
	void OnSliderLightColorSpecG(wxScrollEvent& evt);
	void OnSliderLightColorSpecB(wxScrollEvent& evt);
	void OnSliderRotation(wxScrollEvent& evt);
	void OnNotebookChanged(wxAuiNotebookEvent& evt);
	void OnCheckBoxClicked(wxCommandEvent& evt);
	void Close();


private:
	wxAuiNotebook* CreateRightWindow();
	wxAuiNotebook* mRightWindow;
	wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent);
	wxString GetIntroText();
	wxScrolledWindow* CreatePropertyGridProperty();
	wxScrolledWindow*  CreatePanelBottom();
	wxScrolledWindow* CreateMaterialsPanel();
	wxScrolledWindow* CreateLODsPanel();
	void LoadTexture(int matPosition, std::string textureName, int textureType); //textureType ==> 0 -> DiffuseMap; 1 -> NormalMap; 2 -> ASH_Map
};



#endif // !__KLMESH_VIEWER__