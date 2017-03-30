#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>

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
        return L[i][j] = LCS(i-1, j-1) + 1;
    }
    else {
        return L[i][j] = max(LCS(i-1, j), LCS(i, j-1));
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

    for (int count = 1; count < max(A.size(), B.size()); count++) {
        cout << "Caculate LCS[" << i << "][" << j << "]" << endl;
        cout << "last LCS ! " << i - 1 << " " << j - 1 <<  " " << L[i-1][j-1] << endl;
        //A[i]比较
        bool isFound = false;

        cout << "Compare A[" << i << "]" << endl;
        for (int idx = lastB + 1; idx <= j; idx++) {
            cout << "lastA = " << lastA << " lastB = " << lastB << " idx = " << idx << endl;
            if (A[i] == B[idx]) {
                L[i+1][j+1] = L[i][j] + 1;
                lastA = i;
                lastB = idx;
                isFound = true;
                cout << "Found ! " << i << " " << j <<  " " << L[i+1][j+1] << " new lastA = " << lastA << " new lastB = " << lastB << endl;
                break;
            }
        }
        
        if (!isFound) {
            //B[j]比较
            cout << "Compare B[" << j << "]" << endl;
             for (int idx = lastA + 1; idx <= i; idx++) {
                 cout << "lastA = " << lastA << " lastB = " << lastB << " idx = " << idx << endl;
                if (B[j] == B[idx]) {
                    L[i+1][j+1] = L[i][j] + 1;
                    lastB = j;
                    lastA = idx;
                    isFound = true;
                    cout << "Found ! " << i << " " << j <<  " " << L[i+1][j+1] << " new lastA = " << lastA << " new lastB = " << lastB << endl;
                    break;
                }
            }
        }

        if (!isFound) {
            L[i+1][j+1] = L[i][j];
            cout << "Not found set new = " << L[i+1][j+1];
        }

        if (i != A.size() - 1) {
            i++;
        }

        if (j != B.size() - 1) {
            j++;
        }
    }
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

     //cout << "LCS IS " << LCS(6, 5) << endl;
     LCS2();
     cout << "LCS is " << L[6][5] << endl;
 }