#include <fcntl.h>
#include <unistd.h>

void	ft_putstr_fd(char *str, int fd)
{
	while (*str)
	{
		write(fd, str, 1);
		str++;
	}
}

int	main(int argc, char **argv)
{
	int		fd;
	ssize_t	bytes;
	char	buf[4096];

	if (argc < 2)
		return (ft_putstr_fd("File name missing.\n", 2), 1);
	if (argc > 2)
		return (ft_putstr_fd("Too many arguments.\n", 2), 1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	bytes = read(fd, buf, sizeof(buf));
	while (bytes > 0)
	{
		write(1, buf, bytes);
		bytes = read(fd, buf, sizeof(buf));
	}
	close(fd);
	return (0);
}
