#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;



void adjust_length(double **motifs_table, int **table, int motif_num, int &starting_motif_length, int bases_motif, double **background_frequency_table, int *motif_start, double *motif_scores_table, double *final_scores, double *total_score, double final_best, int go){

	//trying to see if length adjustment will improve score
	
	int adjustment=0;
	
	adjustment = (-4)+rand()/((double)RAND_MAX+1.0)*(4-(-4)+1);
	starting_motif_length = starting_motif_length+adjustment;
	
	//cout << "\n\nMotif length is " << starting_motif_length;
	//cout<< "\nAdjustment " << adjustment;
	
	int new_end[50];
	
	//setting up new motifs table
   	double **new_motifs_table = new double*[motif_num];
	for (int i=0; i<motif_num; i++){
		new_motifs_table[i]=new double[80];
	}
		
	for(int i=0; i<motif_num; i++){
		new_end[i]= (motif_start[i]+adjustment);
	}
	
	for(int i=0; i<motif_num; i++){
		if((new_end[i]+starting_motif_length)> bases_motif){
		//	cout<< "\n OVER MAX old start " << motif_start[i] << " new end " << (motif_start[i]+starting_motif_length) << " adjusted end " << (new_end[i]+starting_motif_length);
			motif_start[i]=bases_motif-starting_motif_length-adjustment;
			new_end[i]=bases_motif-starting_motif_length;
		//	cout<< "\n\nFIXXX";
		}
		if(new_end[i]<0){
	//		cout<< "\n UNDER MIN old start " << motif_start[i] << " new end " << (motif_start[i]+starting_motif_length) << " adjusted end " << (new_end[i]+starting_motif_length);
			motif_start[i]=(0+(abs(adjustment)));
			new_end[i]=0;

		}
	//	cout<< "\n old start " << motif_start[i] << " new end " << (motif_start[i]+starting_motif_length) << " adjusted end " << (new_end[i]+starting_motif_length);
	}
	
	for(int i=0; i<motif_num; i++){
		int new_start = motif_start[i];
		for(int j=0; j<starting_motif_length; j++){
	
		new_motifs_table[i][j]=table[i][new_start];
		new_start++;
		}
	}
	
	double new_motif_scores_table[motif_num] ={0};
	double new_score_num=0;
	
	double new_score[motif_num]={0};
	
	
	
}


void scanning(int iteration, int bases_motif, int &starting_motif_length, int **table, double **scores, double **motifs_table, int *motif_start, int motif_num, double *motif_scores_table, double *final_scores, double *total_score){
	
		
		char foundMotif[starting_motif_length];
		for(int a=0; a<(bases_motif); ++a){
		double motifScore = 0.00;

			if(a == bases_motif){
				break;
			}
			int i = 0;
			for(int b = a; b < (a + starting_motif_length); ++b){
				if(table[iteration][b]==0 && a<=bases_motif){							//change backgroundArray to table[iteration][b]
					foundMotif[i]='A';
					motifScore+=scores[i][0];
					i++;
				}else if(table[iteration][b]==1 && a<=bases_motif){
					foundMotif[i]='C';
					motifScore+=scores[i][1];
					i++;
				}else if(table[iteration][b]==2 && a<=bases_motif){
					foundMotif[i]='G';
					motifScore+=scores[i][2];
					i++;
				}else if(table[iteration][b]==3 && a<=bases_motif){
					foundMotif[i]='T';
					motifScore+=scores[i][3];
					i++;
				}
			}
			
			motif_scores_table[a]=motifScore;
			final_scores[a]=motif_scores_table[a];
		
		motifScore=0;	
			
	}	
	

	int I;
	double R;
    for (I =0;I<=bases_motif-starting_motif_length+1;++I)    {
      motif_scores_table[I]=exp2(motif_scores_table[I]);          									 //switching to ratio of target to background probabilities instead of the log of the ratio, which is the standard PSSM score.
    };
    for (I=1;I<=bases_motif-starting_motif_length+1;++I)    {
      motif_scores_table[I]+=motif_scores_table[I-1];
    };  																	//at this point, Score[I] is not the score at position I but sum of all scores between 0 and I
    R=drand48()*motif_scores_table[bases_motif-starting_motif_length+1];  								//drand48 returns a random double between 0 and 1. 
    I=0;
    while (R>=motif_scores_table[I] && I<bases_motif-starting_motif_length+1)    ++I;  					//choosing the new location of the motif, I
    motif_start[iteration]=I;  														// store I as the new location of the motif in sequence II	
	
	total_score[iteration] = final_scores[I];
	
		
		int c=0;
		int start =motif_start[iteration];
		for(int i=start; i<(start+(starting_motif_length+1)); i++){
			motifs_table[iteration][c]=table[iteration][i];
			c++;
		}
		
}

