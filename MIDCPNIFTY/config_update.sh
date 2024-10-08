#!/bin/bash

# Check if index name is provided as input
if [[ -z "$1" ]]; then
    echo "Error: No index name provided. Please provide an index (e.g., NIFTY, FINNIFTY) as input."
    exit 1
fi

# Get the input index name (e.g., NIFTY, FINNIFTY)
index_name="$1"

# Get the current date in YYYYMMDD format
current_date=$(date +"%Y%m%d")

# Paths to input files with dynamic names based on the index name
weekly_file="expiryListWeekly_${index_name}.txt"
monthly_file="expiryListMonthly_${index_name}.txt"
new_config_file="strategyConfig.txt"  # Updated output file name

# Path to the second file to modify
snap_config_file="config-FO-SNAP-Paper.txt"  # File to modify SYMBOL in

# Epoch adjustment to account for time since 1980 (subtract 10 years in seconds)
epoch_adjustment=315513000

# Function to convert YYYYMMDD with time to epoch time since 1980
convert_to_epoch() {
    local date_str="$1"
    local time_str="$2"
    # Combine date and time, then convert to epoch
    date -d "$date_str $time_str" +"%s" | awk -v adj="$epoch_adjustment" '{print $1 - adj}'
}

# Check if the weekly and monthly files exist
if [[ ! -f "$weekly_file" ]] || [[ ! -f "$monthly_file" ]]; then
    echo "Error: One or both of the expiry files ($weekly_file, $monthly_file) do not exist."
    exit 1
fi

# Read the single line of comma-separated values from weekly and monthly files
IFS=',' read -r -a weekly_expiry_list <<< "$(cat "$weekly_file")"
IFS=',' read -r -a monthly_expiry_list <<< "$(cat "$monthly_file")"

# Find the next weekly expiry date (including today) after the current date
next_weekly_expiry=""
for expiry in "${weekly_expiry_list[@]}"; do
    if [[ "$expiry" == "$current_date" || "$expiry" > "$current_date" ]]; then  # Use == to include today
        next_weekly_expiry="$expiry"
        break
    fi
done

# Find the next monthly expiry date (including today) after the current date
next_monthly_expiry=""
for expiry in "${monthly_expiry_list[@]}"; do
    if [[ "$expiry" == "$current_date" || "$expiry" > "$current_date" ]]; then  # Use == to include today
        next_monthly_expiry="$expiry"
        break
    fi
done

# Define the specific time to use for epoch conversion
specific_time="14:30:00"  # 2:30 PM

# Convert weekly and monthly expiry dates to epoch times since 1980 with specific time
if [[ -n "$next_weekly_expiry" ]]; then
    weekly_epoch=$(convert_to_epoch "$next_weekly_expiry" "$specific_time")
else
    echo "No future weekly expiry found!"
    exit 1
fi

if [[ -n "$next_monthly_expiry" ]]; then
    monthly_epoch=$(convert_to_epoch "$next_monthly_expiry" "$specific_time")
else
    echo "No future monthly expiry found!"
    exit 1
fi

# Always set input_date to the current date
input_date="$current_date"

# Variable that will be used to update SYMBOL in the SNAP config file
symbol2="${index_name}_${monthly_epoch}_FUT"

echo "Next Weekly Expiry: $next_weekly_expiry"
echo "Next Monthly Expiry: $next_monthly_expiry"
echo "Updated inputDate: $input_date"

# Write the new configuration values to the new config file
{
    echo "symbol1=${symbol2}"   # Updated key name for symbol1
    echo "strike_gap=25"
    echo "strike_level=1"   # Fixed value as per requirement
    echo "weekely_expiry=${weekly_epoch}"
} > "$new_config_file"

echo "Config file created successfully: $new_config_file!"

# Now update SYMBOL in config-FO-SNAP-Paper.txt

if [[ -f "$snap_config_file" ]]; then
    # Use sed to replace the SYMBOL line in the config-FO-SNAP-Paper.txt file
    sed -i "/^SYMBOL=/c\SYMBOL=${symbol2}" "$snap_config_file"
    echo "Updated SYMBOL in $snap_config_file to: SYMBOL=${symbol2}"
else
    echo "Error: $snap_config_file does not exist!"
    exit 1
fi

