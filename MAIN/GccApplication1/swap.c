#include <stdio.h>
#include <avr/io.h>
#include "swap.h"

void swap(int *n1s , int *n2s ) {
	int temp;
	temp = *n1s;
	*n1s = *n2s;
	*n2s = temp;

}
