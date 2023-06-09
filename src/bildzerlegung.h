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
}

void BildZerlegenSchwerpunkt(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite);

typedef struct{
	double wert[3][3];//[rgb][achse]
}schwerPunkt;

void SchwerpunktBild(unsigned char *Bild, int iBreite, int iHoehe, double dFarbHoehenFkt, schwerPunkt& swPkt);
#endif //__BILDZERLEGUNG_H