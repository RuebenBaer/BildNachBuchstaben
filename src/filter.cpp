#include "filter.h"

filter::filter()
{
	filterMaske = NULL;
	if(!Einlesen())
	{
		std::cout<<"StandardFilter wird erzeugt\n";
		StandardFilter();
	}
	MaskeMitteln();
	std::cout<<"filter const verlassen\n";
}

filter::~filter()
{
	std::cout<<"filter destr: filterMaske loeschen"<<std::endl;
	if(filterMaske)delete [] filterMaske;
}

bool filter::FilterAnwenden(unsigned char* urBild, int urBildBreite, int urBildHoehe, int& neuBreite, int& neuHoehe)
{
	neuBreite = urBildBreite - maskenGroesse + 1;
	neuHoehe = urBildHoehe - maskenGroesse + 1;
	unsigned char* neuBild = new unsigned char[3 * urBildBreite * urBildHoehe];
	if(NULL == neuBild)return false;
	if(!filterMaske)return false;
	
	for(int i = 0; i < urBildBreite; i++)
		for(int k = 0; k < urBildHoehe; k++)
		{
			neuBild[(i+k*urBildBreite)*3 + 0] = urBild[(i+k*urBildBreite)*3 + 0];
			neuBild[(i+k*urBildBreite)*3 + 1] = urBild[(i+k*urBildBreite)*3 + 1];
			neuBild[(i+k*urBildBreite)*3 + 2] = urBild[(i+k*urBildBreite)*3 + 2];
		}
	
	float wert;
	int halbeMaske = maskenGroesse / 2;
	for(int x = 0; x < neuBreite; x++)
	{
		for(int y = 0; y < neuHoehe; y++)
		{
			wert = 0;
			for(int mx = -halbeMaske; mx < halbeMaske + 1; mx++)
			{
				for(int my = -halbeMaske; my < halbeMaske + 1; my++)
				{
					wert += (neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 0] +
							neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 1] +
							neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 2]) * 
							filterMaske[(mx+halbeMaske)+(my+halbeMaske)*maskenGroesse] / 3;
				}
			}
			
			if(maskenMittel)
			{
				wert /= maskenMittel;
				if (wert > 255) wert = 255;
				if (wert < 0) wert = 0;
			}else
			{
				wert = 255 * (wert < threshold);
			}
			urBild[(x + y * urBildBreite) * 3 + 0] = wert;
			urBild[(x + y * urBildBreite) * 3 + 1] = wert;
			urBild[(x + y * urBildBreite) * 3 + 2] = wert;
		}
	}
	delete []neuBild;
	return true;
}

bool filter::SetzeGroesse(int gr)
{
	if((gr%2 != 0) && !(gr < 2))
	{
		maskenGroesse = gr;
		if(filterMaske)
		{
			delete [] filterMaske;
			filterMaske = NULL;
		}
		filterMaske = new float[gr*gr];
		if(filterMaske)
		{
			MaskeMitteln();
			return true;
		}
	}
	return false;
}

float filter::HoleInhalt(int a, int b) const
{
	if(!(a < maskenGroesse))return NAN;
	if(!(b < maskenGroesse))return NAN;
	if(!filterMaske)return NAN;

	return filterMaske[a + b * maskenGroesse];
}

void filter::SetzeInhalt(int a, int b, float wert)
{
	if(!(a < maskenGroesse))return;
	if(!(b < maskenGroesse))return;
	if(!filterMaske)return;
	
	filterMaske[a + b * maskenGroesse] = wert;
	
	MaskeMitteln();
	
	return;
}

float filter::HoleThreshold(void) const
{
	return threshold;
}

void filter::SetzeThreshold(float th)
{
	threshold = th;
	return;
}

bool filter::Einlesen(void)
{
	if(!filterMaske)return false;
	std::fstream datei;
	datei.open("filter.flt", std::ios::in|std::ios::binary);
	if(!datei.good())return false;
	
	int dimension;
	datei.read((char*)(&dimension), sizeof(dimension));
	std::cout<<"Gespeicherte Groesse = "<<dimension<<"\n";
	if(maskenGroesse != dimension)
	{
		if(!SetzeGroesse(dimension))
		{
			std::cerr<<"Groesse des Filters ("<<dimension<<") muss groesse als zwei und ungerade sein!\n";
			std::cerr<<"Voreingestellter Filter wird verwendet\n";
			StandardFilter();
			return false;
		}
	}
	for(int i = 0; i < dimension; i++)
	{
		for(int k = 0; k < dimension; k++)
		{
			datei.read((char*)&(filterMaske[i + k * dimension]), sizeof(float));
		}
	}
	datei.read((char*)&threshold, sizeof(float));
	datei.close();
	MaskeMitteln();
	return true;
}

bool filter::Speichern(void)
{
	if(filterMaske == NULL)return false;
	
	std::ofstream output;
	output.open("filter.flt", std::ios::out|std::ios::binary);
	if(!output.good())return false;
	
	output.write((char*)&maskenGroesse, sizeof(int));
	for(int i = 0; i < maskenGroesse; i++)
	{
		for(int k = 0; k < maskenGroesse; k++)
		{
			output.write((char*)&(filterMaske[i + k * maskenGroesse]), sizeof(float));
		}
	}
	output.write((char*)&threshold, sizeof(float));
	output.close();
	return true;
}

void filter::StandardFilter(void)
{
	if(!SetzeGroesse(3))return;
	filterMaske[0] = 0;
	filterMaske[1] = 4;
	filterMaske[2] = 0;

	filterMaske[3] = 4;
	filterMaske[4] = -20;
	filterMaske[5] = 4;

	filterMaske[6] = 0;
	filterMaske[7] = 4;
	filterMaske[8] = 0;

	threshold = 10.0;

	MaskeMitteln();
	Speichern();
	return;
}

void filter::MaskeMitteln(void)
{
	maskenMittel = 0;
	for(int i = 0; i < maskenGroesse; i++)
		for(int k = 0; k < maskenGroesse; k++)
		{
			maskenMittel += filterMaske[i + k * maskenGroesse];
		}
	std::cout<<"maskenMittel: "<<maskenMittel<<"\n";
	return;
}