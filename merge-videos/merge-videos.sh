#! /bin/bash

folder=$PWD

while getopts u:a:f: flag
do
    case "${flag}" in
        f) folder=${OPTARG};;
    esac
done

for show in "$folder/*"
do
    for file in "$show/Video/*"
    do
        name=$(basename $file)
        path=`echo $file | sed 's/'"Video\/$name"'$//g'`
        ffmpeg -n -i "$path/Video/$name" -i "$path/Audio/$name" -c copy "$path/$name"
    done
done
