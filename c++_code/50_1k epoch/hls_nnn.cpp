#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

#define dim 50

long double w0[9][50],w1[50][50],w2[50][50],w3[50][50],w4[50][2],b0[50][1],b1[50][1],b2[50][1],b3[50][1],b4[2][1];
long double datamatrix[12374][9];
int labels[12374];




void assignment(vector< vector<long double> >  matrix,vector< vector<long double> > weigh0,
                vector< vector<long double> > weigh1,
                vector< vector<long double> > weigh2,
                vector< vector<long double> > weigh3,
                vector< vector<long double> > weigh4,
                vector< vector<long double> > bia0,
                vector< vector<long double> > bia1,
                vector< vector<long double> > bia2,
                vector< vector<long double> > bia3,
                vector< vector<long double> > bia4,
				vector<int> label ) {

	for(int i = 0; i<label.size(); i++) {
		labels[i]=label[i];
	}


	for(int i=0; i<matrix.size(); i++) {
		for(int j=0; j<matrix[i].size(); j++) {
			datamatrix[i][j] = matrix[i][j];
		}
	}


	for(int i = 0; i<weigh0.size(); i++) {
		for(int j=0; j<weigh0[i].size(); j++) {
			w0[i][j] = weigh0[i][j];
		}
	}

	for(int i = 0; i<weigh1.size(); i++) {
		for(int j=0; j<weigh1[i].size(); j++) {
			w1[i][j] = weigh1[i][j];
		}
	}

	for(int i = 0; i<weigh2.size(); i++) {
		for(int j=0; j<weigh2[i].size(); j++) {
			w2[i][j] = weigh2[i][j];
		}
	}

	for(int i = 0; i<weigh3.size(); i++) {
		for(int j=0; j<weigh3[i].size(); j++) {
			w3[i][j] = weigh3[i][j];
		}
	}
	
		for(int i = 0; i<weigh4.size(); i++) {
		for(int j=0; j<weigh4[i].size(); j++) {
			w4[i][j] = weigh4[i][j];
		}
	}


	for(int i = 0; i<bia0.size(); i++) {
		for(int j=0; j<bia0[i].size(); j++) {
			b0[i][j] = bia0[i][j];
		}
	}

	for(int i = 0; i<bia1.size(); i++) {
		for(int j=0; j<bia1[i].size(); j++) {
			b1[i][j] = bia1[i][j];
		}
	}

	for(int i = 0; i<bia2.size(); i++) {
		for(int j=0; j<bia2[i].size(); j++) {
			b2[i][j] = bia2[i][j];
		}
	}

	for(int i = 0; i<bia3.size(); i++) {
		for(int j=0; j<bia3[i].size(); j++) {
			b3[i][j] = bia3[i][j];
		}
	}
	
	
		for(int i = 0; i<bia4.size(); i++) {
		for(int j=0; j<bia4[i].size(); j++) {
			b4[i][j] = bia4[i][j];
		}
	}


	cout<<"assignment is done\n";

}


