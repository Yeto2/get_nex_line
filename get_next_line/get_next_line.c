/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yessemna <yessemna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 05:06:30 by yessemna          #+#    #+#             */
/*   Updated: 2023/12/16 19:51:00 by yessemna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	cleener(void *buffer)
{
	free(buffer);
	buffer = NULL;
}

void	ft_bzero(char *s)
{
	int	i;

	if (s == NULL)
		return ;
	i = 0;
	while (s[i])
		s[i++] = 0;
}

char	*set_rest(char *line)
{
	int		i;
	char	*left_c;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == 0 || line[1] == 0)
		return (NULL);
	left_c = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!left_c)
		return (free(left_c), NULL);
	if (*left_c == 0)
	{
		free(left_c);
		left_c = NULL;
	}
	line[i + 1] = 0;
	return (left_c);
}

char	*fill_line_buffer(int fd, char *left_c, char *buffer)
{
	int		readed;
	char	*tmp;

	readed = 1;
	while (readed)
	{
		readed = read(fd, buffer, BUFFER_SIZE);
		if (readed == 0)
			break ;
		if (readed < 0)
		{
			if (left_c)
				ft_bzero(left_c);
			return (NULL);
		}
		buffer[readed] = 0;
		tmp = left_c;
		left_c = ft_strjoin(tmp, buffer);
		cleener(tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	if (ft_strlen(left_c) == 0)
		return (NULL);
	return (left_c);
}

char	*get_next_line(int fd)
{
	static char	*left_c;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0
		|| read(fd, &buffer, 0) == -1
		|| BUFFER_SIZE >= INT_MAX)
	{
		ft_bzero(left_c);
		return (NULL);
	}
	buffer = malloc(sizeof(char *) * BUFFER_SIZE + 1);
	if (buffer == NULL)
	{
		free(buffer);
		return (NULL);
	}
	line = fill_line_buffer(fd, left_c, buffer);
	free(buffer);
	if (!line)
		return (NULL);
	left_c = set_rest(line);
	return (line);
}
