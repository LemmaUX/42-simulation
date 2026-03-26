#include "ft_select.h"

int	get_terminal_size(int *cols, int *rows)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return (-1);
	*cols = ws.ws_col;
	*rows = ws.ws_row;
	return (0);
}

static int	count_items(t_select *select)
{
	t_item	*ptr;
	int		count;

	count = 0;
	ptr = select->items;
	while (ptr)
	{
		if (!ptr->deleted)
			count++;
		ptr = ptr->next;
		if (ptr == select->items)
			break ;
	}
	return (count);
}

static int	get_max_len(t_select *select)
{
	t_item	*ptr;
	int		max_len;
	int		current_len;

	max_len = 0;
	ptr = select->items;
	while (ptr)
	{
		if (!ptr->deleted)
		{
			current_len = ft_strlen(ptr->str);
			if (current_len > max_len)
				max_len = current_len;
		}
		ptr = ptr->next;
		if (ptr == select->items)
			break ;
	}
	return (max_len);
}

void	calculate_layout(t_select *select)
{
	int	max_len;

	select->display.total_items = count_items(select);
	max_len = get_max_len(select);
	select->display.col_width = max_len + 4;
	if (select->display.col_width > select->cols)
		select->display.col_width = select->cols;
	select->display.max_rows = select->rows - 2;
	if (select->display.max_rows < 1)
		select->display.max_rows = 1;
	select->display.max_cols = select->cols / select->display.col_width;
	if (select->display.max_cols < 1)
		select->display.max_cols = 1;
	select->display.items_per_col = (select->display.total_items + select->display.max_cols - 1) / select->display.max_cols;
	if (select->display.items_per_col > select->display.max_rows)
		select->display.items_per_col = select->display.max_rows;
	select->display.cols_needed = (select->display.total_items + select->display.items_per_col - 1) / select->display.items_per_col;
}
