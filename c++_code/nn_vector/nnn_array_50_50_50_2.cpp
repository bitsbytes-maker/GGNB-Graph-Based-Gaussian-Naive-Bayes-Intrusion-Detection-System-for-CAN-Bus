#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>


#define nhiddenneurons 50
#define training_samples  4096

using namespace std;

int iterative_data = 0;



long double out1[training_samples][50],out2[training_samples][50];

long double out5[training_samples][2];




long double  bia0[nhiddenneurons];
long double  bia1[nhiddenneurons];

long double  bia4[2];


long double w0[9][50];
long double w1[50][50];
//long double w2[50][50];
//long double w3[50][50];
long double w4[50][2];






long double outt3[50][training_samples];
long double outt1[50][training_samples];


long double input0[training_samples][nhiddenneurons];
long double input1[training_samples][nhiddenneurons];
long double input2[training_samples][nhiddenneurons];
long double input3[training_samples][nhiddenneurons];
long double input4[training_samples][9];



long double training_data[training_samples][9];
int label[training_samples];

long double loss_grad[training_samples][2];


long double loss[training_samples];



void forwardvectormatrixwithbias() {


	int row1,col2,col1;
	row1 = training_samples;   //512x2 //57748x2
	col2 = 50;
	col1= 9;

	/// first layer w0*data+b

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=training_data[i][k]*w0[k][j];
			}

			out1[i][j] = sum+bia0[j];

			if(out1[i][j]<0) {
				out1[i][j]=0;
			}


		}

	}

	// second layer
	col2 = 50;
	col1= 50;

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=out1[i][k]*w1[k][j];
			}

			out2[i][j] = sum+bia1[j];

			if(out2[i][j]<0) {
				out2[i][j]=0;
			}


		}

	}


	col2 = 2;
	col1= 50;

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=out2[i][k]*w4[k][j];
			}

			out5[i][j] = sum+bia4[j];

			if(out5[i][j]<0) {
				out5[i][j]=0;
			}

		}

	}




	//return res;

}









void softmax_crossentropy_with_logits() {


	for(int i=0; i<training_samples; i++) {
		long double iterator;
		int ind = label[i];


		///  - logits_for_answers + part
		iterator=-out5[i][ind]+log(exp(out5[i][0])+exp(out5[i][1]));

		loss[i] = iterator;
	}


}

void  grad_softmax_crossentropy_with_logits() {


	cout<<"I am loss\n";
	for(int i =0; i<training_samples; i++) {
		long double sum0,sum1;

		long double exp_scale =  exp(out5[i][0])+  exp(out5[i][1]);
		sum0 = exp(out5[i][0])/exp_scale;
		sum1 = exp(out5[i][1])/exp_scale;

		int iblind = label[i];
		if(iblind == 0) {

			sum0 = (-1 + sum0)/training_samples;
			sum1 = sum1/training_samples;
		} else {
			sum0 = (0 + sum0)/training_samples;
			sum1 = (-1+sum1)/training_samples;

		}

		loss_grad[i][0] = sum0;
		loss_grad[i][1] = sum1;


	}
}

//57748x50

//512x50


//ofstream myfile ("examp.txt",ios::app);

