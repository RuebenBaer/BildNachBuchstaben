#include "FilterDialog.h"

FilterDialog::FilterDialog(int filterGroesse,
				wxWindow *parent,
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
				:wxDialog(parent, id, title, pos, size, style, name)
{
	if((filterGroesse % 2 == 0)||(filterGroesse == 1))
	{
		SetReturnCode(wxCANCEL);
		wxMessageDialog(this, wxT("Die Filtergröße muss ungerade sein."), wxT("Fehler bei Filtergröße")).ShowModal();
		QueueEvent(new wxCloseEvent(wxEVT_CLOSE_WINDOW));
	}
	
	DialogErneuern(filterGroesse);
	
	Bind(wxEVT_CLOSE_WINDOW, &FilterDialog::OnQuit, this);
	Bind(wxEVT_BUTTON, &FilterDialog::NeuerFilter, this, BTN_ID_NEU);
	Bind(wxEVT_BUTTON, &FilterDialog::FilterAnwenden, this, wxID_OK);
}

FilterDialog::~FilterDialog()
{
	std::cout<<"TextCtrlContainer leeren\n";
	while(!TextCtrlContainer.empty())
	{
		wxTextCtrl* txtctrl = TextCtrlContainer.back();
		TextCtrlContainer.pop_back();
		txtctrl->Destroy();
	};
	std::cout<<"FilterDialog verlassen...\n";
}

void FilterDialog::NeuerFilter(wxCommandEvent &event)
{
	DialogErneuern(3);
	Layout();
	Refresh();
	return;
}

void FilterDialog::FilterAnwenden(wxCommandEvent &event)
{
	wxMessageDialog(this, wxString::Format("Filter wird angewendet"), "Filter anwenden").ShowModal();
	return;
}

void FilterDialog::DialogErneuern(int filterGroesse)
{
	while(!TextCtrlContainer.empty())
	{
		wxTextCtrl* txtctrl = TextCtrlContainer.back();
		TextCtrlContainer.pop_back();
		txtctrl->Destroy();
	};

	if(this->GetSizer() != NULL)
	{
		this->GetSizer()->Clear(true);
	}
	this->SetSizer(NULL, true);
	Layout();

	filterSizer = new wxBoxSizer(wxVERTICAL);
	hauptSizer = new wxBoxSizer(wxVERTICAL);

	wxNumericPropertyValidator numVal(wxNumericPropertyValidator::Signed);
	
	for(int i=0; i< filterGroesse; i++)
	{
		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		for(int k=0; k< filterGroesse; k++)
		{
			wxTextCtrl* txtctrl = new wxTextCtrl(this, 10000+(i*filterGroesse+k), wxString::Format("%d", i*filterGroesse+k), wxPoint(0, 0), wxSize(25, 25), wxTE_CENTRE, numVal);
			hSizer->Add(txtctrl, wxFIXED_MINSIZE|wxEXPAND);
			
			TextCtrlContainer.push_back(txtctrl);
		}
		filterSizer->Add(hSizer);
	}

	hauptSizer->Add(filterSizer);
	hauptSizer->Add(new wxButton(this, wxID_OK, "OK"));
	hauptSizer->Add(new wxButton(this, BTN_ID_NEU, "Neuer Filter"));
	SetSizer(hauptSizer);
	return;
}

void FilterDialog::SetzeFilterGroesse(int groesse)
{
	wxMessageDialog(this, wxString::Format("Setzt die Filtergröße auf den Wert %d", groesse), "Filtergröße").ShowModal();
	return;
}
	
void FilterDialog::OnQuit(wxCloseEvent &event)
{
	this->Destroy();
}