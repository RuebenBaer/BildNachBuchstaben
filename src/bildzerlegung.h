#ifndef __BILDZERLEGUNG_H
#define __BILDZERLEGUNG_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>


void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite);
void BildZerlegenNormalverteilung(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite);

namespace NormalVerteilung
{
	struct Parameter{
		double mittelwert;
		double varianz;
	};
	
	Parameter Analyse(int* einKanalBild, int bildBreite, int bildHoehe);
	double Uebereinstimmung(Parameter verteilung1, Parameter verteilung2);
	void SchnittpunktNV(Parameter verteilung1, Parameter verteilung2, double &sp1, double &sp2);
}

namespace SchwerPunkt
{
	typedef struct{
		double wert[3];//[achse]
	}schwerPunkt;

	void BildZerlegenSchwerpunkt(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite, double fkt);
	int KleinsterSchwerpunktAbstand(schwerPunkt vergleichSP, schwerPunkt* listeSP, int listenLaenge);
	void SchwerpunktBild(unsigned char *Bild, int iBreite, int iHoehe, double dFarbHoehenFkt, schwerPunkt& swPkt);
}
#endif //__BILDZERLEGUNG_H