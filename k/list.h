#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/*
 * This list data structure is a verbatim copy from wayland-util.h from the
 * Wayland project; except that wl_ prefix has been removed.
 */

struct list {
	struct list *prev;
	struct list *next;
};

void list_init(struct list *list);
void list_insert(struct list *list, struct list *elm);
void list_remove(struct list *elm);
int list_empty(const struct list *list);

#define container_of(ptr, type, member)					\
	(__typeof__(type) *)((char *)(ptr) -				\
		 offsetof(__typeof__(type), member))

#define list_first_entry(head, pos, member)				\
	container_of((head)->next, __typeof__(*pos), member)

#define list_for_each(pos, head, member)				\
	for (pos = 0, pos = list_first_entry(head, pos, member);	\
	     &pos->member != (head);					\
	     pos = list_first_entry(&pos->member, pos, member))

#define list_for_each_safe(pos, tmp, head, member)			\
	for (pos = 0, tmp = 0, 						\
	     pos = list_first_entry(head, pos, member),			\
	     tmp = list_first_entry(&pos->member, tmp, member);		\
	     &pos->member != (head);					\
	     pos = tmp,							\
	     tmp = list_first_entry(&pos->member, tmp, member))

#endif /* LIST_H */
