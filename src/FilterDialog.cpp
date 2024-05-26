#include "FilterDialog.h"

FilterDialog::FilterDialog(int fltGr,
				wxWindow *parent,
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
				:wxFrame(parent, id, title, pos, size, style, name)
{
	filterGroesse = fltGr;
	
	if((filterGroesse % 2 == 0)||(filterGroesse == 1))
	{
		wxMessageDialog(this, wxT("Die Filtergröße muss ungerade sein."), wxT("Fehler bei Filtergröße")).ShowModal();
		QueueEvent(new wxCloseEvent(wxEVT_CLOSE_WINDOW));
	}
	
	DialogErneuern();
	
	Bind(wxEVT_CLOSE_WINDOW, &FilterDialog::OnQuit, this);
	Bind(wxEVT_BUTTON, &FilterDialog::NeuerFilter, this, BTN_ID_NEU);
	Bind(wxEVT_BUTTON, &FilterDialog::FilterAnwenden, this, BTN_ID_ANWENDEN);
	Bind(wxEVT_BUTTON, &FilterDialog::OnOK, this, wxID_OK);
}

FilterDialog::~FilterDialog()
{
	std::cout<<"TextCtrlContainer leeren\n";
	while(!TextCtrlContainer.empty())
	{
		wxTextCtrl* txtctrl = TextCtrlContainer.back();
		TextCtrlContainer.pop_back();
		std::cout<<txtctrl<<" zerstoeren"<<std::endl;
		txtctrl->Destroy();
	};
	std::cout<<"FilterDialog verlassen...\n";
}

void FilterDialog::NeuerFilter(wxCommandEvent &event)
{
	DialogErneuern();
	Layout();
	Refresh();
	return;
}

void FilterDialog::FilterAnwenden(wxCommandEvent &event)
{
	wxMessageDialog(this, wxString::Format("Filter wird angewendet"), "Filter anwenden").ShowModal();
	return;
}

void FilterDialog::OnOK(wxCommandEvent &event)
{
	Show(false);
	return;
}

void FilterDialog::DialogErneuern()
{
	while(!TextCtrlContainer.empty())
	{
		wxTextCtrl* txtctrl = TextCtrlContainer.back();
		TextCtrlContainer.pop_back();
		try{
			txtctrl->Destroy();
		}
		catch(...){
			std::cout<<"txtctrl->Destroy(); fehlgeschlagen"<<std::endl;
		}
	};

	if(this->GetSizer() != NULL)
	{
		this->GetSizer()->Clear(true);
	}
	this->SetSizer(NULL, true);
	Layout();

	filterSizer = new wxBoxSizer(wxVERTICAL);
	hauptSizer = new wxBoxSizer(wxVERTICAL);

	wxNumericPropertyValidator numVal(wxNumericPropertyValidator::Float);

	for(int i=0; i < filterGroesse; i++)
	{
		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		for(int k=0; k < filterGroesse; k++)
		{
			wxTextCtrl* txtctrl = new wxTextCtrl(this, 10000+(i+k*filterGroesse),
												wxString::Format("%.0f", (float)(i+k*3)),
												wxPoint(0, 0), wxSize(30, 30), wxTE_CENTRE, numVal);
			if(txtctrl != NULL)
			{
				hSizer->Add(txtctrl, wxFIXED_MINSIZE|wxEXPAND);
				TextCtrlContainer.push_back(txtctrl);
			}
		}
		filterSizer->Add(hSizer);
	}

	hauptSizer->Add(filterSizer);
	
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_OK, "OK"));
	buttonSizer->Add(new wxButton(this, BTN_ID_NEU, "Neuer Filter"));
	hauptSizer->Add(buttonSizer);
	
	SetSizer(hauptSizer);
	return;
}

void FilterDialog::SetzeFilterGroesse(int groesse)
{
	wxMessageDialog(this, wxString::Format("Setzt die Filtergröße auf den Wert %d", groesse), "Filtergröße").ShowModal();
	return;
}

bool FilterDialog::MatrizeFuellen(filter *maske)
{
	if(maske == NULL)return false;
	if(maske->HoleGroesse() != filterGroesse)return false;
	
	return true;
}
	
void FilterDialog::OnQuit(wxCloseEvent &event)
{
	this->Destroy();
}