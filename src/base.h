#ifndef __BASE_H
#define __BASE_H

#include "bildzerlegung.h"

class MainApp: public wxApp
{
  public:
      virtual bool OnInit();
};

class MainFrame: public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	void OnQuit(wxCommandEvent &event);
	void OnOpenBild(wxCommandEvent &event);
	void OnPaint(wxPaintEvent &event);
private:
  wxFileDialog *PictureOpener;
  wxImage WandelBild, BildMaske;
  DECLARE_EVENT_TABLE()
};

enum
{
	ID_MAINWIN_QUIT = wxID_HIGHEST+1,
	IdMenuOpenPic
};


#endif
