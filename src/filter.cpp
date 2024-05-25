#include "filter.h"
#include <cstddef>

filter::filter(int gr)
{
	if((gr%2 != 0) && !(gr < 2))
	{
		filterMaske = new float[gr];
		maskenGroesse = gr;
	}
}

filter::~filter()
{
	std::cout<<"filterMaske loeschen"<<std::endl;
	delete [] filterMaske;
}

bool filter::filterAnwenden(unsigned char* urBild, int urBildBreite, int urBildHoehe, int& neuBreite, int& neuHoehe)
{
	neuBreite = urBildBreite - maskenGroesse + 1;
	neuHoehe = urBildHoehe - maskenGroesse + 1;
	unsigned char* neuBild = new unsigned char[3 * urBildBreite * urBildHoehe];
	if(NULL == neuBild)return false;
	
	for(int i = 0; i < urBildBreite; i++)
		for(int k = 0; k < urBildHoehe; k++)
		{
			neuBild[(i+k*urBildBreite)*3 + 0] = urBild[(i+k*urBildBreite)*3 + 0];
			neuBild[(i+k*urBildBreite)*3 + 1] = urBild[(i+k*urBildBreite)*3 + 1];
			neuBild[(i+k*urBildBreite)*3 + 2] = urBild[(i+k*urBildBreite)*3 + 2];
		}
	
	float wert;
	for(int x = 0; x < neuBreite; x++)
	{
		for(int y = 0; y < neuHoehe; y++)
		{
			wert = 0;
			for(int mx = -(maskenGroesse-1)/2; mx < (maskenGroesse-1)/2 + 1; mx++)
			{
				for(int my = -(maskenGroesse-1)/2; my < (maskenGroesse-1)/2 + 1; my++)
				{
					wert += (neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 0] +
							neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 1] +
							neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 2]) * 
							filterMaske[(mx+maskenGroesse-1)+(my+maskenGroesse-1)*maskenGroesse] / 3;
				}
			}
			
			wert = 255 * (wert < threshold);
			urBild[(x + y * urBildBreite) * 3 + 0] = wert;
			urBild[(x + y * urBildBreite) * 3 + 1] = wert;
			urBild[(x + y * urBildBreite) * 3 + 2] = wert;
		}
	}
	delete []neuBild;
	return true;
}

void filter::setzeGroesse(int gr)
{
	if((gr%2 != 0) && !(gr < 2))
	{
		maskenGroesse = gr;
		delete [] filterMaske;
		filterMaske = new float[gr];
	}
	return;
}