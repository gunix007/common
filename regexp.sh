#!/bin/sh

# mac regular expression start
mac=00:16:FB:B1:75:85
mac_exp="^[[:xdigit:]]{2}(([:-]{1}[[:xdigit:]]{2}){5})$"
echo $mac | grep -E "${mac_exp}" > /dev/null
echo $?
mac_exp="^[0-9a-fA-F]{2}(([:-]{1}[0-9a-fA-F]{2}){5})$"
echo $mac | grep -E "${mac_exp}" > /dev/null
echo $?
# mac regular expression end

# valid phone number start
# (xxx) xxx-xxxx or xxx-xxx-xxxx are valid phone number
phone="987-123-4567\n123 456 7890\n(123) 456-7890"
echo $phone | grep -P '^(\d{3}-|\d{3} )\d{3}-\d{4}$' > /dev/null
echo $?
echo $phone | sed -n -r '/^(\d{3}-|\d{3} )\d{3}-\d{4}$/p' > /dev/null
echo $?
echo $phone | awk '/^([0-9]{3}-|\([0-9]{3}\) )[0-9]{3}-[0-9]{4}$/' > /dev/null
echo $?
# valid phone number end

# print the tenth line start
file=./tmp.txt
awk 'NR == 10' $file
sed -n 10p $file
tail -n+10 $file | head -1
# print the tenth line end