void run_gibbs(double **motifs_table, int **table, int motif_num, int &starting_motif_length, int bases_motif, double **background_frequency_table, int *motif_start, double *motif_scores_table, double *final_scores, double *total_score, int go, double final_best){
	

	if(go%5==0){
		//start adjusting function
	adjust_length(motifs_table, table, motif_num, starting_motif_length, bases_motif, background_frequency_table, motif_start, motif_scores_table, final_scores, total_score, final_best, go);
	}
		
	double **frequency_table = new double*[starting_motif_length];
	for(int i=0; i<starting_motif_length; i++){
		frequency_table[i]=new double[4];
	}
	
	//setting up scores matrix
   	double **scores = new double*[starting_motif_length];
	for (int i=0; i<starting_motif_length; i++){
		scores[i]=new double[4];
	}
	
	int motif_to_test =0;

	for(int i=0; i<motif_num; i++){		//run through each motif
		for(int b=0; b<starting_motif_length; b++){
			for(int c=0; c<4; c++){
				frequency_table[b][c]=0;
			}
		}
		
		for(int j=0; j<motif_num; j++){
			for(int a=0; a<starting_motif_length; a++){
				if(i==j){
					continue;
				}
				if(motifs_table[j][a]==0){
					frequency_table[a][0]+=1;	
				}else if(motifs_table[j][a]==1){
					frequency_table[a][1]+=1;	
				}else if(motifs_table[j][a]==2){
					frequency_table[a][2]+=1;	
				}else if(motifs_table[j][a]==3){
					frequency_table[a][3]+=1;	
				}
			}
			

		}		
			
		for(int b=0; b<starting_motif_length; b++){
				for(int c=0; c<4; c++){
					frequency_table[b][c]=(frequency_table[b][c]+.25);
					frequency_table[b][c]=(frequency_table[b][c]/(motif_num));
				}
			}
					
		
		for(int b=0; b<starting_motif_length; b++){
			for(int j=0; j<4; j++){
				if(j==0){
					scores[b][j]=log2(frequency_table[b][j]/background_frequency_table[i][0]);
				}else if(j==1){
					scores[b][j]=log2(frequency_table[b][j]/background_frequency_table[i][1]);
				}else if(j==2){
					scores[b][j]=log2(frequency_table[b][j]/background_frequency_table[i][2]);
				}else if(j==3){
					scores[b][j]=log2(frequency_table[b][j]/background_frequency_table[i][3]);
				}
			}	
		}
					
		scanning(i, bases_motif, starting_motif_length, table, scores, motifs_table, motif_start, motif_num, motif_scores_table, final_scores, total_score);
		
		
	}

}

void random_selection(int start_location, int end_location, int **table, double **motifs_table, int motif_num, int bases_motif, int &starting_motif_length, int *motif_start){
	
	srand(time(NULL));
	for(int i=0; i<motif_num; i++){
		int random_start=0;
		int b=0;
		random_start= start_location+rand()/((double)RAND_MAX+1.0)*(end_location-start_location+1);
		//cout<< "\nstarting nucleotide is " << random_start << "\tlast nucleotide is " << (random_start+starting_motif_length)<< "\n";
		motif_start[i]=random_start;
		for(int j=random_start; j<(random_start+starting_motif_length); j++){
			//cout<<table[i][j];
			motifs_table[i][b] = table[i][j];
			b++;
		}
	}
	
}

