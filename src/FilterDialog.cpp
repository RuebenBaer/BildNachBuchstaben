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
	
	wxNumericPropertyValidator numVal(wxNumericPropertyValidator::Signed);
	
	wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
	for(int i=0; i< filterGroesse; i++)
	{
		filterSizer = new wxBoxSizer(wxHORIZONTAL);
		for(int k=0; k< filterGroesse; k++)
		{
			filterSizer->Add(new wxTextCtrl(this, 10000+(i*filterGroesse+k), "", wxPoint(0, 0), wxSize(25, 25), wxTE_CENTRE, numVal),wxFIXED_MINSIZE|wxEXPAND);
		}
		vSizer->Add(filterSizer);
	}
	SetSizer(vSizer);
	//filterSizer->FitInside(this);
	
	Bind(wxEVT_CLOSE_WINDOW, &FilterDialog::OnQuit, this);
}
FilterDialog::~FilterDialog()
{}

void FilterDialog::SetzeFilterGroesse(int groesse)
{
	wxMessageDialog(this, wxString::Format("Setzt die Filtergröße auf den Wert %d", groesse), "Filtergröße").ShowModal();
	return;
}
	
void FilterDialog::OnQuit(wxCloseEvent &event)
{
	this->Destroy();
}