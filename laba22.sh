#!/bin/bash
count=0
handler() {
    echo "Interuption"
    ((count++))
    if [ $((count%5)) -eq 0 ]
    then
        for var in $tty_name
        do
            echo $var
        done
    fi
}
trap 'handler' 2
tty_name=$(who | sed 's/ \{1,\}/ /g' | cut -d ' ' -f 2)
echo "Type your message:"
read message 
wall $message
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
sleep 2
echo "Farewell!"
