// 测试call指令
// 191180028 刁培杰

#include "trap.h"

int add(int a, int b){
	return a + b;
}
int sub(int a, int b){
	return a - b;
}
int mul(int a, int b){
	return a * b;
}
int div(int a, int b){
	return a / b;
}

static struct{
	int (*function)(int a, int b);
}functions[] = {
	{&add},
	{&sub},
	{&mul},
	{&div},
};

int main(){
	int a = 1, b = 2;
	int i;
	int ans[] = {3, -1, 2, 0};
	for(i = 0; i < sizeof(functions)/sizeof(functions[0]); i++){
		nemu_assert(functions[i].function(a, b) == ans[i]);
	}
	nemu_assert(add(a, b) == 3);
	nemu_assert(sub(a, b) == -1);
	nemu_assert(mul(a, b) == 2);
	nemu_assert(div(a, b) == 0);
	HIT_GOOD_TRAP;
	return 0;
}