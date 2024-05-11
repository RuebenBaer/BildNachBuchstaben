#ifndef __FILTERDIALOG_H_
#define __FILTERDIALOG_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif
#include <wx/dialog.h>
#include <wx/propgrid/props.h>

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
	void SetzeFilterGroesse(int groesse);
	void OnQuit(wxCloseEvent &event);
	
	wxTextCtrl *filterGroesseCaption, *filterGroesseTextCtrl;
	
	wxBoxSizer *filterSizer;
};

enum
{
	MENU_ID_QUIT = wxID_HIGHEST+1
};

#endif //__FILTERDIALOG_H_