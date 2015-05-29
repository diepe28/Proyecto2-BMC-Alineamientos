#include "Globals.h"

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
	return AMINO_ACIDS[(16 * valueOfBase(codon[0])) + (4 * valueOfBase(codon[1])) + valueOfBase(codon[0]) ];
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

gint valueOfMatrix(gint matrix[26][26], gchar first, gchar second){
	return matrix[(int) first - 'A'][(int) second - 'A'];
}

int createBenchmarkGraph(long* times, int n){
	char * commandsForGnuplot[] = {
		"set terminal png large size 1920, 1080",
		"set output \"printme.png\"",
		"set style line 1 lc rgb 'black' lt 1 lw 2 pt 7 ps 1", //0060ad blue
		"set style line 2 lc rgb '#33CC33' lt 1 lw 2 pt 7 ps 1.5", //green
		"set title \"Curva de aceleracion\"",
		"set xlabel \"Cantidad de hilos\"",
		"set ylabel \"Tiempo en milisegundos\"",
		"unset key",
		"set offset 1,1,1,1",
		"plot 'data.temp' index 0 with linespoints ls 1, 'data.temp' using 1:2:(sprintf(\"(%d, %d)\", $1, $2)) with labels offset char 2,2 notitle"};
		//"plot 'data.temp' index 0 with linespoints ls 1, '' index 1 with linespoints ls 2"};
	
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	FILE * tempFile = fopen("data.temp", "w");
	char xRangeCommand[30], yRangeCommand[50], lowestPoint[150], highestPoint[150];
	
	int i = 0, numCommands = 10, minTimeIndex = 0, maxTimeIndex = 0;
	long maxTime = 0, minTime = LONG_MAX, yMargin;

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
	sprintf (xRangeCommand, "set xrange [%d:%d]", 0, n+1);
	sprintf (yRangeCommand, "set yrange [%ld:%ld]", minTime - yMargin, maxTime + yMargin);
	sprintf (lowestPoint, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb '#007A00' fillstyle solid noborder",
	         minTimeIndex+1, minTime);
	sprintf (highestPoint, "set object circle at first %d,%ld radius char 1.5 fillcolor rgb 'red' fillstyle solid noborder",
	         maxTimeIndex+1, maxTime);
	
	if(gnuplotPipe != NULL && tempFile != NULL){
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
	}
	return 0;
}

void testGNUPLOT(){
	char * commandsForGnuplot[] = {
		"set terminal png large size 1920, 1080",
		"set output \"printme.png\"",
		"set xrange [0:6]",
		"set yrange [0:1500]",
		"set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5", //blue
		"set style line 2 lc rgb '#dd181f' lt 1 lw 2 pt 7 ps 1.5", //red
		"set style line 3 lc rgb '#33CC33' lt 1 lw 2 pt 7 ps 1.5", //green
		"set title \"Curva de aceleracion\"",
		"set xlabel \"Cantidad de hilos\"",
		"set ylabel \"Tiempo en milisegundos\"",
		"unset key",
		"plot 'data.temp' index 0 with linespoints ls 1, '' index 1 with linespoints ls 2, '' index 2 with linespoints ls 3 "};
	int numPoints = 6;
	int numCommands = 12;
	
    long xvals[6] = {1,    5 ,    1,   5,   1,  5};
    long yvals[6] = {1000, 1000,  700, 700, 600, 600};
    FILE * temp = fopen("data.temp", "w");
    /*Opens an interface that one can use to send commands as if they were typing into the
     *     gnuplot command line.  "The -persistent" keeps the plot open even after your
     *     C program terminates.
     */
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    int i;
    for (i=0; i < numPoints; i++){
		fprintf(temp, "%ld %ld \n", xvals[i], yvals[i]); //Write the data to a temporary file
		if( (i+1) % 2 == 0)
			fprintf(temp, "\n\n"); //Write the data to a temporary file
    }
	
    for (i=0; i < numCommands; i++){
		fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }

	fclose(temp);
	pclose(gnuplotPipe);
}




