#include "ft_select.h"

t_item	*create_item(char *str)
{
	t_item	*item;

	item = (t_item *)malloc(sizeof(t_item));
	if (!item)
		return (NULL);
	item->str = ft_strdup(str);
	if (!item->str)
	{
		free(item);
		return (NULL);
	}
	item->selected = 0;
	item->deleted = 0;
	item->next = NULL;
	item->prev = NULL;
	return (item);
}

void	add_item(t_item **items, t_item *new_item)
{
	t_item	*ptr;

	if (!*items)
	{
		*items = new_item;
		new_item->next = new_item;
		new_item->prev = new_item;
	}
	else
	{
		ptr = *items;
		while (ptr->next != *items)
			ptr = ptr->next;
		ptr->next = new_item;
		new_item->prev = ptr;
		new_item->next = *items;
		(*items)->prev = new_item;
	}
}

void	delete_item(t_select *select, t_item *item)
{
	if (item->deleted)
		return ;
	item->deleted = 1;
	select->display.deleted_count++;
	if (item->next != item && item->next->deleted == 0)
		select->current = item->next;
	else if (item->prev != item && item->prev->deleted == 0)
		select->current = item->prev;
	else
	{
		t_item	*ptr;

		ptr = item->next;
		while (ptr != item && ptr->deleted)
			ptr = ptr->next;
		if (ptr != item)
			select->current = ptr;
		else
			select->running = 0;
	}
}

void	free_items(t_item *items)
{
	t_item	*ptr;
	t_item	*next;

	if (!items)
		return ;
	ptr = items;
	next = ptr->next;
	while (next != items)
	{
		free(ptr->str);
		free(ptr);
		ptr = next;
		next = ptr->next;
	}
	free(ptr->str);
	free(ptr);
}
