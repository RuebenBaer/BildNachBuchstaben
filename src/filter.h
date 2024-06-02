#ifndef __FILTER_H_
#define __FILTER_H_

#include <iostream>
#include <fstream>
#include <cstddef>
#include <cmath>

class filter{
private:
	float threshold = 10.0;
	int maskenGroesse;
	float *filterMaske;
	void StandardFilter(void);
	bool Einlesen(void);
public:
	filter(int gr);
	~filter();
	bool FilterAnwenden(unsigned char* urBild, int urBildBreite, int urBildHoehe, int& neuBreite, int& neuHoehe);

	bool SetzeGroesse(int gr);
	int HoleGroesse(void) const {return maskenGroesse;};
	float HoleInhalt(int, int) const;
	void SetzeInhalt(int, int, float);
	float HoleThreshold(void) const;
	void SetzeThreshold(float);
	
	bool Speichern(void);
};

#endif //__FILTER_H_