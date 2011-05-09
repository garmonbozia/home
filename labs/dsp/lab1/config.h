/*
 * config.h
 *
 *  Created on: May 9, 2011
 *      Author: lk
 */

#ifndef CONFIG_H_
#define CONFIG_H_

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

#endif /* CONFIG_H_ */
