#include "FilterDialog.h"

FilterDialog::FilterDialog(filter *maske,
				wxWindow* parent,
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
				:wxFrame(parent, id, title, pos, size, style, name)
{
	m_maske = maske;
	
	if((m_maske->HoleGroesse() % 2 == 0)||(m_maske->HoleGroesse() == 1))
	{
		wxMessageDialog(this, wxT("Die Filtergröße muss ungerade sein."), wxT("Fehler bei Filtergröße")).ShowModal();
	}else{
		DialogErneuern();
	}
	
	Bind(wxEVT_CLOSE_WINDOW, &FilterDialog::OnQuit, this);
	Bind(wxEVT_BUTTON, &FilterDialog::NeuerFilter, this, BTN_ID_NEU);
	Bind(wxEVT_BUTTON, &FilterDialog::FilterAnwenden, this, BTN_ID_ANWENDEN);
	Bind(wxEVT_BUTTON, &FilterDialog::OnOK, this, wxID_OK);
	Bind(wxEVT_TEXT_ENTER, &FilterDialog::FilterAnwenden, this, BTN_ID_THRESHOLD);
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
	//wxMessageDialog(this, wxString::Format("Filter wird angewendet"), "Filter anwenden").ShowModal();
	for(int i = 0; i < m_maske->HoleGroesse(); i++)
	{
		for(int k = 0; k < m_maske->HoleGroesse(); k++)
		{
			double wert;
			if(!TextCtrlContainer[i + k * m_maske->HoleGroesse()]->GetValue().ToDouble(&wert)) wert = 0;
			m_maske->SetzeInhalt(i, k, (float)wert);
		}
	}
	double wert;
	if(!filterTHTextCtrl->GetValue().ToDouble(&wert))wert = 0;
	m_maske->SetzeThreshold((float)wert);
	m_maske->Speichern();
	
	GetParent()->GetEventHandler()->QueueEvent(new wxCommandEvent(wxEVT_MENU, idMenuBildMaskieren));
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
	DestroyChildren();

	if(this->GetSizer() != NULL)
	{
		this->GetSizer()->Clear(true);
	}
	this->SetSizer(NULL, true);
	Layout();

	filterSizer = new wxBoxSizer(wxVERTICAL);
	hauptSizer = new wxBoxSizer(wxVERTICAL);

	wxNumericPropertyValidator numVal(wxNumericPropertyValidator::Float);

	for(int i=0; i < m_maske->HoleGroesse(); i++)
	{
		wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
		for(int k=0; k < m_maske->HoleGroesse(); k++)
		{
			wxTextCtrl* txtctrl = new wxTextCtrl(this, 10000+(i+k*m_maske->HoleGroesse()),
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
	buttonSizer->Add(new wxButton(this, BTN_ID_ANWENDEN, "Filter anwenden"));
	buttonSizer->Add(new wxButton(this, BTN_ID_NEU, "Neuer Filter"));
	hauptSizer->Add(buttonSizer);
	
	wxBoxSizer *thresholdSizer = new wxBoxSizer(wxHORIZONTAL);
	thresholdSizer->Add(new wxTextCtrl(this, BTN_ID_THRESHOLD, "Threshold setzen",
										wxPoint(0, 0), wxDefaultSize, wxTE_RIGHT|wxTE_READONLY), wxEXPAND);
	filterTHTextCtrl = new wxTextCtrl(this, BTN_ID_THRESHOLD, "", wxPoint(0, 0),
										wxDefaultSize, wxTE_RIGHT|wxTE_PROCESS_ENTER, numVal);
	thresholdSizer->Add(filterTHTextCtrl, wxEXPAND);
	hauptSizer->Add(thresholdSizer);
	
	SetSizer(hauptSizer);
	
	MatrizeFuellen();
	return;
}

bool FilterDialog::MatrizeFuellen()
{
	if(m_maske == NULL)return false;
	
	for(int i = 0; i < m_maske->HoleGroesse(); i++)
	{
		for(int k = 0; k < m_maske->HoleGroesse(); k++)
		{
			TextCtrlContainer[i + k * m_maske->HoleGroesse()]->SetValue(wxString::Format("%.1f", m_maske->HoleInhalt(i, k)));
		}
	}
	
	filterTHTextCtrl->SetValue(wxString::Format("%0.1f", m_maske->HoleThreshold()));

	return true;
}
	
void FilterDialog::OnQuit(wxCloseEvent &event)
{
	this->Destroy();
}