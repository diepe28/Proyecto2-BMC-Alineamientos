#include "Globals.h"

char* SEQUENCE_TYPES[3] = {
	"ADN",
	"Proteina",
	"Texto"
};

char AMINO_ACIDS[64]  = {
	'K', //0-  AAA: Lysine 
	'N', //1-  AAC: Asparagine 
	'K', //2-  AAG: Lysine
	'N', //3-  AAU: Asparagine

	'T', //4-  ACA: Threonine 
	'T', //5-  ACC: Threonine
	'T', //6-  ACG: Threonine 
	'T', //7-  ACU: Threonine

	'S', //8-  AGU: Serine
	'S', //9-  AGC: Serine
	'R', //10- AGA: Arginine
	'R', //11- AGG: Arginine

	'I', //12- AUU: Isoleucine
	'I', //13- AUC: Isoleucine
	'I', //14- AUA: ISoleucine 
	'M', //15- AUG: Metionine

	'Q', //16- CAA: Glutamine
	'H', //17- CAC: Histidine 
	'Q', //18- CAG: Glutamine 
	'H', //19- CAU: Histidine
 
	'P', //20- CCA: Proline 
	'P', //21- CCC: Proline
	'P', //22- CCG: Proline
	'P', //23- CCU: Proline

	'R', //24- CGA: Arginine
	'R', //25- CGC: Arginine
	'R', //26- CGG: Arginine 
	'R', //27- CGU: Arginine
 	   
	'L', //28- CUA: Leucine
	'L', //29- CUC: Leucine 	   		  	    
	'L', //30- CUG: Leucine
	'L', //31- CUU: Leucine

	'E', //32- GAA: GlutamicAcid
	'D', //33- GAC: AsparticAcid 
	'E', //34- GAG: GlutamicAcid 
	'D', //35- GAU: AsparticAcid

	'A', //36- GCA: Alanine
	'A', //37- GCC: Alanine
	'A', //38- GCG: Alanine
	'A', //39- GCU: Alanine

	'G', //40- GGA: Glycine
	'G', //41- GGC: Glycine 
	'G', //42- GGG: Glycine
	'G', //43- GGU: Glycine
			  
	'V', //44- GUA: Valine
	'V', //45- GUC: Valine
	'V', //46- GUG: Valine
	'V', //47- GUU: Valine

	'*', //48- UAA: STOP
	'Y', //49- UAC: Tyrosine
	'*', //50- UAG: STOP
	'Y', //51- UAU: Tyrosine

	'S', //52- UCA: Serine
	'S', //53- UCC: Serine
	'S', //54- UCG: Serine
	'S', //55- UCU: Serine

	'*', //56- UGA: STOP
	'C', //57- UGC: Cysteine
	'W', //58- UGG: Tryptophan 
	'C', //59- UGU: Cysteine 
	  	   
	'L', //60- UUA: Leucine 
	'F', //61- UUC: Phenylalanine  		  	   
	'L', //62- UUG: Leucine
	'F'  //63- UUU: Phenylalanine 
};

//  A = 0 , C = 1, G = 2, U = 3
gint valueOfBase(gchar base){
	return base == 'A' ? 0 : base == 'C'? 1 : base == 'G'? 2 : 3;
}

//*16 + *4 + *1 = INDEX
gchar codonToOneLetter(gchar codon[3]){
	return AMINO_ACIDS[(16 * valueOfBase(codon[0])) + (4 * valueOfBase(codon[1])) + valueOfBase(codon[2]) ];
}

gchar* dnaToProtein(gchar* dna){
	int dnaLength = strlen(dna), currentCodon = 0, i = 0, codonCount = (int) (dnaLength / 3);
	gchar* protein = (gchar*) g_malloc((codonCount + 1) * sizeof(gchar));
	gchar codon[3];

	for(; currentCodon < codonCount; i+=3, currentCodon++){
		codon[0] = dna[i];
		codon[1] = dna[i+1];
		codon[2] = dna[i+2];
		protein[currentCodon] = codonToOneLetter(codon);
	}
	protein[currentCodon] = '\0';

	return protein;
}

