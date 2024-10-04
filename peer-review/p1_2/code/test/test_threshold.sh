#!/bin/bash

thresholds=(0.2 0.4 0.6 0.8 1.0)

query="Then go we near her, that her ear lose nothing"
input_path="../test/shakespeare"

for threshold in "${thresholds[@]}"; do
    output_file="output/threshold/${threshold}.txt"
    echo "Running with threshold $threshold, output to $output_file"
    ../build/ADS_search_engine search "$input_path" -q "$query" -t "$threshold" > "$output_file"
done