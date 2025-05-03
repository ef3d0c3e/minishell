/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WORDS_H
#define WORDS_H

/**
 * @file Words definitions
 *
 * `cmd arg1$par1 $(subexp)`
 *  |   |   |     ` Subexpr atom ── Word 3 ┐
 *  |   |   ` Parameter atom  ───┐         ├ Word list
 *  |   ` Literal atom  ─────────┴─ Word 2 │
 *  ` Literal atom ──────────────── Word 1 ┘
 */

# include <tokenizer/tokenizer.h>

typedef struct s_ast_node	t_ast_node;
typedef struct s_parser	t_parser;

/******************************************************************************/
/* Atoms                                                                      */
/******************************************************************************/

/** @brief The type of an argument item */
enum e_atom_type
{
	/** @brief Content is to be interpreted as literal text */
	W_LITERAL,
	/** @brief Content is to be interpreted as a variable needing expansion
	 * at run-time */
	W_PARAMETER,
	/** @brief Sub expression, needs evaluation at run-time */
	W_SUBEXPR,
};

/** @brief Data for parameter expansion */
struct s_atom_param
{
	/** @brief Parameter name */
	char			*name;
	/** @brief The special operator: ":-", "-", ":=", "=", ":?", "?", ":+", "+",
	 * "#", "##", "%", "%%", ":offset", ":offset:length" (or NULL) */
	const char		*op;
	/** @brief Content for the special operator */
	char			*word;
	/** @brief Offset for substring */
	size_t			offset;
	/** @brief Length for substring */
	size_t			length;
	/** @brief Set to 1 if atom comes from a simple parameter token */
	int				simple_param;
};

/** @brief A single argument item */
typedef struct s_atom
{
	/** @brief Flags of the source token */
	int						flags;
	/** @brief Type of arugment */
	enum e_atom_type		type;
	union
	{
		/** @brief Text data */
		t_string_buffer		text;
		/** @brief Param data */
		struct s_atom_param	param;
	};
	/** @brief Reference to the next argument, NULL for none */
	struct s_atom			*next;
}	t_atom;

/**
 * @brief Makes a copy of an atom
 *
 * @param in Atom to copy
 *
 * @returns A copy of `in`
 */
t_atom
atom_copy(const t_atom *in);

/**
 * @brief Parses parameter token into parameter atom
 *
 * Attempts to parse the parameter token under cursor as a parameter atom.
 *
 * This function expects the current token to be a parameter token.
 * The parser's cursor will not be advanced by this function.
 *
 * @param parser The parser
 * @param arg Atom to parse into
 */
void
parse_param_atom(t_parser *parser, t_atom *arg);

/******************************************************************************/
/* Words                                                                      */
/******************************************************************************/

/** @brief Data for command arguments */
typedef struct s_word
{
	/** @brief Items in this argument */
	t_atom	*atoms;
	/** @brief Number of items in this argument */
	size_t	natoms;
}	t_word;

/**
 * @brief Pushes current token to word
 *
 * This function does not advance the parser's cursor
 *
 * @param parser Parser to read from
 * @param word Word to push to
 */
void
word_push(t_parser *parser, t_word *word);
/**
 * @brief Frees a word
 *
 * @param word Word to free
 */
void
word_free(t_word *word);
/**
 * @brief Makes a copy of a word
 *
 * @param in Word to copy
 *
 * @returns A copy of `in`
 */
t_word
word_copy(const t_word *in);
/**
 * @brief Prints a word
 *
 * @param depth Depth padding
 * @param word Word to print
 */
void
word_print(size_t depth, const t_word *word);
/**
 * @brief Builds a subword from another word
 *
 * @param word Original word
 * @param range Start and end position in the start and end atoms respectively
 *
 * @retruns A word constructed from `(range[0], range[1])` to
 * `(range[2], range[3])`
 */
t_word
word_sub(t_word *word, const size_t range[4]);
/**
 * @brief Creates a word from a string
 *
 * @param str String to create word from
 * @param flags Word flags, 0 for `FL_NONE`
 *
 * @returns The created word containing a single @ref W_LITERAL @ref t_atom
 */
t_word
word_from_string(const char *str, int flags);
/**
 * @brief Creates a word from an integer
 *
 * @param value Value to create word from
 * @param flags Word flags, 0 for `FL_NONE`
 *
 * @returns The created word containing a single @ref W_LITERAL @ref t_atom
 */
t_word
word_from_int(int value, int flags);
/**
 * @brief Parses word under cursor until word delimited is encountered
 *
 * @param parser Parser to read from
 * @param eat_minus Whether to treat `-` (@ref TOK_MINUS) as an atom
 *
 * @returns The parsed word
 */
t_word
parse_word(t_parser *parser, int eat_minus);

/******************************************************************************/
/* Words list                                                                 */
/******************************************************************************/

/**
 * @brief Frees an array of words
 *
 * @param list The array of words
 * @param size Size of the array
 */
void
wordlist_free(t_word *list, size_t size);
/**
 * @brief Displays an array of words to stderr
 *
 * @param depth Depth padding
 * @param list List of words
 * @param size Of the word list
 */
void
wordlist_print(size_t depth, const t_word *list, size_t size);
/**
 * @brief Pushes the current token to the word list
 *
 * @param parser The parser
 * @param list Pointer to the array of words
 * @param len Pointer to the array's length
 */
void
wordlist_push(t_parser *parser, t_word **list, size_t *len);

#endif // WORDS_H