gchar* threeLetterCodedProteinToOneLetterCodedProtein(const gchar* seq, gint seqSize)
{
	if (seqSize % 3 != 0)
		return NULL;
	
	gint i = 0, currentPosition = 0;
	gint newSeqSize = seqSize / 3;
	gchar* newSeq = (gchar*) g_malloc(sizeof(gchar) * (newSeqSize + 1));
	while (currentPosition < seqSize) {
		if (g_str_has_prefix(&(seq[currentPosition]), "Ala")) { newSeq[i++] = 'A'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Arg")) { newSeq[i++] = 'R'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Asn")) { newSeq[i++] = 'N'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Asp")) { newSeq[i++] = 'D'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Asx")) { newSeq[i++] = 'B'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Cys")) { newSeq[i++] = 'C'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Glu")) { newSeq[i++] = 'E'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Gln")) { newSeq[i++] = 'Q'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Glx")) { newSeq[i++] = 'Z'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Gly")) { newSeq[i++] = 'G'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "His")) { newSeq[i++] = 'H'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Ile")) { newSeq[i++] = 'I'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Leu")) { newSeq[i++] = 'L'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Lys")) { newSeq[i++] = 'K'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Met")) { newSeq[i++] = 'M'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Phe")) { newSeq[i++] = 'F'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Pro")) { newSeq[i++] = 'P'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Ser")) { newSeq[i++] = 'S'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Thr")) { newSeq[i++] = 'T'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Trp")) { newSeq[i++] = 'W'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Tyr")) { newSeq[i++] = 'Y'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "Val")) { newSeq[i++] = 'V'; currentPosition += 3; continue; }
		if (g_str_has_prefix(&(seq[currentPosition]), "***")) { newSeq[i++] = '*'; currentPosition += 3; continue; }
		
		g_free(newSeq);
		return NULL;
	}
	newSeq[newSeqSize] = '\0';
	return newSeq;
}

gchar* sequenceFromFile(gchar * filePath, gulong *length){
	gulong fileSize = 0, i = 0;
	gchar *sequence = NULL;
	FILE * file = fopen(filePath, "r");
		
	if(file != NULL){
		fseek(file, 0L, SEEK_END);
		fileSize = ftell(file);
		fseek(file, 0L, SEEK_SET);

		sequence = (gchar*) (g_malloc((fileSize+1) * sizeof(gchar)));
		if(sequence != NULL){
			while( (sequence[i++] = (gchar) fgetc(file)) != EOF);
			sequence[i] = '\0';
		}
		fclose(file);
	}else{
		perror ( filePath ); /* why didn't the file open? */
		fileSize = 0;
	}
	*length = fileSize;
	return sequence;
}

gint valueOfMatrix(gint matrix[27][27], gchar first, gchar second){
	int firstIndex  = first  == '*'? 26 : (int) first  - 'A';
	int secondIndex = second == '*'? 26 : (int) second - 'A';
	return matrix[firstIndex][secondIndex];
}

