#ifndef __IMAGE_VIEWER__
#define  __IMAGE_VIEWER__

#include "Utility.h"
#include <wincodec.h>

class ImageViewer : public wxFrame
{
public:
	ImageViewer(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
	~ImageViewer();

private:
	wxBitmap* mImage;
	wxString mPathToFile;
	wxString mFormat;

	wxDECLARE_EVENT_TABLE();

	void OnClose(wxCloseEvent& evt);
	void OnPaint(wxPaintEvent & evt);
};



#endif // !__IMAGE_VIEWER__