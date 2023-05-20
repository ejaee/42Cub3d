/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: choiejae <choiejae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:31 by choiejae          #+#    #+#             */
/*   Updated: 2023/05/20 15:46:50 by choiejae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	is_newline(char *s)
{
	int	idx_nl;

	idx_nl = -1;
	while (s[++idx_nl])
	{
		if (s[idx_nl] == '\n')
			return (idx_nl);
	}
	return (-1);
}

static char	*split_to_line(char **backup, char *buf)
{
	char	*line;
	char	*temp_new_backup;
	int		idx_nl;

	line = NULL;
	idx_nl = is_newline(*backup);
	if (idx_nl == -1)
	{
		if (*backup[0] != '\0')
			line = gnl_strndup(*backup, 0, gnl_strlen(*backup));
		free(*backup);
		*backup = NULL;
	}
	else
	{
		line = gnl_strndup(*backup, 0, idx_nl + 1);
		temp_new_backup = \
		gnl_strndup(*backup, idx_nl + 1, gnl_strlen(*backup) - idx_nl);
		free(*backup);
		*backup = temp_new_backup;
	}
	free(buf);
	return (line);
}

static char	*get_line(int fd, char **backup, char *buf)
{
	char	*new_backup;
	int		read_byte;
	int		idx_next;

	read_byte = read(fd, buf, BUFFER_SIZE);
	while (read_byte > 0)
	{
		buf[read_byte] = '\0';
		new_backup = gnl_strjoin(*backup, buf);
		free(*backup);
		*backup = new_backup;
		idx_next = is_newline(*backup);
		if (idx_next != -1)
			return (split_to_line(backup, buf));
		read_byte = read(fd, buf, BUFFER_SIZE);
	}
	return (split_to_line(backup, buf));
}

char	*get_next_line(int fd)
{
	char		*buf;
	static char	*backup;

	buf = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (buf == NULL)
		return (NULL);
	if (BUFFER_SIZE <= 0 && read(fd, buf, 0) == -1)
	{
		free(buf);
		return (NULL);
	}
	if (backup != NULL && is_newline(backup) != -1)
		return (split_to_line(&backup, buf));
	if (backup == NULL)
		backup = gnl_strndup("", 0, 0);
	return (get_line(fd, &backup, buf));
}
