#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Hybrid.h"
using namespace std;

Hybrid::Hybrid(int kbits)
{
	m = kbits;
	predictiontablesize = pow(2, m);
	prediction_table = new int[predictiontablesize];

	for (int i = 0; i < predictiontablesize; ++i) //all entries in the prediction table should be intitialised to 2
	{
		prediction_table[i] = 1;
	}

}

Hybrid::~Hybrid()
{
	delete prediction_table;
}

int Hybrid::prediction(unsigned int index)
{
	tableindex = index;
	return(prediction_table[tableindex]);
}

void Hybrid::update_table(unsigned int index, int actualtakenvalue)
{
	tableindex = index;
	taken_actual_status = actualtakenvalue;
	if (taken_actual_status == 1) //increment
	{
		if (prediction_table[tableindex] < 3)
		{
			++prediction_table[tableindex];
		}
	}

	if (taken_actual_status == 0) // decrement
	{
		if (prediction_table[tableindex] > 0)
		{
			--prediction_table[tableindex];
		}
	}
}

void Hybrid::getprint(void)
{
	cout << "FINAL CHOOSER CONTENTS" << endl;
	for (int i = 0; i < predictiontablesize; ++i)
	{
		cout << dec << i  << "  	 " << dec << prediction_table[i] << endl;
	}
}