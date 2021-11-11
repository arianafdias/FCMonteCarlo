#include <iostream>
#include <fstream>
#include <vector>
#include <math.h> 
#include <bits/stdc++.h>
#include <sstream>
#include <string.h>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <numeric>

using namespace std;

double random_nr(float miuTotal){
	double y = (double) drand48(); //intervalo de 0 a 1
	double x = -(log(1-y))/miuTotal; // intervalo 0 a infinito
    return x;
}

vector<double> monteCarlo(float miuTotal, unsigned int L = 10000 , string filename = "feixeMonocromatico.txt"){
	//lei de atenuação    -> I = I0 e^(-miuTotal*x)
	ofstream testeFile(filename);
	
	vector<unsigned int> s(L,0); //a espessura total está dividada em L espaços/bins
	vector<double> X(1000000,0); // lista dos valores de x 
	float dx = 1*pow(10,-5);
	
	for(int i = 0; i<1000000; i++){
	
		double x = random_nr(miuTotal); // espessura que o fotão atravessa, é gerada aleatoriamente
		
		int bin = (int) (x/dx); 
		//testeFile << bin << endl;
		if (bin > L-1){
			s[L-1]++;
		
		}else{
			s[bin]++;
		}
		//testeFile << x << endl;
		X[i] = x;	
	
	}
	
	
	
	//grafico
	vector<int> ss(L,0);
	for(int i = 0; i< L; i++){
		if(i==0){
			ss[i] = 1000000 -s[i];
		} else {
			ss[i] = ss[i-1]-s[i];
		}
		testeFile <<log((double)  ss[i] /1000000) << endl;
		//testeFile << s[i] << endl;
	}
	
	
	testeFile.close();
	return s;
	
	

}
vector<double> distribuicaoFeixePoli() {
	vector <string> filedata;
	ifstream myfile;
	myfile.open("110kV.spec");
	string myText;
	
	while(getline(myfile, myText)){
	filedata.push_back((myText));
	}
	vector<double> Energy;
	vector<double> N;
	for(int i = 0; i< filedata.size(); i++){
		string s = filedata[i] ;
		string::size_type sz;
		
		double energy = stod(s, &sz);
		double n = stod(s.substr(sz));
		Energy.push_back(energy*pow(10,3)); // ev
		N.push_back(n*pow(10,-2));			  // m^-2
	}
	

	return N;
	}

vector<double> NIST(){
	vector <string> filedata;
	ifstream nist;
	nist.open("NIST.txt");
	
	double Energy[100];
	double miuR[100];
	double miuC[100];
	double miuF[100];
	vector <double> miuT;
	

	int num = 0;
	while (!nist.eof()){
		nist >> Energy[num];
		nist >> miuR[num];
		nist >> miuC[num];
		nist >> miuF[num];
		num++;	
	}
	nist.close();
	
	for(int i=0; i<100; i++){
		miuT.push_back(miuR[i]+miuC[i]+miuF[i]);
	}
	
	return miuT;
	
	}


int ex32(vector<double> vec){
	 //ordenar as probabilidades
	vector<double> cdf;
	for(int i = 0; i<vec.size(); i++){
		if(!cdf.empty()){
		cdf.push_back(vec[i]+cdf.back());
		} else {
		cdf.push_back(vec[i]);
		}
	} 
	
	vector<double> cdfnorm;
	for (int i = 0; i < cdf.size(); i++){
	 cdfnorm.push_back(cdf[i]/cdf.back()) ;
	 }
	vector<double> miuT = NIST();
	
	for (int i = 0; i < 1000000; i++){
		double y = (double) drand48(); // gerar random nr
		bool flag = false;
		int j = 0;
		
		int bin;
		
		while( j!= cdfnorm.size() && flag != true){ 
			//cout << probs_ord[j] << " probabilidade" << endl;
			if(y < cdfnorm[j]){
				if(j == 0){
				bin = 0;
				}else{
				bin = j-1;
				}
				flag = true;
			}
			j++;
		}// -----> vê em que bin fica o eletrão
		
		int energia1 = bin;
		double miuTassociadoE1 = miuT[energia1];
		vector<double> distribuicaoFotoes = monteCarlo(miuTassociadoE1, 10000 ,"FeixePolicromatico.txt");
		
		
		//cout << j << endl;
		
		
		
		
		
	
	
	
	
	
	}  
	//    NIST file Z = 42
	
	
	
	
	
	
	

	return 0;
	}



int main() {
	//data
	float miuF = 3.19* pow (10,3)*pow(10,-2) ; //m^-1
	float miuR = 6.09 *pow(10,-2); //m^-1
	float miuC = 3.83 * pow(10,-2)*pow(10,-2); //m^-1
	float miuTotal = miuF + miuR + miuC; //m^-1
	float energiaFeixeMono = 1*pow(10,3); //eV
	
	//monteCarlo(miuTotal, energiaFeixeMono);
	
	vector<double> vec = distribuicaoFeixePoli();
	 ex32(vec);
	 //NIST();

    
    return 0;
    }
