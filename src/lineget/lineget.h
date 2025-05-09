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
# include <termios.h>

typedef struct s_shell		t_shell;
typedef struct s_lineget	t_lineget;

/******************************************************************************/
/* Key handling                                                               */
/******************************************************************************/

/** @brief Keybinding function */
typedef void(*t_keybind_fn)(t_lineget *);

void
lineget_setup_keys(t_lineget *line);
int
lineget_handle_key(t_lineget *line, int c);

typedef struct s_buffer_attr
{
	size_t	start;
	size_t	end;
	int		color;
	int		bold:1;
	int		italic:1;
	int		underline:1;
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
	/** @brief Associated shell session */
	t_shell			*shell;
	/** @brief Input FD */
	int				in_fd;
	/** @brief Output FD */
	int				out_fd;
	/** @brief Getchar function */
	int				(*getc)(int fd);

	/** @brief List of key bindings */
	t_rbtree		keybinds;

	/** @brief User specified prompt */
	char			*prompt;

	/** @brief Input buffer */
	t_buffer		buffer;
	/** @brief Cursor's byte position in the prompt */
	size_t			cursor_index;
	/** @brief Window width */
	size_t			display_width;
	/** @brief Key sequence sliding window */
	unsigned char	sequence[16];
	size_t			sequence_len;

	/** @brief Terminal handling */
	struct termios	tio;
}	t_lineget;

t_lineget
lineget_setup(t_shell *shell);
void
lineget_cleanup(t_lineget *line);

char
*lineget_read(t_lineget *line, char *prompt);

#endif // LINEGET_H
