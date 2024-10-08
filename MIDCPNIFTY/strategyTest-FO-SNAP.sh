#!/bin/sh

scp raghavm@172.31.100.129:/home/maitreya/spot_fut_spread_automation/MIDCPNIFTY_spot.csv .
export LD_LIBRARY_PATH=/home/dev/StrategyBoxCom:$LD_LIBRARY_PATH
 
file_name=StrategyTest_log.txt
errorfile_name=StrategyTest_error.txt
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
 
new_outputfileName=$file_name"_"$current_time
new_errorfileName=$errorfile_name"_"$current_time
#python3 script.py
echo "Parameters downloaded"

#make
echo "build done"

touch $new_outputfileName
ln -sf `pwd`/$new_outputfileName /tmp/MIDCPNIFTYVol.txt
taskset -c 78 ./strategyFoSnap.out "config-FO-SNAP-Paper.txt" "strategyConfig.txt" 1>$new_outputfileName 2>$new_errorfileName
