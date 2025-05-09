/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GETLINE_H
# define GETLINE_H

# include <util/util.h>
# include <termios.h>

typedef struct s_shell		t_shell;
typedef struct s_getline	t_getline;

/******************************************************************************/
/* Input handling                                                             */
/******************************************************************************/

/** @brief The default `getc` function */
int
getline_getc(t_getline *line);
int
getline_read_char(t_getline *line);

/******************************************************************************/
/* Key handling                                                               */
/******************************************************************************/

/** @brief Keybinding function */
typedef void(*t_keybind_fn)(t_getline *);

void
getline_setup_keys(t_getline *line);
int
getline_handle_key(t_getline *line, int c);

/******************************************************************************/
/* Grapheme handling                                                          */
/******************************************************************************/

/**
 * @brief Represents a single grapheme cluster according to the terminal.
 *
 * It is impossible to perform clustering programmatically since different
 * terminals + font stack will handle unicode differently. So we nicely ask the
 * terminal about the width of our codepoint sequences and trust it.
 *
 * Hence this is called `cluster` because `grapheme` would be incorrect.
 * Even though this is what visually appears on the terminal screen.
 */
typedef struct s_cluster
{
	/** @brief Index in the input line */
	size_t	index;
	/** @brief Size in bytes of the cluster */
	size_t	size;
	/** @brief Visual width (measured) of the cluster */
	int		width;
}	t_cluster;

/******************************************************************************/
/* Buffer management                                                          */
/******************************************************************************/

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
	/** @brief The line's buffer */
	t_string_buffer		buffer;
	/** @brief Attributes buffer */
	struct
	{
		t_buffer_attr	*data;
		size_t			capacity;
		size_t			size;
	}	s_attrs;
}	t_buffer;

t_buffer
getline_buffer_new(void);
void
getline_buffer_insert(t_getline *line, int c);

typedef struct s_getline
{
	/** @brief Associated shell session */
	t_shell			*shell;

	/*-- Input/Output stack --*/
	/** @brief Input FD */
	int				in_fd;
	/** @brief Output FD */
	int				out_fd;
	/** @brief Getchar function */
	int				(*getc)(t_getline *line);
	/** @brief Queed input */
	t_string_buffer	input_queue;

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
}	t_getline;

t_getline
getline_setup(t_shell *shell);
void
getline_cleanup(t_getline *line);

char
*getline_read(t_getline *line, char *prompt);

#endif // GETLINE_H
