
#include <iostream>
#include <vector>
#include<math.h>
 
using namespace std;
 
// Activation function and its derivative


static const int numInputs = 2;
static const int numHiddenNodes = 2;
static const int numOutputs = 1;
double hiddenLayer[numHiddenNodes];
double outputLayer[numOutputs];
double hiddenLayerBias[numHiddenNodes];
double outputLayerBias[numOutputs];
double hiddenWeights[numInputs][numHiddenNodes];
double outputWeights[numHiddenNodes][numOutputs];

double sigmoid(double x) 
{ return 1 / (1 + exp(-x)); }
double dSigmoid(double x)
{ return (x) * (1 — x); }
double init_weight() 
{ return ((double)rand())/((double)RAND_MAX); }
