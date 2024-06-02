#ifndef __FILTERDIALOG_H_
#define __FILTERDIALOG_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif
#include <wx/dialog.h>
#include <wx/propgrid/props.h>
#include <vector>
#include "filter.h"

class MainFrame;

class FilterDialog : public wxFrame
{
public:
	FilterDialog(filter *maske,
				wxWindow *parent,
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos=wxDefaultPosition,
				const wxSize &size=wxDefaultSize,
				long style=(wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxCLOSE_BOX)),
				const wxString &name=wxFrameNameStr);
	~FilterDialog();
private:
	void DialogErneuern();
	void NeuerFilter(wxCommandEvent &event);
	void FilterAnwenden(wxCommandEvent &event);
	void OnQuit(wxCloseEvent &event);
	void OnOK(wxCommandEvent &event);
	void OnThreshold(wxCommandEvent &event);
	
	bool MatrizeFuellen();

	
	wxTextCtrl *filterTHCaption, *filterTHTextCtrl;
	
	filter *m_maske;
	
	std::vector<wxTextCtrl*> TextCtrlContainer;
	wxBoxSizer *filterSizer = NULL;
	wxBoxSizer *hauptSizer;
};

enum
{
	MENU_ID_QUIT = wxID_HIGHEST+1,
	BTN_ID_NEU, BTN_ID_ANWENDEN, BTN_ID_THRESHOLD
};

#include "base.h"

#endif //__FILTERDIALOG_H_