#include <stdio.h>
void fun(int*);
int main(int argc, char** argv)
{
	int a = 4;
	fun(&a);
	fprintf(stderr, "ok %d\n", a);
	return 0;
}
