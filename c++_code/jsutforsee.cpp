#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

#define Train_Set_Size 20
#define PI 3.141592653589793238463
#define N 5
#define epsilon 0.05
#define epoch 50000

double c[N] = {};
double W[N] = {};
double V[N] = {};
double b = 0;

double sigmoid(double x) {
	return (1.0f / (1.0f + std::exp(-x)));
}

double f_theta(double x) {
	double result = b;
	for (int i = 0; i < N; i++) {
		result += V[i] * sigmoid(c[i] + W[i] * x);
	}
	return result;
}

void train(double x, double y) {
	for (int i = 0; i < N; i++) {
		W[i] = W[i] - epsilon * 2 * (f_theta(x) - y) * V[i] * x * 
               (1 - sigmoid(c[i] + W[i] * x)) * sigmoid(c[i] + W[i] * x);
	}
	for (int i = 0; i < N; i++) {
		V[i] = V[i] - epsilon * 2 * (f_theta(x) - y) * sigmoid(c[i] + W[i] * x);
	}
	b = b - epsilon * 2 * (f_theta(x) - y);
	for (int i = 0; i < N; i++) {
		c[i] = c[i] - epsilon * 2 * (f_theta(x) - y) * V[i] * 
               (1 - sigmoid(c[i] + W[i] * x)) * sigmoid(c[i] + W[i] * x);
	}
}

int main() {
	srand(time(NULL));
	for (int i = 0; i < N; i++) {
		W[i] = 2 * rand() / RAND_MAX -1;
		V[i] = 2 * rand() / RAND_MAX -1;
		c[i] = 2 * rand() / RAND_MAX -1;
	}
	vector<pair<double, double>> trainSet;
	trainSet.resize(Train_Set_Size);

	for (int i = 0; i < Train_Set_Size; i++) {
		trainSet[i] = make_pair(i * 2 * PI / Train_Set_Size, sin(i * 2 * PI / Train_Set_Size));
	}

	for (int j = 0; j < epoch; j++) {
		for (int i = 0; i < Train_Set_Size; i++) {
			train(trainSet[i].first, trainSet[i].second);
		}
		std::cout << j << "\r";
	}

	//Plot the results
	vector<float> x;
	vector<float> y1, y2;

	for (int i = 0; i < 1000; i++) {
		x.push_back(i * 2 * PI / 1000);
		y1.push_back(sin(i * 2 * PI / 1000));
		y2.push_back(f_theta(i * 2 * PI / 1000));
	}

	FILE * gp = _popen("gnuplot", "w");
	fprintf(gp, "set terminal wxt size 600,400 \n");
	fprintf(gp, "set grid \n");
	fprintf(gp, "set title '%s' \n", "f(x) = sin (x)");
	fprintf(gp, "set style line 1 lt 3 pt 7 ps 0.1 lc rgb 'green' lw 1 \n");
	fprintf(gp, "set style line 2 lt 3 pt 7 ps 0.1 lc rgb 'red' lw 1 \n");
	fprintf(gp, "plot '-' w p ls 1, '-' w p ls 2 \n");

	//Exact f(x) = sin(x) -> Green Graph
	for (int k = 0; k < x.size(); k++) {
		fprintf(gp, "%f %f \n", x[k], y1[k]);
	}
	fprintf(gp, "e\n");

	//Neural Network Approximate f(x) = sin(x) -> Red Graph
	for (int k = 0; k < x.size(); k++) {
		fprintf(gp, "%f %f \n", x[k], y2[k]);
	}
	fprintf(gp, "e\n");
	
	fflush(gp);


	system("pause");
	_pclose(gp);
	return 0;
}
