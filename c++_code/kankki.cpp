#include<iostream>
using namespace std;


#define R 6
#define C 6
#define M 4
#define N 4
#define K 3
#define S 1


int main() {

	for( int e =0; e<R ; e++) {

		for( int f =0; f <C ; f++) {

			for( int r =0; r<K; r++) {

				for(int  s = 0; s <K; s++) {
					
					cout<< "("<<e+r<<f+s<<")" <<"  ";

				}
				
				cout<<endl;
			}
			cout<<"\\\\\\\\\\\\\\\\\pp"<<endl;
		}
	}

	return 0;

}





