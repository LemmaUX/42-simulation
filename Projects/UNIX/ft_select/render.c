#include "ft_select.h"

static void	print_item(t_select *select, t_item *item)
{
	char	*str;
	int		padding;
	int		i;

	if (item->selected && item == select->current)
	{
		tputs(tgetstr("so", NULL), 1, (int (*)(int))putchar);
		tputs(tgetstr("us", NULL), 1, (int (*)(int))putchar);
	}
	else if (item->selected)
		tputs(tgetstr("so", NULL), 1, (int (*)(int))putchar);
	else if (item == select->current)
		tputs(tgetstr("us", NULL), 1, (int (*)(int))putchar);
	str = item->str;
	while (*str)
		ft_putchar(*str++);
	padding = select->display.col_width - ft_strlen(item->str);
	i = 0;
	while (i < padding)
	{
		ft_putchar(' ');
		i++;
	}
	tputs(tgetstr("se", NULL), 1, (int (*)(int))putchar);
	tputs(tgetstr("ue", NULL), 1, (int (*)(int))putchar);
}

void	render_items(t_select *select)
{
	t_item	*ptr;
	int		col;
	int		row;
	int		item_count;

	clear_display(select);
	if (select->display.total_items == 0)
	{
		move_cursor(0, 0);
		ft_putstr("No items");
		return ;
	}
	ptr = select->items;
	col = 0;
	row = 0;
	item_count = 0;
	while (item_count < select->display.total_items && ptr)
	{
		if (!ptr->deleted)
		{
			move_cursor(col * select->display.col_width, row);
			print_item(select, ptr);
			row++;
			if (row >= select->display.items_per_col)
			{
				row = 0;
				col++;
			}
			item_count++;
		}
		ptr = ptr->next;
		if (ptr == select->items)
			break ;
	}
	fflush(stdout);
}

void	display_list(t_select *select)
{
	calculate_layout(select);
	render_items(select);
}
