#ifndef __BASE_H
#define __BASE_H

#include "bildzerlegung.h"
#include <wx/numdlg.h>
//#include "filter.h"
#include "FilterDialog.h"

class MainApp: public wxApp
{
  public:
      virtual bool OnInit();
};

class MainFrame: public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	~MainFrame();
	void OnQuit(wxCommandEvent &event);
	void OnOpenBild(wxCommandEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnFarbFaktor(wxCommandEvent &event);
	
	void OnBildInBuchstabe(wxCommandEvent& event);
	void OnBildMaske(wxCommandEvent& event);
	
	void FilterAnwenden(void);
private:
	void FilterDialogErneuern(void);
	
	wxFileDialog *PictureOpener;
	FilterDialog *FltDlg = NULL;
	filter *maske = NULL;
	wxImage WandelBild, BildMaske;
	wxImage AnzeigeBild, ArbeitsBild;
	double dFarbFaktor;
};

enum
{
	ID_MAINWIN_QUIT = wxID_HIGHEST+1,
	IdMenuOpenPic, idMenuFarbFaktor, idMenuBildInBuchstabe, idMenuBildMaskieren
};

#endif
