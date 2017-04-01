#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>
#include <algorithm>

using namespace std;
/*
串A,串B，A0-Ai,B0-Bj的LCS记为L[i][j]
采用递归的LCS算法：
当A[i] == B[j]，L[i][j] = L[i-1][j-1]
当A[i] != B[j], L[i][j] = max(L[i-1][j], L[i][j-1])
*/
vector<char> A;
vector<char> B;
map<size_t, map<size_t, int> > L;

int LCS(int i, int j) {
	if (i < 0 || j < 0) {
		return 0;
	}

	if (A[i] == B[j]) {
		return L[i][j] = LCS(i - 1, j - 1) + 1;
	}
	else {
		return L[i][j] = max(LCS(i - 1, j), LCS(i, j - 1));
	}
}

/*
* 假设已知L[i][j]，怎么求L[i+1][j+1]
* 还需要一个条件，就是L[i][j]所对应的的LCS分别在两个串中的下标lastA,lastB
* 且lastA<=i,lastB<=j。
* 那么就可以这样:
* A[i+1]与B中属于范围[lastB+1, j+1]的进行比较，如果找到了第一个匹配位置pos，
* 这个时候LCS[i+1][j+1]=LCS[i][j]+1。
* 同时更新lastA=i+1,lastB=pos来保持循环不变式
*
* 依次可以循环遍历i,j去求所有的L[i][j]
* 在循环当i,j中有一个已经达到最大值时，就停止增长该下标，当两个都达到最大值时就结束。
* 初始：先求出L[0][0]
* 循环：从1开始
* 结束：当i,j都已遍历完结束
*/
void LCS2() {
	int lastA = -1;
	int lastB = -1;
	size_t i = 0;
	size_t j = 0;

	if (A[0] == B[0]) {
		L[0][0] = 1;
		lastA = 0;
		lastB = 0;
	}
	else {
		L[0][0] = 0;
		lastA = -1;
		lastB = -1;
	}

	i++;
	j++;

	for (size_t count = 1; count < max(A.size(), B.size()); count++) {
		//A[i]比较
		bool isFound = false;
		int prevL = L[i - 1][j - 1];
		cout << "********Caculate prev " << prevL << "LCS[" << i << "][" << j << "]" << "last LCS ! "
			<< i - 1 << " " << j - 1 << " " << L[i - 1][j - 1] << endl;

		cout << "Compare A[" << i << "]" << endl;
		for (int idx = lastB + 1; idx <= j; idx++) {
			cout << "lastA = " << lastA << " lastB = " << lastB << " idx = " << idx << endl;
			if (A[i] == B[idx]) {
				L[i][j] = prevL + 1;
				lastA = i;
				lastB = idx;
				isFound = true;
				cout << "Found ! " << i << " " << j << " " << L[i][j] << " new lastA = " << lastA << " new lastB = " << lastB << endl;
				break;
			}
		}

		if (!isFound) {
			//B[j]比较
			cout << "Compare B[" << j << "]" << endl;
			for (int idx = lastA + 1; idx <= i; idx++) {
				cout << "lastA = " << lastA << " lastB = " << lastB << " idx = " << idx << endl;
				if (B[j] == A[idx]) {
					L[i][j] = prevL + 1;
					lastB = j;
					lastA = idx;
					isFound = true;
					cout << "Found ! " << i << " " << j << " " << L[i][j] << " new lastA = " << lastA << " new lastB = " << lastB << endl;
					break;
				}
			}
		}

		if (!isFound) {
			L[i][j] = prevL;
			cout << "Not found set new = " << prevL << endl;;
		}
		
		cout << "nextI " << i << " nextJ " << i << " L " << L[i][j] << endl;

		if (i != A.size() - 1) {
			i++;
		}

		if (j != B.size() - 1) {
			j++;
		}

	}
}

