#!/bin/bash
root=~/inputs/tr/2

#for k in 4 8 16
#do
#	for (( i=1; i<=1024; i=i*2 ))
#	do
#		let n=i*1024
#		let e=n*k
#		file=${root}/n${i}k${k}.txt
#		echo $file
#		timeout -k 120 120 ./ggraph -p -t -sssp -norm $n $e > $file
#	done
#done
#
#for (( d=9; d<=19; d++ ))
#do
#	file=${root}/t2d${d}.txt
#	echo $file
#	timeout -k 120 120 ./ggraph -p -t -sssp -tree 2 $d > $file
#done
#
#for (( i=1; i<=1024; i=i*2 ))
#do
#	let n=i*1024
#	file=${root}/ring${i}k.txt
#	echo $file
#	timeout -k 120 120 ./ggraph -p -t -sssp -ring $n > $file
#done

for s in 32 45 64 91 128 181 256 362 512 724 1024
do
	file=${root}/grid2d4s${s}.txt
	echo $file
	timeout -k 120 120 ./ggraph -p -t -sssp -root -grid2d4 $s $s > $file
	file=${root}/grid2d8s${s}.txt
	echo $file
	timeout -k 120 120 ./ggraph -p -t -sssp -root -grid2d8 $s $s > $file
done

for s in 10 13 16 20 25 32 40 51 64 81 102
do
	file=${root}/grid3d6s${s}.txt
	echo $file
	timeout -k 120 120 ./ggraph -p -t -sssp -root -grid3d6 $s $s $s > $file
	file=${root}/grid3d26s${s}.txt
	echo $file
	timeout -k 120 120 ./ggraph -p -t -sssp -root -grid3d26 $s $s $s > $file
done

#for (( i=1; i<=1024; i=i*2 ))
#do
#	let n=i*1024
#	let e=n*8
#	file=${root}/sfree${i}k.txt
#	echo $file
#	timeout -k 120 120 ./ggraph -p -t -sssp -sfree $n $e > $file
#done
