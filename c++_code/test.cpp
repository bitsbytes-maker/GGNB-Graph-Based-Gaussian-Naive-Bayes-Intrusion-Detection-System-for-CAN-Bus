#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;



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
				vector<int> label );

void scoring();

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
