#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "base.h"

void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite);

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
   MainFrame *win = new MainFrame(_("Frame"), wxPoint (100, 100),
     wxSize(450, 340));
   win->Show(TRUE);
   SetTopWindow(win);

   return TRUE;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
   EVT_MENU(ID_MAINWIN_QUIT, MainFrame::OnQuit)
   EVT_MENU(IdMenuOpenPic, MainFrame::OnOpenBild)
   EVT_PAINT(OnPaint)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
    wxMenu *FileMenu = new wxMenu;
    wxMenuBar *MenuBar = new wxMenuBar;

    FileMenu->Append(ID_MAINWIN_QUIT, wxT("&Beenden"));
	FileMenu->Append(IdMenuOpenPic, wxT("Bild &öffenen"));

    MenuBar->Append(FileMenu, _("&File"));
    SetMenuBar(MenuBar);

    PictureOpener = new wxFileDialog(this, wxT("Bild auswaehlen"),wxT(""),wxT(""),wxT("*.jpg"),1);
	wxImageHandler *JPEGHandler = new wxJPEGHandler();
    wxImage::AddHandler(JPEGHandler);
	wxImageHandler *TIFFHandler = new wxTIFFHandler();
    wxImage::AddHandler(TIFFHandler);
	
	BildMaske.LoadFile(".\\img\\Buchstaben.tiff", wxBITMAP_TYPE_TIFF);

    CreateStatusBar(2);
    SetStatusText(_("Hello World!"));
}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(TRUE);
}

void MainFrame::OnOpenBild(wxCommandEvent &event)
{
	PictureOpener->SetMessage(wxT("Bild öffnen"));
	int Rueckgabe = PictureOpener->ShowModal();

	if(Rueckgabe==wxID_CANCEL)return;
	SetStatusText(PictureOpener->GetPath());
	
	if(WandelBild.Ok())WandelBild.Destroy();
	WandelBild.LoadFile(PictureOpener->GetPath(), wxBITMAP_TYPE_JPEG);
	
	unsigned char *urDaten = WandelBild.GetData();
	unsigned char *buchstabenDaten = BildMaske.GetData();
	
	BildZerlegen(urDaten, WandelBild.GetWidth(), WandelBild.GetHeight(), buchstabenDaten, BildMaske.GetWidth(), BildMaske.GetHeight(),  BildMaske.GetWidth()/95);

	Refresh();
	return;
}

void MainFrame::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	if(BildMaske.Ok())dc.DrawBitmap(wxBitmap(BildMaske), 0, 0);
	if(WandelBild.Ok())dc.DrawBitmap(wxBitmap(WandelBild), 0, BildMaske.GetHeight());
	
	return;
}