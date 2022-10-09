#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;



const double LANE_WIDTH = 4.0;
const int NUM_FEATURES = 4;

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


string possible_labels[3] = {"left", "keep", "right"};

vector<double>  left_means;
vector<double>  left_stddev;

vector<double>  keep_means;
vector<double>  keep_stddev;

vector<double>  right_means;
vector<double>  right_stddev;

vector<vector<double> >  feature_data_left;
vector<vector<double> >  feature_data_keep;
vector<vector<double> >  feature_data_right;

double ls[4];
double ks[4];
double rs[4];
double lm[4];
double km[4];
double rm[4];
void train(vector<vector<double> > data, vector<string> labels) {
	cout<<"I am in train\n";

	vector<vector<double> >  feature_data_left;
	vector<vector<double> >  feature_data_keep;
	vector<vector<double> >  feature_data_right;

	for (int i=0; i < labels.size(); i++) {
		//cout << labels[i] << endl;
		vector<double> data_input = data[i];

		if (labels[i] == "left") add_feature(feature_data_left, data_input);
		else if (labels[i] == "keep") add_feature(feature_data_keep, data_input);
		else if (labels[i] == "right") add_feature(feature_data_right, data_input);
	}

	left_means = calculate_mean(feature_data_left);
	left_stddev = calculate_stddev(feature_data_left, left_means);

	keep_means = calculate_mean(feature_data_keep);
	keep_stddev = calculate_stddev(feature_data_keep, keep_means);

	right_means = calculate_mean(feature_data_right);
	right_stddev = calculate_stddev(feature_data_right, right_means);


	for (int i=0; i<4; i++) {

		//cout<<left_means[i]<<" ";
		lm[i] = left_means[i];
		km[i] = keep_means[i];
		rm[i] = right_means[i];
		ls[i] = left_stddev[i];
		ks[i] = keep_stddev[i];
		rs[i] = right_stddev[i];

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

		while (getline(ss, value, ',')) {
			double b;
			ss >> b;
			x_coord.push_back(b);
		}

		state_out.push_back(x_coord);
	}
	return state_out;
}
vector<string> Load_Label(string file_name) {
	ifstream in_label_(file_name.c_str(), ifstream::in);
	vector< string > label_out;
	string line;
	while (getline(in_label_, line)) {
		istringstream iss(line);
		string label;
		iss >> label;

		//cout << label<<"\n";

		label_out.push_back(label);
	}
	return label_out;

}

double x_test_array[750][4];
int  y_test_label_array[250];


double calculate_prob(int j, int which) {

	double temp_var= 1.0;
	double pdf_gaussian = 1.0;
	// Since this is Naive Bayes, we consider the
	// prob(x1,x2,x3,x4 | class) = p(x1|class) * p(x2|class) * p(x3|lass) * p(x4|class)

	double x,mean,sigma;

	if (which == 1) {


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = km[i];
			sigma = ks[i];

			//cout<< x <<"  " << i << " "<<mean<<" "<<sigma<<endl;

			temp_var = (1 / (sigma * sqrt(2 * M_PI))) * exp(-0.5 * pow(((x - mean) / sigma), 2.0 ));
			pdf_gaussian *= temp_var;
		}


	}


	else if (which == 0) {


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = lm[i];
			sigma = ls[i];

			//cout<< x <<"  " << i << " "<<mean<<" "<<sigma<<endl;

			temp_var = (1 / (sigma * sqrt(2 * M_PI))) * exp(-0.5 * pow(((x - mean) / sigma), 2.0 ));
			pdf_gaussian *= temp_var;
		}


	}

	else {


		for (int i = 0; i < NUM_FEATURES; i++) {

			x = x_test_array[j][i];
			mean = rm[i];
			sigma = rs[i];

			//cout<< x <<"  " << i << " "<<mean<<" "<<sigma<<endl;

			temp_var = (1 / (sigma * sqrt(2 * M_PI))) * exp(-0.5 * pow(((x - mean) / sigma), 2.0 ));
			pdf_gaussian *= temp_var;
		}

	}


	return pdf_gaussian;

}



int predict(int j) {

	double prob_classes[3];

	prob_classes[0]=calculate_prob(j,0);
	prob_classes[1]=calculate_prob(j,1);
	prob_classes[2]=calculate_prob(j,2);
	int idx = 0;
	double best_p = 0;

	for (int p = 0; p < 3; p++) {
		cout<<prob_classes[p] <<" ";
		if (prob_classes[p] > best_p) {
			best_p = prob_classes[p];
			idx = p;
		}
	}
	cout<<endl;
	//cout<<possible_labels[idx]<<endl;
	return idx;

}


void assignment(vector< vector<double> > X_test, vector <string> Y_test)
{
		for (int i = 0; i<X_test.size(); i++) {
		for(int j=0; j<X_test[i].size(); j++) {

			x_test_array[i][j] = X_test[i][j];

		}

		if (Y_test[i] == "left") {
			y_test_label_array[i] = 0;
		}

		else if (Y_test[i] == "keep") {
			y_test_label_array[i] = 1;
		}

		else {
			y_test_label_array[i] = 2;
		}
	}
}


int scoring(vector <vector<double> > X_test)
{
		int score = 0;
	for(int i = 0; i < X_test.size(); i++) {
		vector<double> coords = X_test[i];
		int predicted = predict(i);
		if(predicted == y_test_label_array[i]) {
			score += 1;
		}
	}
	
	return score;
}




int main() {

	vector< vector<double> > X_train = Load_State("train_states.txt");
	vector< vector<double> > X_test  = Load_State("test_states.txt");
	vector< string > Y_train  = Load_Label("train_labels.txt");
	vector< string > Y_test   = Load_Label("test_labels.txt");

	cout << "X_train number of elements " << X_train.size() << endl;
	cout << "X_train element size " << X_train[0].size() << endl;
	cout << "Y_train number of elements " << Y_train.size() << endl << endl;


	cout << "X_test number of elements " << X_test.size() << endl;
	cout << "X_test element size " << X_test[0].size() << endl;
	cout << "Y_test number of elements " << Y_test.size() << endl << endl;



    assignment(X_test,Y_test);





	train(X_train, Y_train);


//    train(X_train, Y_train);
//
	cout << "X_test number of elements " << X_test.size() << endl;
	cout << "X_test element size " << X_test[0].size() << endl;
	cout << "Y_test number of elements " << Y_test.size() << endl << endl;

    int score = scoring(X_test);

	float fraction_correct = float(score) / Y_test.size();
	cout << "You got " << (100*fraction_correct) << " correct" << endl;

	return 0;
}



