#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

vector< vector<long double> > weigh0; //= Load_State("weight0.txt");
vector< vector<long double> > weigh1; //= Load_State("weight1.txt");
vector< vector<long double> > weigh2;//= Load_State("weight2.txt");
vector< vector<long double> > weigh3;
vector< vector<long double> > weigh4; //= Load_State("weight3.txt");
vector< vector<long double> > bia0; //= Load_State("bias0.txt");
vector< vector<long double> > bia1; //= Load_State("bias1.txt");
vector< vector<long double> > bia2; //= Load_State("bias2.txt");
vector< vector<long double> > bia3;
vector< vector<long double> > bia4;                                      //= Load_State("bias3.txt");



vector<vector<long double> > vectormatrix(vector<vector<long double> > a, vector<vector<long double> >b, vector<vector<long double> >c) {
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




int main() {

	vector< vector<long double> > data = Load_State("data.txt");
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
	

	//vector< vector<long double> > data = Load_State("data.txt");

	vector< vector<long double> > features;

	vector<int> labels;

	//cout<<data.size()<<endl;



	cout<<"*************************"<<endl;


	for (int i = 0; i<data.size(); i++) {

		vector<long double> interfeature;
		labels.push_back(data[i][0]);
		for(int j = 1; j<=784; j++) {
			interfeature.push_back(data[i][j]);
		}

		features.push_back(interfeature);

	}


	cout<<features.size()<<"\n";
	cout<<features[0].size()<<endl;


	shape(weigh0);
	shape(weigh1);
	shape(weigh2);
	shape(weigh3);
	shape(weigh4);
	shape(bia0);
	shape(bia1);
	shape(bia2);
	shape(bia3);
	shape(bia4);



//	vector<vector <long double> > out1 = vectormatrix(features,weigh0,bia0);
//	vector<vector <long double> > out2 = vectormatrix(out1,weigh1,bia1);
//	vector<vector <long double> > out3 = vectormatrix(out2,weigh2,bia2);
//	vector<vector <long double> > out4 = vectormatrix(out3,weigh3,bia3);


	int score = 0;
	//shape(out4);

	for(int i=0;i<features.size();i++)
	{
		vector <vector<long double> > lop;
		vector<long double> play_it;
		int index =0, maxim = 0;
		
		for(int j=0;j<features[i].size();j++)
		{
			play_it.push_back(features[i][j]);
		}
		lop.push_back(play_it);
		
		vector<vector <long double> > out1 = vectormatrix(lop,weigh0,bia0);
		//shape(out1);
		vector<vector <long double> > out2 = vectormatrix(out1,weigh1,bia1);
		//shape(out2);
		vector<vector <long double> > out3 = vectormatrix(out2,weigh2,bia2);
		//shape(out3);
		vector<vector <long double> > out4 = vectormatrix(out3,weigh3,bia3);
		//shape(out4);
		vector<vector <long double> > out5 = vectormatrix(out4,weigh4,bia4);
		
	
	    
	    index = 0; maxim = out5[0][0];
	    
	    for(int k =0;k<10;k++)
	    {
	    	if(out5[0][k] > maxim)
	    	{
	    		index = k;
	    		maxim = out5[0][k];
			}
		}
		
		if(labels[i] == index)
		{
			score+=1;
		}

	
	}


	cout << "You got " << (100.0*score)/labels.size() << " correct" << endl;


	return 0;

}
