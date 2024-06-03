#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "base.h"

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

	maske = new filter();
	FilterDialogErneuern();
	
    CreateStatusBar(2);
    SetStatusText(_("Hello World!"));
	
	
	Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, ID_MAINWIN_QUIT);
	Bind(wxEVT_MENU, &MainFrame::OnOpenBild, this, IdMenuOpenPic);
	Bind(wxEVT_MENU, &MainFrame::OnFarbFaktor, this, idMenuFarbFaktor);
	Bind(wxEVT_MENU, &MainFrame::OnBildInBuchstabe, this, idMenuBildInBuchstabe);
	Bind(wxEVT_MENU, & MainFrame::OnBildMaske, this, idMenuBildMaskieren);
}

MainFrame::~MainFrame()
{
	if(maske)delete maske;
	if(FltDlg)FltDlg->Destroy();
	std::cout<<"Alles kaputt gemacht\n";
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
	
	if(ArbeitsBild.Ok())
	{
		ArbeitsBild.Destroy();
		std::cout<<"ArbeitsBild ist "<<ArbeitsBild.Ok()<<std::endl;
	}
	
	Refresh();
	return;
}

void MainFrame::OnBildInBuchstabe(wxCommandEvent& event)
{
	if(ArbeitsBild.Ok())
	{
		ArbeitsBild.Destroy();
		std::cout<<"ArbeitsBild ist "<<ArbeitsBild.Ok()<<std::endl;
	}
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

	if(ArbeitsBild.Ok())
	{
		dc.DrawBitmap(wxBitmap(ArbeitsBild), 0, 0);
	}
	else if(WandelBild.Ok())
	{
		if(BildMaske.Ok())dc.DrawBitmap(wxBitmap(BildMaske), 0, 0);
		dc.DrawBitmap(wxBitmap(WandelBild), 0, BildMaske.GetHeight());
	}else{
		//std::cout<<"Wandelbild defekt\n";
	}
	
	return;
}

void MainFrame::OnBildMaske(wxCommandEvent& event)
{
	if(maske == NULL)
	{
		std::cout<<"Maske erstellen fehlgeschlagen\n";
		return;
	}
	std::cout<<"Maske:\n";
	int mgr = maske->HoleGroesse();
	for(int k = 0; k < mgr; k++)
	{
		for(int i = 0; i < mgr; i++)
		{
			std::cout<<"\t"<<maske->HoleInhalt(i, k);
		}
		std::cout<<"\n";
	}
	std::cout<<"Threshold: "<<maske->HoleThreshold()<<"\n";
	if(FltDlg)
	{
		FltDlg->Show();
	}
	
	if(!WandelBild.Ok())
	{
		std::cout<<"Kein Bild geladen\n"<<std::flush;
		return;
	}
	int nB, nH;
	if(ArbeitsBild.Ok())ArbeitsBild.Destroy();
	ArbeitsBild = WandelBild.Copy();
	unsigned char *urDaten = ArbeitsBild.GetData();
	
	
	if(maske->FilterAnwenden(urDaten, ArbeitsBild.GetWidth(), ArbeitsBild.GetHeight(), nB, nH))
	{
		std::cout<<"SetData ArbeitsBild - b = "<<nB<<" - h = "<<nH<<"\n"<<std::flush;
		std::cout<<"Resize ArbeitsBildgroesse\n"<<std::flush;
		ArbeitsBild.Resize(wxSize(nB, nH), wxPoint(0, 0));
	}
	Refresh();
	return;
}

void MainFrame::FilterDialogErneuern(void)
{
	if(maske == NULL) return;
	if(FltDlg != NULL)
	{
		FltDlg->Destroy();
		FltDlg = NULL;
	}
	
	FltDlg = new FilterDialog(maske, this, wxID_ANY,
							wxString::Format("%dx%d-Maske", maske->HoleGroesse(), maske->HoleGroesse()));
	return;
}

void MainFrame::FilterAnwenden()
{
	QueueEvent(new wxCommandEvent(wxEVT_MENU, idMenuBildMaskieren));
	return;
}
