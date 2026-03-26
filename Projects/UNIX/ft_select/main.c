#include "ft_select.h"

t_select	*g_select = NULL;

static void	init_select(t_select *select, int argc, char **argv)
{
	int	i;

	select->items = NULL;
	select->current = NULL;
	select->running = 1;
	select->display.deleted_count = 0;
	get_terminal_size(&select->cols, &select->rows);
	i = 0;
	while (i < argc)
	{
		add_item(&select->items, create_item(argv[i]));
		if (!select->current)
			select->current = select->items;
		i++;
	}
	select->display.total_items = argc;
}

static void	output_selection(t_select *select)
{
	t_item	*ptr;
	int		first;

	ptr = select->items;
	first = 1;
	while (ptr)
	{
		if (ptr->selected && !ptr->deleted)
		{
			if (!first)
				ft_putchar(' ');
			ft_putstr(ptr->str);
			first = 0;
		}
		ptr = ptr->next;
		if (ptr == select->items)
			break ;
	}
	if (!first)
		ft_putchar('\n');
}

int	main(int argc, char **argv)
{
	t_select	select;
	int			key;
	char		buffer[100];

	if (argc < 2)
		return (1);
	g_select = &select;
	if (tgetent(buffer, getenv("TERM")) < 1)
		return (1);
	init_select(&select, argc - 1, argv + 1);
	setup_terminal();
	setup_signals();
	display_list(&select);
	while (select.running == 1)
	{
		key = read_key();
		if (key >= 0)
			handle_key(&select, key);
	}
	restore_terminal();
	if (select.running == 2)
		output_selection(&select);
	free_items(select.items);
	return (0);
}
