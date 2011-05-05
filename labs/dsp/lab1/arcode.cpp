#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include "arcode.hpp"

using std::cout;
using std::endl;

arcode_t::arcode_t ( )
{
	low            = 0;
	high           = 0;
	value          = 0;
	buffer         = 0;
	bits_to_go     = 0;
	garbage_bits   = 0;
	bits_to_follow = 0;

	adaptive    = false;
	with_memory = false;
}

void arcode_t::initial ( task_t * task_in )
{
	task        = task_in;
	adaptive    = task->adaptive;
	with_memory = task->with_memory;
	if ( (in=fopen( task->file_in,"r+b" ))==NULL )
	{
		cout << endl << "Incorrect input file" << endl;
		exit( 1 );
	}
	if ( (out=fopen( task->file_out,"w+b" ))==NULL )
	{
		cout << endl << "Incorrect output file" << endl;
		exit( 1 );
	}
	if ( task->mode == task_t::encoding )
		encode( );
	else
		decode( );
	fclose( in );
	fclose( out );
}

void arcode_t::start_model ( )
{ // изначально все символы в сообщении считаем равновероятными
	for ( int i=0; i<=NO_OF_SYMBOLS; i++ )
		cum_freq[i] = i;
}

void arcode_t::update_model ( const int symbol )
{
/// если произошло переполнение
	if ( cum_freq[NO_OF_SYMBOLS] == MAX_FREQUENCY )
	{
		for ( int i=0; i<=NO_OF_SYMBOLS; i++ )
/// нормализация (делим все частоты на 2)
			cum_freq[i] = cum_freq[i] >> 1;
/// для нестационарных входных последовательностей
		for ( int i=1; i<=NO_OF_SYMBOLS; i++ )
		{
			if ( cum_freq[i]<=cum_freq[i-1] )
			{
				cum_freq[i] = cum_freq[i-1] + 1;
			}
		}
	}
/// увеличивается частота symbol'а
	for ( int i=symbol+1; i<=NO_OF_SYMBOLS; i++ )
		cum_freq[i]++;
}

int arcode_t::input_bit ( ) // ввод 1 бита из сжатого файла
{
	int t;
	if ( 0 == bits_to_go )
	{
		buffer = getc(in);	// заполняем буфер битового ввода
		if ( buffer == EOF )	// входной поток сжатых данных исчерпан
		{
			// Причина попытки дальнейшего чтения: следующим
			// декодируемым символом должен быть EOF_SYMBOL,
			// но декодер об этом пока не знает и может готовиться
			// к дальнейшему декодированию, втягивая новые биты
			// (см. цикл for(;;) в процедуре decode_symbol). Эти
			// биты — "мусор", реально не несут никакой
			// информации и их можно выдать любыми
			garbage_bits++;
			if ( garbage_bits > BITS_IN_REGISTER - 2 )
			{	// больше максимально возможного числа мусорных битов
				printf( "ERROR IN COMPRESSED FILE ! \n" );
				exit( -1 );
			}
			bits_to_go = 1;
		}
		else
			bits_to_go = 8;
	}
	t = buffer&1;
	buffer >>= 1;
	bits_to_go--;
	return t;
}

void arcode_t::output_bit ( int bit ) // вывод одного бита в сжатый файл
{
	buffer = (buffer>>1) + (bit<<7); // в битовый буфер (один байт)
	bits_to_go--;
	if ( bits_to_go == 0 ) // битовый буфер заполнен, сброс буфера
	{
		putc( buffer, out );
		bits_to_go = 8;
	}
}
void arcode_t::output_bit_plus_follow ( int bit ) // вывод одного очередного бита и тех, которые были отложены
{
	output_bit( bit );
	while ( bits_to_follow > 0 )
	{
		output_bit( !bit );
		bits_to_follow--;
	}
}

void arcode_t::start_encoding ( )
{
	bits_to_go		= 8;				// свободно бит в битовом буфере вывода
	bits_to_follow	= 0;				// число бит, вывод которых отложен
	low				= 0;				// нижняя граница интервала
	high			= TOP_VALUE;		// верхняя граница интервала
}
void arcode_t::done_encoding ()
{
	bits_to_follow++;
	if ( low < FIRST_QTR )
		output_bit_plus_follow( 0 );
	else
		output_bit_plus_follow( 1 );
	putc( buffer>>bits_to_go, out ); // записать незаполненный буфер
}

