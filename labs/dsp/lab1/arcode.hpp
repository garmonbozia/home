#ifndef ARCODE_HPP
#define ARCODE_HPP

#include <stdlib.h>

//для избежания переполнения:	MAX_FREQUENCY * (TOP_VALUE+1) < ULONG_MAX
//число MAX_FREQUENCY должно быть не менее, чем в 4 раза меньше TOP_VALUE
//число символов NO_OF_CHARS должно быть много меньше MAX_FREQUENCY
#define BITS_IN_REGISTER	17
#define TOP_VALUE			(((unsigned long)1<<BITS_IN_REGISTER)-1)	// 1111...1
#define FIRST_QTR			((TOP_VALUE>>2) +1)							// 0100...0
#define HALF				(2*FIRST_QTR)								// 1000...0
#define THIRD_QTR			(3*FIRST_QTR)								// 1100...0
#define MAX_FREQUENCY		((unsigned)1<<15)
#define NO_OF_CHARS			256
#define EOF_SYMBOL			NO_OF_CHARS			// char-коды: 0..NO_OF_CHARS-1
#define NO_OF_SYMBOLS		(NO_OF_CHARS+1)		// + EOF_SYMBOL

class task_t
{
public:
	char * file_in;
	char * file_out;
	enum mode_t { encoding=0, decoding=1 } mode;
	bool adaptive;
	bool with_memory;
};

class arcode_t
{
public:
	arcode_t ( );
	void initial ( task_t * task_in );
	void start_model ( );
	void update_model ( const int symbol );
	int input_bit ( ); // ввод 1 бита из сжатого файла
	void output_bit ( int bit ); // вывод одного бита в сжатый файл
	void output_bit_plus_follow ( int bit ); // вывод одного очередного бита и тех, которые были отложены
	void start_encoding ( );
	void done_encoding ( );
	void start_decoding ( );
	void encode_symbol ( const int symbol );
	int decode_symbol ( );
	void encode ( );
	void decode ( );

private:
	unsigned long low, high, value;
	int	          buffer, bits_to_go, garbage_bits, bits_to_follow;

	//интервалы частот символов
	unsigned int cum_freq[NO_OF_SYMBOLS + 1];
	// относительная частота появления символа s (оценка вероятности его появления)
	// определяется как p(s)=(cum_freq[s+1]-cum_freq[s])/cum_freq[NO_OF_SYMBOLS]

	FILE *in, *out;
	task_t * task;
	bool adaptive;
	bool with_memory;
};

#endif /* ARCODE_HPP */
