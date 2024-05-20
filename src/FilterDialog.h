#ifndef __FILTERDIALOG_H_
#define __FILTERDIALOG_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif
#include <wx/dialog.h>
#include <wx/propgrid/props.h>
#include <vector>

class FilterDialog : public wxDialog
{
public:
	FilterDialog(int filterGroesse,
				wxWindow *parent,
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos=wxDefaultPosition,
				const wxSize &size=wxDefaultSize,
				long style=wxDEFAULT_DIALOG_STYLE,
				const wxString &name=wxDialogNameStr);
	~FilterDialog();
private:
	void DialogErneuern(int filterGroesse);
	void SetzeFilterGroesse(int groesse);
	void NeuerFilter(wxCommandEvent &event);
	void FilterAnwenden(wxCommandEvent &event);
	void OnQuit(wxCloseEvent &event);
	
	wxTextCtrl *filterGroesseCaption, *filterGroesseTextCtrl;
	
	std::vector<wxTextCtrl*> TextCtrlContainer;
	wxBoxSizer *filterSizer = NULL;
	wxBoxSizer *hauptSizer;
};

enum
{
	MENU_ID_QUIT = wxID_HIGHEST+1,
	BTN_ID_NEU
};

#endif //__FILTERDIALOG_H_