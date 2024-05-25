#ifndef __FILTER_H_
#define __FILTER_H_

#include <iostream>

class filter{
private:
	float threshold = 10.0;
public:
	filter(int gr);
	~filter();
	float *filterMaske;
	int maskenGroesse;
	bool filterAnwenden(unsigned char* urBild, int urBildBreite, int urBildHoehe, int& neuBreite, int& neuHoehe);
	void setzeGroesse(int gr);
};

#endif //__FILTER_H_