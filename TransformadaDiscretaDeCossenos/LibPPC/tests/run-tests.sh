#!/bin/bash

make all

rm -f *.output

[[ $? -ne 0 ]] && exit -1

for test in $(find . -regex '\./[0-9]+.*' -executable | sort); do

    echo -n "Running $test ... "

    $test 2>&1 > $test.output

    ret=$?

    if [[ $ret -eq 0 ]]; then 
        echo "ok";
    else
        echo "FAIL"
        tail $test.output
        exit -1
    fi
    
done