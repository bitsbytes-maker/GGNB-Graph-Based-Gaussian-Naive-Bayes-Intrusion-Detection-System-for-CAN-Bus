#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>


#define nhiddenneurons 50
#define training_samples  57748
using namespace std;





long double out1[training_samples][nhiddenneurons],out2[training_samples][nhiddenneurons];
long double out3[training_samples][nhiddenneurons],out4[training_samples][nhiddenneurons];
long out5[training_samples][2];


vector< vector<long double> > weigh0;
vector< vector<long double> > weigh1;
vector< vector<long double> > weigh2;
vector< vector<long double> > weigh3;
vector< vector<long double> > weigh4;

long double  bia0[nhiddenneurons];
long double  bia1[nhiddenneurons];
long double  bia2[nhiddenneurons];
long double  bia3[nhiddenneurons];
long double  bia4[2];


long double w0[9][50];
long double w1[50][50];
long double w2[50][50];
long double w3[50][50];
long double w4[50][2];




long double input0[training_samples][nhiddenneurons];



long double w0t[50][9];
long double w1t[50][50];
long double w2t[50][50];
long double w3t[50][50];
long double w4t[2][50];

long double training_data[training_samples][9];
int label[training_samples];

long double loss_grad[training_samples][2];


long loss[training_samples];

vector<vector<long double> > vectormatrix(vector<vector<long double> > &a, vector<vector<long double> >&b) {
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

void forwardvectormatrixwithbias() {


	int row1 = training_samples, col2 = 50,col1= 9;

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

	// third layer

	col2 = 50;
	col1= 50;

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=out2[i][k]*w2[k][j];
			}

			out3[i][j] = sum+bia2[j];

			if(out3[i][j]<0) {
				out3[i][j]=0;
			}


		}

	}



	/// 4 th layer

	col2 = 50;
	col1= 50;

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=out3[i][k]*w3[k][j];
			}

			out4[i][j] = sum+bia3[j];

			if(out4[i][j]<0) {
				out4[i][j]=0;
			}


		}

	}


	col2 = 2;
	col1= 50;

	for(int i=0; i<row1; i++) {
		for(int j= 0; j<col2; j++) {

			long double sum =0;

			for(int k=0; k<col1; k++) {
				sum+=out4[i][k]*w4[k][j];
			}

			out5[i][j] = sum+bia4[j];

			if(out5[i][j]<0) {
				out5[i][j]=0;
			}


		}

	}




	//return res;

}





void shape(vector<vector<long double> > b) {
	cout<<"shape\n";
	cout<<b.size()<<endl;
	cout<<b[0].size()<<endl;
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
	
	
	cout<<"jdkfsjdf\n";

}


vector< vector<long double> >  backwardrelu(vector< vector<long double> > &out, vector< vector<long double> > &grad_output) {

	//ofstream myfile ("examp.txt",ios::app);

	for(int i=0; i<out.size(); i++) {
		for(int j =0; j<out[i].size(); j++) {
			if(out[i][j]<=0) {
				grad_output[i][j] =0;
			}
			//myfile<<out[i][j]<<"\t";

		}
		//myfile<<"\n";
	}

	return grad_output;
}

