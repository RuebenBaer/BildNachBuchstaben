#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "base.h"
#include "filter.h"

//void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite);

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
   MainFrame *win = new MainFrame(_("Frame"), wxPoint (100, 100),
     wxSize(450, 340));
   win->Show(TRUE);
   SetTopWindow(win);

   return TRUE;
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
    wxMenu *FileMenu = new wxMenu;
    wxMenuBar *MenuBar = new wxMenuBar;

    FileMenu->Append(ID_MAINWIN_QUIT, wxT("&Beenden"));
	FileMenu->Append(IdMenuOpenPic, wxT("Bild &öffenen"));
	FileMenu->Append(idMenuFarbFaktor, wxT("&Farbdivisor eingeben"));
	FileMenu->Append(idMenuBildInBuchstabe, wxT("Bild &in Buchstaben wandeln"));
	FileMenu->Append(idMenuBildMaskieren, wxT("Bild &maskieren"));

    MenuBar->Append(FileMenu, _("&File"));
    SetMenuBar(MenuBar);

    PictureOpener = new wxFileDialog(this, wxT("Bild auswählen"),wxT(""),wxT(""),wxT("*.jpg"),1);
	wxImageHandler *JPEGHandler = new wxJPEGHandler();
    wxImage::AddHandler(JPEGHandler);
	wxImageHandler *TIFFHandler = new wxTIFFHandler();
    wxImage::AddHandler(TIFFHandler);
	
	BildMaske.LoadFile("./img/Buchstaben.tiff", wxBITMAP_TYPE_TIFF);

	dFarbFaktor = 0.5;
	
    CreateStatusBar(2);
    SetStatusText(_("Hello World!"));
	
	
	Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, ID_MAINWIN_QUIT);
	Bind(wxEVT_MENU, &MainFrame::OnOpenBild, this, IdMenuOpenPic);
	Bind(wxEVT_MENU, &MainFrame::OnFarbFaktor, this, idMenuFarbFaktor);
	Bind(wxEVT_MENU, &MainFrame::OnBildInBuchstabe, this, idMenuBildInBuchstabe);
	Bind(wxEVT_MENU, & MainFrame::OnBildMaske, this, idMenuBildMaskieren);
}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(TRUE);
}

void MainFrame::OnFarbFaktor(wxCommandEvent &event)
{
	wxNumberEntryDialog nmbDlg(this, wxT("Bitte den Farbdivisor eingeben\n(Schwerpunkt)"), wxT("Zahleneingabe"), wxT("Caption"), (long)(1/dFarbFaktor), 0.1 , 512);
	nmbDlg.ShowModal();
	long zahl = nmbDlg.GetValue();
	dFarbFaktor = 1/double(zahl);
	return;
}

void MainFrame::OnOpenBild(wxCommandEvent &event)
{
	PictureOpener->SetMessage(wxT("Bild öffnen"));
	int Rueckgabe = PictureOpener->ShowModal();

	if(Rueckgabe==wxID_CANCEL)return;
	SetStatusText(PictureOpener->GetPath());
	
	if(WandelBild.Ok())WandelBild.Destroy();
	WandelBild.LoadFile(PictureOpener->GetPath(), wxBITMAP_TYPE_JPEG);
	
	Refresh();
	return;
}

void MainFrame::OnBildInBuchstabe(wxCommandEvent& event)
{
	if(!WandelBild.Ok())
	{
		std::cout<<"Kein Bild geladen\n"<<std::flush;
		return;
	}
	unsigned char *urDaten = WandelBild.GetData();
	unsigned char *buchstabenDaten = BildMaske.GetData();
	
	BildZerlegen(urDaten, WandelBild.GetWidth(), WandelBild.GetHeight(), buchstabenDaten, BildMaske.GetWidth(), BildMaske.GetHeight(), BildMaske.GetWidth()/95);
	BildZerlegenNormalverteilung(urDaten, WandelBild.GetWidth(), WandelBild.GetHeight(), buchstabenDaten, BildMaske.GetWidth(), BildMaske.GetHeight(), BildMaske.GetWidth()/95);
	//SchwerPunkt::BildZerlegenSchwerpunkt(urDaten, WandelBild.GetWidth(), WandelBild.GetHeight(), buchstabenDaten, BildMaske.GetWidth(), BildMaske.GetHeight(),  BildMaske.GetWidth()/95, dFarbFaktor);

	Refresh();
	return;
}

void MainFrame::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	if(BildMaske.Ok())dc.DrawBitmap(wxBitmap(BildMaske), 0, 0);
	if(WandelBild.Ok())
	{
		dc.DrawBitmap(wxBitmap(WandelBild), 0, BildMaske.GetHeight());
	}else{
		std::cout<<"Wandelbild defekt\n";
	}
	
	return;
}

void MainFrame::OnBildMaske(wxCommandEvent& event)
{
	if(!WandelBild.Ok())
	{
		std::cout<<"Kein Bild geladen\n"<<std::flush;
		return;
	}
	int nB, nH;
	unsigned char *urDaten = WandelBild.GetData();
	
	filter maske;
	if(maske.filterAnwenden(urDaten, WandelBild.GetWidth(), WandelBild.GetHeight(), nB, nH))
	{
		std::cout<<"SetData WandelBild - b = "<<nB<<" - h = "<<nH<<"\n"<<std::flush;
		std::cout<<"Resize Wandelbildgroesse\n"<<std::flush;
		//WandelBild.Resize(wxSize(nB, nH), wxPoint(0, 0));
	}else{
		std::cout<<"Loesche neuBild\n"<<std::flush;
	}
	Refresh();
	return;
}