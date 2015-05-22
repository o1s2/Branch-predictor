#ifndef Hybrid_H_INCLUDED
#define Hybrid_H_INCLUDED

class Hybrid
{
public:
	Hybrid(int kbits);
	~Hybrid();
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