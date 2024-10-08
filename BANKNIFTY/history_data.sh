#!/bin/bash

# Get today's date in the format YYYY.MM.DD
today=$(date +"%Y.%m.%d")

# Define the directory where the file is located
log_directory="/home/raghav/Dummy-Strat/get_vol/FINNIFTY"  # You can replace this with the actual path to the directory
history_directory="$log_directory/history"

# Ensure the 'history' directory exists, if not create it
if [ ! -d "$history_directory" ]; then
    mkdir -p "$history_directory"
fi

# Find the log file for today's date
log_file=$(ls ${log_directory}/StrategyTest_log.txt_${today}-* 2>/dev/null)

# Check if the file exists
if [[ -z "$log_file" ]]; then
    echo "No log file found for today's date ($today)"
    exit 1
else
    # Move the log file to the history directory
    mv "$log_file" "$history_directory"
    
    if [ $? -eq 0 ]; then
        echo "Successfully moved $log_file to $history_directory"
    else
        echo "Failed to move the log file."
        exit 1
    fi
fi

