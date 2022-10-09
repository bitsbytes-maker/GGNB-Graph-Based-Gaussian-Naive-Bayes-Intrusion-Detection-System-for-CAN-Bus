#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>


#define nhiddenneurons 50
#define training_samples  57748
using namespace std;





vector<vector <long double> > out1,out2,out3,out4,out5;


vector< vector<long double> > weigh0;
vector< vector<long double> > weigh1;
vector< vector<long double> > weigh2;
vector< vector<long double> > weigh3;
vector< vector<long double> > weigh4;

vector<long double>  bia0(nhiddenneurons);
vector<long double>  bia1(nhiddenneurons);
vector<long double>  bia2(nhiddenneurons);
vector<long double>  bia3(nhiddenneurons);
vector<long double>  bia4(2);



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


vector<vector<long double> > transPosematrix(vector<vector<long double> > &t) {
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



void shape(vector<vector<long double> > b) {
	cout<<"shape\n";
	cout<<b.size()<<endl;
	cout<<b[0].size()<<endl;
}
vector< vector<long double> > forward(vector< vector<long double> > data,vector<int> labels) {

	cout<<"I am in forward\n";
	out1 = vectormatrixwithbias(data,weigh0,bia0);
	//shape(out1);
	out2 = vectormatrixwithbias(out1,weigh1,bia1);
	//shape(out2);
	out3 = vectormatrixwithbias(out2,weigh2,bia2);
	//shape(out3);
	out4 = vectormatrixwithbias(out3,weigh3,bia3);
	//shape(out4);
	out5 = vectormatrixwithbias(out4,weigh4,bia4);
	//shape(out5);


	return out5;
}


vector<long double> softmax_crossentropy_with_logits(vector< vector<long double> > logits,vector<int> labels) {
	vector<long double> logits_for_answers;
	vector<long double>  logits_exp_sum_diff;

	for(int i=0; i<logits.size(); i++) {
		long double iterator;
		int ind = labels[i];


		///  - logits_for_answers + part
		iterator=-logits[i][ind]+log(exp(logits[i][0])+exp(logits[i][1]));

		logits_exp_sum_diff.push_back(iterator);
	}

	return logits_exp_sum_diff;

}

vector<vector<long double> >  grad_softmax_crossentropy_with_logits(vector< vector<long double> > logits,vector<int> labels) {


	///long double softmax_for_answers[training_samples][2];
	vector<vector<long double> > softmax_for_answers( training_samples , vector<long double> (2, 0));
	cout<<"I am loss\n";
	for(int i =0; i<training_samples; i++) {
		long double sum0,sum1;

		long double exp_scale =  exp(logits[i][0])+  exp(logits[i][1]);
		sum0 = exp(logits[i][0])/exp_scale;
		sum1 = exp(logits[i][1])/exp_scale;

		int iblind = labels[i];
		if(iblind == 0) {

			sum0 = (-1 + sum0)/training_samples;
			sum1 = sum1/training_samples;
		} else {
			sum0 = (0 + sum0)/training_samples;
			sum1 = (-1+sum1)/training_samples;

		}

		softmax_for_answers[i][0] = sum0;
		softmax_for_answers[i][1] = sum1;

//		if(i<10) {
//			cout<<sum0<<"\t"<<sum1<<endl;
//		}
	}


	return softmax_for_answers;


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

// weigh , bia4, out, grad_output
vector< vector<long double> >  backward(vector< vector<long double> > &weigh,vector<long double> &bia,vector< vector<long double> > out,vector< vector<long double> > grad_output) {
	
	vector< vector<long double> >  we = transPosematrix(weigh);
	vector< vector<long double> > grad_input = vectormatrix(grad_output,we);
	vector< vector<long double> > ou = transPosematrix(out);
	vector< vector<long double> > grad_weights = vectormatrix(ou,grad_output);


	//ofstream myfile ("example1.txt",ios::app);
	cout<<"hmmmsd\n";
//	shape(grad_weights);
//    shape(grad_output);
	vector<long double> grad_biases(grad_output[0].size());


	//myfile<<"grad_bias"<< grad_output.size()<<"\t"<<grad_output[0].size()<<"\n";
	long double sum0=0,sum1=0;

	for(int i =0; i<grad_output.size(); i++) {
		for(int j=0; j<grad_output[i].size(); j++) {

			grad_biases[j] = grad_biases[j]+grad_output[i][j];
			//myfile<<i<<"\t"<<j<<"\n";
		}
	}

//	for(int i = 0; i<grad_biases.size(); i++) {
//		grad_biases[i] /= grad_output.size();
//		//grad_biases[i] *= out.size();
//	}

    long double ll =0.1;
	//myfile<<"weight"<<"\n";
	for(int i = 0; i<weigh.size(); i++) {
		for(int j =0; j<weigh[i].size(); j++) {
			weigh[i][j] -=ll*grad_weights[i][j];
	//		myfile<< weigh[i][j]<<"\t";
		}
	//	myfile<<"\n";
	}
	//myfile<<"bias"<<"\n";
    //cout<<"shaper of bia and gradbia\n";
    //cout<<bia.size()<<endl;
    //cout<<grad_biases.size()<<endl;
	for(int i = 0; i<bia.size(); i++) {
		bia[i]-= ll*grad_biases[i];

	//	myfile<<bia[i]<<"\t";

	}
	//myfile<<"\n";

	return grad_input;

}


void train(vector< vector<long double> > data,vector<int> labels) {
   
    vector< vector<long double> > data1;
    
    for(int i =0;i<training_samples;i++)
    {
    	vector<long double> interdata = data[i];
    	data1.push_back(interdata);
	}
       
        
	vector< vector<long double> > logits = forward(data1,labels);
	vector<long double> loss = softmax_crossentropy_with_logits(logits,labels);

	vector< vector<long double> > loss_grad = grad_softmax_crossentropy_with_logits(logits,labels);

//	shape(loss_grad);
	vector< vector<long double> > input0= backward(weigh4,bia4,out4,loss_grad);
	vector< vector<long double> > input1= backwardrelu(out4,input0);
	vector< vector<long double> > input2 = backward(weigh3,bia3,out3,input1);
	vector< vector<long double> > input3= backwardrelu(out3,input2);
	vector< vector<long double> > input4 = backward(weigh2,bia2,out2,input3);
	vector< vector<long double> > input5 = backwardrelu(out2,input4);
	vector< vector<long double> > input6 = backward(weigh1,bia1,out1,input5);
	vector< vector<long double> > input7 = backward(weigh0,bia0,data1,input6);
	
	
	cout<<"hmmmmmmm\n";


}



void predict(vector< vector<long double> > features, vector<int> labels)
{
	int score = 0;
	//shape(out4);

	for(int i=57748+12374;i<features.size();i++)
	{
		vector <vector<long double> > lop;
		vector<long double> play_it;
		int index =0, maxim = 0;
		
		for(int j=0;j<features[i].size();j++)
		{
			play_it.push_back(features[i][j]);
		}
		lop.push_back(play_it);
		
		vector<vector <long double> > outp1 = vectormatrixwithbias(lop,weigh0,bia0);
		//shape(out1);
		vector<vector <long double> > outp2 = vectormatrixwithbias(outp1,weigh1,bia1);
		//shape(out2);
		vector<vector <long double> > outp3 = vectormatrixwithbias(outp2,weigh2,bia2);
		//shape(out3);
		vector<vector <long double> > outp4 = vectormatrixwithbias(outp3,weigh3,bia3);
		//shape(out4);
		vector<vector <long double> > outp5 = vectormatrixwithbias(outp4,weigh4,bia4);
		
	
	    
	    index = 0; maxim = outp5[0][0];
	    
	    for(int k =0;k<2;k++)
	    {
	    	if(outp5[0][k] > maxim)
	    	{
	    		index = k;
	    		maxim = outp5[0][k];
			}
		}
		
		if(labels[i] == index)
		{
			score+=1;
		}

	
	}


	cout << "You got " << (100.0*score)/12375 << " correct" << endl;

	
}



void biasassignment(int r, vector<long double>  bia ) {
	for(int i =0; i<r; i++) {
		bia[i] = 0;
	}
}


void assignment() {
	biasassignment(nhiddenneurons,bia0);
	biasassignment(nhiddenneurons,bia1);
	biasassignment(nhiddenneurons,bia2);
	biasassignment(nhiddenneurons,bia3);
	biasassignment(2,bia4);

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



	for(int i =0;i<140;i++)
		train(features,labels);
		
	cout<<"**********************************************"<<endl;
	predict(features,labels);



	return 0;

}
