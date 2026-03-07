/* ************************************************************************** */
/*                                                                            */
/*   exec_utils.c - Busqueda de ejecutables en PATH                           */
/*                                                                            */
/*   Cuando el usuario escribe "ls", el shell necesita encontrar              */
/*   "/usr/bin/ls". El proceso:                                               */
/*     1. Si el comando contiene '/' -> es una ruta, usar directamente        */
/*     2. Si no -> buscar en cada directorio de $PATH                         */
/*     3. Verificar con access(path, X_OK) si el archivo es ejecutable        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** find_in_path: Busca el ejecutable en los directorios de PATH.
** Retorna la ruta completa (malloc'd) o NULL si no se encuentra.
*/
static char	*find_in_path(const char *cmd, const char *path_var)
{
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	dirs = ft_split(path_var, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_str_array(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_str_array(dirs);
	return (NULL);
}

/*
** find_executable: Resuelve la ruta de un comando.
*/
char	*find_executable(const char *cmd, t_env *env)
{
	char	*path_var;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_var = env_get(env, "PATH");
	if (!path_var)
		return (NULL);
	return (find_in_path(cmd, path_var));
}
