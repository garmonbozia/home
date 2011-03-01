#!/bin/sh

DIRECTORY='./'
OUTPUT_FILE=${DIRECTORY}/m100_output
SOURCE_FILE=${DIRECTORY}/m100.pl
NUMBER=$1
#LENGTH=$((`echo $NUMBER | wc -c`-1))

echo -n "" > $OUTPUT_FILE
swipl -q -t halt  -s ${SOURCE_FILE} -g "N = $NUMBER, R = 100, makeAns(N, R, X), open('$OUTPUT_FILE', append, OS), format(OS, '~w =  ~w~n', [R, X]), close(OS), false"

if [ "$2" == "-n" ] 
then
	echo -n "$1:"
	[ $((`cat $OUTPUT_FILE | wc -l`)) == 0 ] && echo -e "\t \033[1;31m not this one \033[0;37m" || echo -e "\t \033[1;32m oook \033[0;37m"
	cat $OUTPUT_FILE
fi
 
