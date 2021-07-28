#!/bin/bash
echo "Setup weights"

cd resources
cd weights

for d in */; do
	cd "$d"
	list=()
	for file in 0*.weights; do
		list+=("$file")
	done
	echo concat "${list[@]}" in "$d"
	cat "${list[@]}" > "${d::-1}".weights
	cd ..
done



echo "Done"
read -p "Press enter to continue"