#include <stdio.h>

#include "cpu_test.h"

int main()
{
	printf("Initiating test suite...\n");
	
	test_cpu_restart();
	test_cpu_branch_instructions();
	test_cpu_load_instructions();
	test_cpu_store_instructions();

	printf("Tests completed successfully.\n");
	return 0;
}