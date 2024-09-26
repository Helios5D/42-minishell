/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 14:42:11 by mrochedy          #+#    #+#             */
/*   Updated: 2024/07/27 15:54:17 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 64
#endif

/**
 * @brief Reads the file pointed to by the file descriptor
 *
 * This function entirely reads and returns the content of the file
 * pointed to by the file descriptor as a freeable string
 *
 * @param fd The file descriptor pointing to the file we want to read
 * @return String containing all the content of the file read
 */
char	*ft_read_fd(int fd)
{
	int		i;
	int		read_val;
	int		str_len;
	char	*str;
	char	buf[BUFFER_SIZE + 1];

	str = malloc(sizeof(char));
	if (!str)
		return (NULL);
	str[0] = '\0';
	read_val = read(fd, buf, BUFFER_SIZE);
	while (read_val > 0)
	{
		buf[read_val] = '\0';
		str_len = ft_strlen(str);
		str = ft_realloc(str, str_len + 1, str_len + read_val + 1);
		if (!str)
			return (NULL);
		i = -1;
		while (buf[++i])
			str[str_len + i] = buf[i];
		str[str_len + i] = '\0';
		read_val = read(fd, buf, BUFFER_SIZE);
	}
	return (str);
}