// input0backward(weigh4,bia4,out4,loss_grad);/        weigh , bia4, out, grad_output
void  inputbackward0() {
	
	cout<<"inputObackward\n";

	long double w4t[2][50];
	int neurons = 50;

	for(int i =0; i<neurons; i++) {
		for(int j=0; j<2; j++) {
			w4t[j][i]=w4[i][j];
		}
	}
	
	
   

	for(int i=0; i<training_samples; i++) {
		for(int j= 0; j<neurons; j++) {

			long double sum =0;

			for(int k=0; k<2; k++) {
				sum+=loss_grad[i][k]*w4t[k][j];
			}

			input0[i][j] = sum;

		}
	}
	

	long double out4t[50][training_samples];

	for(int i=0; i<training_samples; i++) {
		for(int j=0; j<neurons; j++) {
			out4t[j][i] = out4[i][j];
		}
	}
	
	
	
	
	long double grad_weights[50][2];
	
	for(int i=0; i<neurons; i++) {
		for(int j= 0; j<2; j++) {

			long double sum =0;

			for(int k=0; k<training_samples; k++) {
				sum+=out4t[i][k]*loss_grad[k][j];
			}

			grad_weights[i][j] = sum;

		}
	}


	//vector< vector<long double> > grad_input = vectormatrix(grad_output,we);
	//vector< vector<long double> > ou = transPosematrix(out);
	//vector< vector<long double> > grad_weights = vectormatrix(ou,grad_output);


	//ofstream myfile ("example1.txt",ios::app);
	cout<<"hmmmsd\n";
//	shape(grad_weights);
//    shape(grad_output);
	long double grad_biases[2];


	//myfile<<"grad_bias"<< grad_output.size()<<"\t"<<grad_output[0].size()<<"\n";
	long double sum0=0,sum1=0;

	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<2; j++) {

			grad_biases[j] = grad_biases[j]+loss_grad[i][j];
		}
	}

	long double ll =0.1;
	//myfile<<"weight"<<"\n";
	for(int i = 0; i<neurons; i++) {
		for(int j =0; j<2; j++) {
			w4[i][j] -=ll*grad_weights[i][j];
		}
	}
	cout<<"I am not suee\n";

	for(int i = 0; i<2; i++) {
		bia4[i]-= ll*grad_biases[i];
    }
    
    

}



void train() {
	forwardvectormatrixwithbias();
	softmax_crossentropy_with_logits(); // loss

	grad_softmax_crossentropy_with_logits();   // loss_grad


	inputbackward0();
//	vector< vector<long double> > input1= backwardrelu(out4,input0);
//	vector< vector<long double> > input2 = backward(weigh3,bia3,out3,input1);
//	vector< vector<long double> > input3= backwardrelu(out3,input2);
//	vector< vector<long double> > input4 = backward(weigh2,bia2,out2,input3);
//	vector< vector<long double> > input5 = backwardrelu(out2,input4);
//	vector< vector<long double> > input6 = backward(weigh1,bia1,out1,input5);
//	vector< vector<long double> > input7 = backward(weigh0,bia0,data1,input6);


	cout<<"hmmmmmmm\n";


}




void assignment(vector<vector<long double> >features,vector<int> labels) {

	for(int i =0; i<training_samples; i++) {
		for(int j=0; j<features[i].size(); j++) {
			training_data[i][j] = features[i][j];
		}
	}

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

	for(int i =0; i<weigh2.size(); i++) {
		for(int j=0; j<weigh2[i].size(); j++) {
			w2[i][j]=weigh2[i][j];
		}
	}

	for(int i =0; i<weigh3.size(); i++) {
		for(int j=0; j<weigh3[i].size(); j++) {
			w3[i][j]=weigh3[i][j];
		}
	}

	for(int i =0; i<weigh4.size(); i++) {
		for(int j=0; j<weigh4[i].size(); j++) {
			w4[i][j]=weigh4[i][j];
		}
	}

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

	vector< vector<long double> > data = Load_State("mixed200222.txt");


	/// weight matrix loading here as per the gaussian distribution

	weigh0 = Load_State("weight0.txt");
	weigh1 = Load_State("weight1.txt");
	weigh2 = Load_State("weight2.txt");
	weigh3 = Load_State("weight3.txt");
	weigh4 = Load_State("weight4.txt");



//	shape(weigh0);
//	shape(weigh1);
//	shape(weigh2);
//	shape(weigh3);
//	shape(weigh4);

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


//	cout<<features.size()<<"\n";
//	cout<<features[0].size()<<endl;

//
//	for (int i = 0; i<10; i++) {
//		for(int j=0; j<features[i].size(); j++) {
//			cout<<features[i][j] <<" ";
//		}
//		cout<<"\n";
//	}


	assignment(features,labels);
	for(int i =0; i<1; i++)
		train();

	cout<<"**********************************************"<<endl;
	//predict(features,labels);



	return 0;

}