int createBenchmarkGraph(long* times, int n){
	char * commandsForGnuplot[] = {
		"set terminal png large size 1080, 600",
		"set output \"BENCHMARK.png\"",
		"set style line 1 lc rgb 'black' lt 1 lw 2 pt 7 ps 1", //0060ad blue
		"set style line 2 lc rgb '#33CC33' lt 1 lw 2 pt 7 ps 1.5", //green
		"set title \"Curva de aceleracion\"",
		"set xlabel \"Cantidad de hilos\"",
		"set ylabel \"Tiempo en microsegundos\"",
		"unset key",
		"set offset 1,1,1,1",
		"plot 'data.temp' index 0 with linespoints ls 1, 'data.temp' using 1:2:(sprintf(\"(%d, %d)\", $1, $2)) with labels offset char 2,2 notitle"};
	
	char xRangeCommand[30], yRangeCommand[50], lowestPoint[150], highestPoint[150];
	int i = 0, numCommands = 10, minTimeIndex = 0, maxTimeIndex = 0;
	long maxTime = 0, minTime = LONG_MAX, yMargin;
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	FILE * tempFile = fopen("data.temp", "w");

	if(gnuplotPipe == NULL || tempFile == NULL)
		return 1;
	
	//finding min and max time
	for(i =0; i < n; i++){
		if(times[i] > maxTime){
			maxTime = times[i];
			maxTimeIndex = i;
		}
		if(times[i] < minTime){
			minTime = times[i];
			minTimeIndex = i;
		}
	}

	yMargin = (maxTime - minTime) / 4;
	if(yMargin <= 0){
		yMargin = maxTime / 4;
	}
	sprintf (xRangeCommand, "set xrange [%d:%d]", 0, n+1);
	sprintf (yRangeCommand, "set yrange [%ld:%ld]", minTime - yMargin, maxTime + yMargin);
	sprintf (lowestPoint, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb '#007A00' fillstyle solid noborder",
	         minTimeIndex+1, minTime);
	sprintf (highestPoint, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb 'red' fillstyle solid noborder",
	         maxTimeIndex+1, maxTime);
	
	
	for(i=0; i < n; i++){
		fprintf(tempFile, "%d %ld \n", i+1, times[i]); //Write the data to a temporary file
	}
	
	fprintf(gnuplotPipe, "%s \n", xRangeCommand);
	fprintf(gnuplotPipe, "%s \n", yRangeCommand);
	fprintf(gnuplotPipe, "%s \n", lowestPoint);
	fprintf(gnuplotPipe, "%s \n", highestPoint);
	for (i=0; i < numCommands; i++){
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
	}

	fclose(tempFile);
	pclose(gnuplotPipe);
	
	return 0;
}

