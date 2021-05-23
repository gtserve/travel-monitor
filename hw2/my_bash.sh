#!/bin/bash

# my_bash.sh
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# project:  (#2) Travel Monitor
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------

# /travelRequest 4801 20-11-2020 Afghanistan China Rubella

# 5899 Randolph Howard Iceland 64 Measles YES 18-06-1818

country_to="Greece"

requests=""
IFS=' '

while read -r line; do

    read -a word_array <<<"$line"

    id="${word_array[0]}"
    country_from="${word_array[3]}"
    virus="${word_array[5]}"

    # Get a random date.
    year=$((1796 + (RANDOM % 225))) # 1796 - 2021
    month=$((1 + (RANDOM % 12)))    # 1 - 12
    day=$((1 + (RANDOM % 30)))      # 1 - 30

    date=$(printf "%02d-%02d-%04d" "$day" "$month" "$year")

    requests+="/travelRequest $id $date $country_from $country_to $virus"$'\n'

done <"./input.txt"

echo -n "$requests" > ./requests.txt
