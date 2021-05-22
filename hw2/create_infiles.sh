#!/bin/bash

# create_infiles.sh
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# project:  (#2) Travel Monitor
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------

# Error codes
ERR_ARG_NUM=1
ERR_FILE_IO=2
ERR_ARG_TYPE=3

MAX_ID=10000

# ---------------------------------------- Functions ----------------------------------------------

print_usage() {
    # Print the proper usage of the script.
    echo "Script usage:"
    echo "  bash ./create_infiles.sh [inputFile] [input_dir] [numFilesPerDirectory]"
}

# -------------------------------------------------------------------------------------------------

# Check the number of arguments.
if [[ "$#" -ne 3 ]]; then
    echo "Error: Wrong number of arguments."
    print_usage
    exit "$ERR_ARG_NUM"
fi

# Check that [inputFile] exists and is readable.
if [[ ! -e "$1" ]] || [[ ! -f "$1" ]]; then
    echo "Error: File '$1' does not exists."
    exit "$ERR_FILE_IO"
fi
if [[ ! -r "$1" ]]; then
    echo "Error: File '$1' is not readable."
    exit "$ERR_FILE_IO"
fi

# Check that [input_dir] doesn't exist.
#if [[ -e "$2" ]] && [[ -d "$2" ]]; then
#    echo "Error: Directory '$2' exists."
#    exit "$ERR_FILE_IO"
#fi

# Check that [numFilesPerDirectory] is integer
re='^[0-9]+$'
if [[ ! "$3" =~ $re ]] ; then
   echo "Error: File '$2' is not readable."
   exit "$ERR_ARG_TYPE"
fi


# Reset Directory
rm -rf "$2"
echo "Directory '$2' has been deleted."

# Create [input_dir].
mkdir "$2"
echo "A new directory '$2' has been created."

# Save arguments to variables.
input_file="$1"
input_dir="$2"
num_files_per_dir="$3"

# Set delimeter
IFS=' '

# Get unique countries from records.
declare -A countries_records
num_records=0
while read -r record; do
    # Get country from each record.
    read -a word_array <<<"$record"
    country="${word_array[3]}"

    # If country is new, make its directory and files.
    if [[ "${countries_records["$country"]}" == "" ]]; then
        mkdir "$input_dir"/"$country"
        i=0
        while [[ "$i" -lt "$num_files_per_dir" ]]; do
            i_file="$input_dir"/"$country"/"$country"-"$i".txt
            if [[ -e "$i_file" ]]; then rm -f "$i_file"; fi
            touch "$i_file"
            i=$((i + 1))
        done
    fi

    # Save record
    countries_records["$country"]+="$record"$'~'

    num_records=$((num_records + 1))
done <"$input_file"

IFS='~'
for country in "${!countries_records[@]}"; do
    # Get records
    read -a records_array <<<"${countries_records["$country"]}"

    # Distribute records in round-robin fashion.
    declare -A file_records
    i=0
    while [[ "$i" -lt "$num_files_per_dir" ]]; do
        file_records["$i"]=""
        i=$((i + 1))
    done

    i=0
#    echo "Country $country:"
    for record in "${records_array[@]}"; do
#        echo "    $record"
        file_records["$i"]+="${record}"$'\n'
        if [[ "$i" -lt "$num_files_per_dir" ]];
        then
            i=$((i + 1))
        else
            i=0
        fi
    done

    # Write records to files
    i=0
    while [[ "$i" -lt "$num_files_per_dir" ]]; do
#        echo "File $country-$i.txt: ${file_records[$i]}"
        file_path="$input_dir/$country/$country-$i.txt"
        echo -n "${file_records["$i"]}">>"$file_path"
        i=$((i + 1))
    done
done

exit 0
