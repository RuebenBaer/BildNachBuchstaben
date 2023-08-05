#include "bildzerlegung.h"


void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite) 
{
	int maxX = urBildBreite / zeichenBreite;
	int maxY = urBildHoehe / buchstabenHoehe;
	
	std::cout<<"Zerlegung nach Helligkeit\n";
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


//NORMALVERTEILUNG
void BildZerlegenNormalverteilung(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite) 
{
	int maxX = urBildBreite / zeichenBreite;
	int maxY = urBildHoehe / buchstabenHoehe;
	
	std::cout<<"Zerlegung nach Normalverteilung\n";
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
				if(aktStreuung < GesamtStreuung)
				{
					Zerlegung[x_aussen + y_aussen * maxX] = bstbNr;
					GesamtStreuung = aktStreuung;
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

void NormalVerteilung::SchnittpunktNV(Parameter verteilung1, Parameter verteilung2, double &sp1, double &sp2)
{
	double mu1, mu2, qmu1, qmu2, qsig1, qsig2, lnsigquot;
	mu1 = verteilung1.mittelwert;
	mu2 = verteilung2.mittelwert;
	qmu1 = mu1 * mu1;
	qmu2 = mu2 * mu2;
	qsig1 = verteilung1.varianz;
	qsig2 = verteilung2.varianz;
	if(qsig1 == qsig2)
	{
		sp1 = sp2 = 0.5 * (mu1 + mu2);
		return;
	}
	if(qsig1 == 0)
	{
		sp1 = sp2 = mu1;
		return;
	}
	lnsigquot = log(sqrt(qsig2) / sqrt(qsig1));
	double Radikant = pow(mu1 * qsig2 - mu2 * qsig1, 2) - (qsig2 - qsig1) * (qmu1 * qsig2 - qmu2 * qsig1 - 2 * qsig1 * qsig2 * lnsigquot);
	
	std::printf("\nlnsigquot = %0.5f", lnsigquot);
	std::printf("\nRadikant = %0.5f\n\n", Radikant);
	
	if(Radikant < 0)
	{
		std::cout<<"Der Radikant (SchnittpunktNV) ist kleiner als Null!\n";
		sp1 = std::numeric_limits<double>::min();
		sp2 = std::numeric_limits<double>::max();
		return;
	}
	
	sp1 = (mu1 * qsig2 - mu2 * qsig1 + sqrt(Radikant)) / (qsig2 - qsig1);
	sp2 = (mu1 * qsig2 - mu2 * qsig1 - sqrt(Radikant)) / (qsig2 - qsig1);
	
	return;
}

//NORMALVERTEILUNG

//SCHWERPUNKT
void SchwerPunkt::BildZerlegenSchwerpunkt(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite)
{
	int maxX = urBildBreite / zeichenBreite;
	int maxY = urBildHoehe / buchstabenHoehe;
	
	std::cout<<"Schwerpunktzerlegung\n";
	std::cout<<"zeichenBreite = "<<zeichenBreite<<'\n';
	std::cout<<"maxX / maxY : "<<maxX<<" / "<<maxY<<'\n';
	
	int Zerlegung[maxX * maxY];
	
	double dFarbHoehenFkt = 0.02;
	int anzBuchstaben = buchstabenBreite / zeichenBreite;
	SchwerPunkt::schwerPunkt* buchstabenSP = new SchwerPunkt::schwerPunkt[anzBuchstaben];
	unsigned char* tempBild = new unsigned char[buchstabenHoehe * zeichenBreite * 3];
	
	for(int bstNr = 0; bstNr < anzBuchstaben; bstNr++)
	{
		for(int x = 0; x < zeichenBreite; x++)
		{
			for(int y = 0; y < buchstabenHoehe; y++)
			{
				for(int rgb = 0; rgb < 3; rgb++)
				{
					tempBild[(x + y * zeichenBreite) * 3 + rgb] = 
						buchstaben[(bstNr * zeichenBreite + x + y * buchstabenBreite) * 3 + rgb];
				}
			}
		}
		//std::cout<<(char)(32 + bstNr)<<",";
		SchwerPunkt::SchwerpunktBild(tempBild, zeichenBreite, buchstabenHoehe, dFarbHoehenFkt, buchstabenSP[bstNr]);
	}
	
	SchwerPunkt::schwerPunkt aktBildSP;
	//Teilbild extrahieren
	int iStelle;
	int zerlX = 0;
	for(int x = 0; x < urBildBreite - zeichenBreite; x += zeichenBreite)
	{
		int zerlY = 0;
		for(int y = 0; y < urBildHoehe - buchstabenHoehe; y += buchstabenHoehe)
		{
			for(int b = 0; b < zeichenBreite; b++)
			{
				for(int h = 0; h < buchstabenHoehe; h++)
				{
					for(int rgb = 0; rgb < 3; rgb++)
					{
						iStelle = (x + b + (y + h) * urBildBreite) * 3 + rgb;
						tempBild[(b + h * zeichenBreite) * 3 + rgb] = urBild[iStelle];
					}
				}
			}
			SchwerPunkt::SchwerpunktBild(tempBild, zeichenBreite, buchstabenHoehe, dFarbHoehenFkt, aktBildSP);
			Zerlegung[zerlX + zerlY * maxX] = SchwerPunkt::KleinsterSchwerpunktAbstand(aktBildSP, buchstabenSP, anzBuchstaben);
			zerlY++;
		}
		zerlX++;
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

	delete []buchstabenSP;
	delete []tempBild;
	
	return;
}

void SchwerPunkt::SchwerpunktBild(unsigned char *Bild, int iBreite, int iHoehe, double dFarbHoehenFkt, SchwerPunkt::schwerPunkt& swPkt)
{
	double dGesamtFarbHoehe[3];
	for(int achse = 0; achse < 3; achse++)
	{
		swPkt.wert[0][achse] = 0;
		swPkt.wert[1][achse] = 0;
		swPkt.wert[2][achse] = 0;
		dGesamtFarbHoehe[achse] = 0;
	}
	int iStelle;
	for(int x = 0; x < iBreite; x++)
	{
		for(int y = 0; y < iHoehe; y++)
		{
			iStelle = (x + y * iBreite) * 3;
			for(int rgb = 0; rgb < 3; rgb++)
			{
				dGesamtFarbHoehe[rgb] += Bild[iStelle + rgb];
				swPkt.wert[rgb][0] += Bild[iStelle + rgb] * x;
				swPkt.wert[rgb][1] += Bild[iStelle + rgb] * y;
				swPkt.wert[rgb][2] += Bild[iStelle + rgb] * Bild[iStelle + rgb] * dFarbHoehenFkt;
			}
		}
	}
	for(int rgb = 0; rgb < 3; rgb++)
	{
		for(int achse = 0; achse < 3; achse++)
		{
			swPkt.wert[rgb][achse] /= dGesamtFarbHoehe[rgb];
			//std::cout<<swPkt.wert[rgb][achse]<<",";
		}
		//if(rgb == 2)std::cout<<"\n";
	}
	return;
}

int SchwerPunkt::KleinsterSchwerpunktAbstand(SchwerPunkt::schwerPunkt vergleichSP, SchwerPunkt::schwerPunkt* listeSP, int listenLaenge)
{
	float kleinsterAbstand = std::numeric_limits<float>::max();
	float aktAbstand;
	int summenVektor[3];
	int rueckgabeSP = 0;
	for(int i = 0; i < listenLaenge; i++)
	{
		aktAbstand = 0;
		/*
		//abstandSchwerpunkte ausrechnen
		for(int rgb = 0; rgb < 3; rgb++)
		{
			summenVektor[rgb] = 0;
			for(int achse = 0; achse < 3; achse++)
			{
				summenVektor[rgb] += vergleichSP.wert[rgb][achse] - listeSP[i].wert[rgb][achse];
			}
			aktAbstand += summenVektor[rgb] * summenVektor[rgb];
		}
		aktAbstand = sqrt(aktAbstand);
		*/
		//abstandSchwerpunkte ausrechnen
		for(int rgb = 0; rgb < 3; rgb++)
		{
			summenVektor[rgb] = 0;
			for(int achse = 0; achse < 3; achse++)
			{
				summenVektor[rgb] += pow(vergleichSP.wert[rgb][achse] - listeSP[i].wert[rgb][achse], 2);
			}
			aktAbstand += sqrt(summenVektor[rgb]);
		}
		aktAbstand /= 3;
		//wenn neuer Abstand kleiner als gespeicherter -> neuen speichern
		if(aktAbstand < kleinsterAbstand)
		{
			kleinsterAbstand = aktAbstand;
			rueckgabeSP = i;
		}
	}
	return rueckgabeSP;
}