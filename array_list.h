#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ArrayList
{
	size_t n_elem;
	size_t elem_size;
	size_t block_size;

	void *data;
	
	void (*push_back)(struct ArrayList *, void *);
	void (*pop_back)(struct ArrayList *, void *);
	void (*get)(struct ArrayList *, int, void *);
	void (*set)(struct ArrayList *, int, void *);
	void (*sort)(struct ArrayList *);
	int (*cmp)(const void *, const void *);
	void (*elem_destructor)(void *);
	void (*destructor)(struct ArrayList *);
};

void al_push_back(struct ArrayList *list, void *src);
void al_pop_back(struct ArrayList *list, void *out);
void al_get(struct ArrayList *list, int index, void *out);
void al_set(struct ArrayList *list, int index, void *src);
void al_sort(struct ArrayList *list);
void al_destructor(struct ArrayList *list);

struct ArrayList al_init(size_t size)
{
	struct ArrayList out;
	
	out.n_elem = 0;
	out.elem_size = size;
	out.block_size = 0;
	out.data = NULL;

	out.push_back = al_push_back;
	out.pop_back = al_pop_back;
	out.sort = al_sort;
	out.get = al_get;
	out.set = al_set;
	out.destructor = al_destructor;	
	
	out.cmp = NULL;
	out.elem_destructor = NULL;

	return out;
}

void al_push_back(struct ArrayList *list, void *src)
{
	if(list->n_elem * list->elem_size >= list->block_size)
	{
		if(list->block_size == 0)
			list->block_size = list->elem_size;
		else
			list->block_size *= 2;
	}
	list->data = realloc(list->data, list->block_size);
	if(list->data == NULL)
	{
		fprintf(stderr, "al_push_back: realloc error\n");
		exit(1);
	}
	memcpy(((char *)(list->data)) + (list->elem_size * list->n_elem), src, list->elem_size);  
	list->n_elem++;
}
void al_pop_back(struct ArrayList *list, void *out)
{
	memcpy(out, (char *)(list->data) + (list->elem_size * (list->n_elem-1)), list->elem_size);
	
	if(list->block_size >= 2 * list->elem_size * (list->n_elem-1))
	{
		list->block_size = (list->block_size / list->elem_size) / 2 * list->elem_size;
		if(list->block_size == 0)
		{
			free(list->data);
			list->data = NULL;
		}
		else
		{
			list->data = realloc(list->data, list->block_size);
			if(list->data == NULL)
			{
				fprintf(stderr, "al_pop: realloc error\n");
				exit(1);
			}
		}
	}

	list->n_elem--;
}
void al_sort(struct ArrayList *list)
{
	if(list->cmp == NULL)
	{
		fprintf(stderr, "al_sort: cmp method must be overridden to sort\n");
		exit(4);
	}
	qsort(list->data, list->n_elem, list->elem_size, list->cmp);	
}
void al_destructor(struct ArrayList *list)
{
	size_t i;

	if(list->elem_destructor)
	{
		for(i = 0; i < list->n_elem; i++)
		{
			list->elem_destructor(((char *)(list->data)) + list->elem_size * i);
		}
	}
	free(list->data);
}
void al_get(struct ArrayList *list, int index, void *out)
{
	if(index >= list->n_elem || index < 0)
	{
		fprintf(stderr, "al_get: list index %d out of range\n", index);
		exit(4);
	}
	memcpy(out, (char *)(list->data) + index * list->elem_size, list->elem_size);
}
void al_set(struct ArrayList *list, int index, void *src)
{
	if(index > list->n_elem || index < 0)
	{
		fprintf(stderr, "al_get: list index %d out of range\n", index);
		exit(4);
	}
	else if(index == list->n_elem)
	{
		al_push_back(list, src);
		return;
	}
	if(list->elem_destructor)
	{
		list->elem_destructor(((char *)(list->data)) + index * list->elem_size);
	}
	memcpy(((char *)(list->data)) + index * list->elem_size, src, list->elem_size);
}
