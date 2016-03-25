#include <iostream>
using namespace std;

int a[10]={100,150,200,250,300,350,400,450,500,550};
int MIN=200;
int MAX=300;

int main(){
	int i,j;
	int t1,t2;
	t1=t2=0;
	for(i=0;i<=8;i++){
		while(a[t1]<a[i]+MIN){
			t1++;
			if(t1>=9){system("pause"); return 0;}
		}
		if(t2<t1)t2=t1;
		while(a[t2+1]<a[i]+MAX){
			t2++;
			if(t2>=9)break;
		}
		for(j=t1;j<=t2;j++){ 
			cout << a[i] << ' ' << a[j] <<endl;
		}
	}
	system("pause");
	return 0;
}