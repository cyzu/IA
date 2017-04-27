#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int max(int a, int b){
	if (a < b) return b;
	else return a;
}

int min(int a, int b){
	if (a < b) return a;
	else return b;
}

int difference(int a, int b){
	int m, n;
	m = max(a, b);
	n = min(a, b);
	
	return (m-n);
}