// input0backward(weigh4,bia4,out4,loss_grad);/        weigh , bia4, out, grad_output
void  inputbackward0() {

	//

	long double wt4[2][50];
	int neurons = 50;

	for(int i =0; i<neurons; i++) {
		for(int j=0; j<2; j++) {
			wt4[j][i]=w4[i][j];
		}
	}


	//	//vector< vector<long double> > grad_input = vectormatrix(grad_output,we);
	//static long double input0[training_samples][nhiddenneurons];
	for(int i=0; i<training_samples; i++) {
		for(int j= 0; j<neurons; j++) {

			long double sum =0;

			for(int k=0; k<2; k++) {
				sum+=loss_grad[i][k]*wt4[k][j];
			}

			input0[i][j] = sum;

		}
	}

	// 57748


	//cout<<training_samples<<"\n";

	for(int i=0; i<training_samples; i++) {
		for(int j=0; j<50; j++) {
			outt3[j][i] = out2[i][j];

		}
	}

	long double grad_weights[50][2];

	for(int i=0; i<neurons; i++) {
		for(int j= 0; j<2; j++) {

			long double sum =0;

			for(int k=0; k<training_samples; k++) {
				sum+=outt3[i][k]*loss_grad[k][j];
			}

			grad_weights[i][j] = sum;

		}
	}

	long double grad_biases[2] = {0,0};
//
//
//	//myfile<<"grad_bias"<< grad_output.size()<<"\t"<<grad_output[0].size()<<"\n";


	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<2; j++) {

			grad_biases[j] = grad_biases[j]+loss_grad[i][j]; //512 //57748x9 9x50
		}
	}

	long double ll =0.1;
	//myfile<<"weight"<<"\n";
	for(int i = 0; i<neurons; i++) {
		for(int j =0; j<2; j++) {
			w4[i][j] -=ll*grad_weights[i][j];
			//	myfile<<w4[i][j]<<"\t";
		}
		//		myfile<<"\n";
	}
	//myfile<<"I am not suee\n";

	for(int i = 0; i<2; i++) {
		bia4[i]-= ll*grad_biases[i];
		//myfile<<bia4[i]<<"\t";
	}

	//myfile<<"\n";

	cout<<training_samples<<endl;
	for(int i=0; i<training_samples; i++) {
		for(int j=0; j<50; j++) {
			if(out2[i][j]<=0) {
				input0[i][j] = 0;
			}
		}
	}

}





void inputbackward3() {
	long double wt1[50][50];
	int neurons = 50;

	for(int i =0; i<neurons; i++) {
		for(int j=0; j<neurons; j++) {
			wt1[j][i]=w1[i][j];
		}
	}


	//	//vector< vector<long double> > grad_input = vectormatrix(grad_output,we);
	//static long double input0[training_samples][nhiddenneurons];
	for(int i=0; i<training_samples; i++) {
		for(int j= 0; j<neurons; j++) {

			long double sum =0;

			for(int k=0; k<neurons; k++) {
				sum+=input0[i][k]*wt1[k][j];
			}

			input3[i][j] = sum;

		}
	}

	// 57748
	//static

	//cout<<training_samples<<"\n";

	for(int i=0; i<training_samples; i++) {
		for(int j=0; j<50; j++) {
			outt1[j][i] = out1[i][j];

		}
	}

	long double grad_weights[50][50];

	for(int i=0; i<neurons; i++) {
		for(int j= 0; j<neurons; j++) {

			long double sum =0;

			for(int k=0; k<training_samples; k++) {
				sum+=outt1[i][k]*input0[k][j];
			}

			grad_weights[i][j] = sum;

		}
	}

	long double grad_biases[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//
//
//	//myfile<<"grad_bias"<< grad_output.size()<<"\t"<<grad_output[0].size()<<"\n";


	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<50; j++) {

			grad_biases[j] = grad_biases[j]+input0[i][j];
		}
	}

	long double ll =0.1;
	//myfile<<"weight"<<"\n";
	for(int i = 0; i<neurons; i++) {
		for(int j =0; j<neurons; j++) {
			w1[i][j] -=ll*grad_weights[i][j];
			//myfile<<w1[i][j]<<"\t";
		}
		//	myfile<<"\n";
	}
	//myfile<<"I am not suee\n";

	for(int i = 0; i<50; i++) {
		bia1[i]-= ll*grad_biases[i];
		//myfile<<bia1[i]<<"\t";
	}

	//myfile<<"\n";



}

