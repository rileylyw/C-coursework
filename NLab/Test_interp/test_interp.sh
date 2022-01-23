#!/bin/bash

# An array, do not have to declare it, but is supposedly faster
declare -a file_base=("1.nlb" "2.nlb" "3.nlb" "4.nlb" "5.nlb" "6.nlb" "7.nlb" )
declare -a file_base2=("1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt" "7.txt" )

e_code=$?

# Loop the array
for file in "${!file_base[@]}"; do
    ./interp "${file_base[file]}" | tee output.txt >/dev/null
    if cmp -s "output.txt" "${file_base2[file]}" ; then
    	#printf "%s is in %s\n" "${file_base[file]}" "${file_base2[file]}"
    	printf "Test %s OK\n" "${file_base2[file]}"
    else
    	printf "Test %s NOT OK\n" "${file_base2[file]}" 
    fi

    # Check exit code from previous command (ie diff)
    # We need to add this to a variable else we can't print it
    # as it will be changed by the if [
    # Iff not 0 then the files differ (at least with diff)
    
    #if [ $e_code -ne 0 ] ; then
    #    printf "TEST FAIL : %d\n" "$e_code"
    #else
    #    printf "TEST OK!\n"
    #fi

    # Pause by prompt
    #read -p "Enter a to abort, anything else to continue: " input_data
    # Iff input is "a" then abort
    #[ "$input_data" == "a" ] && break

done

# Clean exit with status 0
exit 0
