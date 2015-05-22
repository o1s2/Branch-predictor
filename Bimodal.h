#ifndef Bimodal_H_INCLUDED
#define Bimodal_H_INCLUDED

class Bimodal
{
public:
	Bimodal(int pc_bits_index);
	~Bimodal();
	int prediction(unsigned int index); //to get the prediction
	void update_table(unsigned int index, int actualtakenvalue);
	void getprint(void);

private:
	int m;
	int *prediction_table;//dynamic array intialised variable for prediction table
	unsigned int tableindex;
	int taken_actual_status;
	int predictiontablesize;
};

#endif 