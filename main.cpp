#include <string>
#include <fstream>
#include <cmath>
#include <io.h>
#include <iostream>
using namespace std;

struct species{//种节点
	string code;
	species* next;
	species(){
		code = "";
		next = NULL;
	}
};

struct genus{//属节点
	string name;//属名
	int spe_num;//属内种个数
	species* first_species;
	genus* next;
	genus(){
		name = "";
		spe_num = 0;
		first_species = NULL;
		next = NULL;
	}
};

species* load_species(string file, genus* genus){//读取单个属文件内部种基因数据
	fstream input;
	input.open(file.c_str());
	if(!input){
        return NULL;
	}
	species* head;
	species* t1;
	species* t2;
	int num = 1;
	head = new species;
	t1 = head;
	string temp_sting;
	getline(input,temp_sting);//抛去第一行
	while(getline(input,temp_sting)){
		if(temp_sting[0]!='>'){
			t1->code += temp_sting;
		}
		else{
			num++;
			t2 = new species;
			t1 -> next = t2;
			t1 = t2;
		}
	}
	input.close();
	genus->spe_num = num;
	return head;
}

genus* load(string location){//读取目录下所有属文件
	_finddata_t file;
	long lf;
	if((lf = _findfirst(location.c_str(), &file)) == -1){
		cout << "No such folder!" << endl;
		system("pause");
		exit(-1);
	}
	_findnext(lf, &file);//去除第一个无效".."文件
	genus* head;
	genus* t1;
	genus* t2;
	string folder = location.substr(0, location.length()-3);
	string temp;
	head = new genus;
	t1 = head;
	while(_findnext(lf, &file) == 0){//构建链表并分别进入每个文件
		if(t1->name!=""){
			t2 = new genus;
			t1 -> next = t2;
			t1 = t2;
		}
		temp = folder + file.name;
		t1 -> name = file.name;
		t1 -> first_species = load_species(temp, t1);
		if(t1 -> first_species)
			cout << "succeed in loading  " << file.name << endl;
		else{
			cout << "loading " << file.name << " error" << endl;
			cout << "ignore and continue?(y/n)";
			char res;
			cin >> res;
			if(res == 'y' || res == 'Y')
				t1 -> name = "";
			else{
				cout << "exit" << endl;
				system("pause");
				exit(-1);
			}
		}
	}
	_findclose(lf);
	return head;
}

int check_other_genus(string temp, genus* root, genus* current_genus){
	int flag=0;
	genus* t1;
	species* t2;
	t1=root;
	while(t1!=NULL){
		if(flag==1){
			break;
		}
		if(t1==current_genus){
			t1=t1->next;
			continue;
		}
		t2=t1->first_species;
		while(t2!=NULL){
			if(t2->code.find(temp)!=-1){
				flag=1;
				break;
			}
			t2=t2->next;
		}
		t1=t1->next;
	}
	return flag;
}

void find(genus* root, genus* current_genus){
	int num_need;
	num_need = ceil(current_genus -> spe_num * 0.7);
	species* head;
	species* t1;
	species* t2;
	string temp;
	int count_ok, count_fail, part, i;
	head = current_genus -> first_species;
	t1 = t2 = head;
	while(t1 != NULL){
		part = t1->code.length() / 10;
		for(i = 0; i < part; i = i + 10){
			temp = t1 -> code.substr(i, i + 10);
			count_ok = 0;
			count_fail = 0;
			t2 = head;
			while(t2 != NULL){
				if(t2 -> code.find(temp) != -1){
					count_ok++;
					if(count_ok >= num_need){
						break;
					}
				}
				else{
					count_fail++;
					if(count_fail > current_genus -> spe_num - num_need){
						break;
					}
				}
				t2 = t2 -> next;
			}
			if(count_ok >= num_need && check_other_genus(temp, root, current_genus) != 1){
				cout << "found!" << endl;//次回作战始于处起
			}
		}t1 = t1 -> next;
	}
}

int main(){
	genus* root;
	genus* t;
	root = load("D:\\文档\\fragment\\fasta\\*.*");
	system("pause");
	t = root;
	while(t!=NULL){
		cout << "finding in " << t->name << endl;
		find(root, t);
		t=t->next;
	}
	system("pause");
	return 0;
}