#ifndef BTB_H_INCLUDED
#define BTB_H_INCLUDED

class BTB
{
public:
	BTB(int cachesize = 0, int assoc = 0);
	~BTB();
	int Read(unsigned long int pcaddress);
	void getprint(void);

private:
	int BTBcachesize, BTBassoc;
	typedef struct
	{
		unsigned long int tagaddress;
		int LRU_counter;
		int validbit;
	}cacheblock;
	cacheblock **BTBmatrix; //dynamic 2D array of struct
	int Blocksize, indexcount, indexbits, LRUtemp, tagnotfound, emptynotfound;
	unsigned long int pc_address;
	unsigned long int pctemp, pctag, pcindextemp;
	int pcindex;
};

#endif 