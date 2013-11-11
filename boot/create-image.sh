#!/bin/sh

dir="$(cd "$(dirname "$0")" && pwd)"
files="$dir/stage1 $dir/stage2"

is_readable() {
	if test ! -f "$1" -o ! -r "$1"
	then
		echo "File not found or readable: $1" 2>&1
		echo "Exiting." 2>&1
		exit 1
	fi
}

if test $# -lt 1
then
	echo "usage: $0 [optional files] <output file>"
fi

echo "[*] Creating image..."

sum=0
for i in $files
do
	is_readable "$i"
	sum=$((sum+$(wc -c "$i" | cut -d ' ' -f 1)))
done

ofiles=""
ofsum=0
while test $# -gt 1
do
	size=$(($(wc -c "$1" | cut -d ' ' -f 1)))
	ofsum=$(($ofsum+$size))
	echo "    - include $1 ($size bytes, $(((size+511)/512)) blocks)"
	ofiles="$files $i"
	shift
done

echo "[*] Image was written to: $1."
dd if=/dev/zero of="$1" bs=1 count=$((102400+$ofsum)) 2> /dev/null

seek=0
for i in $files
do
	dd conv=notrunc if=$i of="$1" seek="$seek" bs=1 2> /dev/null
	seek=$(($seek+$(wc -c $i | cut -d ' ' -f 1)))
done

seek=102400
for i in $ofiles
do
	dd conv=notrunc if=$i of="$1" seek="$seek" bs=1 2> /dev/null
	seek=$(($seek+$(wc -c $i | cut -d ' ' -f 1)))
done
