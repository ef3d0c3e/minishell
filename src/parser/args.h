/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ARGS_H
#define ARGS_H

# include <tokenizer/tokenizer.h>

typedef struct s_ast_node	t_ast_node;
typedef struct s_parser	t_parser;

/******************************************************************************/
/* Arguments                                                                  */
/******************************************************************************/

/** @brief The type of an argument item */
enum e_arg_type
{
	/** @brief Content is to be interpreted as literal text */
	ARG_LITERAL,
	/** @brief Content is to be interpreted as a variable needing expansion
	 * at run-time */
	ARG_PARAMETER,
	/** @brief Sub expression, needs evaluation at run-time */
	ARG_SUBEXPR,
};

/** @brief Data for parameter expaision */
struct s_arg_param
{
	/** @brief Parameter name */
	char			*name;
	/** @brief The special operator: ":-", "-", ":=", "=", ":?", "?", ":+", "+",
	 * "#", "##", "%", "%%", ":offset", ":offset:length" (or NULL) */
	const char		*op;
	/** @brief Ast for the special operator */
	t_ast_node		*word;
	/** @brief Offset for substring */
	size_t			offset;
	/** @brief Length for substring */
	size_t			length;
};

/** @brief A single argument item */
struct s_arg_item
{
	/** @brief Flags of the source token */
	int						flags;
	/** @brief Type of arugment */
	enum e_arg_type			type;
	union
	{
		/** @brief Text data */
		t_string_buffer		text;
		/** @brief Param data */
		struct s_arg_param	param;
	};
	/** @brief Reference to the next argument, NULL for none */
	struct s_arg_item		*next;
};

/** @brief Data for command arguments */
struct s_argument
{
	/** @brief Items in this argument */
	struct s_arg_item	*items;
	/** @brief Number of items in this argument */
	size_t				nitems;
};

/**
 * @brief Parses parameter token into argument
 *
 * @param parser The parser
 * @param arg Argument to parse into
 */
void
parse_param(t_parser *parser, struct s_arg_item *arg);
/** @brief Pushes a token to an argument */
void
arg_push(t_parser *parser, struct s_argument *arg);
/** @brief Frees an argument structure */
void
arg_free(struct s_argument *arg);
/** @brief Displays an argument to stderr */
void
arg_print(size_t depth, const struct s_argument *arg);
/** @brief Parses a single argument */
struct s_argument
arg_parse(t_parser *parser);

/******************************************************************************/
/* Arguments list                                                             */
/******************************************************************************/

/** @brief Frees an array of arguments */
void
arglist_free(struct s_argument *list, size_t size);
/** @brief Displays an array of arguments to stderr */
void
arglist_print(size_t depth, struct s_argument *list, size_t size);
/**
 * @brief Pushes the current token to the argument list
 *
 * @param parser The parser
 * @param list Pointer to the array of arguments
 * @param len Pointer to the array's length
 */
void
arglist_push(t_parser *parser, struct s_argument **list, size_t *len);

#endif // ARGS_H
