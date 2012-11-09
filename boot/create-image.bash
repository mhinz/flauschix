#!/bin/bash

# Copyright (c) 2012 Marco Hinz
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# - Neither the name of the author nor the names of its contributors may be used
#   to endorse or promote products derived from this software without specific
#   prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

dir="$( cd "$(dirname "$0")" && pwd )"
files=($dir/stage{1,2})

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
    #echo "$i: $(du -b "$i" | cut -f1)"
    let "sum += $(du -b "$i" | cut -f1)"
done

dd if=/dev/zero of=pad bs=1 count=$((102400 - sum)) 2>/dev/null
#echo "pad: $(du -b pad | cut -f1)"

echo "[*] Creating image ${@:$(($#))}..."
#echo "cat "${files[@]}" pad "${@:1:$#-1}" > "${@:$(($#))}""
cat "${files[@]}" pad "${@:1:$#-1}" > "${@:$(($#))}"

rm pad
