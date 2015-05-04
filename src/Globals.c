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
int valueOfBase(char base){
	return base == 'A' ? 0 : base == 'C'? 1 : base == 'G'? 2 : 3;
}

//*16 + *4 + *1 = INDEX
char codonToOneLetter(char codon[3]){
	return AMINO_ACIDS[(16 * valueOfBase(codon[0])) + (4 * valueOfBase(codon[1])) + valueOfBase(codon[0]) ];
}




