#!/usr/bin/env bash

function print_usage {
    echo "USAGE: vizualizer.sh [options] input_file"
    echo "OPTIONS:"
    echo "-n                 output a graph without minimazing the DFA"
    echo '-o <file_name>     specify output file name (default is "<input_file>.png")'
}

input_file=""
input_file_set=false
output_file=""
minimize=""

while true; do
    case $1 in
        -n)
            minimize="-n"
            ;;
        -o)
            if [ -z "$2" ]; then
                print_usage
                exit 1
            fi
            output_file="$2"
            shift
            ;;
        *)
            if [ -z "$1" ]; then
                break
            fi

            if [ "$input_file_set" = true ]; then
                print_usage
                exit 1
            fi

            input_file="$1"
            input_file_set=true
            ;;
    esac

    shift
done

if [ "$input_file_set" = false ]; then
    print_usage
    exit 1
fi

if [ -z "$output_file" ]; then
    output_file="$input_file.png"
fi

temp_file="$(mktemp)"

./dfa_minimizer $minimize -o "$temp_file" "$input_file"

dot -Gdpi=200 -Tpng "$temp_file" -o "$output_file"
rm "$temp_file"