int main(int argc, char* argv[]){
	
	
	
	ifstream motifs(argv[1]);


	string line;
	int motif_num=0;	//number of motifs
	int total_count=0;	//total amount of letters
	int bases_motif=0;
	char c;
	bool header=0;
	int max_motif_length = 80;
	int starting_motif_length= std::stoi(argv[2]);

	int start = starting_motif_length;
	
	while(motifs.get(c)){
    	if(c == '>'){
        	++motif_num;
    	}
	}
	
	//reset file back to 0
	motifs.clear();
	motifs.seekg(0);		
    
	while(motifs.get(c)){
        if(c == '>'){
            header = 1;
        }else if(header == 1 && c == '\n'){
            header = 0;
        }else if(c == '\n'){
            continue;
        }else if(header == 0){
			total_count++;
			}
    }
    
    bases_motif= (total_count/motif_num);

	cout<< "\nThere are " << motif_num << " motifs";
	cout<< "\nThere are a total of " << bases_motif << " bases per motif";
	cout<< "\nThe initial length is " << starting_motif_length;
	
	//reset file back to 0
	motifs.clear();
	motifs.seekg(0);
	
	
	//pointer to a pointer
   	int **table = new int*[motif_num];
	for (int i=0; i<motif_num; i++){
		table[i]=new int[bases_motif];
	}
	
 	for(int i=0; i<motif_num; i++){
		for(int j=0; j<bases_motif; j++){
			table[i][j]=0;
		}
	}
	
	int base_num=0;
	int motif_row=0;
	
	//pointer to a pointer
   	double **background_frequency_table = new double*[motif_num];
	for (int i=0; i<motif_num; i++){
		background_frequency_table[i]=new double[4];
	}
	
	int background_row=0;
	
    while(motifs.get(c)){
        if(c == '>'){
            header = 1;
           // base_num=0;
        }else if(header == 1 && c == '\n'){
            header = 0;
        }else if(c == '\n'){
            continue;
        }else if(header == 0){
            if(c=='A'){
            	table[motif_row][base_num]=0;
            	base_num++;
            	background_frequency_table[motif_row][0]+=1;
            	if(base_num==bases_motif){
            		motif_row++;
            		base_num=0;
				}
			}else if(c=='C'){
            	table[motif_row][base_num]=1;
            	base_num++;
            	background_frequency_table[motif_row][1]+=1;
            	if(base_num==bases_motif){
            		motif_row++;
            		base_num=0;
				}
			}else if(c=='G'){
            	table[motif_row][base_num]=2;
            	base_num++;
            	background_frequency_table[motif_row][2]+=1;
            	if(base_num==bases_motif){
            		motif_row++;
            		base_num=0;
				}
			}else if(c=='T'){
            	table[motif_row][base_num]=3;
            	base_num++;
            	background_frequency_table[motif_row][3]+=1;
            	if(base_num==bases_motif){
            		motif_row++;
            		base_num=0;
				}
			}else{
            	table[motif_row][base_num]=4;
            	base_num++;
            	    if(base_num==bases_motif){
            		motif_row++;
            		base_num=0;
				}
			}
        }
    }
    
 	for(int i=0; i<motif_num; i++){
	//	cout<< "\n";
		for(int j=0; j<bases_motif; j++){
		//	cout<< table[i][j]<< " ";
		}
	} 
	
	
	//cout<< "\n\nBackground frequencies of motifs\n\t\tA\tC\tG\tT";
	for(int i=0; i<motif_num; i++){
		//cout << "\nMotif #"<<i << "\t";
		for(int j=0; j<4; j++){
			background_frequency_table[i][j]=(background_frequency_table[i][j]/bases_motif);
			//cout<< background_frequency_table[i][j]<< "\t";
		}
		
	}
	
	
	//setting up motifs table
   	double **motifs_table = new double*[motif_row];
	for (int i=0; i<motif_row; i++){
		motifs_table[i]=new double[max_motif_length];
	}
	
	//setting up motifs table
   	double **old_motifs_table = new double*[motif_row];
	for (int i=0; i<motif_row; i++){
		old_motifs_table[i]=new double[max_motif_length];
	}
	
	//setting up motifs table
   	double **best_motifs_table = new double*[motif_row];
	for (int i=0; i<motif_row; i++){
		best_motifs_table[i]=new double[max_motif_length];
	}
	
	int old_motif_starts[motif_row];
	int best_motif_starts[motif_row];
	
	int motif_start[motif_row];
	int best_motif_length;
	
	int random_start_position= bases_motif-starting_motif_length;
	double final_best=0;
	
	for(int p=0; p<200; p++){
	
	starting_motif_length =start;
	
//	cout<< "\n\nset initial points\n";
	random_selection(0, random_start_position,table, motifs_table, motif_num, bases_motif, starting_motif_length, motif_start);
	
	//cout<< "\n\nDisplay stats\n";
	for(int i=0; i<motif_row; i++){
	//	cout<< "\nStarting nucleotide of motif " << i << " :" << motif_start[i]<< "\n";
		for(int j=0; j<starting_motif_length; j++){
			//cout<< motifs_table[i][j];
		}
	}
	
	double final_scores[bases_motif];
	double motif_scores_table[bases_motif];		
	double total_score[motif_num];	
	double best_score=0;
		

	for(int run=0; run<300; run++){
		
	int old_starting_motif_length= starting_motif_length;	
	int go=0;
	run_gibbs(motifs_table, table, motif_num, starting_motif_length, bases_motif, background_frequency_table, motif_start, motif_scores_table, final_scores, total_score, go, final_best);
	
	double final_score=0;
		
		for(int i=0; i<motif_num; i++){
		//cout<< "\nscore "<< total_score[i];
		final_score+= total_score[i];
		}
		
		//cout << "\nScore for the run is " << final_score;
		
		if(final_score<best_score && (go%5==0)){
			starting_motif_length=old_starting_motif_length;
			for(int i=0; i<motif_num; i++){
				for(int j=0; j<starting_motif_length; j++){
					motifs_table[i][j]=old_motifs_table[i][j];
				}
				motif_start[i]=old_motif_starts[i];
			}
			
		}
		if(final_score>best_score){
			//cout<< "\nIteration" <<run <<"\n";

			best_score=final_score;
		//cout<< "\nUpdated best score is "<< best_score;
			
				/*
				for(int i=0; i<motif_num; i++){
					//cout << "\nUpdated motif position is for motif "<< i << " is: "<< motif_start[i]; 
				}*/
				for(int i=0; i<motif_num; i++){
				//	cout<< "\nMotif "<< i << " sequence ";
				/*
				for(int j=0; j<starting_motif_length; j++){
					if(motifs_table[i][j]==0){							//change backgroundArray to table[iteration][b]
					//	cout<<'A';
					}else if(motifs_table[i][j]==1){
						//cout<<'C';
					}else if(motifs_table[i][j]==2){
						//cout<<'G';
					}else if(motifs_table[i][j]==3){
						//cout<<'T';
					}
				}
				*/
				for(int i=0; i<motif_num; i++){
					old_motif_starts[i]=motif_start[i];
				}
				for(int j=0; j<starting_motif_length; j++){
					old_motifs_table[i][j]=motifs_table[i][j];
				}
			}
			
	
		}
		
		go++;
	}
	
	if(best_score>final_best){
		final_best=best_score;
		//cout<<"\nThe final score is " << final_best << "\n\nDone";
		
		best_motif_length=starting_motif_length;
		for(int i=0; i<motif_num; i++){
			for(int j=0; j<starting_motif_length; j++){
				best_motifs_table[i][j]=motifs_table[i][j];
			}
			best_motif_starts[i]=motif_start[i];
		}
		
	}
	
	}
	cout<< "\nThe final score is " << final_best << "\n\nSequences and Locations";
	cout<< "\nThe best motif length is "<< best_motif_length;
	for(int i=0; i<motif_num; i++){
		cout<< "\nMotif  "<< i+1 << " start " << best_motif_starts[i]+1 << "\tend " << best_motif_starts[i]+best_motif_length << "\tsequence\t";
		for(int j=0; j<best_motif_length; j++){
			if(best_motifs_table[i][j]==0){							//change backgroundArray to table[iteration][b]
				cout<<'A';
			}else if(best_motifs_table[i][j]==1){
				cout<<'C';
			}else if(best_motifs_table[i][j]==2){
				cout<<'G';
			}else if(best_motifs_table[i][j]==3){
				cout<<'T';
			}
		}
		
	
	}
}