/*
 * 串A，B， P[i][j] 指示了子串A[0...i]与B[0...j]的LCS的最末尾的位置
 * L[i][j]表示该LCS的长度
 *
 * 已知P[i][j] = (x,y), L[i][j] = len(x的范围是[0,j])，如何求P[i][j+1]和L[i][j+1]
 * 
 * 1)情况1：i == x
	在这种情况下，A[i]本身就是LCS的末尾了，所以之后j再增长，LCS都不会变，P[i][j+1] == P[i][j]
   2）情况2：i != x
	在这种情况下，A[i]并不是LCS的末尾，所以在[x+1, i]之间还有可能会有与B[j+1]匹配的
	因此在这个范围内遍历，看是否有B[j+1]相等的
		a）存在相等的位置x'
			记录P[i][j+1] = (x', j+1)
			L[i][j+1] = L[i][j]+1
		b）不存在相等的
			P[i][j+1] = P[i][j]
			L[i][j+1] = L[i][j]

	已知P[i][x]（x属于[0, B.size()]），怎么扩展到P[i+1][x]呢？
	由上面的推断，我们可以在已知P[i][j]的情况下计算P[i][j+1]，现在的问题是如何进入这个计算循环
	
	分析：
	在已知P[i][x]的情况下，我们就已经得知了当i'<= i时的LCS的末尾(a,b)，当扩展到i+1时，应该在此基础
	上去计算LCS了。也就是说这时LCS在B上的最末尾是b，那么计算P[i+1][x]的时候就从b+1开始

	对于P[i+1][x]的计算，首先假设P[i+1][x]的LCS在B的末尾是b
	1） 0<= x' <= b
		在这个范围内，P[i+1][x] == P[i][x]
	2)  b+1 <= x' < B.szie()
		遍历这个范围的时候
		a）A[i+1] != B[x']，则P[i+1][x']=P[i+1][x'-1]
		b)A[i+1] == B[x']， 则P[i+1][x']=
 */
map < int, map<int, pair<int, int> > > P;

void LCS3() {
	int last = -1;

	for (int i = 0; i < A.size(); i++) {
		P[i][-1] = make_pair(-1, -1);
	}

	for (int i = 0; i < A.size(); i++) {
		cout << "计算A串下标为 i = " << i  << " last = " << last << " 时的所有结果" << endl;
		for (int j = 0; j <= last; j++) {
			P[i][j] = P[i - 1][j];
			cout << "***计算前半部分， j = " << j
				<< " P[" << i << "]" << "[" << j << "] = "
				<< P[i][j].first << ", " << P[i][j].second << endl;
		}

		for (int j = last + 1; j < B.size(); j++) {
			cout << "***查找是否相等， j = " << j << endl;
			if (A[i] == B[j]) {
				P[i][j] = make_pair(i, j);
				cout << "******相等， j = " << j 
				<< " P[" << i << "]" << "[" << j << "] = "
					<< P[i][j].first << ", " << P[i][j].second << endl;

				int pos = j;
				for (int j = pos + 1; j < B.size(); j++) {
					P[i][j] = P[i][pos];
					cout << "******继续填充后半部分， j = " << j 
						<< " P[" << i << "]" << "[" << j << "] = "
						<< P[i][j].first << ", " << P[i][j].second << endl;
				}
				break;
			}
			else {
				P[i][j] = P[i][j - 1];
				cout << "******不相等， j = " << j
					<< " P[" << i << "]" << "[" << j << "] = "
					<< P[i][j].first << ", " << P[i][j].second << endl;
			}
		}

		last = P[i][B.size() - 1].second;
		cout << "last is " << last << endl;
	}
}

