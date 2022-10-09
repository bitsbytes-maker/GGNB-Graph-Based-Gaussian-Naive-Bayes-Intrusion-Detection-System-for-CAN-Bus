#include<iostream>
#include <vector>
using namespace std;



int main()
{
	
	vector<vector<long double> > matrix;
	
	for(int i=1;i<6;i++)
	{
		for(int j=1;j<6;j++)
		{
			matrix[i-1].push_back(j);
		}
	}
	
	
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			cout<<matrix[i][j]<<" ";
		}
		cout<<"\n";
	}
	
	
	
	
	
	
	return 0;
}
