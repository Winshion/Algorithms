#include<cstdio>
#include<iostream>
#include<vector>
#include<Windows.h>
using namespace std;

bool checkFinish(vector<bool> finish) {
	// 检查finish向量是不是全true
	// 只要有false，就返回true
	for (int i = 0; i < finish.size(); i++)
		if (finish[i] == false) return true;
	return false;
}

bool checkAvai(vector<int> arr, vector<int> available, bool b) {
	// 检查我可不可以给当前进程分配资源
	if (b) return false;
	for (int i = 0; i < available.size(); i++)
		if (available[i] < arr[i])	// 只要一不够
			return false;			// 就返回false
	return true;
}

vector<int> operator+(vector<int>& vec1, vector<int>& vec2) {
	vector<int> ret;
	for (int i = 0; i < min(vec1.size(), vec2.size()); i++)
		ret.push_back(vec1[i] + vec2[i]);
	return ret;
}

vector<int> operator-(vector<int>& vec1, vector<int>& vec2) {
	vector<int> ret;
	for (int i = 0; i < min(vec1.size(), vec2.size()); i++)
		ret.push_back(vec1[i] - vec2[i]);
	return ret;
}

template<typename E>
void show(E mat, string name) {
	cout << "-----------" << name << "-----------" << endl;
	for (int i = 0; i < mat.size(); i++) {
		for (int j = 0; j < mat[0].size(); j++) {
			printf("%3d\t|", mat[i][j]);
		}
		cout << endl;
	}
	cout << "-----------" << name << "-----------" << endl;
}

vector<int> bnkrAlg(vector<vector<int>>& allocation, vector<vector<int>>& mmax,
	vector<int>& available, int proc_num, int rsc_num) {
	vector<vector<int>> Need;
	vector<int> work;
	vector<int> result;
	for (int i = 0; i < proc_num; i++)
		Need.push_back(mmax[i] - allocation[i]);	// 计算Need矩阵
	show<vector<vector<int>>>(Need, "Need");
	vector<bool> finish;
	for (int i = 0; i < proc_num; i++)
		finish.push_back(false);	// 构造finish向量
	for (int cnt = 0; cnt < proc_num; cnt++) {
		// 按照道理，若存在死锁，则进行n次（n为进程数）后就能检测出来。
		// 这一部分开始,贷款
		work = available;
		cout << "Resources available now: " << endl;
		for (int i = 0; i < work.size(); i++) cout << work[i] << '\t';
		cout << endl;
		for (int i = 0; i < proc_num; i++) {
			// 进行一次贷款轮回
			if (checkAvai(Need[i], work, finish[i])) {	// 如果当前资源可供分配给这个未完成的进程
				work = work - Need[i];
				available = available - Need[i];
				available = available + mmax[i];
				cout << "Carry out proccess" << i << endl;
				finish[i] = true;
				result.push_back(i);
			}
		}
	}
	if (checkFinish(finish))
		cout << "Unsafe state." << endl;
	else cout << "Safe state." << endl;
	return result;
}

int main() {
	vector<vector<int>> allocation, mmax;
	vector<int> available;
	int proc_num, rsc_num;
	int temp;	// 输入数组使用
	vector<int> v;	// 输入数组使用
	cout << "Input the number of proccesses." << endl;
	cin >> proc_num;
	cout << "Input the number of resources." << endl;
	cin >> rsc_num;
	cout << "Please input the matrix \"Max\"" << " ("
		<< proc_num << 'x' << rsc_num << ")" << endl;
	
	for (int i = 0; i < proc_num; i++) {
		v.clear();
		for (int j = 0; j < rsc_num; j++) {
			cin >> temp;
			v.push_back(temp);
		}
		mmax.push_back(v);
	}
	cout << "Please input the matrix \"Allocation\"" << " ("
		<< proc_num << 'x' << rsc_num << ")" << endl;
	for (int i = 0; i < proc_num; i++) {
		v.clear();
		for (int j = 0; j < rsc_num; j++) {
			cin >> temp;
			v.push_back(temp);
		}
		allocation.push_back(v);
	}
	cout << "Please input the vector \"available\" ("
		<< rsc_num << "x1)" << endl;
	for (int i = 0; i < rsc_num; i++) {
		cin >> temp;
		available.push_back(temp);
	}
	system("cls");
	show<vector<vector<int>>>(mmax, "Max");
	show<vector<vector<int>>>(allocation, "Allocation");
	vector<int>result = 
		bnkrAlg(allocation, mmax, available, proc_num, rsc_num);
	cout << "Sequences: ";
	for (int i = 0; i < result.size(); i++)
		cout << "P" << result[i] << " ";
	cout << endl;
	system("pause");
	return 0;
}
