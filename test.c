#include <stdio.h>
#include <stdlib.h>
#include "employee_list.h"

int main(void)
{
	struct EmployeeList list;
	
	list = el_init("Joja corp");

	list.add_employee(&list, "Steve", "Senior programmer", 42);
	list.add_employee(&list, "John", "Junior programmer", 24);
	list.add_employee(&list, "Alex", "CEO", 64);
	list.add_employee(&list, "Stacy", "CEO", 50);
	list.summary(&list);
	
	((struct ArrayList *)&list)->sort((struct ArrayList *)&list);
	printf("\n\n---SORTED BY NAME---\n");
	list.summary(&list);

	list.destructor(&list);

	return 0;
}
