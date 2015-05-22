#ifndef Gshare_H_INCLUDED
#define Gshare_H_INCLUDED

class Gshare
{
public:
	Gshare(int pc_bits_index, int BHRbits);
	~Gshare();
	unsigned int Makeindex(unsigned int pcindex);
	int prediction(unsigned int index); //to get the prediction
	void update_table(unsigned int index, int actualtakenvalue);
	void update_BHR(int actualtakenvalue);
	void getprint(void);

private:
	int *prediction_table;//dynamic array intialised variable for prediction table
	unsigned int BHRpc, pcindexvalue, xorBHR, xorBHRtemp, pctemp1, pcindexrest, gshareindex, BHRvaluetemp;
	int m;
	int BHRbitscount;
	int taken_actual_status;
	unsigned int BHRvalue ;
	int predictiontablesize, tablevalue;
};

#endif