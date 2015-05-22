#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <stdlib.h>
#include "Bimodal.h"
#include "Gshare.h"
#include "Hybrid.h"
#include "BTB.h"

using namespace std;

int main(int argc, char *argv[])
{
	/*For Bimodal predictor*/
	if (argc == 6)
	{
		int pc_bits_index = atoi(argv[2]);
		int BTB_size = atoi(argv[3]);
		int BTB_assoc = atoi(argv[4]);
		char *trace_file = argv[5];

		//cout << pc_bits_index << "\t" << BTB_size << "\t" << BTB_assoc << "\t" << trace_file;

		ifstream myfile;
		myfile.open(trace_file);

		unsigned long int programcounter_hex;
		char Taken_actual_status;
		char Taken_prediction_status;
		char BTB_prediction_status;

		unsigned int pctemp, pctemp1 = 0, pcindex;

		int getpredictioncounter, BTB_miss_hit;
		int noofbranches = 0, branchmispredictions = 0, BTBhits = 0, missBTBandtaken = 0, hitBTBmisprediction = 0;
		float rate;

		Bimodal bimodalpredictor = Bimodal(pc_bits_index); //initialise object of Bimodal class
		BTB BTBcache = BTB(BTB_size, BTB_assoc);


		while (myfile >> hex >> programcounter_hex >> Taken_actual_status)
		{
			//cout << "pcinput \t" << programcounter_hex << "taken status \t" << Taken_actual_status << endl;

			if (myfile.eof() || (programcounter_hex == 0) || (Taken_actual_status == '\0'))
			{
				break;
			}
			++noofbranches; //branch count

			pctemp = programcounter_hex >> 2; //drop the least significant 2 bits or right most 2 bits

			//cout << "pctemp after shifting right most 2 bits \t" << pctemp << endl;
			pctemp1 = 0;
			for (int i = 0; i < pc_bits_index; ++i) //to generate pc index bits of all ones
			{
				pctemp1 += (1 << i);
				//cout << "pcindex all 1 \t" << pctemp1 << endl;
			}

			pcindex = pctemp & pctemp1; //gives the index bits of the program counter
			//cout << "pcindex \t" << pcindex << endl;

			getpredictioncounter = bimodalpredictor.prediction(pcindex); //gets the value of the counter for the index passed

			if (getpredictioncounter >= 2) //predict the taken or not taken status based on the counter value
			{
				Taken_prediction_status = 't';
			}
			else
			{
				Taken_prediction_status = 'n';
			}
			/*BTB start here*/
			if (BTB_size != 0) //if the cache size is not equal to zero
			{
				BTB_miss_hit = BTBcache.Read(programcounter_hex); //returns 0 for hit and 1 for miss
				if (BTB_miss_hit == 0)//hit
				{
					++BTBhits;
					//cout << "BTB hit Bimodal index \t" << pcindex << "old value" << getpredictioncounter << "new value" << Taken_actual_status << endl;

					if (Taken_actual_status != Taken_prediction_status)
					{
						++hitBTBmisprediction;
					}

				}
				if (BTB_miss_hit == 1) //miss
				{
					BTB_prediction_status = 'n';
					if (Taken_actual_status == 't')
					{
						++missBTBandtaken;
					}
					goto missinBTB;
				}
			}




			if ((Taken_actual_status == 't') || (Taken_actual_status == 'T'))
			{
				bimodalpredictor.update_table(pcindex, 1); //1 for taken and 0 for not taken
			}

			if ((Taken_actual_status == 'n') || (Taken_actual_status == 'N'))
			{
				bimodalpredictor.update_table(pcindex, 0); //1 for taken and 0 for not taken
			}

		missinBTB:
			if (Taken_actual_status != Taken_prediction_status)
			{
				++branchmispredictions;
			}


		}
		myfile.close();

		if (BTB_size == 0)
		{

			cout << "COMMAND" << endl;
			cout << "./sim bimodal " << dec << pc_bits_index << " " << dec << BTB_size << " " << dec << BTB_assoc << " " << trace_file << " " << endl;
			cout << "OUTPUT" << endl;
			cout << "number of predictions:	 " << dec << noofbranches << endl;
			cout << "number of mispredictions:	 " << dec << branchmispredictions << endl;
			rate = ((float)branchmispredictions / noofbranches) * 100;
			printf("misprediction rate:    %0.2f \% \n", rate);
			bimodalpredictor.getprint();
		}


		if (BTB_size != 0)
		{
			cout << "COMMAND" << endl;
			cout << "./sim bimodal " << dec << pc_bits_index << " " << dec << BTB_size << " " << dec << BTB_assoc << " " << trace_file << " " << endl;
			cout << "OUTPUT" << endl;
			cout << "size of BTB:	  " << dec << BTB_size << endl;
			cout << "number of branches:	  " << dec << noofbranches << endl;
			cout << "number of predictions from branch predictor:	 " << dec << BTBhits << endl;
			cout << "number of mispredictions from branch predictor:	 " << dec << hitBTBmisprediction << endl;
			cout << "number of branches miss in BTB and taken:	  " << dec << missBTBandtaken << endl;
			cout << "total mispredictions:	 " << dec << hitBTBmisprediction + missBTBandtaken << endl;
			//cout << setprecision(2);
			rate = ((float)(hitBTBmisprediction + missBTBandtaken) / noofbranches) * 100;
			printf("misprediction rate:    %0.2f \% \n", rate);
			cout << endl;
			BTBcache.getprint();
			cout << endl;
			bimodalpredictor.getprint();
		}
		
	}

	if (argc == 7)
	{
		int pc_bits_index = atoi(argv[2]);
		int BHRbits = atoi(argv[3]);
		int BTB_size = atoi(argv[4]);
		int BTB_assoc = atoi(argv[5]);
		char *trace_file = argv[6];

		//cout << "pc bits \t" << dec << pc_bits_index << "BHR bits \t" << dec << BHRbits << "BTB size \t" << dec << BTB_size << "assoc \t" << dec << BTB_assoc << "file \t" << trace_file << endl;

		ifstream myfile;
		myfile.open(trace_file);

		unsigned long int programcounter_hex;
		char Taken_actual_status;
		char Taken_prediction_status;
		char BTB_prediction_status;

		unsigned int pctemp, pctemp1 = 0, pcindex, ghareindex;

		int getpredictioncounter;
		int noofbranches = 0, branchmispredictions = 0, BTBhits = 0, missBTBandtaken = 0, hitBTBmisprediction = 0, BTB_miss_hit;
		float rate;

		Gshare gsharepredictor = Gshare(pc_bits_index, BHRbits); //initialise object of Gshare class
		BTB BTBcache = BTB(BTB_size, BTB_assoc);

		while (myfile >> hex >> programcounter_hex >> Taken_actual_status)
		{
			if (myfile.eof() || (programcounter_hex == 0) || (Taken_actual_status == '\0'))
			{
				break;
			}
			++noofbranches;

			pctemp = programcounter_hex >> 2; //drop the least significant 2 bits or right most 2 bits

			pctemp1 = 0;
			for (int i = 0; i < pc_bits_index; ++i) //to generate pc index bits of all ones
			{
				pctemp1 += (1 << i);
			}

			pcindex = pctemp & pctemp1; //gives the index bits of the program counter

			ghareindex = gsharepredictor.Makeindex(pcindex);

			//cout << "pc address \t" << programcounter_hex << "\t" << pctemp << "\t" << pctemp1 << "pc index \t" << pcindex << "gshare index\t" << ghareindex << endl;

			getpredictioncounter = gsharepredictor.prediction(ghareindex); //gets the value of the counter for the index passed
			if (getpredictioncounter >= 2) //predict the taken or not taken status based on the counter value
			{
				Taken_prediction_status = 't';
			}
			else
			{
				Taken_prediction_status = 'n';
			}

			/*BTB start here*/
			if (BTB_size != 0) //if the cache size is not equal to zero
			{
				BTB_miss_hit = BTBcache.Read(programcounter_hex); //returns 0 for hit and 1 for miss
				if (BTB_miss_hit == 0)//hit
				{
					++BTBhits;


					if (Taken_actual_status != Taken_prediction_status)
					{
						++hitBTBmisprediction;
					}

				}
				if (BTB_miss_hit == 1) //miss
				{
					BTB_prediction_status = 'n';
					if (Taken_actual_status == 't')
					{
						++missBTBandtaken;
					}
					goto missBTBgshare;
				}

			}



			if ((Taken_actual_status == 't') || (Taken_actual_status == 'T'))
			{
				gsharepredictor.update_table(ghareindex, 1); //1 for taken and 0 for not taken
			}

			if ((Taken_actual_status == 'n') || (Taken_actual_status == 'N'))
			{
				gsharepredictor.update_table(ghareindex, 0); //1 for taken and 0 for not taken
			}

		missBTBgshare:

			if (Taken_actual_status != Taken_prediction_status)
			{
				++branchmispredictions;
			}
		}

		myfile.close();

		if (BTB_size == 0)
		{
			cout << "COMMAND" << endl;
			cout << "./sim gshare " << dec << pc_bits_index << " " << dec << BHRbits << " " << dec << BTB_size << " " << dec << BTB_assoc << " " << trace_file << endl;
			cout << "OUTPUT" << endl;
			cout << "number of predictions:	 " << dec << noofbranches << endl;
			cout << "number of mispredictions:	 " << dec << branchmispredictions << endl;
			rate = ((float)branchmispredictions / noofbranches) * 100;
			printf("misprediction rate:    %0.2f \% \n", rate);
			gsharepredictor.getprint();
		}

		if (BTB_size != 0)
		{
			cout << "COMMAND" << endl;
			cout << "./sim gshare " << dec << pc_bits_index << " " << dec << BHRbits << " " << dec << BTB_size << " " << dec << BTB_assoc << " " << trace_file << endl;
			cout << "OUTPUT" << endl;
			cout << "size of BTB:	 " << dec << BTB_size << endl;
			cout << "number of branches:	  " << dec << noofbranches << endl;
			cout << "number of predictions from branch predictor:	 " << dec << BTBhits << endl;
			cout << "number of mispredictions from branch predictor:	 " << dec << hitBTBmisprediction << endl;
			cout << "number of branches miss in BTB and taken:	  " << dec << missBTBandtaken << endl;
			cout << "total mispredictions:	 " << dec << hitBTBmisprediction + missBTBandtaken << endl;
			rate = ((float)(hitBTBmisprediction + missBTBandtaken) / noofbranches) * 100;
			printf("misprediction rate:    %0.2f \% \n", rate);
			cout << endl;
			BTBcache.getprint();
			cout << endl;
			gsharepredictor.getprint();
		}
		
	}

	if (argc == 9)
	{
		int kbits = atoi(argv[2]);
		int pc_bits_index_gshare = atoi(argv[3]);
		int BHRbits = atoi(argv[4]);
		int pc_bits_index_bimodal = atoi(argv[5]);
		int BTB_size = atoi(argv[6]);
		int BTB_assoc = atoi(argv[7]);
		char *trace_file = argv[8];

		ifstream myfile;
		myfile.open(trace_file);

		unsigned long int programcounter_hex;
		char Taken_actual_status;
		char Taken_Bimodal_prediction_status;
		char Taken_Gshare_prediction_status;
		char Taken_Hybrid_prediction_status;

		unsigned int pctempBimodal, pctempGshare, pctempBimodal1 = 0, pctempGshare1 = 0, pcindexBimodal, pcindexGshare, ghareindex;
		unsigned int pctempHybrid, pctempHybrid1 = 0, pcindexHybrid;

		int getBimodalpredictioncounter;
		int getGsharepredictioncounter;
		int getHybridpredictioncounter;

		int noofbranches = 0, branchmispredictions = 0, BTBhits = 0, missBTBandtaken = 0, hitBTBmisprediction = 0, BTB_miss_hit, BTB_prediction_status;
		float rate;

		Bimodal bimodalpredictor = Bimodal(pc_bits_index_bimodal); //initialise object of Bimodal class
		Gshare gsharepredictor = Gshare(pc_bits_index_gshare, BHRbits); //initialise object of Gshare class
		Hybrid hybridpredictor = Hybrid(kbits); //initialise object of Hybrid class
		BTB BTBcache = BTB(BTB_size, BTB_assoc);

		while (myfile >> hex >> programcounter_hex >> Taken_actual_status)
		{
			if (myfile.eof() || (programcounter_hex == 0) || (Taken_actual_status == '\0'))
			{
				break;
			}
			++noofbranches;


			/*Bimodal*/
			pctempBimodal = programcounter_hex >> 2; //drop the least significant 2 bits or right most 2 bits

			pctempBimodal1 = 0;
			for (int i = 0; i < pc_bits_index_bimodal; ++i) //to generate pc index bits of all ones
			{
				pctempBimodal1 += (1 << i);
			}

			pcindexBimodal = pctempBimodal & pctempBimodal1; //gives the index bits of the program counter

			getBimodalpredictioncounter = bimodalpredictor.prediction(pcindexBimodal); //gets the value of the counter for the index passed

			if (getBimodalpredictioncounter >= 2) //predict the taken or not taken status based on the counter value
			{
				Taken_Bimodal_prediction_status = 't';
			}
			else
			{
				Taken_Bimodal_prediction_status = 'n';
			}


			/*G share*/
			pctempGshare = programcounter_hex >> 2; //drop the least significant 2 bits or right most 2 bits

			pctempGshare1 = 0;
			for (int i = 0; i < pc_bits_index_gshare; ++i) //to generate pc index bits of all ones
			{
				pctempGshare1 += (1 << i);
			}

			pcindexGshare = pctempGshare & pctempGshare1; //gives the index bits of the program counter

			ghareindex = gsharepredictor.Makeindex(pcindexGshare); //gshare index obtained after XOR of BHR and first n bits of PC bits
			getGsharepredictioncounter = gsharepredictor.prediction(ghareindex); //gets the value of the counter for the index passed

			if (getGsharepredictioncounter >= 2) //predict the taken or not taken status based on the counter value
			{
				Taken_Gshare_prediction_status = 't';
			}
			else
			{
				Taken_Gshare_prediction_status = 'n';
			}

			/*Hybrid*/
			pctempHybrid = programcounter_hex >> 2; //drop the least significant 2 bits or right most 2 bits

			pctempHybrid1 = 0;
			for (int i = 0; i < kbits; ++i) //to generate pc index bits of all ones
			{
				pctempHybrid1 += (1 << i);
			}

			pcindexHybrid = pctempHybrid & pctempHybrid1; //gives the index bits of the program counter

			getHybridpredictioncounter = hybridpredictor.prediction(pcindexHybrid); //gets the value of the counter for the index passed

			if (getHybridpredictioncounter >= 2) //predict the taken or not taken status based on the counter value
			{
				Taken_Hybrid_prediction_status = Taken_Gshare_prediction_status;

				if ((Taken_actual_status == 't') || (Taken_actual_status == 'T'))
				{
					gsharepredictor.update_table(ghareindex, 1); //1 for taken and 0 for not taken
				}

				if ((Taken_actual_status == 'n') || (Taken_actual_status == 'N'))
				{
					gsharepredictor.update_table(ghareindex, 0); //1 for taken and 0 for not taken
				}
			}
			else
			{
				Taken_Hybrid_prediction_status = Taken_Bimodal_prediction_status;
				if ((Taken_actual_status == 't') || (Taken_actual_status == 'T'))
				{
					bimodalpredictor.update_table(pcindexBimodal, 1); //1 for taken and 0 for not taken
					gsharepredictor.update_BHR(1);
				}

				if ((Taken_actual_status == 'n') || (Taken_actual_status == 'N'))
				{
					bimodalpredictor.update_table(pcindexBimodal, 0); //1 for taken and 0 for not taken
					gsharepredictor.update_BHR(0);
				}
			}

			if (BTB_size != 0) //if the cache size is not equal to zero
			{
				BTB_miss_hit = BTBcache.Read(programcounter_hex); //returns 0 for hit and 1 for miss
				if (BTB_miss_hit == 0)//hit
				{
					++BTBhits;
					/*Bimodal*/


					if (Taken_actual_status != Taken_Hybrid_prediction_status) //if there is mismatch in the prediction though BTB is hit
					{
						++hitBTBmisprediction;
					}
				}

				if (BTB_miss_hit == 1)
				{
					BTB_prediction_status = 'n';
					if (Taken_actual_status == 't')
					{
						++missBTBandtaken;
					}
					goto withBTBjumphybrid;
				}

			}

			/*If Gshare match/correct and bimodal does not match/incorrect then increment*/
			if ((Taken_actual_status == Taken_Gshare_prediction_status) && (Taken_actual_status != Taken_Bimodal_prediction_status))
			{
				hybridpredictor.update_table(pcindexHybrid, 1);
			}

			/*If bimodal match/correct and Gshare does not match/incorrect then decrement*/
			if ((Taken_actual_status != Taken_Gshare_prediction_status) && (Taken_actual_status == Taken_Bimodal_prediction_status))
			{
				hybridpredictor.update_table(pcindexHybrid, 0);
			}

		withBTBjumphybrid:

			if (Taken_actual_status != Taken_Hybrid_prediction_status)
			{
				++branchmispredictions;
			}

		}

		myfile.close();

		if (BTB_size == 0)
		{
			cout << "COMMAND" << endl;
			cout << "./sim hybrid  " << dec << kbits << " " << dec << pc_bits_index_gshare << " " << dec << BHRbits << " " << dec << pc_bits_index_bimodal << " " << dec << BTB_size << " " << dec << BTB_assoc << " " << trace_file << endl;
			cout << "OUTPUT" << endl;
			cout << "number of predictions:	 " << dec << noofbranches << endl;
			cout << "number of mispredictions:	 " << dec << branchmispredictions << endl;
			rate = ((float)branchmispredictions / noofbranches) * 100;
			printf("misprediction rate:    %0.2f \% \n", rate);
			hybridpredictor.getprint();
			gsharepredictor.getprint();
			bimodalpredictor.getprint();
		}

		if (BTB_size != 0)
		{
			cout << "COMMAND" << endl;
			cout << "./sim hybrid  " << dec << kbits << " " << dec << pc_bits_index_gshare << " " << dec << BHRbits << " " << dec << pc_bits_index_bimodal << " " << dec << BTB_size << " " << dec << BTB_assoc << " " << trace_file << endl;
			cout << "OUTPUT" << endl;
			cout << "size of BTB:	  " << dec << BTB_size << endl;
			cout << "number of branches:	  " << dec << noofbranches << endl;
			cout << "number of predictions from branch predictor:	 " << dec << BTBhits << endl;
			cout << "number of mispredictions from branch predictor:	 " << dec << hitBTBmisprediction << endl;
			cout << "number of branches miss in BTB and taken:	  " << dec << missBTBandtaken << endl;
			cout << "total mispredictions:	%d" << dec << hitBTBmisprediction + missBTBandtaken << endl;
			rate = ((float)branchmispredictions / noofbranches) * 100;
			printf("misprediction rate:    %0.2f \% \n", rate);
			cout << endl;
			BTBcache.getprint();
			cout << endl;
			hybridpredictor.getprint();
			gsharepredictor.getprint();
			bimodalpredictor.getprint();
		}
		
	}
	return 0;
}