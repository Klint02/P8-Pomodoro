#!/bin/bash

board="esp8266:esp8266:d1_mini"
baud=""
path=""
device="/dev/ttyUSB0"
serial_flag=false
compile_flag=false
upload_flag=false
pipeline_failure=false

while getopts ":p:b:s:d:cuh" opt; do 
    case $opt in
        h)
            echo "Welcome to arduino-cli build wrapper"
            echo "Usage: "
            echo ""
            echo "Flags:"
            echo "  -h                  'Show this message'"
            echo "  -p \"path\"           'Provide path to project ino file'"
            echo "  -b \"board fqbn\"     'Change default fqbn for arduino-cli. Default is \"esp8266:esp8266:d1_mini\"'"
            echo "  -s \"baud\"           'Opens serial monitor with provided baud number'"
            echo "  -d \"device\"         'Change default device from \"/dev/ttyUSB0\" to something else'"
            echo "  -c                  'Compile project using arduino-cli'"
            echo "  -u                  'Upload project using arduino-cli'"
            exit $E_OPTERROR
        ;;
        p) path=$OPTARG
        ;;
        b) board=$OPTARG
        ;;
        s) 
            baud=$OPTARG
            serial_flag=true
        ;;
        d) device=$OPTARG
        ;;
        c) compile_flag=true
        ;;
        u) upload_flag=true
        ;;
    esac
done

echo "Current config"        
echo "  Board: $board"
echo "  Ino Path: $path"
echo "  Device path: $device"
echo "  Compile?: $compile_flag"
echo "  Upload?: $upload_flag"
echo "  Serial monitor on: $serial_flag at $baud baud"

if [ $compile_flag = true ]; then
    echo "arduino-cli compile $path --fqbn $board"
    arduino-cli compile $path --fqbn $board
    status=$?
    if [ $status -eq 1 ]; then
        echo "compilation exitted with a $status status"
        pipeline_failure=true
    fi
fi

if [[ $upload_flag = true && $pipeline_failure = false ]]; then
    echo "arduino-cli upload $path -p $device -b $board"
    arduino-cli upload $path -p $device -b $board
fi

if [[ $serial_flag = true && $pipeline_failure = false ]]; then
    echo "arduino-cli monitor -p $device --config $baud"
    arduino-cli monitor -p $device --config $baud
fi
