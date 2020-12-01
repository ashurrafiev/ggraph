#!/bin/bash
out=~/out_single.txt
rm -f $out
for f in $(ls ~/inputs/tr/2/*.txt)
do
  echo $f
  echo $f >> $out
  head -1 $f >> $out
  tail -6 $f >> $out
  echo "------" >> $out
done
