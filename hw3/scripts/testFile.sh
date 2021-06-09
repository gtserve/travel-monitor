#!/bin/bash

# testFile.sh
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------

ERR_NUM_ARGS=1
ERR_FILE_IO=2
#ERR_WRONG_ARG=3

RES_DIR="../resources"
NAMES_FILE="$RES_DIR/names.txt"
SURNAMES_FILE="$RES_DIR/surnames.txt"
RECORDS_FILE="$RES_DIR/records.txt"

MAX_ID=10000

# ---------------------------------------- Functions ----------------------------------------------

print_usage() {
    # Print the proper usage of the script.
    echo "Script usage:"
    echo "  bash ./testFile.sh [virusesFile] [countriesFile]" \
        "[numLines] [duplicatesAllowed]"
}

# -------------------------------------------------------------------------------------------------

# Check the number of arguments.
if [[ "$#" -ne 4 ]]; then
    echo "Error: Wrong number of arguments."
    print_usage
    exit "$ERR_NUM_ARGS"
fi

# Check that the files exist and are readable.
if [[ ! -e "$1" ]] || [[ ! -f "$1" ]]; then
    echo "Error: File '$1' does not exists."
    exit "$ERR_FILE_IO"
fi
if [[ ! -e "$2" ]] || [[ ! -f "$2" ]]; then
    echo "Error: File '$2' does not exists."
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

viruses_file="$1"
countries_file="$2"
num_records="$3"
dupl_flag="$4"

# Print a useful message.
message="Creating $num_records records with"
if [[ "$dupl_flag" -eq 0 ]]; then
    message="$message no"
fi
echo "$message duplicates, using '$viruses_file' and '$countries_file'..."

# Save files to arrays for faster access.
num_names=0
declare -a name_array
while read -r line; do
    name_array+=("$line")
    num_names=$((num_names + 1))
done <"$NAMES_FILE"

num_surnames=0
declare -a surname_array
while read -r line; do
    surname_array+=("$line")
    num_surnames=$((num_surnames + 1))
done <"$SURNAMES_FILE"

num_countries=0
declare -a country_array
while read -r line; do
    country_array+=("$line")
    num_countries=$((num_countries + 1))
done <"$countries_file"

num_viruses=0
declare -a virus_array
while read -r line; do
    virus_array+=("$line")
    num_viruses=$((num_viruses + 1))
done <"$viruses_file"

echo "Using $num_viruses viruses, $num_countries countries," \
    "$num_names names and $num_surnames surnames."

# Create new empty file for the records.
if [[ -e "$RECORDS_FILE" ]]; then rm -f "$RECORDS_FILE"; fi
touch "$RECORDS_FILE"

declare -a id_array
num=0
while [ "$num" -lt "$num_records" ]; do
    id_array+=($(seq 0 $((MAX_ID - 1))))
    num=$((num + MAX_ID))
done
id_array=($(printf '%s\n' "${id_array[@]}" | shuf | head -n "$num_records"))


if [[ "$dupl_flag" -eq 1 ]]; then
    num_duplicates=$((1 + (num_records / 10) + (RANDOM % (num_records / 2))))
    while [[ "$num_duplicates" -gt 0 ]]; do
        i=$((RANDOM % num_records))
        j=$((RANDOM % num_records))
        id_array[i]="${id_array[j]}"
        num_duplicates=$((num_duplicates - 1))
    done
#    id_array=($(seq "$MAX_ID" | shuf | head -n $((num_records - num_duplicates))))
#    id_array+=($(printf '%s\n' "${id_array[@]}" | shuf | head -n "$num_duplicates"))
#    id_array=($(printf '%s\n' "${id_array[@]}" | shuf))
fi

index=0
records=""
while [[ "$index" -lt "$num_records" ]]; do

    # Get a random citizen_id between [0 - MAX_ID]
    citizen_id="${id_array[$index]}"

    # Get a random first name from array.
    i=$((RANDOM % num_names))
    name="${name_array[$i]}"

    # Get a random surname from array.
    i=$((RANDOM % num_surnames))
    surname="${surname_array[$i]}"

    # Get a random country from array.
    i=$((RANDOM % num_countries))
    country="${country_array[$i]}"

    # Get random age between [0 - 120]
    age=$((RANDOM % 121))

    # Get a random virus from array.
    i=$((RANDOM % num_viruses))
    virus="${virus_array[$i]}"

    record="$citizen_id $name $surname $country $age $virus"

    # Random YES/NO for vaccination.
    vaccinated=$((RANDOM % 2))
    if [[ "$vaccinated" -eq 1 ]]; then
        # Get a random date.
        year=$((1796 + (RANDOM % 225))) # 1796 - 2021
        month=$((1 + (RANDOM % 12)))    # 1 - 12
        day=$((1 + (RANDOM % 30)))      # 1 - 30
        record=$(printf "%s YES %02d-%02d-%04d" "$record" "$day" "$month" "$year")
    else
        record="$record NO"
    fi

    records+="${record}"$'\n'
    index=$((index + 1))
done

echo -n "$records" >>"$RECORDS_FILE"

exit 0
