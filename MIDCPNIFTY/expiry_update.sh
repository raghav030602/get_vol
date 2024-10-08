index_name="$1"
          echo "expiry using new contract NSE_FO_contract.csv"
          cat NSE_FO_contract.csv | grep ",${index_name}," | grep OPTIDX | cut -d"," -f5 | sort -u |awk -v epoch='$1' -F"," 'BEGIN {OFS=","}{epoch=strftime("%Y%m%d",($1+315513000));print epoch}' |tr '\n' ',' | rev | cut -d',' -f2- | rev > expiryListWeekly_${index_name}.txt

          cat NSE_FO_contract.csv | grep ",${index_name}," | grep FUTIDX | cut -d"," -f5 | sort -u | awk -v epoch='$1' -F"," 'BEGIN {OFS=","}{epoch=strftime("%Y%m%d",($1+315513000));print epoch}'|tr '\n' ',' | rev | cut -d',' -f2- | rev > expiryListMonthly_${index_name}.txt

