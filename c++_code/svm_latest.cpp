#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;


const int NUM_FEATURES = 10;
const int training_samples = 57748;
const int testing_samples = 24749;

long double x_test_array[testing_samples][NUM_FEATURES];
int  y_test_label_array[testing_samples];

long double weightsForPredict[NUM_FEATURES];


long double regularization_strength = 10000;
long double learning_rate = 0.000001;

void assignment(vector< vector<long double> > X_test, vector <int> Y_test)
{
		for (int i = 0; i<testing_samples; i++) {
		for(int j=0; j<NUM_FEATURES; j++) {

			x_test_array[i][j] = X_test[i+training_samples][j];

		}

		y_test_label_array[i] = Y_test[i+training_samples];
	
	}
	
}





vector<long double> calculate_cost_gradient(vector<long double> W, vector<long double> X_batch, int Y_batch){

    long double distance,sum=0;
    vector<long double> dw;
    vector<long double> di(NUM_FEATURES);
    
    for(int i=0;i<NUM_FEATURES;i++)
    {
    	sum+= X_batch[i]*W[i];
    
	}
	//sum*= Y_batch;
	
	distance = 1 - sum*Y_batch;
	

    if(distance<0)
    {
        for(int i=0;i<NUM_FEATURES;i++)
       {
    	di[i] = W[i];
	   }
	}
    else{
    	
      for(int i=0;i<NUM_FEATURES;i++)
       {
    	di[i]=W[i] - regularization_strength * Y_batch * X_batch[i];
	   }
    	
    	
	}
	
	
	return di;
 


}


long double  compute_cost(vector<long double> w, vector <vector<long double> >x, vector<int> y){
    //# calculate hinge loss
    
    vector<long double> distance(training_samples);
    long double sum=0;
    
    for(int i =0;i<training_samples;i++)
    {
    	distance[i]=0;
    	sum=0;
    	for(int j=0;j<NUM_FEATURES;j++)
    	{
    	   sum+= x[i][j]*w[j];
	
		}
		distance[i] = 1- sum*y[i];
		if (distance[i]<0)
		{
		   distance[i]=0;
		}
	}
	sum=0;
	for(int i = 0;i<training_samples;i++)
	{
		sum+=distance[i];
	}
	sum=sum/training_samples;
	long double hinge_loss = regularization_strength*sum;
	
	long double cost =0;
	for(int j=0;j<NUM_FEATURES;j++)
	{
		cost+=(w[j]*w[j]);
	}
	cost = 0.5*cost+hinge_loss;
	
	//cout<<"cost===="<<cost<<"\n";

    return cost;

}




vector<long double> sgd(vector< vector<long double> > features, vector<int> labels)
{
    int max_epochs = 5000;
    printf("sgd\n");
    vector<long double> weights(NUM_FEATURES);
    
    
    int nth = 0;
    long double prev_cost = 2.14748e+009;
    cout<<prev_cost<<endl;
    long double cost_threshold = 0.01;  
    vector<long double> ascent;
    
    for(int i =0;i<weights.size();i++)
    {
    	cout<<weights[i]<<"\t";///=0;
	}
    cout<<"\n";
    for(int epoch=1;epoch<=max_epochs;epoch++)
    {
    	
    	for(int i=0;i<training_samples;i++)
    	{
    		int ind = i;
    		vector<long double> data = features[ind];
    		ascent = calculate_cost_gradient(weights, data, labels[ind]);
    		for(int j=0;j<NUM_FEATURES;j++)
            	weights[j] = weights[j]- (learning_rate * ascent[j]);
    		
		}
		
		//compute_cost(weights,features,labels);
		
		int nnnn = pow(2,nth);
		
	   if (epoch == nnnn || epoch == max_epochs - 1){
	   
            long double cost = compute_cost(weights, features, labels);
            cout<<"epoch ==="<<epoch<<"  cost====="<<cost<<endl;
            //# stoppage criterion
            if (fabs(prev_cost - cost) < cost_threshold * prev_cost)
                return weights;
            prev_cost = cost;
            nth += 1;
       }
    	
	}
	
	
	
	return weights;
       

}

int predict(int i)
{
	
	long double sum = 0;
	
	for(int j=0;j<NUM_FEATURES;j++)
    {
        sum+= x_test_array[i][j]*weightsForPredict[j];
	}
	
	if(sum<0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
	
}


void scoring()
{
	
    int scoring = 0;
    int sum =0;
    
    
    for(int i =0;i<testing_samples;i++)
    {
    	sum = predict(i);
    	
    	if(y_test_label_array[i] == sum)
    	{
    		scoring+=1;
		}
    	
	}
	

	
	cout<<endl<<"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"<<endl;
	cout<<(100.0*scoring)/testing_samples<<"\n";

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


long double scaling_std(long double max, long double min, long double values)
{
	long double std;
	std = (values-min)/(max-min);

	
	return std;
	
}















int main() {

	vector< vector<long double> > data = Load_State("mixed200222.txt");
	
	vector< vector<long double> > features;
	
	vector<int> labels;
	
    long double Max[10] ={-100,137.000,197.000000 ,27.000000 ,2.000000 ,27.000000,2.000000,0.125365,0.473688,0.044516};
    long double Min[10] ={-100,10.000000,13.000000,2.000000,0.000000,2.000000,0.000000,0.005385,0.027911,0.001095};
	
//No.	Nodes	Edges	MaximumIndegree	MinimumIndegree	MaximumOutdegree	
//MinimumOutdegree	MedianPagerank	MaximumPagerank	MininumPagerank	StdPageRank	VarPageRank	State	


	for (int i = 0;i<data.size();i++)
	{
		vector<long double> interfeature;
		for(int j=1;j<10;j++)
		{
			interfeature.push_back(scaling_std(Max[j],Min[j],data[i][j]));
		}
		interfeature.push_back(1.00);
		features.push_back(interfeature);
		
		
		if (data[i][12] > 0)
		{
			labels.push_back(1);
		}
		else
		{
			labels.push_back(-1);
		}
		
		
	}
	
	
	cout<<features.size()<<"\n";
	cout<<features[0].size()<<endl;
	
	
	for (int i = 0;i<10;i++)
	{
		for(int j=0;j<features[i].size();j++)
		{
			cout<<features[i][j] <<" ";
		}
		cout<<"\n";
	}
    assignment(features,labels);
    vector<long double> weights = sgd(features,labels);
    
    
    for(int j=0;j<NUM_FEATURES;j++)
    {
    	weightsForPredict[j]=weights[j];
    	cout<<weightsForPredict[j]<<" ";
	}
    
    scoring();
    
    	
	
	return 0;

}


            





