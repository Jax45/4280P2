#include <stdio.h>

int x = 0;

int main(){
	int a = 0;
	scanf("%d", &a);
	if (a >= 1){
		goto countdown;
	}
	goto exit;
	decrement:
	printf("%d\n", a);
	a = a - 1;
	goto countdown;
	countdown:
	while (a != 0){
		goto decrement;
	}
	goto exit;
	exit:
	return 0;
}