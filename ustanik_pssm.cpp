#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char* argv[]){
	
	ifstream motif(argv[1]);
    //ifstream background;
    ifstream background(argv[2]);
	int minScore= std::stoi(argv[3]);


	
	int char_count=0;
	int countA=0;
	int countC=0;
	int countG=0;
	int countT=0;
	char b;
    bool header=0;
    
    while(background.get(b)){
        if(b == '>'){
            header = 1;
        }else if(header == 1 && b == '\n'){
            header = 0;
        }else if(b == '\n'){
            continue;
        }else if(header == 0){
            ++char_count;
            if(b=='A'){
            	countA++;
			}else if(b=='C'){
            	countC++;
			}else if(b=='G'){
				countG++;
			}else if(b=='T'){
				countT++;
			}
        }
    }
    
	//reset file back to 0
	background.clear();
	background.seekg(0);
	
	int backgroundCount=0;
	int otherCount=0;
	
	int*backgroundArray = new int [char_count];
	
    while(background.get(b)){
        if(b == '>'){
            header = 1;
        }else if(header == 1 && b == '\n'){
            header = 0;
        }else if(b == '\n'){
            continue;
        }else if(header == 0){
            if(b=='A'){
            	backgroundArray[backgroundCount]=0;
            	backgroundCount++;
			}else if(b=='C'){
            	backgroundArray[backgroundCount]=1;
            	backgroundCount++;
			}else if(b=='G'){
				backgroundArray[backgroundCount]=2;
				backgroundCount++;
			}else if(b=='T'){
				backgroundArray[backgroundCount]=3;
				backgroundCount++;
			}else{
				backgroundArray[backgroundCount]=4;
				backgroundCount++;
				otherCount++;
			}
        }
    }
    /*
    for(int i=0; i<char_count; i++){
    	cout<< i<< " "<< backgroundArray[i]<<" ";
	}
	*/
    
    double probA=0.000000;
	double probC=0.000000;
	double probG=0.000000;
	double probT=0.000000;
	
	probA= (countA)/(double)char_count;
	probC= (countC)/(double)char_count;
	probG= (countG)/(double)char_count;
	probT= (countT)/(double)char_count;
	
	cout<< "\nThe DNA sequence is "<< char_count << " bases long" << "\nThere are "<< countA << " As\nThere are " <<countC << " Cs\nThere are " << countG << " Gs\nThere are " << countT << " Ts\n" << "Other Count "<<otherCount;  
	cout<< "\nA probability is "<< probA << "\nC probability is "<< probC << "\nG probability is "<< probG << "\nT probability is "<< probT;
	string line;
	int motifRows=0;
	char c;
	int characters=0;
	int charsRow=0;
	
	while(getline(motif,line)){
		motifRows++;
	}
	
	//reset file back to 0
	motif.clear();
	motif.seekg(0);
	
	while(motif.get(c)){
    if(c != '\n'){
        ++characters;
    }
	}

	charsRow=(characters/motifRows);
	cout<< "\nThere are " << motifRows << " motifs\nThere are " << charsRow << " nucleotides per motif\n";
	
	//reset file back to 0
	motif.clear();
	motif.seekg(0);
	
	
	//pointer to a pointer
   	int **table = new int*[motifRows];
	for (int i=0; i<motifRows; i++){
		table[i]=new int[charsRow];
	}	

	int j=0;
	int i=0;
	while (motif.get(c)){
		if(c!='\n'){
			if(c=='a' or c=='A'){
				table[i][j]=0;
				j++;
			}
			else if(c=='c' or c=='C'){
				table[i][j]=1;
				j++;
			}
			if(c=='g' or c=='G'){
				table[i][j]=2;
				j++;
			}
			if(c=='t' or c=='T'){
				table[i][j]=3;
				j++;
			}			
		}
		else if(c=='\n'){
			i++;
			j=0;
		}
		
	}
	
	for(int i=0; i<motifRows; i++){
		cout<< "\n";
		for(int j=0; j<charsRow; j++){
			cout<< table[i][j]<< " ";
		}
	}
	
	//setting up frequency matrix
   	double **frequency = new double*[charsRow];
	for (int i=0; i<charsRow; i++){
		frequency[i]=new double[4];
	}	
	
	//initialize 
	for(int i=0; i<charsRow; i++){
		for(int j=0; j<4; j++){
			frequency[i][j]=0;
		}
	}	
	
	//nucleotides at each position
	cout<< "\n\n---------------------------------------------------------------------------------------------------\nNucleotides at each position\n\tA\tC\tG\tT";
	for(int i=0; i<motifRows; i++){
		for(int j=0; j<charsRow; j++){
			if(table[i][j]==0){
				frequency[j][0]+=1;	
			}else if(table[i][j]==1){
				frequency[j][1]+=1;	
			}else if(table[i][j]==2){
				frequency[j][2]+=1;	
			}else if(table[i][j]==3){
				frequency[j][3]+=1;	
			}
		}
	}
	
	//addint .25 to each value
	for(int i=0; i<charsRow; i++){
		for(int j=0; j<4; j++){
			frequency[i][j]=frequency[i][j]+.25;
		}
	}
	
	//display nucleotieds at each position
	for(int i=0; i<charsRow; i++){
		cout<< "\nBase #"<<i<<"\t";
		for(int j=0; j<4; j++){
			cout<< frequency[i][j]<< "\t";
		}
	}
	
	//find frequency
	for(int i=0; i<charsRow; i++){
		for(int j=0; j<4; j++){
			frequency[i][j]=((frequency[i][j])/(motifRows+1));
		}
	}
	
	cout<< "\n\n---------------------------------------------------------------------------------------------------\nFrequency Matrix\n";	
	for(int i=0; i<charsRow; i++){
		cout<< "\nBase #"<<i<<"\t";
		for(int j=0; j<4; j++){
			cout<<frequency[i][j]<< "\t";
		}
	}

	//setting up scores matrix
   	double **scores = new double*[charsRow];
	for (int i=0; i<charsRow; i++){
		scores[i]=new double[4];
	}

	for(int i=0; i<charsRow; i++){
		for(int j=0; j<4; j++){
			if(j==0){
				scores[i][j]=log2(frequency[i][j]/probA);
			}else if(j==1){
				scores[i][j]=log2(frequency[i][j]/probC);
			}else if(j==2){
				scores[i][j]=log2(frequency[i][j]/probG);
			}else if(j==3){
				scores[i][j]=log2(frequency[i][j]/probT);
			}
		}
	}
	
	cout<< "\n\n---------------------------------------------------------------------------------------------------\nScores Assigned Including Background model\n";	
	for(int i=0; i<charsRow; i++){
		cout<< "\nBase #"<<i<<"\t";
		for(int j=0; j<4; j++){
			cout<<scores[i][j]<< "\t";
		}
	}

	//setting up scores matrix
   	double **flippedScores = new double*[charsRow];
	for (int i=0; i<charsRow; i++){
		flippedScores[i]=new double[4];
	}
	
	int f=charsRow-1;
	for(int i=0; i<charsRow; i++){
			int t=3;
		for(int j=0; j<4; j++){
			flippedScores[f][t]=scores[i][j];
			t--;
		}
		f--;
	}
	
	cout<< "\n\n---------------------------------------------------------------------------------------------------\nFlipped scores array\n";	
	for(int i=0; i<charsRow; i++){
		cout<< "\nFlipped Base #"<<i<<"\t";
		for(int j=0; j<4; j++){
			cout<<flippedScores[i][j]<< "\t";
		}
	}
	char foundMotif[charsRow];
	//supervised motif finding

	int backgroundPosition=0;
	
	cout<< "\n\n---------------------------------------------------------------------------------------------------\nForward Strand\n";
	for(int a=0; a<(char_count); ++a){
		double motifScore = 0.00;

			if(a == char_count){
				break;
			}
			int i = 0;
			for(int b = a; b < (a + charsRow); ++b){
				if(backgroundArray[b]==0 && a<=char_count){
					foundMotif[i]='A';
					backgroundPosition=0;
					motifScore+=scores[i][0];
					i++;
				}else if(backgroundArray[b]==1 && a<=char_count){
					foundMotif[i]='C';
					backgroundPosition=1;
					motifScore+=scores[i][1];
					i++;
				}else if(backgroundArray[b]==2 && a<=char_count){
					foundMotif[i]='G';
					backgroundPosition=2;
					motifScore+=scores[i][2];
					i++;
				}else if(backgroundArray[b]==3 && a<=char_count){
					foundMotif[i]='T';
					backgroundPosition=3;
					motifScore+=scores[i][3];
					i++;
				}
			}
			
		if(motifScore>minScore){
			cout<<"\nForward Strand Score: "<<motifScore << '\t';
			cout<<"Motif Start is "<< a + 1<< "\tMotif End is "<<(a + charsRow)<< '\t';
			for(int p=0; p<charsRow; p++){
				cout<< foundMotif[p];
			}
		}
		motifScore=0;
			
			
	}
	
	cout<< "\n\n---------------------------------------------------------------------------------------------------\nReverse Strand\n";
	for(int a=0; a<(char_count); ++a){
		double motifScore = 0.00;

			if(a == char_count){
				break;
			}
			int i = 0;
			for(int b = a; b < (a + charsRow); ++b){
				if(backgroundArray[b]==0 && a<=char_count){
					foundMotif[i]='A';
					backgroundPosition=0;
					motifScore+=flippedScores[i][0];
					i++;
				}else if(backgroundArray[b]==1 && a<=char_count){
					foundMotif[i]='C';
					backgroundPosition=1;
					motifScore+=flippedScores[i][1];
					i++;
				}else if(backgroundArray[b]==2 && a<=char_count){
					foundMotif[i]='G';
					backgroundPosition=2;
					motifScore+=flippedScores[i][2];
					i++;
				}else if(backgroundArray[b]==3 && a<=char_count){
					foundMotif[i]='T';
					backgroundPosition=3;
					motifScore+=flippedScores[i][3];
					i++;
				}
			}
			
		if(motifScore>minScore){
			cout<<"\nReverse Strand Score: "<<motifScore << '\t';
			cout<<"Motif Start is "<< a + 1<< "\tMotif End is "<<(a + charsRow)<< '\t';
			for(int p=0; p<charsRow; p++){
				cout<< foundMotif[p];
			}
		}
		motifScore=0;
			
		
		
	}
	
	cout<< "\n\nDone";
}


