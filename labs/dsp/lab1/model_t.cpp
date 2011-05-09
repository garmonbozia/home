#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include "model_t.hpp"

using std::cout;
using std::endl;

model_t::model_t ( )
{
	with_memory = false;
}

void model_t::generate ( task_t * task_in )
{
	task        = task_in;
	with_memory = task->with_memory;
	if ( ( in=fopen( task->file_in,"r+b" ) ) == NULL )
	{
		cout << endl << "Incorrect input file" << endl;
		exit( 1 );
	}
	if ( ( out=fopen( task->file_out,"w+b" ) ) == NULL )
	{
		cout << endl << "Incorrect output file" << endl;
		exit( 1 );
	}

	start ( );

	int symbol, symbol_prev = 0;
	while ( (symbol = getc(in)) != EOF )
	{
		( with_memory ) ? update( symbol, symbol_prev ) : update( symbol );
		symbol_prev = symbol;
	}

	( with_memory ) ? update( EOF_SYMBOL, symbol_prev ) : update( symbol );

	for ( int i=0; i<NO_OF_SYMBOLS; i++ )
		for ( int j=0; j<NO_OF_SYMBOLS; j++ )
			( with_memory ) ? putc( cum_freq_with_memory[i][j], out ) : putc( cum_freq[i], out );

	fclose( in );
	fclose( out );
}
void model_t::start ( )
{
/// изначально все символы в сообщении считаем равновероятными
	if ( !with_memory )
	{
		for ( int i=0; i<=NO_OF_SYMBOLS; i++ )
			cum_freq[i] = i;
	}
	else
	{
/// изначально все символы в сообщении считаем равновероятными
	for ( int i=0; i<NO_OF_SYMBOLS; i++)
		for ( int j=0; j<=NO_OF_SYMBOLS; j++)
			cum_freq_with_memory[i][j] = j;
	}
}

void model_t::update ( const int symbol, int symbol_prev )
{
	if ( !with_memory )
	{
/// если произошло переполнение
		if ( cum_freq[NO_OF_SYMBOLS] == MAX_FREQUENCY )
		{
			for ( int i=0; i<=NO_OF_SYMBOLS; i++ )
/// нормализация (делим все частоты на 2)
				cum_freq[i] = cum_freq[i] >> 1;
/// для нестационарных входных последовательностей
			for ( int i=1; i<=NO_OF_SYMBOLS; i++ )
				if ( cum_freq[i]<=cum_freq[i-1] )
					cum_freq[i] = cum_freq[i-1] + 1;
		}
/// увеличивается частота symbol'а
		for ( int i=symbol+1; i<=NO_OF_SYMBOLS; i++ )
			cum_freq[i]++;
	}
	else
	{
/// если произошло переполнение
		if ( cum_freq_with_memory[symbol_prev][NO_OF_SYMBOLS]==MAX_FREQUENCY )
		{
			for ( int i=0; i<=NO_OF_SYMBOLS; i++ )
/// нормализация (делим все частоты на 2)
				cum_freq_with_memory[symbol_prev][i] = cum_freq_with_memory[symbol_prev][i]>>1;
/// для нестационарных входных последовательностей
			for ( int i=1; i<= NO_OF_SYMBOLS; i++ )
				if ( cum_freq_with_memory[symbol_prev][i] <= cum_freq_with_memory[symbol_prev][i-1])
					cum_freq_with_memory[symbol_prev][i] = cum_freq_with_memory[symbol_prev][i-1]+1;
		}
/// увеличивается частота symbol'а
		for ( int i=symbol+1; i<=NO_OF_SYMBOLS; i++ )
			cum_freq_with_memory[symbol_prev][i]++;
	}
}

void model_t::show_model ( )
{
	cout << "MODEL" << endl;
	if ( with_memory )
	{
		for ( int i=0; i<NO_OF_SYMBOLS; i++ )
			for ( int j=0; j<=NO_OF_SYMBOLS; j++ )
				cout << "#" << i << "," << j << ": " << cum_freq_with_memory[i][j] << endl;
	}
	else
	{
		for ( int i=0; i<NO_OF_SYMBOLS; i++)
			cout << "#" << i << ": " << cum_freq[i] << endl;
	}
}

void model_t::gen_prob ( )
{
	if ( with_memory )
	{

	}
	else
	{
		prob[0] = cum_freq[0] / ( cum_freq[NO_OF_SYMBOLS] + 0. );
		for ( int i=1; i<NO_OF_SYMBOLS; i++ )
			prob[i] = (cum_freq[i]-cum_freq[i-1]) / ( cum_freq[NO_OF_SYMBOLS]-1 + 0. );
		float prob_sum = 0;
		for ( int i=0; i<NO_OF_SYMBOLS; i++, prob_sum += prob[i] );
		cout << "prob_sum= " << prob_sum << endl;
	}
}

void model_t::show_prob ( )
{
	cout << "PROBABILITIES" << endl;
	if ( with_memory )
	{
	}
	else
	{
		for ( int i=0; i<NO_OF_SYMBOLS; i++ )
			cout << "#" << i << ": " << prob[i] << endl;
	}
}

float model_t::get_entropy ( )
{
	float entropy_tmp = 0;
	for ( int i=0; i<NO_OF_SYMBOLS; i++ )
		entropy_tmp += -prob[i]*log(prob[i])/log(2.0);
	entropy = entropy_tmp;
	return entropy;
}
