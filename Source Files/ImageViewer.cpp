#include "../Header Files/ImageViewer.h"


wxBEGIN_EVENT_TABLE(ImageViewer, wxFrame)
EVT_END_SESSION(ImageViewer::OnClose)
EVT_CLOSE(ImageViewer::OnClose)
EVT_PAINT(ImageViewer::OnPaint)

wxEND_EVENT_TABLE()

//************************************************************************************************************************************************************************************************
//ImageViewer - C-TOR
//************************************************************************************************************************************************************************************************
ImageViewer::ImageViewer(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	wxSize client_size = GetClientSize();
	// set frame icon
	SetIcon(wxIcon(wxT("KL_ICON_Logo")));

	mFormat << title.at((int)title.size() - 3);
	mFormat << title.at((int)title.size() - 2);
	mFormat << title.at((int)title.size() - 1);

	wxString tempText = "";
	for (int i = 13; i < (int)title.size(); ++i)
	{
		tempText << title.at(i);	
	}
	this->mPathToFile = tempText;

	wxImage* image = NULL;
	if (mFormat == "JPG" || mFormat == "jpg")
	{
		wxImageHandler* jpgLoader = new wxJPEGHandler();
		wxImage::AddHandler(jpgLoader);
		image = new wxImage(mPathToFile, wxBITMAP_TYPE_JPEG);
		*image = image->Scale(512, 512);
		mImage = new wxBitmap(*image, wxBITMAP_SCREEN_DEPTH);
	}
	else if (mFormat == "PNG" || mFormat == "png")
	{
		wxImageHandler* pngLoader = new wxPNGHandler();
		wxImage::AddHandler(pngLoader);
		image = new wxImage(mPathToFile, wxBITMAP_TYPE_PNG);
		*image = image->Scale(512, 512);
		mImage = new wxBitmap(*image, wxBITMAP_SCREEN_DEPTH);
	}
	else if (mFormat == "TGA" || mFormat == "tga")
	{
		wxImageHandler* tgaLoader = new wxTGAHandler();
		wxImage::AddHandler(tgaLoader);
		image = new wxImage(mPathToFile, wxBITMAP_TYPE_TGA);
		*image = image->Scale(512, 512);
		mImage = new wxBitmap(*image, wxBITMAP_SCREEN_DEPTH);
	}

	else if (mFormat == "DDS" || mFormat == "dds")
	{
		wxString fileName = "";
		ScratchImage imageSI;
		TexMetadata metadata;
		ScratchImage imageSI_Decompress;
		ScratchImage imageSI_JPG;
		HRESULT hr;
		hr = LoadFromDDSFile(mPathToFile, DDS_FLAGS_NONE, &metadata, imageSI);
		if (FAILED(hr))
		{
			wxMessageBox("Cannot load texture: " + mPathToFile, "Error!", wxICON_ERROR | wxOK);
		}
		if (imageSI.GetMetadata().IsCubemap() == false)
		{
			hr = Decompress(imageSI.GetImages(), imageSI.GetImageCount(), imageSI.GetMetadata(), DXGI_FORMAT_UNKNOWN, imageSI_Decompress);
			if (FAILED(hr))
			{
				wxMessageBox("Cannot Decompress texture: " + mPathToFile, "Error!", wxICON_ERROR | wxOK);
			}

			for (int j = 0; j < (int)mPathToFile.size() - 4; ++j)
				fileName += mPathToFile.at(j);

			fileName += "__";
			mPathToFile = fileName;
			mPathToFile += ".temp";

			hr = Resize(imageSI_Decompress.GetImages(), imageSI_Decompress.GetImageCount(), imageSI_Decompress.GetMetadata(), 512, 512, TEX_FILTER_DEFAULT, imageSI_JPG);
			if (FAILED(hr))
			{
				wxMessageBox("Cannot resize texture: " + mPathToFile, "Error!", wxICON_ERROR | wxOK);
			}

			hr = SaveToWICFile(*imageSI_JPG.GetImage(0, 0, 0), WIC_FLAGS_NONE, GUID_ContainerFormatJpeg, mPathToFile, &GUID_WICPixelFormat24bppBGR);
			if (FAILED(hr))
			{
				wxMessageBox("Cannot save texture to JPG format: " + mPathToFile, "Error!", wxICON_ERROR | wxOK);
			}

			wxImageHandler* jpgLoader = new wxJPEGHandler();
			wxImage::AddHandler(jpgLoader);
			image = new wxImage(mPathToFile, wxBITMAP_TYPE_JPEG);
			*image = image->Scale(512, 512);
			mImage = new wxBitmap(*image, wxBITMAP_SCREEN_DEPTH);
		}
		else
		{
			wxMessageBox("Cannot open cubeMap.", "Sorry", wxICON_INFORMATION | wxOK);
			/*image = new wxImage(mPathToFile, wxBITMAP_TYPE_JPEG);
			*image = image->Scale(512, 512);
			mImage = new wxBitmap(*image, wxBITMAP_SCREEN_DEPTH);*/
		}
	}

	if (mImage != nullptr)
	if (!mImage->IsOk())
	{
		wxMessageDialog *errorMsg = new wxMessageDialog(NULL, wxT("ERROR"), wxT("INFO"), wxOK | wxICON_INFORMATION);
		errorMsg->ShowModal();
		errorMsg->Destroy();
		Hide();
		//Destroy();
	}
}

//************************************************************************************************************************************************************************************************
//ImageViewer - D-TOR
//************************************************************************************************************************************************************************************************
ImageViewer::~ImageViewer()
{

}
void ImageViewer::OnPaint(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	if(mImage != nullptr)
		dc.DrawBitmap(*mImage, 0, 0);
}

void ImageViewer::OnClose(wxCloseEvent& WXUNUSED(evt))
{
	if (mPathToFile.at(mPathToFile.size() - 7) == '_')
		if (mPathToFile.at(mPathToFile.size() - 6) == '_')
			if (mPathToFile.at(mPathToFile.size() - 5) == '.')
				if (mPathToFile.at(mPathToFile.size() - 4) == 't')
					if (mPathToFile.at(mPathToFile.size() - 3) == 'e')
						if (mPathToFile.at(mPathToFile.size() - 2) == 'm')
							wxRemoveFile(mPathToFile); //delete temp texture (created to load and display dds file format)

	Hide();
}
