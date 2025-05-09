/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lineget.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LINEGET_H
# define LINEGET_H

# include <util/util.h>

typedef struct s_buffer_attr
{
	size_t	start;
	size_t	end;
	int		color;

}	t_buffer_attr;

typedef struct s_buffer
{
	/** @brief Raw line content */
	const char			*line;
	/** @brief Line size */
	size_t				size;
	/** @brief Line capacity */
	size_t				capacity;
	/** @brief Attributes buffer */
	struct
	{
		t_buffer_attr	*data;
		size_t			capacity;
		size_t			size;
	}	s_attrs;
}	t_buffer;

t_buffer
lineget_buffer_new(void);

typedef struct s_lineget
{
	/** @brief Input FD */
	int			in_fd;
	/** @brief Output FD */
	int			out_fd;
	/** @brief Getchar function */
	int			(*getc)(int fd);

	char		*prompt;
	t_buffer	buffer;
}	t_lineget;

t_lineget
lineget_setup(void);
void
lineget_cleanup(t_lineget *line);

char
*lineget_read(t_lineget *line, char *prompt);

#endif // LINEGET_H
