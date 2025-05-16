/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <util/util.h>

typedef struct s_getline		t_getline;
typedef struct s_complete_item	t_complete_item;

/******************************************************************************/
/* Handlers definitions                                                       */
/******************************************************************************/

/** @brief Function to read 1 character from stdin */
typedef int						(*t_getc_fn)(
	t_getline *line);
/** @brief Highlighter function */
typedef void					(*t_highlighter_fn)(
	t_getline *line);
/** @brief Function to draw overflow indicator */
typedef void					(*t_overflow_fn)(
	t_getline *line,
	int position);
/** @brief Function to determine word boundaries */
typedef t_u8_iterator			(*t_boundaries_fn)(
	t_getline *line,
	t_u8_iterator position,
	int direction);
/** @brief Function to draw completion items */
typedef void					(*t_comp_draw_item_fn)(
	t_getline *line,
	size_t index,
	const t_complete_item *item);
/** @brief Function to provide completion items */
typedef t_complete_item			*(*t_comp_provider_fn)(
	t_getline *line,
	size_t *word_start,
	size_t *word_end);

/******************************************************************************/
/* Default handlers                                                           */
/******************************************************************************/

/** @brief Default `getc` function */
int
getline_handlers_getc(t_getline *line);
/** @brief `overflow_fn`, draws overflow indicators */
void
getline_handler_overflow(t_getline *line, int right);
/** @brief Finds word boundaries */
t_u8_iterator
getline_handler_word_boundaries(t_getline *line, t_u8_iterator it, int direction);
/** @brief Default completion item draw function */
void
getline_handler_comp_draw_item(
	t_getline *line,
	size_t i,
	const t_complete_item *item);

/**
 * @brief Registers default handlers for the line
 *
 * @param line Getline instance
 */
void
getline_setup_handlers(t_getline *line);
