#ifndef D3DUTIL_H
#define D3DUTIL_H

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <d3d11.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <sstream>
#include <fstream>
#include <time.h>
#include <vector>
#include <stdexcept> 
#include <process.h>
#include <DirectXMath.h>
#include "MathHelper.h"
#include "GameTimer.h"
#pragma comment (lib, "d3d11.lib")

#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include "wx/aui/aui.h"
#include "wx/listctrl.h"
#include "wx/dir.h"
#include "wx/dnd.h"
#include "wx/slider.h"
#include "wx/scrolwin.h"
#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"
#include "wx/propgrid/editors.h"
#include "wx/filedlg.h"
#include "wx/progdlg.h"
#include "DirectXTex.h"
#include "DirectXPackedVector.h"



//Effects11 --> Trzeba rowniez dodac pliki *cpp i *h z Effects11 do projektu!!!
#include <d3dx11effect.h> //Properties --> c/c++ --> General --> Effects11\Binary & Effects11\Inc !!!
#include "../d3dx11.h"

//For resource window define
#include "Resource.h"


#define AAA			101

#define KL_ICON_Logo			2001
#define KL_ICON_ActiveLayer			2002
#define KL_ICON_AssetBrowser			2003
#define KL_ICON_CloseWorld			2004
#define KL_ICON_Copy			2005
#define KL_ICON_Delete			2006
#define KL_ICON_Global			2007
#define KL_ICON_Grid			2008
#define KL_ICON_GridRotate			2009
#define KL_ICON_Local			2010
#define KL_ICON_Move 			2011
#define KL_ICON_MultiLayers			2012
#define KL_ICON_NewWorld			2013
#define KL_ICON_Open			2014
#define KL_ICON_Paste			2015
#define KL_ICON_RestoreWorld			2016
#define KL_ICON_Rotate			2017
#define KL_ICON_Save 			2018
#define KL_ICON_Scale			2019

#define KL_ICON_Folder       2020
#define KL_ICON_Object3D       2021
#define KL_ICON_TreeLayer       2022
#define KL_ICON_TreeLayerActive       2023
#define KL_ICON_TreeLayerActiveNoSave       2024
#define KL_ICON_TreeLayerNoSave       2025
#define KL_ICON_TreeLayerHide       2026
#define KL_ICON_Object3DSelected	2027

#define KL_ICON_LargeDirectory	2028
#define KL_ICON_Lock	2029

#define KL_ICON_BBox	2030
#define KL_ICON_Gears	2031
#define KL_ICON_INFO	2032
#define KL_ICON_MAT		2033
#define KL_ICON_ZOOM	2034
#define KL_ICON_Property	2035
#define KL_ICON_COLL	2036










class Node
{
public:
	Node(Node* parent, wxString name, std::vector<Node*>* allNodeArray, int level);
	~Node();
	Node* getParent();
	std::vector<Node*>* getChildrenArray();
	bool getIsVisited();
	wxString getNameNode();
	wxString getPathNameNode();
	int getLevel();
	void setLevel(int level);
	int getArrayFolderPosition();
	void setArrayFolderPosition(int pos);
	void setTreeItemId(wxTreeItemId item);
	wxTreeItemId getTreeItemId();

private:
	Node* mParent;
	std::vector<Node*> *mChildrenArray;
	bool mIsVisited;
	wxString mName;
	wxString mPathName;
	int mLevel;
	int mArrayFolderPosition;
	wxTreeItemId mTreeItemId;
};

class DirectoryTree
{
public:
	DirectoryTree(Node* root, std::vector<Node*> childrenArray);
	~DirectoryTree();
	Node* getParent(Node* node);
	std::vector<Node*>* getChildrenArray(Node* node);

private:
	Node* mRoot;
	std::vector<Node*> mChildrenArray;

};


struct SsaoEffects
{
	ID3DX11EffectTechnique* SsaoTech;
	ID3DX11EffectMatrixVariable* mSSAO_ViewToTexSpace;
	ID3DX11EffectVectorVariable* mSSAO_OffsetVectors;
	ID3DX11EffectVectorVariable* mSSAO_FrustumCorners;
	ID3DX11EffectShaderResourceVariable* mSSAO_NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* mSSAO_RandomVecMap;
	ID3DX11EffectTechnique* m_SSAO_BLUR_HorzBlurTech;
	ID3DX11EffectTechnique* m_SSAO_BLUR_VertBlurTech;
	ID3DX11EffectScalarVariable* m_SSAO_BLUR_TexelWidth;
	ID3DX11EffectScalarVariable* m_SSAO_BLUR_TexelHeight;
	ID3DX11EffectShaderResourceVariable* m_SSAO_BLUR_NormalDepthMap;
	ID3DX11EffectShaderResourceVariable* m_SSAO_BLUR_InputImage;
};




//------------------------------------------------------------------------------------------
// Structure, Chunk of mesh
//------------------------------------------------------------------------------------------
struct Chunk
{
	short materialPosition; //Position in the materialBuffer
	UINT numberOfIndicesToDraw;						//Iloœæ indeksów do narysowania dla tego kawa³ka
	UINT startIndex;								//Poczatkowy indeks, od jakiego bêdziemy rysowaæ dla tego kawa³ka
	UINT startVertex;								//Poczatkowy werteks, od jakiego bêdziemy rysowaæ dla tego kawa³ka
};



//Do testowania b³êdów
using namespace std;

#ifdef _DEBUG
#ifndef HR
#define HR(x) \
	{	\
	HRESULT hr = x; \
if (FAILED(hr)) \
	{	\
	DXTraceW(__FILEW__, __LINE__, hr, L#x, TRUE); \
	} \
	}
#endif


#ifndef HR
#define HR(x) x;
#endif
#endif



#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }




namespace Memory
{
	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	template <class T> void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}



namespace Colors
{
	XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

#endif