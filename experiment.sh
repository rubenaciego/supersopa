#!/bin/bash

words=(10 20 40 50 100 200 500 1000)
size=(10 100 200 500 1000 2000 5000 10000)
dictionaries=('diccionarios/dracula-vocabulary-4.txt' 'diccionarios/mare-balena-vocabulary-3.txt' 'diccionarios/quijote-vocabulary-3.txt')

for ((k = 0 ; k < 2 ; ++k)); do
    for ((j = 0 ; j < 3 ; j++)); do
        for ((i = 0 ; i < 8 ; i++)); do
            echo "--------------------------------------------------" >> results.txt
            if [[ $k == 0 ]]; then
                echo "${dictionaries[j]} ${words[i]} ${size[i]} ${size[i]} 5" >> results.txt
                echo "!!!!!" >> results.txt
                ./supersopa ${dictionaries[j]} ${words[i]} ${size[i]} ${size[i]} 5 >> results.txt 2>> results.txt
            else
                echo "${dictionaries[j]} ${words[i]} ${size[i]} ${size[i]} 5 100 5" >> results.txt
                echo "!!!!!" >> results.txt
                ./supersopa ${dictionaries[j]} ${words[i]} ${size[i]} ${size[i]} 5 100 5 >> results.txt 2>> results.txt
            fi
            
        done
    done
done

echo "------------------------------ FINAL ------------------------------" >> results.txt
