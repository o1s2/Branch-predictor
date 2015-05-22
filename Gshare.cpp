#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Gshare.h"

using namespace std;


Gshare::Gshare(int pc_bits_index, int BHRbits)
{
	m = pc_bits_index;
	BHRbitscount = BHRbits;
	predictiontablesize = pow(2, m);
	prediction_table = new int[predictiontablesize];
	BHRvalue = 0;

	for (int i = 0; i < predictiontablesize; ++i) //all entries in the prediction table should be intitialised to 2
	{
		prediction_table[i] = 2;
	}
	/*
	BHRmax = 0;
	for (int j = 0; j < BHRbitscount; ++j)
	{
		BHRmax += (1 << j);
	}
	*/
}

Gshare::~Gshare()
{
	delete prediction_table;
}

unsigned int Gshare::Makeindex(unsigned int pcindex)
{
	pcindexvalue = pcindex;
	BHRpc = pcindexvalue >> (m - BHRbitscount); // gives BHR bits of PC index
	xorBHR = BHRvalue ^ BHRpc; // XOR BHR and BHRpc value
	xorBHRtemp = xorBHR << (m - BHRbitscount); //shift the XOR to left so that it can be concatenated with rest bits
	pctemp1 = 0;
	for (int i = 0; i < (m - BHRbitscount); ++i) //gives all ones of rest bits to retrieve rest bits from PC index
	{
		pctemp1 += (1 << i);
	}
	pcindexrest = pcindexvalue & pctemp1;
	return(xorBHRtemp | pcindexrest);//returns the new index with XOR BHR
}

int Gshare::prediction(unsigned int index) //to get the prediction
{
	gshareindex = index;
	tablevalue = prediction_table[gshareindex];
	return(tablevalue);
}

void Gshare::update_table(unsigned int index, int actualtakenvalue)
{
	gshareindex = index;
	taken_actual_status = actualtakenvalue;
	if (taken_actual_status == 1)
	{
		if (prediction_table[gshareindex] < 3)
		{
			++prediction_table[gshareindex];
		}
		/*Right shift correction in code needed*/
		BHRvalue = (BHRvalue / 2) + (pow(2,(BHRbitscount - 1)));// BHR is shifted to left by 1 bit and 1 is added and masked with all ones to retrieve the BHR n bits
	}

	if (taken_actual_status == 0)
	{
		if (prediction_table[gshareindex] > 0)
		{
			--prediction_table[gshareindex];
		}
		BHRvalue = BHRvalue / 2;//BHR is shifted left by 1 bit and 0 is added and masked with all 1s to get BHR value
	}
}


void Gshare::update_BHR(int actualtakenvalue)
{
	taken_actual_status = actualtakenvalue;
	if (taken_actual_status == 1)
	{
		BHRvalue = (BHRvalue / 2) + (pow(2, (BHRbitscount - 1)));// BHR is shifted to left by 1 bit and 1 is added and masked with all ones to retrieve the BHR n bits
	}

	if (taken_actual_status == 0)
	{
		BHRvalue = BHRvalue / 2;//BHR is shifted left by 1 bit and 0 is added and masked with all 1s to get BHR value
	}

}


void Gshare::getprint(void)
{
	cout << "FINAL GSHARE CONTENTS" << endl;
	for (int i = 0; i < predictiontablesize; ++i)
	{
		cout << dec << i << " 	  " << dec << prediction_table[i] << endl;
	}
}
