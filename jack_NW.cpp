#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;


int maxtrace(int score1, int score2, int score3)
{
	if(score1>score2 && score1>score3){
		return score1 ;
    }else if(score2>score1 && score2>score3){
		return score2 ;
    }else {
        return score3 ;
    }
}

int max(int mismatch, int gap1, int gap2)
{
	if(mismatch>gap1 && mismatch>gap2){
		return mismatch ;
    }else if(gap1>mismatch && gap1>gap2){
		return gap1 ;
    }else {
        return gap2 ;
    }
}

int maxmatch(int match, int gap1, int gap2)
{
	if(match>gap1 && match>gap2){
		return match;
	}else if(gap1>match && gap1>gap2){
		return gap1;
	}else{
		return gap2;
	}
	
}

	//command line arguments
int main(int argc, char* argv[]){
     
	 //sequences and character counts   
	int seq_count = 0;
    int char_count_1 = 0;
    int char_count_2 = 0;
	char c;
    bool header=0;	

	//before
    cout << "seq count before: " << seq_count << '\n';
	
	//read first file. Find char count and sequence count
    ifstream infile;
    infile.open(argv[1]);
    while(infile.get(c)){
        if(c == '>'){
            ++seq_count;
            header = 1;
        }else if(header == 1 && c == '\n'){
            header = 0;
        }else if(c == '\n'){
            continue;
        }else if(header == 0){
            ++char_count_1;
        }
    }
	
	//read second file. Find char count and sequence count
    ifstream inputfile;
    inputfile.open(argv[2]);
    while(inputfile.get(c)){
        if(c == '>'){
            ++seq_count;
            header = 1;
        }else if(header == 1 && c == '\n'){
            header = 0;
        }else if(c == '\n'){
            continue;
        }else if(header == 0){
            ++char_count_2;
        }
    }
	
	//file with higher count is the size of our arrays
    int max_count=0;
    if(char_count_1 > char_count_2){
        max_count = char_count_1;
    }else{
        max_count = char_count_2;
    }
	
	
	//list statistics
    std::cout << "seq count after:  "<< seq_count << '\n';
    std::cout << "char count 1:\t  "<< char_count_1 << '\n';
	cout << "char count 2:\t  " << char_count_2 << '\n';
    std::cout << "Max char count:\t  " << max_count << "\n\n\n";

	//reset first file
	infile.clear();
	infile.seekg(0);
	
	//arrays for characters
	char characters1[char_count_1];
	char characters2[char_count_2];
	
	int num=0;
	
	//characters to an array
    while(infile.get(c)){
        if(c == '>'){
            ++seq_count;
            header = 1;
        }else if(header == 1 && c == '\n'){
            header = 0;
        }else if(c == '\n'){
            continue;
        }else if(header == 0){
        	//charcount1[num]= toupper(c); //get uppercase
        	characters1[num]= toupper(c); //get uppercase
        	num++;
        }
    }	
    
    //close file
	infile.close();
	
	//reset second file
	inputfile.clear();
	inputfile.seekg(0);	
	int nums =0;
    while(inputfile.get(c)){
        if(c == '>'){
            ++seq_count;
            header = 1;
        }else if(header == 1 && c == '\n'){
            header = 0;
        }else if(c == '\n'){
            continue;
        }else if(header == 0){
        	//charcount2[nums]= toupper(c); //get uppercase
        	characters2[nums]= toupper(c); //get uppercase
        	nums++;
        }
    }

	//close file
	inputfile.close();

	cout<< "\n\nSequence1\n";
	for(int i=0; i<char_count_1; ++i){
		cout << characters1[i];
	}    
	cout<< "\n\nSequence2\n";
	for(int i=0; i<char_count_2; ++i){
		cout << characters2[i];
	}  
    cout << endl << endl << "Score array\n";
    
    //command line input for match, mismatch, and gap
    int match = std::stoi(argv[3]);
    int mismatch = std::stoi(argv[4]);
    int gap = std::stoi(argv[5]); 
    
    int size1= char_count_1 +1;
    int size2= char_count_2 +1;
    
    string horizontal[size1];
    string vertical[size2];
   
   
   	int **table = new int*[size1]; //an array of pointers 
	for (int i=0; i<size1; i++)
	{
		table[i]=new int[size2]; ///1d array of cols
	}
    
	//initialize array
	for (int i = 0; i < size1; i++)	{
		for (int j = 0; j < 1; j++)
			table[i][j] = 0;
	}
	for (int i = 0; i <1; i++)	{
		for (int j = 0; j < size2; j++)
			table[i][j] = 0;
	}
	//gap scores
	int gap1=0;
	for (int i = 0; i < size1; i++)	{
		for (int j = 0; j < 1; j++)
			table[i][j] = table[i][j]+gap1;
			gap1+=gap;
	}
	//gap scores
	int gap2 =0;
	for (int i = 0; i < 1; i++)	{
		for (int j = 0; j < size2; j++){
			table[i][j] = table[i][j]+gap2;
			gap2+=gap;
		}
		}
	
	//fill out the scores
	int mismatch_trial, gap1_trial, gap2_trial, match_trial;
	
	for(int i=1; i<size1; i++){
		for(int j=1; j<size2; j++){
			if(characters1[i-1]==characters2[j-1]){
				match_trial = table[i-1][j-1]+match;
				gap1_trial= table[i-1][j]+gap;
				gap2_trial= table[i][j-1]+gap;			
				table[i][j]= maxmatch(match_trial, gap1_trial, gap2_trial);
			}
			else{
				mismatch_trial = table[i-1][j-1]+mismatch;
				gap1_trial= table[i-1][j]+gap;
				gap2_trial= table[i][j-1]+gap;
				table[i][j]= max(mismatch_trial, gap1_trial, gap2_trial);
			}
			}
		}
		/*
		//display characters			//uncomment here to see the score matrix
for(int i = 0; i < size1; ++i){
	for(int j = 0; j < size2; ++j){
        std::cout << table[i][j] <<"\t";
    }
    cout << endl;
    }*/

   	//list the score
	int score =table[size1-1][size2-1];
	cout << "The NW score is " << score << endl;
	
	//total length of output
	int tsize= char_count_1 +char_count_2;
	
	int a=0;
	string aligned1[tsize];
	string aligned2[tsize];
	
	int b= char_count_1 -1;
	int d= char_count_2 -1;
	string dash= "-";
	
	int tracecount =0;
	//traceback
	int i= size1-1;
	int g= size2-1;
	while(i>0 || g > 0){
		
			int score1=table[i][g];
			int score2 =table[i-1][g]+(gap);
			int score3 =table[i][g-1]+(gap);
			int score4= table[i-1][g-1]+(match);
			
			//if gap 
			if(score1==score2){
				aligned1[a]=characters1[b];
				aligned2[a]=dash;
				b--;
				a++;
				i--;
			}
			
			//if gap
			else if(score1==score3){
				aligned1[a]=dash;
				aligned2[a]=characters2[d];
				d--;
				a++;	
				g--;
			}
			
			//if diagonal
			else{
				aligned1[a]=characters1[b];
				aligned2[a]=characters2[d];
				a++;
				b--;
				d--;
				i--;
				g--;	
			}
	++tracecount;
	}

	//display sequences
	cout<< "\nFirst sequence\n";
	for(int i=tsize-1; i>=0; i--){
		cout << aligned1[i];
	}
	
	cout<< "\n\n\nSecond sequence\n";
	for(int i=tsize-1; i>=0; i--){
		cout << aligned2[i];
	}
	
	//displaying sequences
	int j=0;
	int s=0;
	string forward2[tracecount];
	string forward1[tracecount];
	cout<< "\n\n";
	for(int i=tracecount-1; i>=0; i--){
		if(aligned2[i] != ""){
            forward2[j]=aligned2[i];
		    j++;
        }
	}
	cout<< "\n\n";
	for(int i=tracecount-1; i>=0; i--){
        if(aligned1[i] != ""){
		    forward1[s]=aligned1[i];
		    s++;
        }
	}
	int end1=0;
	int p=0;
	int r=0;
	cout<< "\n\n";
    
	for(int p=0; p<(tracecount-1)-59; p+=60){
		for(int r=p; r<p+60; ++r){
			cout <<forward1[r];
		}
		cout<< "\n";
		for(int r=p;r<p+60; ++r){
			cout<< forward2[r];
		}
		cout<< "\n\n";
		end1=p;
    }	

	int newend1 = (end1+60);
	int newend2 = (end1+60);

	for(newend1; newend1<tracecount;newend1++){
		cout <<forward1[newend1];
	}
	cout<< "\n";
	for(newend2; newend2<tracecount;newend2++){
		cout <<forward2[newend2];
	}
	cout<<"\n";
	
	//do output if less than 60 characters in strings
	if(tracecount<60){
		for(int i=0; i<tracecount; i++){
			cout<<forward1[i];
		}
		cout<< "\n";
		for(int j=0; j<tracecount; j++)
			cout<<forward2[j];
		}
		
	cout<<"\n\n";
}
