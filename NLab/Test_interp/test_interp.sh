#!/bin/bash

declare -a file_base=("1.nlb" "2.nlb" "3.nlb" "4.nlb" "5.nlb" "6.nlb" "7.nlb" "8.nlb" "9.nlb" "10.nlb" "11.nlb" "12.nlb" "13.nlb" "14.nlb" "15.nlb" "16.nlb")
declare -a file_base2=("1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" "8.txt" "9.txt" "10.txt")

e_code=$?

# Loop the array
for file in "${!file_base[@]}"; do
    ./interp "${file_base[file]}" | tee output.txt >/dev/null
    if cmp -s "output.txt" "${file_base2[file]}" ; then
    	#printf "%s is in %s\n" "${file_base[file]}" "${file_base2[file]}"
    	printf " Test %s OK\n" "${file_base2[file]}"
    else
    	printf " Test %s NOT OK\n" "${file_base2[file]}" 
    fi


done

# Clean exit with status 0
exit 0
