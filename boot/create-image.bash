#!/bin/bash

files=(stage{1,2})

is_readable()
{
    if [[ ! -f "$1" || ! -r "$1" ]]
    then
        echo "File not found or readable: $1" 2>&1
        exit 1
    fi
}

if [[ $# < 1 ]]
then
    echo "usage: $0 [optional files] <output file>"
    exit 1
fi

for i in ${files[@]}
do
    is_readable "$i"
    let "sum += $(du -b "$i" | cut -f1)"
done

dd if=/dev/zero of=pad bs=1 count=$((102400 - sum)) 2>/dev/null

echo "cat ${files[@]} pad ${@:1:$#-1} > ${@:$(($#))}"
cat ${files[@]} pad ${@:1:$#-1} > ${@:$(($#))}
rm pad
