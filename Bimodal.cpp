#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Bimodal.h"

using namespace std;

Bimodal::Bimodal(int pc_bits_index)
{
	m = pc_bits_index;
	predictiontablesize = pow(2, m);
	prediction_table = new int[predictiontablesize];

	for (int i = 0; i < predictiontablesize; ++i) //all entries in the prediction table should be intitialised to 2
	{
		prediction_table[i] = 2;
		//cout << i <<"\t" << prediction_table[i] << endl;
	}
}

Bimodal::~Bimodal()
{
	delete prediction_table;
}

int Bimodal::prediction(unsigned int index)
{
	tableindex = index;
	//cout << tableindex << "\t" << prediction_table[tableindex] << endl;
	return (prediction_table[tableindex]);
}

void Bimodal::update_table(unsigned int index, int actualtakenvalue)
{
	tableindex = index;
	taken_actual_status = actualtakenvalue;
	if (taken_actual_status == 1)
	{
		if (prediction_table[tableindex] < 3)
		{
			++prediction_table[tableindex];
		}
	}

	if (taken_actual_status == 0)
	{
		if (prediction_table[tableindex] > 0)
		{
			--prediction_table[tableindex];
		}
	}
}

void Bimodal::getprint(void)
{
	cout << "FINAL BIMODAL CONTENTS" << endl;
	for (int i = 0; i < predictiontablesize; ++i)
	{
		cout << dec << i <<"	" << dec << prediction_table[i] << endl;
	}
}