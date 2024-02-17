#!/bin/bash
while true
do
gcc -Wall lib/utils/utils.c lib/linkedlist/linkedlist.c lib/database/database.c main.c -o beleg.cgi
read -p "Press Enter to re-compile" </dev/tty
clear
done