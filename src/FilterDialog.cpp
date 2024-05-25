#include "FilterDialog.h"

FilterDialog::FilterDialog(filter *fltr,
				wxWindow *parent,
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
				:wxDialog(parent, id, title, pos, size, style, name)
{
	std::cout<<"Dialog Konstuktor\n"<<std::flush;
	//maske = fltr;
	
	if((maske->maskenGroesse % 2 == 0)||(maske->maskenGroesse == 1))
	{
		SetReturnCode(wxCANCEL);
		wxMessageDialog(this, wxT("Die Filtergröße muss ungerade sein."), wxT("Fehler bei Filtergröße")).ShowModal();
		QueueEvent(new wxCloseEvent(wxEVT_CLOSE_WINDOW));
	}
	std::cout<<"Dialog erneuern..."<<std::flush;
	DialogErneuern();
	std::cout<<"erfolgreich"<<std::endl;
	
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

	/*for(int i=0; i < maske->maskenGroesse; i++)
	{
		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		for(int k=0; k < maske->maskenGroesse; k++)
		{
			wxTextCtrl* txtctrl = new wxTextCtrl(this, 10000+(i+k*maske->maskenGroesse),
												wxString::Format("%.0f", (maske->filterMaske[i+k*maske->maskenGroesse])),
												wxPoint(0, 0), wxSize(30, 30), wxTE_CENTRE, numVal);
			if(txtctrl != NULL)
			{
				hSizer->Add(txtctrl, wxFIXED_MINSIZE|wxEXPAND);
				TextCtrlContainer.push_back(txtctrl);
			}
		}
		filterSizer->Add(hSizer);
	}*/

	for(int i=0; i < 3; i++)
	{
		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		for(int k=0; k < 3; k++)
		{
			wxTextCtrl* txtctrl = new wxTextCtrl(this, 10000+(i+k*3),
												wxString::Format("%.0f", (i+k*3)),
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
	hauptSizer->Add(new wxButton(this, wxID_OK, "OK"));
	hauptSizer->Add(new wxButton(this, BTN_ID_NEU, "Neuer Filter"));
	std::cout<<"SetSizer(hauptSizer);"<<std::endl;
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