int createBenchmarkGraphKBand(long* times, long* timesKBand, int n){
	char * commandsForGnuplot[] = {
		"set terminal png large size 1080, 600",
		"set output \"BENCHMARK.png\"",
		"set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1", //black
		"set style line 2 lc rgb '#33CC33' lt 1 lw 2 pt 7 ps 1.5", //green
		"set title \"Curva de aceleracion\"",
		"set xlabel \"Cantidad de hilos\"",
		"set ylabel \"Tiempo en microsegundos\"",
		"set key box height 3",
		"set offset 1,1,1,1",
		//"plot 'data.temp' index 0 with linespoints ls 1, '' index 1 with linespoints ls 2, 'data.temp' using 1:2:(sprintf(\"(%d, %d)\", $1, $2)) with labels offset char 2,2 notitle "};
		"plot 'data.temp' index 0 with linespoints ls 1 title \"Full Matrix\" , '' index 1 with linespoints ls 2 title \"Using K-Band\""};
	
	char xRangeCommand[30], yRangeCommand[50],
		lowestPointFull[150],  highestPointFull[150],
		lowestPointLabelFull[150],  highestPointLabelFull[150],
	
		lowestPointKBand[150], highestPointKBand[150],	
		lowestPointKLabelBand[150], highestPointLabelKBand[150];
	
	int i = 0, numCommands = 10, minTimeIndexFull = 0, maxTimeIndexFull = 0,
			minTimeIndexKBand = 0, maxTimeIndexKBand = 0;
	
	long maxTimeTotal, minTimeTotal, yMargin, yMarginForLabel,
		 maxTimeFull = times[0],  minTimeFull = times[0],
		 maxTimeKBand = timesKBand[0], minTimeKBand = timesKBand[0];
	
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	FILE * tempFile = fopen("data.temp", "w");

	if(gnuplotPipe == NULL || tempFile == NULL)
		return 1;
	
	//finding min and max time in both times
	for(i =1; i < n; i++){
		//Full matrix
		if(times[i] > maxTimeFull){
			maxTimeFull = times[i];
			maxTimeIndexFull = i;
		}
		if(times[i] < minTimeFull){
			minTimeFull = times[i];
			minTimeIndexFull = i;
		}

		//Kband
		if(timesKBand[i] > maxTimeKBand){
			maxTimeKBand = timesKBand[i];
			maxTimeIndexKBand = i;
		}
		if(timesKBand[i] < minTimeKBand){
			minTimeKBand = timesKBand[i];
			minTimeIndexKBand = i;
		}
	}
	maxTimeTotal = maxTimeFull > maxTimeKBand? maxTimeFull : maxTimeKBand;
	minTimeTotal = minTimeFull < minTimeKBand? minTimeFull : minTimeKBand;
	
	yMargin = (maxTimeTotal - minTimeTotal) / 4;
	if(yMargin <= 0){
		yMargin = maxTimeTotal / 4;
	}
	yMarginForLabel = (maxTimeTotal - minTimeTotal + 2*yMargin) / 20;
	sprintf (xRangeCommand, "set xrange [%d:%d]", 0, n+1);
	sprintf (yRangeCommand, "set yrange [%ld:%ld]", minTimeTotal - yMargin, maxTimeTotal + yMargin);
	

	// Min, max points and labels of them using full matrix
	sprintf (lowestPointFull, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb '#007A00' fillstyle solid noborder",
	         minTimeIndexFull+1, minTimeFull);
	sprintf (highestPointFull, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb 'red' fillstyle solid noborder",
	         maxTimeIndexFull+1, maxTimeFull);
	
	sprintf (lowestPointLabelFull, "set label \"(%d,%ld) \" at %d,%ld",
	         minTimeIndexFull+1, minTimeFull, minTimeIndexFull+1, minTimeFull + yMarginForLabel);
	sprintf (highestPointLabelFull, "set label \"(%d,%ld) \" at %d,%ld",
	         maxTimeIndexFull+1, maxTimeFull, maxTimeIndexFull+1, maxTimeFull + yMarginForLabel);

	// Min, max points and labels of them using kband
	sprintf (lowestPointKBand, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb '#007A00' fillstyle solid noborder",
	         minTimeIndexKBand+1, minTimeKBand);
	sprintf (highestPointKBand, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb 'red' fillstyle solid noborder",
	         maxTimeIndexKBand+1, maxTimeKBand);

	sprintf (lowestPointKLabelBand, "set label \"(%d,%ld) \" at %d,%ld",
	         minTimeIndexKBand+1, minTimeKBand, minTimeIndexKBand+1, minTimeKBand + yMarginForLabel);
	sprintf (highestPointLabelKBand,"set label \"(%d,%ld) \" at %d,%ld",
	         maxTimeIndexKBand+1, maxTimeKBand, maxTimeIndexKBand+1, maxTimeKBand + yMarginForLabel);
	
	//Write the data of full calc to temporary file
	for(i=0; i < n; i++){
		fprintf(tempFile, "%d %ld \n", i+1, times[i]); 
	}

	fprintf(tempFile, "\n\n");
	//Write the data of kband to to temporary file
	for(i=0; i < n; i++){
		fprintf(tempFile, "%d %ld \n", i+1, timesKBand[i]);
	}
	
	fprintf(gnuplotPipe, "%s \n", xRangeCommand);
	fprintf(gnuplotPipe, "%s \n", yRangeCommand);
	
	fprintf(gnuplotPipe, "%s \n", lowestPointFull);
	fprintf(gnuplotPipe, "%s \n", lowestPointLabelFull);
	fprintf(gnuplotPipe, "%s \n", highestPointFull);
	fprintf(gnuplotPipe, "%s \n", highestPointLabelFull);

	fprintf(gnuplotPipe, "%s \n", lowestPointKBand);
	fprintf(gnuplotPipe, "%s \n", lowestPointKLabelBand);
	fprintf(gnuplotPipe, "%s \n", highestPointKBand);
	fprintf(gnuplotPipe, "%s \n", highestPointLabelKBand);
	
	for (i=0; i < numCommands; i++){
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
	}

	fclose(tempFile);
	pclose(gnuplotPipe);
	
	return 0;
}

gchar* APP_SEQUENCE_TYPE(gint stype) {
	return SEQUENCE_TYPES[stype];
}


