#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;


#define zero_samples 16540
#define one_samples 41208

const int NUM_FEATURES = 9;
const int training_samples = 57748;
const int testing_samples = 24749;



//int counter1=41208,counter0=16540;

long double epsilon = 1e-9;

long double x_test_array[testing_samples][NUM_FEATURES];
int  y_test_label_array[testing_samples];


long double x_train_array[training_samples][NUM_FEATURES];
int y_train_label_array[training_samples];

long double x_zero_array[zero_samples][NUM_FEATURES],x_one_array[one_samples][NUM_FEATURES];


long double onezerototalmeans[NUM_FEATURES],onezerostddev[NUM_FEATURES];

long double jointi0,jointi1;


long double onemean[NUM_FEATURES], zeromean[NUM_FEATURES], onestd[NUM_FEATURES],zerostd[NUM_FEATURES];


void calculate_mean() {

	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			onezerototalmeans[j] += x_train_array[i][j];
		}
	}


	for(int j=0; j<NUM_FEATURES; j++) {
		onezerototalmeans[j] = onezerototalmeans[j]/(training_samples*1.0);
	}


	for(int i =0; i<zero_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			zeromean[j] += x_zero_array[i][j];
		}
	}

	for(int j=0; j<NUM_FEATURES; j++) {
		zeromean[j] = zeromean[j]/(zero_samples*1.0);
	}



	for(int i =0; i<one_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			onemean[j] += x_one_array[i][j];
		}
	}

	for(int j=0; j<NUM_FEATURES; j++) {
		onemean[j] = onemean[j]/(one_samples*1.0);
	}


}

void calculate_stddev() {



	for(int i = 0; i<training_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			onezerostddev[j]+=pow((x_train_array[i][j] - onezerototalmeans[j]),2.0) / (training_samples*1.0);

		}
	}



	for(int i = 0; i<zero_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			zerostd[j]+=pow((x_zero_array[i][j] - zeromean[j]),2.0) / (zero_samples*1.0);

		}
	}


	for(int i = 0; i<one_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			onestd[j]+=pow((x_one_array[i][j] - onemean[j]),2.0) / (one_samples*1.0);

		}
	}

	//return sigma_values;
}





void assignment(vector< vector<long double> > X_test, vector <int> Y_test) {


	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {
			x_train_array[i][j] = X_test[i][j];
		}

		y_train_label_array[i]=Y_test[i];
	}

	int i0 = 0,i1 = 0;
	for(int i =0; i<training_samples; i++) {
		if( Y_test[i] == 0) {

			for(int j=0; j<NUM_FEATURES; j++) {

				x_zero_array[i0][j] = X_test[i][j];

			}
			i0++;

		}

		else {


			for(int j=0; j<NUM_FEATURES; j++) {

				x_one_array[i1][j] = X_test[i][j];

			}
			i1++;

		}


	}

	cout<<i0<<"hmm\n"<<i1<<"hmm\n";



	for (int i = 0; i<testing_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {

			x_test_array[i][j] = X_test[i+training_samples][j];

		}

		y_test_label_array[i] = Y_test[i+training_samples];

	}

	jointi1 = log(one_samples*1.0/(training_samples));
	jointi0 = log(zero_samples*1.0/(training_samples));

}



void train() {
	cout<<"I am in train\n";



	calculate_mean();
	calculate_stddev();

	long double max = 0;
	for (int i = 0; i<NUM_FEATURES; i++) {
		cout << onezerostddev[i]<<" ";
		if(onezerostddev[i]>max) {
			max= onezerostddev[i];
		}
	}
	cout<<"max========"<<max<<"\n";
	epsilon *=max;
	cout<< "epsilon======="<<epsilon<<"\n";

	for (int i=0; i<NUM_FEATURES; i++) {

		//cout<<left_means[i]<<" ";

		onestd[i] = onestd[i]+epsilon;
		zerostd[i] = zerostd[i]+epsilon;

		cout<<zerostd[i] << " ";
		//<< onestd[i] <<"\t"<<zero_stddev[i];

	}
	cout<<"\n";

	for (int i=0; i<NUM_FEATURES; i++) {
		cout<< onestd[i] <<" ";
	}
	cout<<"\n";



	std::cout<< "\nTraining Complete...\n"<< std::endl;
}




int predict(int j) {

	long double prob0,prob1;
	long double temp_var10= 0,temp_var20=0,temp_var11=0,temp_var21=0;

	long double x,mean1,sigma1,mean0,sigma0;

	for (int i = 0; i < NUM_FEATURES; i++) {

		x = x_test_array[j][i];

		sigma1 = onestd[i];

		temp_var11 += -0.5*log(2*M_PI*sigma1);
		mean1 = onemean[i];
		temp_var21-= 0.5*(pow((x - mean1),2)/sigma1);


		sigma0 = zerostd[i];
		temp_var10 +=-0.5*log(2*M_PI*sigma0);
		mean0 = zeromean[i];
		temp_var20-= 0.5*(pow((x - mean0),2)/sigma0);
	}

	prob0=jointi0+temp_var10+temp_var20;
	prob1=jointi1+temp_var11+temp_var21;

	if(j<10) {
		cout<<prob0 <<"\t"<<prob1 <<endl;
	}


	if ( prob0 >prob1 ) {
		return 0;
	}

	else {
		return 1;

	}

}



int scoring() {
	int score = 0;
	for(int i = 0; i < testing_samples; i++) {
		int predicted = predict(i);
		if(predicted == y_test_label_array[i]) {
			//cout<<"I am correct\n";
			score += 1;
		}
	}

	return score;
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

	vector< vector<long double> > data = Load_State("mixed200222.txt");

	vector< vector<long double> > features;

	vector<int> labels;

//No.	Nodes	Edges	MaximumIndegree	MinimumIndegree	MaximumOutdegree
//MinimumOutdegree	MedianPagerank	MaximumPagerank	MininumPagerank	StdPageRank	VarPageRank	State
	for (int i = 0; i<data.size(); i++) {
		vector<long double> interfeature;
		interfeature.push_back(data[i][1]);
		interfeature.push_back(data[i][2]);
		interfeature.push_back(data[i][3]);
		interfeature.push_back(data[i][4]);
		interfeature.push_back(data[i][5]);
		interfeature.push_back(data[i][6]);
		interfeature.push_back(data[i][7]);
		interfeature.push_back(data[i][8]);
		interfeature.push_back(data[i][9]);
		labels.push_back(data[i][12]);
		features.push_back(interfeature);

	}


	cout<<features.size()<<"\n";
	cout<<features[0].size()<<endl;





	assignment(features,labels);
	train();

    int testing_samples = 24749;
    
    
	int score = scoring();
	float fraction_correct = float(score) / testing_samples;
	cout << "You got " << (100*fraction_correct) << " correct" << endl;


	return 0;

}

