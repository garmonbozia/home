#ifndef MODEL_T_HPP
#define MODEL_T_HPP

#include "task_t.cpp"
#include "config.h"

class model_t
{
public:
	model_t ( );
	void generate ( task_t * task_in );
	void gen_prob ( );
	void show_model ( );
	void show_prob ( );
	float get_entropy ( );

private:
	void start ( );
	void update ( const int symbol, const int symbol_prev=0 );
	void make ( );

	unsigned int cum_freq[NO_OF_SYMBOLS + 1];
	unsigned int cum_freq_with_memory[NO_OF_SYMBOLS + 1][NO_OF_SYMBOLS + 1];
	float prob[NO_OF_SYMBOLS + 1];
	float prob_with_memory[NO_OF_SYMBOLS + 1][NO_OF_SYMBOLS + 1];
	float entropy;

	FILE * in;
	FILE * out;
	task_t * task;
	bool with_memory;
};

#endif /* MODEL_T_HPP */