void arcode_t::start_decoding ( )
{
	int i;
	bits_to_go	 = 0;				// свободно бит в битовом буфере ввода
	garbage_bits = 0;				// контроль числа "мусорных" бит в конце сжатого файла
	low			 = 0;				// нижняя граница интервала
	high		 = TOP_VALUE;		// верхняя граница интервала
	value		 = 0;				// "ЧИСЛО"
	for ( i=0; i<BITS_IN_REGISTER; i++ )
	  value = (value<<1) + input_bit( );
}

void arcode_t::encode_symbol ( const int symbol )
{
	// пересчет границ интервала
	unsigned long range;
	range = high - low + 1;
	high  = low	+ range * cum_freq[symbol+1] / cum_freq[NO_OF_SYMBOLS] - 1;
	low	  = low	+ range * cum_freq[symbol]   / cum_freq[NO_OF_SYMBOLS];
	// далее при необходимости — вывод бита или меры от зацикливания
	for ( ; ; )
	{
		assert( low <= high ); // Замечание: всегда low < high (?)
		if ( high < HALF )    // Старшие биты low и high — нулевые (оба)
			output_bit_plus_follow( 0 ); //вывод совпадающего старшего бита
		else if ( low >= HALF )         // старшие биты low и high - единичные
		{
			output_bit_plus_follow( 1 ); // вывод старшего бита
			low  -= HALF;				 // сброс старшего бита в 0
			high -= HALF;				 // сброс старшего бита в 0
		}
		else if ( (low >= FIRST_QTR) && (high < THIRD_QTR) )
		{/* возможно зацикливание, т.к.
			HALF <= high < THIRD_QTR,	i.e. high=10...
			FIRST_QTR <= low < HALF,	i.e. low =01...
			выбрасываем второй по старшинству бит	*/
			high -= FIRST_QTR;		// high	=01...
			low -= FIRST_QTR;		// low	=00...
			bits_to_follow++;		//откладываем вывод (еще) одного бита
			// младший бит будет втянут далее
		}
		else break;		// втягивать новый бит рано
		// старший бит в low и high нулевой, втягиваем новый бит в младший разряд
		low  <<= 1;	// втягиваем 0
		high <<= 1;
		high++;		// втягиваем 1
	}
}

int arcode_t::decode_symbol ( )
{
	unsigned long range, cum;
	int symbol;
	range = high - low + 1;
	// число cum - это число value, пересчитанное из интервала
	// low..high в интервал 0..CUM_FREQUENCY[NO_OF_SYMBOLS]
	cum = ((value - low + 1) * cum_freq[NO_OF_SYMBOLS] - 1) / range;
	// поиск интервала, соответствующего числу cum
	for (symbol = 0; cum_freq[symbol+1] <= cum; symbol++);
	// пересчет границ
	high = low + range * cum_freq[symbol+1] / cum_freq[NO_OF_SYMBOLS] - 1;
	low  = low + range * cum_freq[symbol]   / cum_freq[NO_OF_SYMBOLS];
	for ( ; ; )
	{	// подготовка к декодированию следующих символов
		if ( high < HALF ) {/* cтаршие биты low и high - нулевые */}
		else if ( low >= HALF )
		{	// cтаршие биты low и high - единичные, сбрасываем
			value -= HALF;
			low   -= HALF;
			high  -= HALF;
		}
		else if ( (low >= FIRST_QTR) && (high < THIRD_QTR) )
		{	// поступаем так же, как при кодировании
			value -= FIRST_QTR;
			low   -= FIRST_QTR;
			high  -= FIRST_QTR;
		}
		else break;	// втягивать новый бит рано
		low  <<= 1; // втягиваем новый бит 0
		high <<= 1;
		high++;	// втягиваем новый бит 1
		value = (value<<1) + input_bit( ); // втягиваем новый бит информации
  }
	  return symbol;
}

void arcode_t::encode ( )
{
	int symbol;
	int symbol_previous;
	start_model( );
	start_encoding( );
	while ( (symbol = getc(in)) != EOF )
	{
		encode_symbol( symbol );
		if ( adaptive )
			update_model( symbol );
	}
	encode_symbol( EOF_SYMBOL );
	done_encoding( );
}

void arcode_t::decode ( )
{
	int symbol;
	int symbol_previous;
	start_model( );
	start_decoding( );
	while ( (symbol=decode_symbol()) != EOF_SYMBOL )
	{
		if ( adaptive )
			update_model( symbol );
		putc( symbol, out );
	}
}
