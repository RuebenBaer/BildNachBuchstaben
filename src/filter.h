#ifndef __FILTER_H_
#define __FILTER_H_

struct filter{
	int maskenGroesse = 3;
	float filterMaske[3][3] =
		{{-1, -1, -1},
		{0, 0, 0},
		{1, 1, 1}};
	bool filterAnwenden(unsigned char* urBild, int urBildBreite, int urBildHoehe, int& neuBreite, int& neuHoehe);
}maske;

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
		std::cout<<"Spalte "<<x<<"\n";
		for(int y = 0; y < neuHoehe; y++)
		{
			wert = 0;
			for(int mx = -(maskenGroesse-1)/2; mx < (maskenGroesse-1)/2 + 1; mx++)
			{
				for(int my = -(maskenGroesse-1)/2; my < (maskenGroesse-1)/2 + 1; my++)
				{
					wert += neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 0] * filterMaske[mx+maskenGroesse-1][my+maskenGroesse-1];
					wert += neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 1] * filterMaske[mx+maskenGroesse-1][my+maskenGroesse-1];
					wert += neuBild[(x + 1 + mx + (y + 1 + my) * urBildBreite) *3 + 2] * filterMaske[mx+maskenGroesse-1][my+maskenGroesse-1];
				}
			}
			wert /= (3 * maskenGroesse * maskenGroesse);
			urBild[(x + y * urBildBreite) * 3 + 0] = wert;
			urBild[(x + y * urBildBreite) * 3 + 1] = wert;
			urBild[(x + y * urBildBreite) * 3 + 2] = wert;
		}
	}
	delete []neuBild;
	return true;
}

#endif //__FILTER_H_