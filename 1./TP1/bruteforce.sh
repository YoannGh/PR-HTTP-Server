#!/bin/bash

LIMIT=100

for ((a=1; a <= LIMIT ; a++))
do
   echo -n "$a "
   ./forkfile
   echo -e "\n" >> possibles.txt
   cat fich1 >> possibles.txt
done
