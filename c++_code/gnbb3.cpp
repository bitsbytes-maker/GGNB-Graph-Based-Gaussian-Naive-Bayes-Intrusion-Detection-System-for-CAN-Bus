#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>


    // std::rand, std::srand

using namespace std;




const int NUM_FEATURES = 8;
const int training_samples = 6000;
const int testing_samples = 2249;

double x_test_array[testing_samples][NUM_FEATURES];
int  y_test_label_array[testing_samples];


vector<double>  one_means;
vector<double>  one_stddev;

vector<double>  zero_means;
vector<double>  zero_stddev;

vector<vector<double> >  feature_data_one;
vector<vector<double> >  feature_data_zero;


double onemean[NUM_FEATURES], zeromean[NUM_FEATURES], onestd[NUM_FEATURES],zerostd[NUM_FEATURES];


void add_feature(vector<vector<double> > &feature_data, const std::vector<double> data_input) {
	vector <double> temp_data = data_input;
	feature_data.push_back(temp_data);

}

vector<double> calculate_mean(vector<vector<double> > feature_data) {

	vector<double> mean_values(NUM_FEATURES);

	std::size_t num_elements = feature_data.size();
	for (std::size_t i=0; i < num_elements; i++) {
		vector <double> temp_data =  feature_data[i];
		for (int j = 0; j < NUM_FEATURES; j++) {
			mean_values[j] += temp_data[j];
		}
	}

	for (int j = 0; j < NUM_FEATURES; j++) {
		mean_values[j] /= num_elements;
	}

	return mean_values;
}

vector<double> calculate_stddev(vector<vector<double> > feature_data, vector<double> mean_values) {

	vector<double> sigma_values(NUM_FEATURES);

	std::size_t num_elements = feature_data.size();
	cout<<num_elements<<endl;
	for (std::size_t i=0; i < num_elements; i++) {
		vector <double> temp_data =  feature_data[i];
		for (int j = 0; j < NUM_FEATURES; j++) {
			sigma_values[j] += pow((temp_data[j] - mean_values[j]),2) / num_elements;
		}
	}

	for (int j = 0; j < NUM_FEATURES; j++) {
		sigma_values[j] = sqrt(sigma_values[j]);
	}

	return sigma_values;
}





void assignment(vector< vector<double> > X_test, vector <int> Y_test)
{
		for (int i = 0; i<testing_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {

			x_test_array[i][j] = X_test[i+training_samples][j];

		}

		y_test_label_array[i] = Y_test[i+training_samples];
	
	}
	
}

void train(vector<vector<double> > data, vector<int> labels) {
	cout<<"I am in train\n";

	vector<vector<double> >  feature_data_one;
	vector<vector<double> >  feature_data_zero;
	
	int counter1=0,counter0=0;

	for (int i=0; i < training_samples; i++) {
		//cout << labels[i] << endl;
		vector<double> data_input = data[i];

		if (labels[i] == 1) {
			add_feature(feature_data_one, data_input);
			counter1+=1;
		}
		else if (labels[i] == 0) {
			add_feature(feature_data_zero, data_input);
			counter0+=1;
		}
		
   }
   
   cout<<counter1<<"\t"<<counter0<<endl;

	one_means = calculate_mean(feature_data_one);
	one_stddev = calculate_stddev(feature_data_one, one_means);

	zero_means = calculate_mean(feature_data_zero);
	zero_stddev = calculate_stddev(feature_data_zero, zero_means);
	
	
    for (int i=0; i<NUM_FEATURES; i++) {

		//cout<<left_means[i]<<" ";
		onemean[i] = one_means[i];
		zeromean[i] = zero_means[i];
		onestd[i] = one_stddev[i];
		zerostd[i] = zero_stddev[i];
		
		//cout<<one_stddev[i]<<"\t";

	}
	
	std::cout<< "\nTraining Complete...\n"<< std::endl;
}



vector<vector<double> > Load_State(string file_name) {
	ifstream in_state_(file_name.c_str(), ifstream::in);
	vector< vector<double > > state_out;
	string start;

	while (getline(in_state_, start)) {

		vector<double> x_coord;

		istringstream ss(start);
		double a;
		ss >> a;
		x_coord.push_back(a);

		string value;

		while (getline(ss, value, '\t')) {
			double b;
			ss >> b;
			x_coord.push_back(b);
		}

		state_out.push_back(x_coord);
	}
	return state_out;
}



double calculate_prob(int j, int which) {

	double temp_var= 1.0;
	double pdf_gaussian = 1.0;
	// Since this is Naive Bayes, we consider the
	// prob(x1,x2,x3,x4 | class) = p(x1|class) * p(x2|class) * p(x3|lass) * p(x4|class)

	double x,mean,sigma;
	
	
	//cout<<j<<endl;
	

	if (which == 1) {


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = onemean[i];
			sigma = onestd[i];
			
			temp_var = (1 / (sigma * sqrt(2 * M_PI))) * exp(-0.5 * pow(((x - mean) / sigma), 2.0 ));
			pdf_gaussian *= temp_var;
		}
		
		
		


	}


	else if (which == 0) {


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = zeromean[i];
			sigma = zerostd[i];

			//cout<< x <<"  " << i << " "<<mean<<" "<<sigma<<endl;

			temp_var = (1 / (sigma * sqrt(2 * M_PI))) * exp(-0.5 * pow(((x - mean) / sigma), 2.0 ));
			pdf_gaussian *= temp_var;
		}


	}

    

	return pdf_gaussian;

}



int predict(int j) {

	double prob_classes[2];

	prob_classes[0]=calculate_prob(j,0);
	prob_classes[1]=calculate_prob(j,1);
	
	//cout<<prob_classes[0]<<"\t"<<prob_classes[1]<<endl;

	
	if (prob_classes[0] < prob_classes[1]){
		return 0;
	}
	
	return 1;

}



int scoring()
{
		int score = 0;
	for(int i = 0; i < testing_samples; i++) {
		int predicted = predict(i);
		if(predicted == y_test_label_array[i+training_samples]) {
			cout<<"I am correct\n";
			score += 1;
		}
	}
	
	return score;
}





int main() {

	vector< vector<double> > data = Load_State("mixedGraph_GraphFeatures_2k.txt");
	
	vector< vector<double> > features;
	
	vector<int> labels;
	
//No.	Nodes	Edges	MaximumIndegree	MinimumIndegree	MaximumOutdegree	
//MinimumOutdegree	MedianPagerank	MaximumPagerank	MininumPagerank	StdPageRank	VarPageRank	State	
	for (int i = 0;i<data.size();i++)
	{
		vector<double> interfeature;
		interfeature.push_back(data[i][1]);
		interfeature.push_back(data[i][2]);
		interfeature.push_back(data[i][3]);
		interfeature.push_back(data[i][4]);
		interfeature.push_back(data[i][5]);
		//interfeature.push_back(data[i][6]);
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
