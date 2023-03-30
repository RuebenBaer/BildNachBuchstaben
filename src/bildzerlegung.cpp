#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>

void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite) 
{
	int maxX = urBildBreite / zeichenBreite;
	int maxY = urBildHoehe / buchstabenHoehe;
	
	std::cout<<"zeichenBreite = "<<zeichenBreite<<'\n';
	std::cout<<"maxX / maxY : "<<maxX<<" / "<<maxY<<'\n';
	
	int Zerlegung[maxX * maxY];
	
	int anzBuchstaben = buchstabenBreite / zeichenBreite;
	std::cout<<"anzBuchstaben = "<<anzBuchstaben<<'\n';
	int Gesamtabstand;
	
	int urX, urY, urZ;
	int maskeX, maskeY, maskeZ;
	int aktAbstand;
	
	for(int x_aussen = 0; x_aussen < maxX; x_aussen++)
	{
		for(int y_aussen = 0; y_aussen < maxY; y_aussen++)
		{
			Gesamtabstand = std::numeric_limits<int>::max();
			for(int bstbNr = 0; bstbNr < anzBuchstaben; bstbNr++)
			{
				aktAbstand = urX = urY = urZ = maskeX = maskeY = maskeZ = 1;
				for(int x_innen = 0; x_innen < zeichenBreite; x_innen++)
				{
					for(int y_innen = 0; y_innen < buchstabenHoehe; y_innen++)
					{
						int urIndex = ((x_aussen * zeichenBreite + x_innen) + (y_aussen * buchstabenHoehe + y_innen) * urBildBreite) * 3;
						int bstIndex = ((x_innen + bstbNr * zeichenBreite)  + y_innen * buchstabenBreite) * 3;
						
						urX *= 1 + pow((urBild[urIndex] - buchstaben[bstIndex])/255, 2);
						urY *= 1 + pow((urBild[urIndex + 1] - buchstaben[bstIndex + 1])/255, 2);
						urZ *= 1 + pow((urBild[urIndex + 2] - buchstaben[bstIndex + 2])/255, 2);
					}
				}
				aktAbstand = urX * urY * urZ;
				if(aktAbstand < Gesamtabstand)
				{
					Zerlegung[x_aussen + y_aussen * maxX] = bstbNr;
					Gesamtabstand = aktAbstand;
				}
			}
		}
	}
	
	std::cout<<"Bildausgabe\n\n";
	for(int y_aussen = 0; y_aussen < maxY; y_aussen++)
	{
		for(int x_aussen = 0; x_aussen < maxX; x_aussen++)
		{
			std::cout<<(char)(32+Zerlegung[x_aussen + y_aussen * maxX]);
		}
		std::cout<<"\n";
	}
	return;
}