#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array_list.h"

struct Employee
{
	char *name;
	char *position;
	int age;
};

struct EmployeeList
{
	struct ArrayList base;
	
	char *company_name;

	void (*add_employee)(struct EmployeeList *, char *, char *, int);
	void (*summary)(struct EmployeeList *);
	void (*destructor)(struct EmployeeList *);
};

/*ArrayList overrides*/
void employee_destructor(void *dst);
int el_cmp(const void *a, const void *b);

/*New methods*/
void el_add_employee(struct EmployeeList *list, char *name, char *position, int age);
void el_summary(struct EmployeeList *list);
void el_destructor(struct EmployeeList *list);

struct EmployeeList el_init(char *company_name)
{
	struct EmployeeList out;
	
	out.base = al_init(sizeof(struct Employee));
	((struct ArrayList *)&out)->cmp = el_cmp;
	((struct ArrayList *)&out)->elem_destructor = employee_destructor;
	
	out.add_employee = el_add_employee;
	out.summary = el_summary;
	out.destructor = el_destructor;
	out.company_name = malloc(strlen(company_name) + 1);
	if(out.company_name == NULL)
	{
		fprintf(stderr, "el_init: malloc error\n");
		exit(4);
	}
	strcpy(out.company_name, company_name);
	return out;
}

void employee_destructor(void *dst)
{
	struct Employee *employee = dst;

	free(employee->name);
	free(employee->position);
}
void el_destructor(struct EmployeeList *list)
{
	free(list->company_name);
	((struct ArrayList *)list)->destructor((struct ArrayList *)list);
}
void el_add_employee(struct EmployeeList *list, char *name, char *position, int age)
{
	struct Employee new_employee;

	new_employee.name = malloc(strlen(name) + 1);
	if(new_employee.name == NULL)
	{
		fprintf(stderr, "add_employee: malloc error\n");
		exit(1);
	}
	new_employee.position = malloc(strlen(position) + 1);
	if(new_employee.position == NULL)
	{
		fprintf(stderr, "add_employee: malloc error\n");
		exit(1);
	}
	strcpy(new_employee.name, name);
	strcpy(new_employee.position, position);
	new_employee.age = age;
	((struct ArrayList *)list)->push_back((struct ArrayList *)list, &new_employee);
}
int el_cmp(const void *a, const void *b)
{
	struct Employee *a_i;
	struct Employee *b_i;

	a_i = (struct Employee *)a;
	b_i = (struct Employee *)b;

	return strcmp(a_i->name, b_i->name);
}
void el_summary(struct EmployeeList *list)
{
	size_t i;

	printf("Company: %s\n", list->company_name);
	for(i = 0; i < ((struct ArrayList *)list)->n_elem; i++)
	{
		struct Employee employee;

		((struct ArrayList *)list)->get((struct ArrayList *)list, i, &employee);
		printf("\nName: %s\nPosition: %s\nAge: %d\n", employee.name, employee.position, employee.age);
	}
}