int  vectormatrix(int pp) {

	int col1 = 9;
	int row2 = 9;
	int col2 = dim;

	long double res0[dim];
	//ofstream myfile ("example.txt");





	for(int j= 0; j<col2; j++) {
		long double sum = 0;

		for(int k=0; k<col1; k++) {
			sum=sum+(w0[k][j]*datamatrix[pp][k]);
		}
		sum=sum+b0[j][0];
		res0[j] = sum;
		if (res0[j]<0) {
			res0[j]=0;
		}

		//myfile<<res0[j]<<"\t";

	}
	//myfile<<"\n";
	col1 = dim;
	row2 = dim;
	col2 = dim;

	long double res1[dim];



	for(int j= 0; j<col2; j++) {
		long double sum = 0;

		for(int k=0; k<col1; k++) {
			sum+=w1[k][j]*res0[k];
		}
		sum = sum+b1[j][0];
		res1[j] = sum;
		if (res1[j]<0) {
			res1[j]=0;
		}

		//myfile<<res1[j]<<"\t";

	}

	//myfile<<"\n";




//
	col1 = dim;
	row2 = dim;
	col2 = dim;

	long double res2[dim];



	for(int j= 0; j<col2; j++) {
		long double sum = 0;

		for(int k=0; k<col1; k++) {
			sum+=w2[k][j]*res1[k];
		}
		sum =  sum+b2[j][0];
		res2[j] = sum;
		if (res2[j]<0) {
			res2[j]=0;
		}
		//	myfile<<res2[j]<<"\t";

	}

//	myfile<<"\n";

	col1 = dim;
	row2 = dim;
	col2 = dim;

	long double res3[dim];



	for(int j= 0; j<col2; j++) {
		long double sum = 0;

		for(int k=0; k<col1; k++) {
			sum+=w3[k][j]*res2[k];
		}
		res3[j] = sum+b3[j][0];

		if(res3[j]<0) {
			res3[j] = 0;
		}
		//myfile<<res3[j]<<"\t";
	}
	
	
	
	
	col1 = dim;
	row2 = dim;
	col2 = 2;

	long double res4[2];



	for(int j= 0; j<col2; j++) {
		long double sum = 0;

		for(int k=0; k<col1; k++) {
			sum+=w4[k][j]*res3[k];
		}
		res4[j] = sum+b4[j][0];

		if(res4[j]<0) {
			res4[j] = 0;
		}
		//myfile<<res3[j]<<"\t";
	}

	//myfile<<"\n";


	int score =0;


	int index = 0;
	long double max = res4[0];
	for(int j=0; j<col2; j++) {
		if( max < res4[j]) {
			max = res4[j];
			index = j;
		}
	}

	//myfile.void sclose();

	return index;


}


void scoring() {
	int score = 0,ll=0;
	for(int i = 0; i<12374; i++) {

		ll = vectormatrix(i);

		if ( ll == labels[i]) {
			score+=1;
		}



	}

	cout<< (score*100.0)/12374<<endl;
}






vector<vector<long double> > Load_State(string file_name) {
	ifstream in_state_(file_name.c_str(), ifstream::in);
	vector< vector<long double > > state_out;
	string start;

	while (getline(in_state_, start)) {

		vector<long double> x_coord;


		istringstream ss(start);
		long double a;
		ss >> a;
		x_coord.push_back(a);

		string value;

		while (getline(ss, value, '\t')) {
			long double b;
			ss >> b;
			x_coord.push_back(b);
		}

		state_out.push_back(x_coord);
	}

	return state_out;
}



int main() {

	vector< vector<long double> > weigh0;
	vector< vector<long double> > weigh1;
	vector< vector<long double> > weigh2;
	vector< vector<long double> > weigh3;
	vector< vector<long double> > weigh4;
	vector< vector<long double> > bia0;
	vector< vector<long double> > bia1;
	vector< vector<long double> > bia2;
	vector< vector<long double> > bia3;
	vector< vector<long double> > bia4;
	vector< vector<long double> > features;
	vector<int> label;


	vector< vector<long double> > data = Load_State("data_test.txt");
	weigh0 = Load_State("weight0.txt");
	weigh1 = Load_State("weight1.txt");
	weigh2 = Load_State("weight2.txt");
	weigh3 = Load_State("weight3.txt");
	weigh4 = Load_State("weight4.txt");
	
	bia0 = Load_State("bias0.txt");
	bia1 = Load_State("bias1.txt");
	bia2 = Load_State("bias2.txt");
	bia3 = Load_State("bias3.txt");
	bia4 = Load_State("bias4.txt");


	cout<<"*************************"<<endl;


	for (int i = 0; i<data.size(); i++) {

		vector<long double> interfeature;
		
		for(int j = 0; j<9; j++) {
			interfeature.push_back(data[i][j]);
		}
		label.push_back(data[i][9]);

		features.push_back(interfeature);

	}


	cout<<features.size()<<"\n";
	cout<<features[0].size()<<endl;
	cout<<"*********************************\n";


	assignment(features,weigh0,weigh1,weigh2,weigh3,weigh4,bia0,bia1,bia2, bia3,bia4,label);
	
	scoring();



	return 0;

}
