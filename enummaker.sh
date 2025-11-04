#!/bin/bash

csv_file="$1"
base_name=$(basename "$csv_file" .csv)
h_file="$2"

{
    echo "#pragma once"
    echo ""
    echo "enum ${base_name} : int {"
    
    tail -n +2 "$csv_file" 2>/dev/null | grep -v "NOT COMPLETED" | while IFS=',' read -r first_col rest; do
        element=$(echo "$first_col" | sed -E 's/['\''.!]//g; s/ *\([^)]*\)//g; s/0/zero/g; s/1/one/g; s/2/two/g; s/3/three/g; s/4/four/g; s/5/five/g; s/6/six/g; s/7/seven/g; s/8/eight/g; s/9/nine/g; s/ +$//; s/[- ]+/_/g; s/.*/\U&/')
        echo "    $element,"
    done
    
    echo "    ${base_name^^}_COUNT"
    echo "};"
} > "$h_file"