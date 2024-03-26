#include <iostream>
#include <string>
#include <fstream>
using namespace std; 

//to better understand the swapping I used the APPDIVIDEND article on quicksort. Marked below are places where I used the article as reference

int Partition(string header [], string sequence[], string plus[],string quality[], int min, int max)
{
	string pivot = sequence[max];
	int p = min;
 
 	//swap values
 	for(int i = min; i<max; i++)
 	{
 		if(sequence[i]<pivot)
 	{		
 	 		//The article helped he understand this swapping technique used here
 			//sequence
 			string swapSequence = sequence[i];
 			sequence[i] = sequence[p];
 			sequence[p] = swapSequence;
 			
 			//header
 			string swapHeader = header[i];
 			header[i] = header[p];
 			header[p]= swapHeader;
 			
 			//plus
			string swapPlus = plus[i];
 			plus[i] = plus[p];
 			plus[p]= swapPlus;
 			
 			//quality
 			string swapQuality = quality[i];
 			quality[i] = quality[p];
 			quality[p]= swapQuality;
 			
 			p++;
 	}
 	}
 	//pivot
 	//sequence
 	//The article helped he understand this swapping technique used here
 	string swapSequence = sequence[max];	
 	sequence[max] = sequence[p];
 	sequence[p] = swapSequence;
 	
 	//header
 	string swapHeader = header[max];
 	header[max] = header[p];
 	header[p] = swapHeader;
 	
 	//plus
	string swapPlus = plus[max];
	plus[max] = plus[p];
 	plus[p]= swapPlus;
 	
 	//quality
	string swapQuality = quality[max];
 	quality[max] = quality[p];
 	quality[p]= swapQuality;
 	
 	//return index
 	return p;
}

void sort(string header[], string sequence[], string plus[], string quality[], int min, int max) //pass in arrays, min and max value
{
 	if(min<max)
 	{
 		//Partition function
 		int p = Partition(header, sequence, plus, quality, min, max); 
 		sort(header, sequence, plus, quality, min, (p-1));  
 		sort(header, sequence, plus, quality, (p+1), max); 
 	}
}	

//command arguments
int main(int argc, char* argv[])
{
	//open file
	ifstream infile(argv[1]);
	
	string line;
	int count=0;
	
	//count lines of file
	while(getline(infile, line))
	{
		count++;
	}
	
	//reset file back to 0
	infile.clear();
	infile.seekg(0);
	
	//total lines and size of 4 arrays	
	const int SIZE= count;
	int newsize = SIZE/4;
		
	int num =0;
	
	string*sequence= new string [newsize];
	string*header= new string [newsize];
	string*plus = new string [newsize];
	string*quality = new string [newsize];
	
	int split1=0;
	int split2=0;
	int split3=0;
	int split4=0;
	string lines;
	
	//split 4 arrays up
	while(getline(infile, lines))	
	{	
		if((num+4)%4==0)	//header
			{
			header[split1]=lines;
			split1++;
			}
		if((num+3)%4==0)
			{
			sequence[split2]=lines;	//sequence
			split2++;
			}
		
		if((num+2)%4==0)
			{
			plus[split3]=lines;		//plus
			split3++;
			}
				
		if((num+1)%4==0)
			{
			quality[split4]=lines;	//quality
			split4++;
			}
		num++;
	}
	
	infile.close();

	int mid= newsize/2;
	
	//ensure that the largest isnt last. Median of 3 method (Max, Middle, Minimum array elements)
	if((sequence[0]< sequence[mid]) and (sequence[mid]< sequence[newsize-1]))
	{	
		//sequence
 		string swapSequence = sequence[newsize-1];
 		sequence[newsize-1] = sequence[mid];
 		sequence[mid] = swapSequence;
 		
 		//header
 		string swapHeader = header[newsize-1];
 		header[newsize-1] = header[mid];
 		header[mid]= swapHeader;
 		
 		//plus
		string swapPlus = plus[newsize-1];
 		plus[newsize-1] = plus[mid];
 		plus[mid]= swapPlus;
 			
 		//quality
 		string swapQuality = quality[newsize-1];
 		quality[newsize-1] = quality[mid];
 		quality[mid]= swapQuality; 		
	}
	else if((sequence[0]> sequence[mid]) and (sequence[0]< sequence[newsize-1]))
	{
		//sequence
 		string swapSequence = sequence[newsize-1];
 		sequence[newsize-1] = sequence[0];
 		sequence[0] = swapSequence;
 			
		//header
 		string swapHeader = header[newsize-1];
 		header[newsize-1] = header[0];
 		header[0]= swapHeader;
 			
 		//plus
		string swapPlus = plus[newsize-1];
 		plus[newsize-1] = plus[0];
 		plus[0]= swapPlus;
 			
 		//quality
 		string swapQuality = quality[newsize-1];
 		quality[newsize-1] = quality[0];
 		quality[0]= swapQuality; 		
	}
	else if((sequence[0]> sequence[mid]) and (sequence[mid]> sequence[newsize-1]))
	{	
		//sequence
 		string swapSequence = sequence[newsize-1];
 		sequence[newsize-1] = sequence[mid];
 		sequence[mid] = swapSequence;
 		
 		//header
 		string swapHeader = header[newsize-1];
 		header[newsize-1] = header[mid];
 		header[mid]= swapHeader;
 			
 		//plus
		string swapPlus = plus[newsize-1];
 		plus[newsize-1] = plus[mid];
 		plus[mid]= swapPlus;
 			
 		//quality
 		
 		string swapQuality = quality[newsize-1];
 		quality[newsize-1] = quality[mid];
 		quality[mid]= swapQuality; 		
	}
	else if((sequence[0]< sequence[mid]) and (sequence[0]> sequence[newsize-1]))
	{	
		//sequence
 		string swapSequence = sequence[newsize-1];
 		sequence[newsize-1] = sequence[0];
 		sequence[0] = swapSequence;
 		
 		//header
 		string swapHeader = header[newsize-1];
 		header[newsize-1] = header[0];
 		header[0]= swapHeader;
 			
 		//plus
		string swapPlus = plus[newsize-1];
 		plus[newsize-1] = plus[0];
 		plus[0]= swapPlus;
 			
 		//quality
 		string swapQuality = quality[newsize-1];
 		quality[newsize-1] = quality[0];
 		quality[0]= swapQuality;
	}

	sort(header, sequence, plus, quality, 0, (newsize-1));  // quick sort called
 
 	cout<<"Sorted FASTQ file with " << newsize << " sequences\n\n";
 	for(int i=0;i<newsize;i++)
 	{
 		cout<<header[i]<<"\n";
		cout<<sequence[i]<<"\n";
		cout<<plus[i]<<"\n";
		cout<<quality[i]<<"\n";
 	}
 
 return 0;
}
