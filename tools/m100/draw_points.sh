#!/bin/sh

LOG_FILE=$1
IMAGE_FILE=$2
TMP_FILE="$LOG_FILE."

convert -size 1000x1000 xc:white $IMAGE_FILE

split -l 500 $LOG_FILE $TMP_FILE -a 4
  
for file in $TMP_FILE*
do
	points=`cat $file | sed -r -e "s/^([0-9]{3})([0-9]{3})$/point \2,\1/g"`
	convert $IMAGE_FILE -draw "$points" $IMAGE_FILE
done

rm $TMP_FILE*

echo "done!"
