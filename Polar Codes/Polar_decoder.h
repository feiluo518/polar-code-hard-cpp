#ifndef POLAR_DECODER_H
#define POLAR_DECODER_H

#include"Polar.h"
#include"Channel.h"
#include<vector>
#include<string>
#include<stack>

class Polar_decoder : public Polar
{
public:
	Polar_decoder(const std::vector<double> &_rec_codeword, double _code_rate = 0.5, double _sigma2 = 1.0);
	Polar_decoder(const Channel &_chan, double _code_rate = 0.5);
	Polar_decoder();
	~Polar_decoder();
	void Decode(std::string _method, uint _search_width = 1, uint _CRC_para_r = 24); // _method = SC, SCL, CA-SCL
private:
	//----------------Interface of several decoding algorithms -------------
	void decode_SC();
	void decode_SCL(uint _search_width);
	void decode_CASCL(uint _search_width = 1, uint _CRC_para_r = 24);

	//---------------- Basic compute function -------------------------------
	double f_blue(double L1, double L2);
	double g_red(double L1, double L2, BOOL u);

	//---------------- data struct ------------------------------------------
	void init_data_struct_of_SCL(uint L, uint M, uint N);
	void set_data_struct_of_SCL(uint L, uint M, uint N);
	void free_data_struct_of_SCL(uint L, uint M, uint N);

	void show_data_struct_of_SCL(uint L, uint M, uint N, uint _cur_phase);
	void show_fixed_struct_of_SCL(uint L, uint M, uint N);
	void show_array_struct_of_SCL(uint L, uint M, uint N);
	void show_code_struct_of_SCL(uint L, uint M, uint N, uint _cur_phase);

	//---------------- SC ---------------------------------------------------
	void update_BAT(uint cur_phase, uint path);
	void update_LLR(uint cur_phase, uint path);
	void compute_channel_llr(double *_y_in, uint _length, double *_z_out);	// layer = 0
	void compute_inner_llr(double ***LLR, uint _path, uint _layer, uint _global_phase);	// layer > 0

public:
	double sigma2;
	std::vector<double> recCodeword;	//after gaussin channel, received code
	std::vector<uint> deCodeword;		//codeword decoded by algorithm using recCodeword
	std::vector<uint> deInfoBit;
private:
	//**************  fixed  ***************************************
	char **NodeType;				//N*(M+1) mat of type of node, red/blue
	uint *LayerBlockLen;			//M+1 array of each layer
	uint *LayerRenewed;				//M+1 array, when goes to i, which layer need to be renewed

	//**************  array  ***************************************
	double ***LLR;					//L*(M+1) mat of pointer of LLR
	BOOL ***BAT;					//L*(M+1) mat of pointer of u, bit array of assisting
	int **PathIndexToArrayIndex;	// L*(M+1)
	uint **ArrayReferenceCount;		// L*(M+1)
	std::vector<std::stack<uint>> ArrayInactive; //(M+1)*(0:L)

	//***************  code  ***************************************
	uint **EstimatedWord;			//L*N mat of estimated codeword
	double **PathMetricsOfForks;	//2*L array of extended PM
	double *PathMetrics;			//L array of PM
	BOOL **ForkActiveOrNot;			//2*L array of bool, keep or not
	BOOL *PathActiveOrNot;			//L array
	std::stack<uint> PathInactive;	//0:L
};

#endif		