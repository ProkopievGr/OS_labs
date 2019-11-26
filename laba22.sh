#!/bin/bash
count=0
handler() {
    echo "Interuption"
    ((count++))
    if [ $((count%5)) -eq 0 ]
    then
        for var in ${tty_name[@]}; do 
            echo $var
        done
    fi
}
trap 'handler' 2

echo "Type your message:"
read message 

output_users=$(who | sed 's/ \{1,\}/ /g' | cut -d ' ' -f 1)
output_ttys=$(who | sed 's/ \{1,\}/ /g' | cut -d ' ' -f 2)

users_name=()
tty_name=()
for usr in $output_users; do
    users_name+=($usr)
done
for term in $output_ttys; do
    tty_name+=($term)
done

for i in ${!users_name[@]}; do
    echo $message | write ${users_name[$i]} ${tty_name[$i]}
done


sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
echo "Farewell!"
