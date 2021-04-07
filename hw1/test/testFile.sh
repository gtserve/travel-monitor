#!/bin/bash

# testFile.sh
# -----------------------------------------------------------------------------
# System Programming - Homework 1
# name:    George Tservenis
# email:   sdi1500164@di.uoa.gr
# date:    25-03-2021
# -----------------------------------------------------------------------------

# Error codes
ERR_NUM_ARGS=1
ERR_FILE_IO=2
#ERR_WRONG_ARG=3

# Print the proper usage of the script.
print_usage() {
    echo "Script usage:"
    echo "  bash ./testFile.sh [virusesFile] [countriesFile]" \
         "[numLines] [duplicatesAllowed]"
}

# Check the number of arguments.
if [[ "$#" -ne 4 ]]; then
    echo "Error: Wrong number of arguments."
    print_usage
    exit "$ERR_NUM_ARGS"
fi

# TODO: Check arguments' type.

# Check that the files exist and are readable.
if [[ ! -e "$1" ]] || [[ ! -f "$1" ]]; then
    echo "Error: File '$1' does not exits."
    exit "$ERR_FILE_IO"
fi
if [[ ! -e "$2" ]] || [[ ! -f "$2" ]]; then
    echo "Error: File '$2' does not exits."
    exit "$ERR_FILE_IO"
fi
if [[ ! -r "$1" ]]; then
    echo "Error: File '$1' is not readable."
    exit "$ERR_FILE_IO"
fi
if [[ ! -r "$2" ]]; then
    echo "Error: File '$2' is not readable."
    exit "$ERR_FILE_IO"
fi

v_file="$1"
c_file="$2"
num_lines="$3"
dupl_flag="$4"

# Print a useful message.
message="Creating $num_lines records with"
if [[ "$dupl_flag" = 0 ]]; then
    message="$message no"
fi
echo "$message duplicates, using '$v_file' and '$c_file'..."


# Count viruses and countries
num_viruses=$(wc -l < "$v_file")
num_countries=$(wc -l < "$c_file")
echo "Read $num_viruses viruses and $num_countries countries."

# Create new empty input file.
i_file="input.txt"
if [[ -e "$i_file" ]]; then
    rm -f "$i_file"
fi
touch "$i_file"


index=0
declare -a id_hash_map      # For unique IDs
declare -a names_hash_map   # For unique names
while [[ "$index" -lt "$num_lines" ]]; do

    citizen_id=$((RANDOM % 9999))

    while [[ "${id_hash_map["$citizen_id"]}" -eq 1 ]]; do
        citizen_id=$((RANDOM % 10))
    done

    id_hash_map["$citizen_id"]=1

    echo "$citizen_id" >> input.txt
    index=$((index+1))
done


exit 0
