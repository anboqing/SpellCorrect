#!/bin/bash
cd ../
#echo $PWD
cd ShellScripts
#echo $PWD


function start 
{
	cd /home/anboqing/workspace/SpellCorrection/bin/
	./server
}

function stop
{
	cd /home/anboqing/workspace/SpellCorrection/bin/
	killall -9 server
}

function stat
{
	ps -ef | grep server
}

# like switch
case $1 in
	start) start ;;
	stop) stop ;;
	restart) stop ; start ;;
	stat) stat ;;
	*) echo "Igorn.." ;;
esac



#echo `ps -ef | grep 'sleep.sh'`
#pid=`ps -ef | grep 'sleep.sh' | awk -F' ' '{print $2}'`
#pid=$(pidof -x excute.sh)
#echo $pid

#kill -9 $pid
#echo $?

#tar -czvf shell.tar.gz shellScripts/
