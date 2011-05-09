#ifndef ARCODE_HPP
#define ARCODE_HPP

#include <stdlib.h>
#include "task_t.cpp"
#include "config.h"


class arcode_t
{
public:
	arcode_t ( );
	void initial ( task_t * task_in );
	void start_model ( );
	void start_model_with_memory ( );
	void update_model ( const int symbol );
	void update_model_with_memory ( const int symbol, const int symbol_prev );
	int  input_bit ( ); // ввод 1 бита из сжатого файла
	void output_bit ( int bit ); // вывод одного бита в сжатый файл
	void output_bit_plus_follow ( int bit ); // вывод одного очередного бита и тех, которые были отложены
	void start_encoding ( );
	void done_encoding ( );
	void start_decoding ( );
	void encode_symbol ( const int symbol );
	void encode_symbol_with_memory ( const int symbol, const int symbol_prev );
	int  decode_symbol ( );
	int  decode_symbol_with_memory ( const int symbol_prev );
	void encode ( );
	void decode ( );

private:
	unsigned long low, high, value;
	int	          buffer, bits_to_go, garbage_bits, bits_to_follow;

	//интервалы частот символов
	unsigned int cum_freq[NO_OF_SYMBOLS + 1];
	unsigned int cum_freq_with_memory[NO_OF_SYMBOLS + 1][NO_OF_SYMBOLS + 1];
	// относительная частота появления символа s (оценка вероятности его появления)
	// определяется как p(s)=(cum_freq[s+1]-cum_freq[s])/cum_freq[NO_OF_SYMBOLS]

	FILE *in, *out, *model;
	task_t * task;
	bool adaptive;
	bool with_memory;
};

#endif /* ARCODE_HPP */
