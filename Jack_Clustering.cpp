#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

//K-means algorithim
double Kmeans(int types, int numLabels, int numCents, double** table, double** centroidValues, int* bestCentroid, double *best_distance_array){
	
	double WCSS =0.0;
	double WCSS_New = 0.0;
	double sum[numCents][numLabels];
	for(int i = 0; i < numCents; ++i){
		for(int j = 0; j < numLabels; ++j){
			sum[i][j] = 0.00;
		}
	}
	int sumCount[numCents]={0};
	double euclideanDistance[types]; 

int counter = 0;
bool end = 0;			
while(end != 1){
	for(int i=0; i<types; ++i){
		int best_dist_indx = 0;
		double bestDistance=0.0;
		for(int a=0; a<numCents;a++){
			
			double sums = 0;
			for(int j=0; j<numLabels; ++j){
				sums += ((centroidValues[a][j]-table[i][j]) * (centroidValues[a][j]-table[i][j]));
			}
			double newDistance = sqrt(sums);
			if(a == 0){
				bestDistance = newDistance;
				best_dist_indx = 0;
			}else{
				if(newDistance < bestDistance){
					bestDistance = newDistance;
					best_dist_indx = a;
				}
			}	
		}
		bestCentroid[i] = best_dist_indx;
		best_distance_array[i] = bestDistance;
		euclideanDistance[i] = bestDistance;
	}
	
	for(int i=0; i<types; i++){
		WCSS_New+=((euclideanDistance[i])*(euclideanDistance[i]));
		sumCount[bestCentroid[i]]+=1;
	}
	
	//total the points per centroid point
	for(int i=0; i<types; i++){
		for(int j=0; j<numLabels;j++){
			if(bestCentroid[i] == 0){
				if(j == 12){
					//std::cout << "NAN" << '\t' << table[i][j] << '\n';
				}
			}
		sum[bestCentroid[i]][j]+=table[i][j];
		//std::cout << "SUMARRAy" << '\t' << sum[bestCentroid[i]][j] << '\n';
	}
	}
	
	//centroid values
	for(int i=0; i<numCents; i++){
		for(int j=0; j<numLabels; j++){
		centroidValues[i][j]=sum[i][j]/sumCount[i];
		}
	}
	
	//conditions for the program to run
	if(counter == 0){
		WCSS = WCSS_New;
	}else if(counter == 1000){
		end = 1;
	}else if(WCSS_New < WCSS){
		WCSS = WCSS_New;
	}else if(WCSS_New >= WCSS && counter > 9){
		end = 1;
	}
	++counter;
	WCSS_New = 0;
}
	//WCSS value
	return WCSS;
}

