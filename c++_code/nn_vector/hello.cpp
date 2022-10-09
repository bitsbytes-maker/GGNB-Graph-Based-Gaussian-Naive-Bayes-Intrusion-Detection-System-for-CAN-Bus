#include<iostream>
using namespace std;

int a=3,b =2;

void add(int a,int b)
{
	a=a+2;
	b=b+4;
}

int main()
{
	add(a,b);
	
	cout<<a<<"\t"<<b<<endl;

	
	return 0;
}
