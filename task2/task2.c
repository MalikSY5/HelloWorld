#include <stdio.h>
#include <stdlib.h>

/* IMPLEMENT ME: Declare your functions here */
int add (int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

typedef int (*Operation) (int a, int b);
int main (void)
{
	int num1 = 16;
	int num2 = 8;
	/* IMPLEMENT ME: Insert your algorithm here */
	Operation operations[] = {add, subtract, multiply, divide};

	char input; 
	printf("Operand 'a': 16 | Operand 'b': 8\n");
	printf("Specify the operation to perform (0: add | 1: subtract | 2: Multiply | 3: divide | 4: exit): ");
	scanf(" %c", &input);
	int index = input - '0';

	while (index != 4) {
		int result = operations[index](num1, num2);
		printf("x = %d\n", result);

		printf("Operand 'a': 16 | Operand 'b': 8\n");
		printf("Specify the operation to perform (0: add | 1: subtract | 2: Multiply | 3: divide | 4: exit): ");
		scanf(" %c", &input);
		index = input - '0';
	}

	printf("Exiting Program.\n");

	return 0;
}

/* IMPLEMENT ME: Define your functions here */
int add (int a, int b) { printf ("Adding 'a' and 'b'\n"); return a + b; }

int subtract (int a, int b) { printf ("Subtracting 'a' and 'b'\n"); return a - b; }

int divide (int a, int b) { printf ("Dividing 'a' and 'b'\n"); return a / b; }

int multiply (int a, int b) { printf ("Multiplying 'a' and 'b'\n"); return a * b; }