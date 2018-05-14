/*
 * This list data structure is verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

#include "list.h"

void list_init(struct list *list)
{
	list->prev = list;
	list->next = list;
}

void list_insert(struct list *list, struct list *elm)
{
	elm->prev = list;
	elm->next = list->next;
	list->next = elm;
	elm->next->prev = elm;
}

void list_remove(struct list *elm)
{
	elm->prev->next = elm->next;
	elm->next->prev = elm->prev;
	elm->next = NULL;
	elm->prev = NULL;
}

int list_empty(const struct list *list)
{
	return list->next == list;
}
