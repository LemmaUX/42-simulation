#include "get_next_line.h"

static char	*read_until_newline(int fd, char *stash)
{
	char	*buffer;
	char	*tmp;
	ssize_t	bytes_read;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr_gnl(stash, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin_gnl(stash, buffer);
		free(stash);
		stash = tmp;
		if (!stash)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (stash);
}

static char	*extract_line(char *stash)
{
	size_t	index;

	if (!stash || !stash[0])
		return (NULL);
	index = 0;
	while (stash[index] && stash[index] != '\n')
		index++;
	if (stash[index] == '\n')
		index++;
	return (ft_substr_gnl(stash, 0, index));
}

static char	*update_stash(char *stash)
{
	size_t	index;
	char	*new_stash;

	if (!stash)
		return (NULL);
	index = 0;
	while (stash[index] && stash[index] != '\n')
		index++;
	if (!stash[index])
	{
		free(stash);
		return (NULL);
	}
	new_stash = ft_strdup_gnl(stash + index + 1);
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_until_newline(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = update_stash(stash);
	return (line);
}
