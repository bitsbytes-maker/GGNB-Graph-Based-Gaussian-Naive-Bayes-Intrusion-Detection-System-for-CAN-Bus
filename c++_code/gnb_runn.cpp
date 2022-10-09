#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;




const int NUM_FEATURES = 9;
const int training_samples = 57748;
const int testing_samples = 24749;



int counter1=0,counter0=0;

long double epsilon = 1e-9;

long double x_test_array[testing_samples][NUM_FEATURES];
int  y_test_label_array[testing_samples];


long double jointi0,jointi1;


vector<long double>  one_means;
vector<long double>  one_stddev;

vector<long double>  zero_means;
vector<long double>  zero_stddev;

vector<vector<long double> >  feature_data_one;
vector<vector<long double> >  feature_data_zero;


long double onemean[NUM_FEATURES], zeromean[NUM_FEATURES], onestd[NUM_FEATURES],zerostd[NUM_FEATURES];


void add_feature(vector<vector<long double> > &feature_data, const std::vector<long double> data_input) {
	vector <long double> temp_data = data_input;
	feature_data.push_back(temp_data);

}

vector<long double> calculate_mean(vector<vector<long double> > feature_data) {

	vector<long double> mean_values(NUM_FEATURES);

	std::size_t num_elements = feature_data.size();
	cout<<num_elements<<"\n";
	for (std::size_t i=0; i < num_elements; i++) {
		vector <long double> temp_data =  feature_data[i];
		for (int j = 0; j < NUM_FEATURES; j++) {
			mean_values[j] += temp_data[j];
		}

	}


	for (int j = 0; j < NUM_FEATURES; j++) {
		mean_values[j] = (1.0*mean_values[j])/num_elements;
	}

	return mean_values;
}

vector<long double> calculate_stddev(vector<vector<long double> > feature_data, vector<long double> mean_values) {

	vector<long double> sigma_values(NUM_FEATURES);

	std::size_t num_elements = feature_data.size();
	cout<<num_elements<<endl;
	for (std::size_t i=0; i < num_elements; i++) {
		vector <long double> temp_data =  feature_data[i];
		for (int j = 0; j < NUM_FEATURES; j++) {
			sigma_values[j] += pow((temp_data[j] - mean_values[j]),2.0) / (num_elements*1.0);
		}
	}

	return sigma_values;
}





void assignment(vector< vector<long double> > X_test, vector <int> Y_test)
{
		for (int i = 0; i<testing_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {

			x_test_array[i][j] = X_test[i+training_samples][j];

		}

		y_test_label_array[i] = Y_test[i+training_samples];

	}

}



void train(vector<vector<long double> > data, vector<int> labels) {
	cout<<"I am in train\n";

	vector<vector<long double> >  feature_data_one;
	vector<vector<long double> >  feature_data_zero;



	for (int i=0; i < training_samples; i++) {
		//cout << labels[i] << endl;
		vector<long double> data_input = data[i];

		if (labels[i] == 1) {
			add_feature(feature_data_one, data_input);
			counter1+=1;
		}
		else if (labels[i] == 0) {
			add_feature(feature_data_zero, data_input);
			counter0+=1;
		}

   }



	one_means = calculate_mean(feature_data_one);
	one_stddev = calculate_stddev(feature_data_one, one_means);

	zero_means = calculate_mean(feature_data_zero);
	zero_stddev = calculate_stddev(feature_data_zero, zero_means);

	vector<long double> onezerototalmeans,onezerostddev;

	onezerototalmeans = calculate_mean(data);
	onezerostddev = calculate_stddev(data,onezerototalmeans);

	long double max = 0;
	for (int i = 0;i<NUM_FEATURES;i++)
	{
		cout << onezerostddev[i]<<" ";
		if(onezerostddev[i]>max)
		{
			max= onezerostddev[i];
		}
	}
	cout<<"max========"<<max<<"\n";
	epsilon *=max;
	cout<< "epsilon======="<<epsilon<<"\n";

    for (int i=0; i<NUM_FEATURES; i++) {

		//cout<<left_means[i]<<" ";
		onemean[i] = one_means[i];
		zeromean[i] = zero_means[i];
		onestd[i] = one_stddev[i]+epsilon;
		zerostd[i] = zero_stddev[i]+epsilon;

			cout<<onemean[i] << " ";
			//<< onestd[i] <<"\t"<<zero_stddev[i];

	}
	cout<<"\n";

	for (int i=0;i<NUM_FEATURES;i++)
	{
		cout<< onestd[i] <<" ";
	}
	cout<<"\n";

	jointi1 = log(counter1*1.0/(counter0+counter1));
	jointi0 = log(counter0*1.0/(counter0+counter1));

	std::cout<< "\nTraining Complete...\n"<< std::endl;
}


long double calculate_prob(int j, int which) {

	long double temp_var= 0;
	long double pdf_gaussian = 1.0;


	long double x,mean,sigma;

	long double jointi;



	//cout<<j<<endl;


	if (which == 1) {


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];

			sigma = onestd[i];

			temp_var += log(2*M_PI*sigma);

		}

		temp_var*=-0.5;


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = onemean[i];
			sigma = onestd[i];

			temp_var-= 0.5*(pow((x - mean),2)/sigma);

		}



			return jointi1+temp_var;


	}


	else{



		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			sigma = zerostd[i];
           	temp_var +=log(2*M_PI*sigma);

		}

		temp_var*=-0.5;


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = zeromean[i];
			sigma = zerostd[i];

			temp_var-= 0.5*(pow((x - mean),2)/sigma);

		}




		return jointi0+temp_var;

	}


}



int predict(int j) {

	long double prob0,prob1;
	prob0 = calculate_prob(j,0);
	prob1 = calculate_prob(j,1);

	if ( prob0 >prob1 ){
		return 0;
	}

	else{
		return 1;

	}

}



int scoring()
{
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
	for (int i = 0;i<data.size();i++)
	{
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
    train(features,labels);


    int score = scoring();
	float fraction_correct = float(score) / testing_samples;
	cout << "You got " << (100*fraction_correct) << " correct" << endl;



	return 0;

}

