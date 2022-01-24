#!/bin/bash

declare -a file_base=("1.nlb" "2.nlb" "3.nlb"  )
declare -a file_base2=("1.txt" "2.txt" "3.txt" )

e_code=$?

# Loop the array
for file in "${!file_base[@]}"; do
    ./extension "${file_base[file]}" | tee output.txt >/dev/null
    if cmp -s "output.txt" "${file_base2[file]}" ; then
    	#printf "%s is in %s\n" "${file_base[file]}" "${file_base2[file]}"
    	printf " Test %s OK\n" "${file_base2[file]}"
    else
    	printf " Test %s NOT OK\n" "${file_base2[file]}" 
    fi

done

# Clean exit with status 0
exit 0