void inputbackward4() {

	static long double datat1[9][training_samples];
	int neurons = 50;


	for(int i=0; i<training_samples; i++) {
		for(int j=0; j<9; j++) {
			datat1[j][i] = training_data[i][j];

		}
	}

	long double grad_weights[9][50];

	for(int i=0; i<9; i++) {
		for(int j= 0; j<neurons; j++) {

			long double sum =0;

			for(int k=0; k<training_samples; k++) {
				sum+=datat1[i][k]*input3[k][j];
			}

			grad_weights[i][j] = sum;

		}
	}

	long double grad_biases[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//
//
//	//myfile<<"grad_bias"<< grad_output.size()<<"\t"<<grad_output[0].size()<<"\n";


	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<50; j++) {

			grad_biases[j] = grad_biases[j]+input3[i][j];
		}
	}

	long double ll =0.1;
	//myfile<<"weight"<<"\n";
	for(int i = 0; i<9; i++) {
		for(int j =0; j<neurons; j++) {
			w0[i][j] -=ll*grad_weights[i][j];
			//myfile<<w0[i][j]<<"\t";
		}
		//myfile<<"\n";
	}
//	myfile<<"I am not suee\n";

	for(int i = 0; i<50; i++) {
		bia0[i]-= ll*grad_biases[i];
		//myfile<<bia0[i]<<"\t";
	}

	//myfile<<"\n";

}



void train() {
	forwardvectormatrixwithbias();
	softmax_crossentropy_with_logits(); // loss

	grad_softmax_crossentropy_with_logits();   // loss_grad


	inputbackward0();
	//inputbackward1();
	//inputbackward2();
	inputbackward3();
	inputbackward4();



	cout<<"hmmmmmmm\n";


}



void assigment_train_data(vector<vector<long double> > features,int start) {

	for(int k=0; k<training_samples; k++) {
		for(int j=0; j<9; j++) {
			training_data[k][j] =features[k][j];
		}
	}
}




void assignment(vector<int> labels,vector<vector<long double> > weigh0,vector<vector<long double> > weigh1,vector<vector<long double> > weigh4) {

	for(int i =0; i<training_samples; i++) {
		label[i]=labels[i];
	}


	for(int i =0; i<weigh0.size(); i++) {
		for(int j=0; j<weigh0[i].size(); j++) {
			w0[i][j]=weigh0[i][j];
		}
	}

	for(int i =0; i<weigh1.size(); i++) {
		for(int j=0; j<weigh1[i].size(); j++) {
			w1[i][j]=weigh1[i][j];
		}
	}

	for(int i =0; i<weigh4.size(); i++) {
		for(int j=0; j<weigh4[i].size(); j++) {
			w4[i][j]=weigh4[i][j];
		}
	}

	for(int i =0; i<nhiddenneurons; i++) {
		bia0[i]=0;
		bia1[i]=0;

	}

	bia4[0]=0;
	bia4[1]=0;

}

vector<vector<long double> > vectormatrixwithbias(vector<vector<long double> > &a, vector<vector<long double> > &b, vector<long double> &c) {
	int row1 = a.size();
	int col1 = a[0].size();
	int row2 = b.size();
	int col2 = b[0].size();
	vector<vector<long double> > res( row1 , vector<long double> (col2, 0.0));

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=a[i][k]*b[k][j];
			}

			res[i][j] = sum+c[j];

			if(res[i][j]<0) {
				res[i][j]=0;
			}


		}

	}




	return res;

}



