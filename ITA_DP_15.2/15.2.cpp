#include <vector>
#include <map>
#include <stdio.h>
#include<stdint.h>

using namespace std;


#define INF 999999

typedef struct {
    uint32_t row;
    uint32_t col;
} ivector;

map<uint32_t, map<uint32_t, uint32_t> > min_cost;
map<uint32_t, map<uint32_t, uint32_t> > pos;
vector< ivector> ivectors;

uint32_t get_min() {
    uint32_t n = ivectors.size() - 1;
    printf("n is %d\n", n);

    /*长度从2开始*/
    for (uint32_t l = 2; l <= n; l++) {
        /*元素下标从1开始*/
        for (uint32_t j = 1; j <= n + 1 - l; j++) {
            /*计算k的位置*/
            uint32_t k = j + l - 1;
            min_cost[j][k] = INF;
            /**/
            for (uint32_t p = j; p < k; p++) {
                uint32_t tmp = min_cost[j][p] + min_cost[p+1][k] + ivectors[j].row * ivectors[p].col * ivectors[k].col;
                printf("l=%u, j=%u, k=%u, p=%u, tmp=%u, %u %u %u\n", l, j, k, p, tmp, ivectors[j].row , ivectors[p].col , ivectors[k].col);
                if (tmp < min_cost[j][k]) {
                    min_cost[j][k] = tmp;
                    pos[j][k] = p;
                }
            }
        }
    }
}

void print(uint32_t j, uint32_t k) {
    if (j > k) {
        return;
    }
    
    if (j == k) {
        printf("A%d", j);
        return;
    }

    printf("(");
    print(j, pos[j][k]);
    print(pos[j][k] + 1, k);
    printf(")");
    return;
}

int main() {
    ivectors.push_back({0, 0});

    ivectors.push_back({30, 35});

    ivectors.push_back({35, 15});

    ivectors.push_back({15, 5});

    ivectors.push_back({5, 10});

    ivectors.push_back({10, 20});

    ivectors.push_back({20, 25});

    uint32_t min = get_min();
    printf("min is %d\n", min_cost[1][6]);

    print(1, 6);
    printf("\n");

    return 0;
}