int main(int argc, char* argv[])
{     
	
	//open file
	ifstream infile(argv[1]);
	
	int numCent= std::stoi(argv[2]);
	int maxCent= std::stoi(argv[3]);
	
	string line;
	char c;
	string read;
	int count=0;
	int tabs =0;
	
	//count lines of file
	while(getline(infile, line)){
		count++;
	}
	
	cout << "The file is "<<count<< " lines long\n";
	
	//reset file back to 0
	infile.clear();
	infile.seekg(0);
	
	//count the number of tabs
	int num=0;
    while(infile.get(c)){
        if(c == '\t'){
            ++tabs;
    	}
	}
		
	//tabs per line
	int numLabels =0;
	numLabels = (tabs/count);
	
	int types=0;
	types= count-1;
	
	cout << "There are " <<types << " types of archaea in the file\n";
	cout << "Each Archaea and/or Bacteria contains " <<numLabels << " Amino Acids\n";
	
	//pointer to a pointer
   	double **table = new double*[types];
	for (int i=0; i<types; i++){
		table[i]=new double[numLabels];
	}
    
	infile.clear();
	infile.seekg(0);
	
	string head;
	
	//header file
	int headsize=1;
	int nums=0;
	string header[headsize];
	while(getline(infile,head)){
		header[nums]=head;
		break;  
	}
	
	int readLine =0;
	int tabcount=-1;
	string tempLabel = "";
	string linelabels[types];
	
	while(infile.get(c)){
		if(c =='\t'){
			if(tabcount== -1){
				//cout<< "2";
				linelabels[readLine]=tempLabel;
				tempLabel = "";
			}else if(tabcount >= 0){
				table[readLine][tabcount]= std::stod(tempLabel);
				tempLabel = "";
			}
			++tabcount;
		}else if(c=='\n'){
			table[readLine][tabcount]= std::stod(tempLabel);
			tempLabel = "";
			readLine++;
			tabcount=-1;
			//cout<<"4";
		}else{
			tempLabel+=c;
			//cout<<"5";
		}
	}
	
	cout<< "\n";
	//display values
	for(int i = 0; i < types; ++i){
		cout<<linelabels[i] << " ";
		for(int j = 0; j < numLabels; ++j){
        	std::cout << table[i][j] << " ";
    	}
    	cout << endl;
    }
	
	//data normilization
	double total[numLabels];
	
	//calculate total
	for(int i=0; i<types; ++i){
		for(int j=0; j<numLabels; ++j){
			total[j]+=table[i][j];
		}	
	}
	//calculate means
	for(int i=0; i<numLabels; ++i){
		total[i]=(total[i]/types);
		//cout<< total[i]<< "\n";
	}
	//calculate standard deviation
	double standardDeviation[numLabels];
	
	for(int i=0; i<numLabels; ++i){
		double variance=0;
		for(int j=0; j<types; ++j){
			double difference=0;
			difference= table[j][i]-total[i];
			variance+= difference*difference;
			if(j==types-1){
				variance=variance/(types-1);
				standardDeviation[i] =sqrt(variance);
			}
		}	
	}
	
	for(int i=0; i<numLabels; ++i){
		for(int j=0; j<types; ++j){
			table[j][i]=((table[j][i]-total[i])/(standardDeviation[i]));
		}
	}
		
	cout<< "\n\nDisplay normilization stats\n";
	for(int i=0; i<types; ++i){
		cout<< "\n";
		for(int j=0; j<numLabels; ++j){
			cout <<table[i][j] << " ";
		}
	}
	
	for(int x=numCent; x<(maxCent+1);x++){
	
   	int bestCentroids[types]={0};
   	int finalCentroid[types]={0};
	double best_distance_array[types];
	
	int trialAmount = 100;
	
   	double **centroidValues = new double*[x];
	for (int i=0; i<x; i++){
		centroidValues[i]=new double[numLabels];
	}
	double WCSS=999999;
	double newWCSS =0.0;
	
	srand(time(0));   // seeding the random number generator using local time measured in seconds. Make sure to do it only once.
	for(int i=0; i<trialAmount; ++i){
	//start random number generation
		for (int a=0; a<x; ++a){	
			int r = rand()%types;
			for( int j=0; j<numLabels; ++j){
			centroidValues[a][j] = table[r][j];
			}	
		}
	
		//function
		newWCSS = Kmeans(types, numLabels, x, table, centroidValues, bestCentroids, best_distance_array);
		
		if(i==0){
			WCSS=newWCSS;
		}
		else if(newWCSS<WCSS) {
			WCSS=newWCSS;
			for(int i=0; i<types; i++){
				finalCentroid[i]=bestCentroids[i];
			}
		}

	}
		//once loop is done AIC and BIC and WCSS dispayed
	
		double AIC=0;
		double BIC=0;
		ofstream outputFile;
		outputFile.open("Kmeans"+std::to_string(x)+".txt");
		outputFile<< "\n\n-------------------------------------------------------------------\nNumber of Centroids: "<< x<<"\n";
		for(int i=0; i<x; i++){
			outputFile<<"\nCluster # " << i+1 << "\n";
			for(int j=0; j<types; j++){
				if(finalCentroid[j]==i){
					outputFile<< linelabels[j] <<"\n";
				}
			}
		}
		
		cout<< "\n\nNumber of Centroids: "<< x;
		AIC=((2*(x)*(numLabels))+WCSS);
		BIC= (((log(types))*x*numLabels)+WCSS);
		cout << "\nThe WCSS is: "<< WCSS;
		cout << "\nThe AIC is: " <<AIC;
		cout << "\nThe BIC is: " <<BIC;
		
		outputFile << "\nThe WCSS is: "<< WCSS;
		outputFile << "\nThe AIC is: "<<AIC;
		outputFile << "\nThe BIC is: "<<BIC;
		
		outputFile.close();
	}
}