void predict(vector< vector<long double> > features, vector<int> labels) {
	int score = 0;
	//shape(out4);

	vector< vector<long double> > weigh0( 9 , vector<long double> (50, 0.0));
	vector< vector<long double> > weigh1( 50 , vector<long double> (50, 0.0));
	//vector< vector<long double> > weigh2( 50 , vector<long double> (50, 0.0));
	//vector< vector<long double> > weigh3( 50 , vector<long double> (50, 0.0));
	vector< vector<long double> > weigh4( 50 , vector<long double> (2, 0.0));

	vector<long double>  b0(nhiddenneurons);
	vector<long double>  b1(nhiddenneurons);
	//vector<long double>  b2(nhiddenneurons);
	//vector<long double>  b3(nhiddenneurons);
	vector<long double>  b4(2);

	for(int i =0; i<9; i++) {
		for(int j=0; j<50; j++) {
			weigh0[i][j] = w0[i][j];
		}
	}


	for(int i =0; i<50; i++) {
		b0[i] = bia0[i];
		b1[i] = bia1[i];
		//b2[i] = bia2[i];
		//b3[i] = bia3[i];
		for(int j=0; j<50; j++) {
			weigh1[i][j] = w1[i][j];
			//weigh2[i][j] = w2[i][j];
			//weigh3[i][j] = w3[i][j];
		}
	}

	for(int i =0; i<50; i++) {
		for(int j=0; j<2; j++) {
			weigh4[i][j] = w4[i][j];
		}
	}

	b4[0] = bia4[0];
	b4[1]=bia4[1];


	for(int i=57748+12374; i<features.size(); i++) {
		vector <vector<long double> > lop;
		vector<long double> play_it;
		int index =0, maxim = 0;

		for(int j=0; j<features[i].size(); j++) {
			play_it.push_back(features[i][j]);
		}
		lop.push_back(play_it);

		vector<vector <long double> > outp1 = vectormatrixwithbias(lop,weigh0,b0);
		//shape(out1);
		vector<vector <long double> > outp2 = vectormatrixwithbias(outp1,weigh1,b1);
		//shape(out2);
		//vector<vector <long double> > outp3 = vectormatrixwithbias(outp2,weigh2,b2);
		vector<vector <long double> > outp5 = vectormatrixwithbias(outp2,weigh4,b4);



		index = 0;
		maxim = outp5[0][0];

		for(int k =0; k<2; k++) {
			if(outp5[0][k] > maxim) {
				index = k;
				maxim = outp5[0][k];
			}
		}

		if(labels[i] == index) {
			score+=1;
		}


	}


	cout << "You got " << (100.0*score)/12375 << " correct" << endl;


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




long double scaling_std(long double max, long double min, long double values) {
	long double std;
	std = (values-min)/(max-min);


	return std;

}




int main() {


	vector< vector<long double> > weigh0;
	vector< vector<long double> > weigh1;
	//vector< vector<long double> > weigh2;
	//vector< vector<long double> > weigh3;
	vector< vector<long double> > weigh4;

	vector< vector<long double> > data = Load_State("mixed200222.txt");


	/// weight matrix loading here as per the gaussian distribution

	weigh0 = Load_State("weight0.txt");
	weigh1 = Load_State("weight1.txt");
	//weigh2 = Load_State("weight2.txt");
	//weigh3 = Load_State("weight3.txt");
	weigh4 = Load_State("weight4.txt");



	vector< vector<long double> > features;

	vector<int> labels;

	long double Max[10] = {-100,137.000,197.000000 ,27.000000 ,2.000000 ,27.000000,2.000000,0.125365,0.473688,0.044516};
	long double Min[10] = {-100,10.000000,13.000000,2.000000,0.000000,2.000000,0.000000,0.005385,0.027911,0.001095};

//No.	Nodes	Edges	MaximumIndegree	MinimumIndegree	MaximumOutdegree
//MinimumOutdegree	MedianPagerank	MaximumPagerank	MininumPagerank	StdPageRank	VarPageRank	State
// min_max_scaling

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

	assignment(labels,weigh0,weigh1,weigh4);

	//for(int i=0; i<1; i++) {

	//for(int j=0; j<57748+404; j+=512) {  //1024
	assigment_train_data(features,0);
	for(int i =0; i<130; i++)
		train();

//		}
//
//	}


	cout<<"**********************************************"<<endl;
	predict(features,labels);



	return 0;

}
