#ifndef __BILDZERLEGUNG_H
#define __BILDZERLEGUNG_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>


void BildZerlegen(unsigned char* urBild, int urBildBreite, int urBildHoehe, unsigned char* buchstaben, int buchstabenBreite, int buchstabenHoehe, int zeichenBreite);

namespace NormalVerteilung
{
	struct Parameter{
		double mittelwert;
		double varianz;
	};
	
	Parameter Analyse(unsigned char* rgb_Bild, int bildBreite, int bildHoehe);
	double Uebereinstimmung(Parameter verteilung1, Parameter verteilung2);
}


#endif //__BILDZERLEGUNG_H