gint createBirdWatchGraphNW(Island* island, gint rows, gint cols){
	gchar * commandsForGnuplot[] = {
		"set terminal png large size 1920, 1080",
		"set output \"BIRD_WATCH.png\"",
		"set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1", //0060ad blue
		"set style line 2 lc rgb '#33CC33' lt 1 lw 2 pt 7 ps 1.5", //green
		"set title \"Vista de pajaro alineamiento global\"",
		"unset key",
		"set format y \"\"",
		"set format x \"\"",
		"set offset 1,1,1,1",
		"plot 'birdWatchdata.temp' index 0 with linespoints ls 1"};
	
	gchar xRangeCommand[30], yRangeCommand[50];
	gint i = 0, j = 0, numCommands = 10, minTimeIndex = 0, maxTimeIndex = 0, x, y;
	glong maxTime = 0, minTime = LONG_MAX, yMargin;
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	FILE * tempFile = fopen("birdWatchdata.temp", "w");
	char pointCommand[150];
	
	if(gnuplotPipe == NULL || tempFile == NULL)
		return 1;
		
	sprintf (xRangeCommand, "set xrange [%d:%d]", 0, cols);
	sprintf (yRangeCommand, "set yrange [%d:%d]", 0, rows);
	
	GSList* iterator;
	for(iterator = island->points; iterator; iterator = iterator->next){
		x = (gint) (iterator->data);
		iterator = iterator->next;
		y = (gint) (iterator->data);
		fprintf(tempFile, "%d %d \n", x , rows-y); //Write the data to a temporary file
	}
	
	fprintf(gnuplotPipe, "%s \n", xRangeCommand);
	fprintf(gnuplotPipe, "%s \n", yRangeCommand);
	
	for (i=0; i < numCommands; i++){
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
	}

	fclose(tempFile);
	pclose(gnuplotPipe);
	
	return 0;
}

gint createBirdWatchGraphSW(GSList* islands, gint rows, gint cols){
	gchar * commandsForGnuplot[] = {
		"set terminal png large size 1920, 1080",
		"set output \"BIRD_WATCH.png\"",
		"set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1", //0060ad blue
		"set style line 2 lc rgb '#33CC33' lt 1 lw 2 pt 7 ps 1.5", //green
		"set title \"Vista de pajaro alineamiento local\"",
		"unset key",
		"set format y \"\"",
		"set format x \"\"",
		"set offset 1,1,1,1",
		"plot 'birdWatchdata.temp' with linespoints ls 1"};
	
	gchar xRangeCommand[30], yRangeCommand[50];
	gint i = 0, j = 0, numCommands = 10, minTimeIndex = 0, maxTimeIndex = 0, x, y;
	glong maxTime = 0, minTime = LONG_MAX, yMargin;
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	FILE * tempFile = fopen("birdWatchdata.temp", "w");
	char pointCommand[150], direction;
	GSList* islandIterator;
	Island* currentIsland;
	
	if(gnuplotPipe == NULL || tempFile == NULL)
		return 1;
		
	sprintf (xRangeCommand, "set xrange [%d:%d]", 0, cols);
	sprintf (yRangeCommand, "set yrange [%d:%d]", 0, rows);

	for(islandIterator = islands; islandIterator; islandIterator = islandIterator->next){
		currentIsland = (Island*) (islandIterator->data);
		x = currentIsland->startCol;
		y = currentIsland->startRow;
		fprintf(tempFile, "%d %d \n", x , rows-y); //Write the data to a temporary file
		for(j = 0; (direction = currentIsland->islandPath[j]) != '\0'; j++){
			switch(direction){
				case 'D':
					x--; y--;
					break;
				case 'L':
					x--;
					break;
				case 'U':
					y--;
					break;
			}
			fprintf(tempFile, "%d %d \n", x , rows-y); //Write the data to a temporary file
		}			
		fprintf(tempFile, "\n\n");
	}	
	
	fprintf(gnuplotPipe, "%s \n", xRangeCommand);
	fprintf(gnuplotPipe, "%s \n", yRangeCommand);
	
	for (i=0; i < numCommands; i++){
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
	}

	fclose(tempFile);
	pclose(gnuplotPipe);
	
	return 0;
}
