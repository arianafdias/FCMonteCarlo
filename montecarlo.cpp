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

int monteCarlo(float miuTotal, unsigned int L = 10000 , string filename = "feixeMonocromatico.txt"){
	//lei de atenuação    -> I = I0 e^(-miuTotal*x)
	ofstream File(filename);
	ofstream teste("teste.txt");
	vector<unsigned int> s(L,0); //a espessura total está dividada em L espaços/bins
	vector<double> X(1000000,0); // lista dos valores de x 
	float dx = pow(10,-7);
	
	for(int i = 0; i<1000000; i++){
	
		double x = random_nr(miuTotal); // espessura que o fotão atravessa, é gerada aleatoriamente
		
		teste << x << endl;
		
		
		int bin = (int) (x/dx); 
		if (bin > L-1){
			s[L-1]++;
		
		}else{
			s[bin]++;
		}
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
		File <<log((double)  ss[i] /1000000) << endl;
		
	}
	
	//espessura para a qual a intensidade do feixe passa para metade
	
	int i = 0;
	int sum = 0;
	while( sum <= 500000){
		sum = sum + s[i];
			i++;
		}
	cout << (double) dx*i << "cm é a espessura para a qual a intensidade do feixe para para metade! (3.1)"<< endl;
	
	teste.close();
	File.close();
	return 0;
	
	

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

vector<double> NIST(double rho){
	vector <string> filedata;
	ifstream nist;
	nist.open("neves.txt");
	
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
		miuT.push_back((miuR[i]+miuC[i]+miuF[i])*rho);  // cm^-1
	}
	
	return miuT;
	
	}


int ex32(vector<double> vec, double rho){
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
	vector<double> miuT = NIST(rho);
	
	int L = 100000;
	vector<unsigned int> s(L, 0);
	vector<double> X(1000000, 0);
	float dx = pow(10, -4);
	
	//ofstream testefile ("TesteNEW.txt");
	
	for (int i = 0; i < 1000000; i++){
		double y = (double) drand48(); // gerar random nr
		bool flag = false;
		int j = 0;
		
		int bin;		
		
		while( j!= cdfnorm.size() && flag != true){ 
			if(y < cdfnorm[j]){
				if(j == 0){
				bin = 0;
				}else{
				bin = j-1;
				}
				flag = true;
			}
			j++;
		}// -----> vê em que bin fica o fotao
		
		int energia1 = bin;
		double miuTassociadoE1 = miuT[energia1]; // cm^-1
		
		//montecarlo
		double x = -log(1-drand48())/miuTassociadoE1;
		
		int idx = x/dx;  // vê em que indice/bin se encontra o fotao
				
		if (idx > L -1){
			s[L-1]++;
		} else {
			s[idx]++;
		}
	}
	
	// grafico	
	ofstream fpoli("FeixePolicromatico.txt");
		
	vector<int> ss(L,0);
	for(int i = 0; i< L; i++){
		if(i==0){
			ss[i] = 1000000 -s[i];
		} else {
			ss[i] = ss[i-1]-s[i];
		}
		fpoli <<log((double)  ss[i] /1000000) << endl;
		
	}
		
	//espessura para a qual a intensidade do feixe passa para metade
	
	int i = 0;
	int sum = 0;
	while( sum <= 500000){
		sum = sum + s[i];
			i++;
		}
	cout << (double) i*dx << "cm é a espessura para a qual a intensidade do feixe para para metade! (3.2)"<< endl;
	
	fpoli.close();
	
	//linear regression
	
	int n;
	
	double x[L], y[L], sumX = 0, sumX2 = 0, sumY = 0, sumXY = 0, a, b;
	
	for(int i = 1; i <= L; i++){
		x[i] = i * dx;
		y[i] = log((double)  ss[i] /1000000);
	}
	
	for(int i=1;i<=n;i++){
		sumX = sumX + x[i];
  		sumX2 = sumX2 + x[i]*x[i];
  		sumY = sumY + y[i];
  		sumXY = sumXY + x[i]*y[i];
	}

	b = (n*sumXY-sumX*sumY)/(n*sumX2-sumX*sumX);
 	a = (sumY - b*sumX)/n;
	cout << a << "\t" << "-> declive (3.2)" << endl;
	cout << b << "\t" << "-> ordenada (3.2)" << endl;
	
	
	

	return 0;
	}



int main() {
	//data
	float miuF = 3.19* pow (10,3) ; //cm^-1
	float miuR = 6.09 ; //cm^-1
	float miuC = 3.83 * pow(10,-2); //cm^-1
	float miuTotal = miuF + miuR + miuC; //cm^-1
	
	//cout << miuTotal << endl;
	
	monteCarlo(miuTotal);
	double rho = 1.82;
	vector<double> vec = distribuicaoFeixePoli();
	//ex32(vec, rho);

    
    return 0;
    }