void LCS4() {
	for (int i = 0; i < A.size(); i++) {
		P[i][-1] = make_pair(-1, -1);
	}

	int lastA = P[0][-1].first;

	for (int i = 0; i < A.size(); i++) {
		cout << "计算 i=" << i << " lastA = " << lastA << endl;
		for (int j = 0; j < B.size(); j++) {
			P[i][j] = P[i][j - 1];
			cout << "***计算 j = " << j  << " lastA = " << lastA<< ", 先给 P[" << i << "][" << j << "] 赋值为 " << P[i][j].first
				<< ", " << P[i][j].second << endl;

			for (int _i = lastA + 1; _i <= i; _i++) {
				if (A[_i] == B[j]) {
					P[i][j] = make_pair(_i, j);
					cout << "******在 _i = " << _i << " 时，找到相等"
						<< "给 P[" << i << "][" << j << "] 赋值为 "
						<< P[i][j].first << ", " << P[i][j].second << endl;
				}
			}

			lastA = P[i][j].first;
		}
	}
}

/*
	L[i][j] =
	1) 情况1 A]i] = B[j]
		L[i-1][j-1] + 1
	2) 情况2 A[i] = B[j]
		max(L[i-1][j], L[i][j-1])
	
	L[i][j-1]与L[i][j]的关系：
	设Li][j-1]的LCS长度为l1
	L[i][j-1]到L[i][j]就多了B[j]这个元素
	如果L[i][j]的LCS的最后一个元素就是由B[j]匹配而得到的


 */
void LCS5() {
	for (int j = 0; j < B.size(); j++) {
		L[-1][j] = 0;
	}

	for (int i = 0; i < A.size(); i++) {
		L[i][-1] = 0;
	}

	L[-1][-1] = 0;

	for (int i = 0; i < A.size(); i++) {
		for (int j = 0; j < B.size(); j++) {
			if (A[i] == B[j]) {
				L[i][j] = L[i - 1][j - 1] + 1;
				P[i][j] = make_pair(i, j);
				cout << "Equal L[" << i << "][" << j << "]" << " = " << L[i][j] << endl;
			}
			else {
				L[i][j] = max(L[i - 1][j], L[i][j - 1]);
				if (L[i - 1][j] > L[i][j - 1]) {
					L[i][j] = L[i - 1][j];
					P[i][j] = P[i - 1][j];
				}
				else {
					L[i][j] = L[i][j - 1];
					//L[i][j] = preLen;
					P[i][j] = P[i][j - 1];
				}
				
				cout << "Not Equal L[" << i << "][" << j << "]" << " = " << L[i][j] << endl;
			}
		}
	}
}

void print(int i, int j) {
	if (i < 0 || j < 0) {
		return;
	}

	print(P[i][j].first - 1, P[i][j].second - 1);

	cout << "A[" << P[i][j].first << "] B[" << P[i][j].second << "] = "  << A[P[i][j].first] << endl;
}

int main() {
	A.push_back('a');
	A.push_back('b');
	A.push_back('c');
	A.push_back('b');
	A.push_back('d');
	A.push_back('a');
	A.push_back('b');

	B.push_back('b');
	B.push_back('d');
	B.push_back('c');
	B.push_back('a');
	B.push_back('b');
	B.push_back('a');

	//A.push_back(1);
	//A.push_back(2);
	//A.push_back(3);
	//A.push_back(4);
	//A.push_back(5);
	//A.push_back(6);

	//B.push_back(3);
	//B.push_back(4);
	//B.push_back(5);
	//B.push_back(8);
	//B.push_back(9);

	/*A.push_back('a');
	A.push_back('a');
	A.push_back('a');
	A.push_back('b');
	A.push_back('a');
	A.push_back('d');
	A.push_back('e');
	A.push_back('c');

	B.push_back('a');
	B.push_back('a');
	B.push_back('b');
	B.push_back('d');
	B.push_back('c');
	B.push_back('w');
	B.push_back('q');*/

	//cout << "LCS IS " << LCS(6, 5) << endl;
	LCS5();
	cout << "LCS is " << L[6][5] << endl;
	print(6, 5);
	/*for (int i = 0; i < A.size(); i++) {
		for (int j = 0; j < B.size(); j++) {
			cout << " P[" << i << "][" << j << "] = " << P[i][j];
		}
		cout << endl;
	}*/
	return 0;
}