#include <stdio.h>

int loop0() {
    static int i = 0;
    for (; i < 10;) {
        printf("This is %d\n", i);
        return i++;
    }
}

/*
 * 如果是协程，return i，应该改为yield i，并且，第二次调用应该直接从yield i之后运行
 * 使用goto可以实现
 */
 int loop1() {
    static int i = 0;
    static int flag = 0;
    
    switch(flag) {
        case 0: goto lable0;
        case 1: goto lable1;
        default: return 0;
    }

lable0:
    flag = 1;
    for (i = 0; i < 10; i++) {
        printf("This is %d\n", i);
        return i;
lable1:
        ;
    }
}

/*
 * 使用宏定义之后
 */

#define Begin do { \
    static int flag = 0; \
    switch(flag) { \
    case 0: flag = 1; goto lable0; \
    case 1: goto lable1; \
    default: return 0; \
    } \
} while(0); lable0:

#define Yield(x) do { \
    return x; \
} while(0); lable1:

int loop2() {
    static int i = 0;
    Begin;
for (i = 0; i < 10; i++) {
    printf("This is %d\n", i);
    Yield(i);
}
}

int main() {
    loop2();
    loop2();
    loop2();
    loop2();

    return 0;
}

