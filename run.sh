MAX=10
echo teste smith_waterman > resultados

for NTHREAD in 1 2 4 8 16
do
	clear;
	echo -e "teste com $NTHREAD nucleos ";
	for ENTRADA in entrada_13000_1 entrada_13000_2 entrada_26000_1 entrada_26000_2 entrada_39000_1 entrada_39000_2
		do
		date
		echo -n "$ENTRADA";
		echo -n "teste com $NTHREAD nucleos : $ENTRADA " >> resultados;
		for (( i = 0; i < MAX; i++ )); do
			 (time ./paralelo.exe $NTHREAD 13000 < $ENTRADA) >> resultados;
			 echo -n ".";
		done
		echo -n "media tempo: " >> resultados;
		echo -e ".";

	done
done
