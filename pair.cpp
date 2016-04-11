#include <iostream>
#include <vector>
using namespace std;

class FragmentPair{
public:
    string fragment;
    int posA;
    int posB;
};

class PairInfo{
public:
    int start1;
    int length1;
	int start2;
	int length2;
	PairInfo(int a,int b,int c,int d){
		start1 = a;
		length1 = b;
		start2 = c;
		length2 = d;
	}
};

vector <PairInfo> pair(string target, vector <FragmentPair> in, int MIN, int MAX){
	int num;
	num = in.capacity();
	vector<PairInfo> ans;
	int i, j;
	int t1, t2;
	t1 = t2 = 0;
	for(i = 0; i <= num - 2; i++){
		while(in[t1].posA < in[i].posA + MIN){
			t1++;
			if(t1 >= num - 1)return ans;
		}
		if(t2 < t1)t2 = t1;
		while(in[t2+1].posA < in[i].posA + MAX){
			t2++;
			if(t2 >= 9)break;
		}
		for(j = t1; j <= t2; j++){
			ans.push_back(PairInfo(in[i].posA, in[i].posB, in[j].posA, in[j].posB));
			cout << in[i].posA << ' ' << in[j].posA <<endl;
		}
	}
	return ans;
}

int main(){
	return 0;
}