#!/bin/bash
out=~/out_asp_cpu.txt

#rm -f $out

echo_all () {
	echo $1
	echo $1 >> $out
}

for c in 1 4 8 12
do
	for f in $(ls ~/inputs/asp/test*.txt)
	do
		echo_all "Next: $f"
		date
		./ggraph -in-edgelist $f -apsp-hops-sum -t -threads $c >> $out
	done
done
