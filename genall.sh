#!/bin/bash
root=~/inputs/tr

for k in 4 8 16
do
	for (( i=1; i<=1024; i=i*2 ))
	do
		let n=i*1024
		let e=n*k
		file=${root}/n${i}k${k}.txt
		echo $file
		timeout -k 120 120 ./ggraph -p -t -sssp -norm $n $e > $file
	done
done

for (( d=9; d<=19; d++ ))
do
	file=${root}/t2d${d}.txt
	echo $file
	timeout -k 120 120 ./ggraph -p -t -sssp -tree 2 $d > $file
done

for (( i=1; i<=1024; i=i*2 ))
do
	let n=i*1024
	file=${root}/ring${i}k.txt
	echo $file
	timeout -k 120 120 ./ggraph -p -t -sssp -ring $n > $file
done
