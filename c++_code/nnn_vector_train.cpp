#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

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



vector<vector<long double> > vectormatrix(vector<vector<long double> > a, vector<vector<long double> >b) {
	int row1 = a.size();
	int col1 = a[0].size();
	int row2 = b.size();
	int col2 = b[0].size();
	vector<vector<long double> > res( row1 , vector<long double> (col2, 0.0));
    //ofstream myfile ("example1.txt",ios::app);
	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {
			
			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=a[i][k]*b[k][j];
			}

			res[i][j] = sum;
			
		}
	}


    

	return res;

}

vector<vector<long double> > vectormatrixwithbias(vector<vector<long double> > a, vector<vector<long double> >b, vector<vector<long double> >c) {
	int row1 = a.size();
	int col1 = a[0].size();
	int row2 = b.size();
	int col2 = b[0].size();
	vector<vector<long double> > res( row1 , vector<long double> (col2, 0.0));
    ofstream myfile ("example1.txt",ios::app);
	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {
			
			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=a[i][k]*b[k][j];
			}

			res[i][j] = sum+c[j][0];
			
			if(res[i][j]<0)
			{
				res[i][j]=0;
			}
			
			myfile<<res[i][j]<<"\t";

		}
		myfile<<"\n";
	}


    

	return res;

}


vector<vector<long double> > transPosematrix(vector<vector<long double> > t) {
	int row = t.size();
	int col = t[0].size();
	vector<vector<long double> > res( col , vector<long double> (row, 0));

	for(int i=0; i<col; i++) {
		for(int j=0; j<row; j++) {
			res[i][j] = t[j][i];
		}
	}


	return res;


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


void shape(vector<vector<long double> > b) {
	cout<<"shape\n";
	cout<<b.size()<<endl;
	cout<<b[0].size()<<endl;
}

long double scaling_std(long double max, long double min, long double values) {
	long double std;
	std = (values-min)/(max-min);


	return std;

}



int main() {

	vector< vector<long double> > data = Load_State("mixed200222.txt");

	vector< vector<long double> > features;

	vector<int> labels;

	long double Max[10] = {-100,137.000,197.000000 ,27.000000 ,2.000000 ,27.000000,2.000000,0.125365,0.473688,0.044516};
	long double Min[10] = {-100,10.000000,13.000000,2.000000,0.000000,2.000000,0.000000,0.005385,0.027911,0.001095};

//No.	Nodes	Edges	MaximumIndegree	MinimumIndegree	MaximumOutdegree
//MinimumOutdegree	MedianPagerank	MaximumPagerank	MininumPagerank	StdPageRank	VarPageRank	State


	for (int i = 0; i<data.size(); i++) {
		vector<long double> interfeature;
		for(int j=1; j<10; j++) {
			interfeature.push_back(scaling_std(Max[j],Min[j],data[i][j]));
		}
		//interfeature.push_back(1.00);
		features.push_back(interfeature);


		if (data[i][12] > 0) {
			labels.push_back(1);
		} else {
			labels.push_back(0);
		}


	}


	cout<<features.size()<<"\n";
	cout<<features[0].size()<<endl;


	for (int i = 0; i<10; i++) {
		for(int j=0; j<features[i].size(); j++) {
			cout<<features[i][j] <<" ";
		}
		cout<<"\n";
	}


	return 0;

}
