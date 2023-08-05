#include <iostream>
#include "bildzerlegung.h"

int main(int argc, char** argv)
{
	NormalVerteilung::Parameter nv1, nv2;
	std::cout<<"Werte fuer erste Glocke:\nMittelwert: ";
	std::cin>>nv1.mittelwert;
	std::cout<<"Varianz:";
	std::cin>>nv1.varianz;
	
	std::cout<<"Werte fuer zweite Glocke:\nMittelwert: ";
	std::cin>>nv2.mittelwert;
	std::cout<<"Varianz:";
	std::cin>>nv2.varianz;
	
	std::printf("Eingaben:\nmw 1: %0.5f | var 1: %0.5f\nmw 2: %0.5f | var 2: %0.5f\n\n",
					nv1.mittelwert, nv1.varianz, nv2.mittelwert, nv2.varianz);
	
	double sp1, sp2;
	NormalVerteilung::SchnittpunktNV(nv1, nv2, sp1, sp2);
	
	std::printf("Schnittpunkte sind: %0.5f | %0.5f\n", sp1, sp2);
	return 0;
}