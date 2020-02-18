#!/bin/bash
#./create_infiles.sh   dir_name    num_of_files    num_of_dirs     levels

if [ "$#" \> 4 ] 
then echo Please use the following structure: ./create_infiles.sh dir_name num_of_files num_of_dirs levels
exit 1
fi

if [ "$#" \< 4 ]
then echo Please use the following structure: ./create_infiles.sh dir_name num_of_files num_of_dirs levels
exit 1
fi

re='^[0-9]+$'
if ! [[  $2  =~ $re ]]
then echo "'num_of_files' needs to be an integer" >&2; exit 1
fi

if ! [[  $3  =~ $re ]]
then echo "'num_of_dirs' needs to be an integer" >&2; exit 1
fi

if ! [[  $4  =~ $re ]]
then echo "'levels' needs to be an integer" >&2; exit 1
fi

mkdir -p $1
dir=$1
#echo $filename
#if [ ! -f $filename ]
#then
#	touch $filename
#fi

Directory=$(head /dev/urandom | tr -dc A-Za-z0-9 | head -c 3)
#echo $Directory

LIM=$3
LIM2=$4
i=1
while [ $i -le "$LIM" ]
do
	a=1
	dirin=$1
	while [[ $a -le "$LIM2" ]] && [[ $i -le "$LIM" ]]
	do
		i=$(($i+1))
		a=$(($a+1))
		name=$(head /dev/urandom | tr -dc a-z0-9 | head -c 3)
		dirin+="/"
		dirin+=$name
		mkdir $dirin
	done
done


LIM3=$2
b=0 #files
shopt -s globstar nullglob
while [ $b -le "$LIM3" ]
do
	for dir in ./$1/**/;do
		b=$(($b+1))
	
		if (( $b > $LIM3 ))
		then
			exit 1
		fi
	
		name=$(head /dev/urandom | tr -dc a-z0-9 | head -c 3)
		dir+=$name
		touch $dir
		echo $dir
		range=$(shuf -i 1000-61440 -n 1)
		dd if=/dev/urandom bs=$range status=none count=1 | base64 > $dir
		truncate -s $range $dir
	done
done
