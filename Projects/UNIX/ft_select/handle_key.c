#include "ft_select.h"

static void	move_next(t_select *select)
{
	t_item	*ptr;

	ptr = select->current->next;
	while (ptr->deleted && ptr != select->current)
		ptr = ptr->next;
	select->current = ptr;
}

static void	move_prev(t_select *select)
{
	t_item	*ptr;

	ptr = select->current->prev;
	while (ptr->deleted && ptr != select->current)
		ptr = ptr->prev;
	select->current = ptr;
}

static void	move_right(t_select *select)
{
	int		i;
	t_item	*ptr;

	i = 0;
	ptr = select->current;
	while (i < select->display.items_per_col && ptr != select->items)
	{
		if (ptr->next == select->items)
			break ;
		ptr = ptr->next;
		if (!ptr->deleted)
			i++;
	}
	select->current = ptr;
}

static void	move_left(t_select *select)
{
	int		i;
	t_item	*ptr;

	i = 0;
	ptr = select->current;
	while (i < select->display.items_per_col && ptr != select->items)
	{
		if (ptr->prev == select->items)
			break ;
		ptr = ptr->prev;
		if (!ptr->deleted)
			i++;
	}
	select->current = ptr;
}

void	handle_key(t_select *select, int key)
{
	if (key == KEY_UP)
		move_prev(select);
	else if (key == KEY_DOWN)
		move_next(select);
	else if (key == KEY_LEFT)
		move_left(select);
	else if (key == KEY_RIGHT)
		move_right(select);
	else if (key == KEY_SPACE)
	{
		select->current->selected = !select->current->selected;
		move_next(select);
	}
	else if (key == KEY_DELETE || key == KEY_BACKSPACE)
		delete_item(select, select->current);
	else if (key == KEY_ESCAPE)
		select->running = 0;
	else if (key == KEY_RETURN)
		select->running = 2;
	else if (key == KEY_SIGWINCH)
	{
		get_terminal_size(&select->cols, &select->rows);
	}
	display_list(select);
}
