#include "bildzerlegung.h"


void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite) 
{
	int maxX = urBildBreite / zeichenBreite;
	int maxY = urBildHoehe / buchstabenHoehe;
	
	std::cout<<"zeichenBreite = "<<zeichenBreite<<'\n';
	std::cout<<"maxX / maxY : "<<maxX<<" / "<<maxY<<'\n';
	
	int Zerlegung[maxX * maxY];
	
	int anzBuchstaben = buchstabenBreite / zeichenBreite;
	std::cout<<"anzBuchstaben = "<<anzBuchstaben<<'\n';
	float Gesamtabstand;
	
	float urX, urY, urZ;
	float maskeX, maskeY, maskeZ;
	float aktAbstand;
	
	for(int x_aussen = 0; x_aussen < maxX; x_aussen++)
	{
		for(int y_aussen = 0; y_aussen < maxY; y_aussen++)
		{
			Gesamtabstand = std::numeric_limits<float>::max();
			for(int bstbNr = 0; bstbNr < anzBuchstaben; bstbNr++)
			{
				aktAbstand = urX = urY = urZ = maskeX = maskeY = maskeZ = 0;
				for(int x_innen = 0; x_innen < zeichenBreite; x_innen++)
				{
					for(int y_innen = 0; y_innen < buchstabenHoehe; y_innen++)
					{
						int urIndex = ((x_aussen * zeichenBreite + x_innen) + (y_aussen * buchstabenHoehe + y_innen) * urBildBreite) * 3;
						int bstIndex = ((x_innen + bstbNr * zeichenBreite)  + y_innen * buchstabenBreite) * 3;
						
						urX += urBild[urIndex]*0.299f + urBild[urIndex + 1]*0.587f + urBild[urIndex + 2]*0.114f;
						maskeX += buchstaben[bstIndex]*0.299f + buchstaben[bstIndex + 1]*0.587f + buchstaben[bstIndex + 2]*0.114f;
					}
				}
				aktAbstand = abs(urX - maskeX);
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

void BildZerlegenNormalverteilung(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite) 
{
	int maxX = urBildBreite / zeichenBreite;
	int maxY = urBildHoehe / buchstabenHoehe;
	
	std::cout<<"zeichenBreite = "<<zeichenBreite<<'\n';
	std::cout<<"maxX / maxY : "<<maxX<<" / "<<maxY<<'\n';
	
	int Zerlegung[maxX * maxY];
	
	int anzBuchstaben = buchstabenBreite / zeichenBreite;
	std::cout<<"anzBuchstaben = "<<anzBuchstaben<<'\n';
	float GesamtStreuung;

	float aktStreuung;
	
	int* einKanalBild = new int[zeichenBreite * buchstabenHoehe];
	
	for(int x_aussen = 0; x_aussen < maxX; x_aussen++)
	{
		for(int y_aussen = 0; y_aussen < maxY; y_aussen++)
		{
			GesamtStreuung = 0;//std::numeric_limits<float>::max();
			for(int bstbNr = 0; bstbNr < anzBuchstaben; bstbNr++)
			{
				aktStreuung = 0;
				for(int x_innen = 0; x_innen < zeichenBreite; x_innen++)
				{
					for(int y_innen = 0; y_innen < buchstabenHoehe; y_innen++)
					{
						int urIndex = ((x_aussen * zeichenBreite + x_innen) + (y_aussen * buchstabenHoehe + y_innen) * urBildBreite) * 3;
						int bstIndex = ((x_innen + bstbNr * zeichenBreite)  + y_innen * buchstabenBreite) * 3;
						
						einKanalBild [x_innen + y_innen * zeichenBreite] = (int)(urBild[urIndex]*0.299f + urBild[urIndex + 1]*0.587f + urBild[urIndex + 2]*0.114f);
						einKanalBild [x_innen + y_innen * zeichenBreite] -= (int)(buchstaben[bstIndex]*0.299f + buchstaben[bstIndex + 1]*0.587f + buchstaben[bstIndex + 2]*0.114f);
					}
				}
				NormalVerteilung::Parameter parm = NormalVerteilung::Analyse(einKanalBild, zeichenBreite, buchstabenHoehe);
				aktStreuung = parm.varianz;
				if(aktStreuung > GesamtStreuung)
				{
					Zerlegung[x_aussen + y_aussen * maxX] = bstbNr;
					GesamtStreuung = aktStreuung;
				}
			}
			std::cout<<(int)Zerlegung[x_aussen + y_aussen * maxX]<<"("<<GesamtStreuung<<")";
		}
		std::cout<<"\n";
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

NormalVerteilung::Parameter NormalVerteilung::Analyse(int* grayscale_Bild, int bildBreite, int bildHoehe)
{
	double mw, var;
	mw = 0;
	for(int b = 0; b < bildBreite; b++)
	{
		for(int h = 0; h < bildHoehe; h++)
		{
			mw += grayscale_Bild[b + h * bildBreite];
		}
	}
	mw /= (bildBreite * bildHoehe);
	
	var = 0;
	for(int b = 0; b < bildBreite; b++)
	{
		for(int h = 0; h < bildHoehe; h++)
		{
			var += pow(mw - grayscale_Bild[b + h * bildBreite], 2);
		}
	}
	var /= (bildBreite * bildHoehe - 1);
	
	Parameter par;
	par.mittelwert = mw;
	par.varianz = var;
	
	return par;
}

double NormalVerteilung::Uebereinstimmung(Parameter verteilung1, Parameter verteilung2)
{
	double rueckgabe = 0;
	return rueckgabe;
}