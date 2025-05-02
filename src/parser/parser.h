/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_H
# define PARSER_H

# include <parser/words/words.h>
# include <parser/redirs/redir_parser.h>
# include <parser/ast/ast.h>

typedef struct s_ast_node	t_ast_node;
typedef struct s_parser		t_parser;

/******************************************************************************/
/* Assignment nodes                                                           */
/******************************************************************************/

/** @brief A variable assignment */
struct s_assignment
{
	/** @brief Name of the variable being assigned to */
	t_string_buffer		variable;
	/** @brief Value to assign to `variable`, may require lazy expansion */
	struct s_wordlist	value;
};

/** @brief Frees an array of assignments */
void
assignlist_free(struct s_wordlist *list, size_t size);
/** @brief Displays an array of assignments to stderr */
void
assignlist_print(size_t depth, struct s_wordlist *list, size_t size);

/******************************************************************************/
/* The parser                                                                 */
/******************************************************************************/

/** @brief The parser */
typedef struct s_parser
{
	t_string		input;
	t_token_list	list;
	char			**errors;
	size_t			errors_size;
	size_t			errors_cap;
	
	/** @brief Current position in the parser's input list */
	size_t			pos;
	/** @brief Determines if keywords have to be treated as words */
	int				allow_reserved;
}	t_parser;

/** @brief Initializes a new parser */
t_parser
parser_init(t_string input, t_token_list list);
/** @brief Frees the parser */
void
parser_free(t_parser *parser);
/**
 * @brief Parser entry point
 *
 * @param parser The parser
 *
 * @return The parsed node, may be NULL (e.g empty expressions)
 */
t_ast_node
*parse(t_parser *parser);
/**
 * @brief Appends an error to the parser
 *
 * @param parser The parser
 * @param msg The error message (an owned string)
 * @param start The error's start range
 * @param end The error's end range
 */
void
parser_error(t_parser *parser, char *msg, size_t start, size_t end);
/**
 * @brief Flushes errors to stderr
 *
 * @param parser The parser
 * @return 1 if the parser has no errors, 0 if at least one error occured
 */
int
parser_error_flush(t_parser *parser);

/******************************************************************************/
/* Individual parsers                                                         */
/******************************************************************************/

/**
 * @brief Parses a list of commands:
 *
 * <list_of_commands> ::= <and_or> ( (';' | '&' | NEWLINE) <list_of_commands> )?
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_cmdlist(t_parser *parser);
/**
 * @brief Parses a compound command clause:
 *
 * <compound_command> ::= '(' <list_of_commands> ')'
 *                        | '{' <list_of_commands> '}'
 *                        | <if_clause>
 *                        | <while_clause>
 *                        | <until_clause>
 *                        | <for_clause>
 *                        | <case_clause>
 *
 * @param parser The parser
 * @return The parsed node
 */
t_ast_node
*parse_compound_command(t_parser *parser);
/**
 * @brief Parses a command clause:
 *
 * <command> ::= <simple_command>
 *               | <compound_command>
 *               | <function_def>
 *
 * @param parser The parser
 * @return The parsed node
 */
t_ast_node
*parse_command(t_parser *parser);
/**
 * @brief Parses a simple command clause:
 *
 * <simple_command> ::= cmd_prefix cmd_word?
 * <cmd_prefix>     ::= io_redirect
 *                      | cmd_prefix io_redirect
 *                      | ASSIGNMENT_WORD
 *                      | cmd_prefix ASSIGNMENT_WORD
 *
 * @param parser The parser
 * @return The parsed node
 */
t_ast_node
*parse_simple_command(t_parser *parser);
/**
 * @brief Parses an and or or cllaus:
 *
 * <and_or> ::= <pipeline> ( ('&&' | '||') <pipeline> )*
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_and_or(t_parser *parser);
/**
 * @brief Parses a pipeline cllaus:
 *
 * <pipeline> ::= <command> ( ('|' | '|&') <command> )*
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_pipeline(t_parser *parser);
/**
 * @brief Parses a function definition clause:
 *
 * <function_def> ::= WORD '()' <compound_command>
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_function_def(t_parser *parser);
/**
 * @brief Parses an if clause:
 *
 * <if_clause> ::= 'if' <list_of_commands> 'then' <list_of_commands>
 *                 ('elif' <list_of_commands> 'then' <list_of_commands>)*
 *                 ['else' <list_of_commands>]
 *                 'fi'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_if(t_parser *parser);
/**
 * @brief Parses a while clause:
 *
 * <while_clause> ::= 'while' <list_of_commands> 'do' <list_of_commands> 'done'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_while(t_parser *parser);
/**
 * @brief Parses an until clause:
 *
 * <until_clause> ::= 'until' <list_of_commands> 'do' <list_of_commands> 'done'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_until(t_parser *parser);
/**
 * @brief Parses a for clause:
 *
 * <for_clause> ::= 'for' WORD 'in' WORD* 'do' <list_of_commands> 'done'
 *
 * @param parser The parser
 * @return The parsed node
*/
t_ast_node
*parse_for(t_parser *parser);


/******************************************************************************/
/* Parsing helpers                                                            */
/******************************************************************************/

/**
 * @brief Checks if the current token is a control token and it's content
 * matches against `word`
 *
 * @param parser The parser
 * @param offset Offset from current position
 * @param word Word to match against
 *
 * @returns 1 If the condition is fulfilled.
 */
int
accept(t_parser *parser, int offset, const char *word);
/**
 * @brief Checks if the current token can be treated as a word
 *
 * @param parser The parser
 * @param offset Offset from current position
 *
 * @returns 1 If the token can be treated as a plain word.
 */
int
accept_word(t_parser *parser, int offset);
/**
 * @brief Checks if the current token is a token of type `type`
 *
 * @param parser The parser
 * @param offset Offset from current position
 * @param type The type to check for
 *
 * @returns 1 If the condition is fulfilled. 0 otherwise
 */
int
accept_tok(t_parser *parser, int offset, enum e_token_type type);
/**
 * @brief Checks if the current token is a control token and it's content
 * matches against `word`. If it doesn't match an error is reported
 *
 * @param parser The parser
 * @param offset Offset from current position
 * @param word Word to match against
 *
 * @returns 1 If the condition is fulfilled. 0 otherwise and an error is
 * reported
 */
int
expect(t_parser *parser, int offset, const char *word);
/**
 * @brief Utility that parses a token to an integer
 *
 * If the token is not a TOK_DIGIT, or the integer overflows, an error is
 * reported.
 *
 * @param parser The parser
 * @param pos Position of the token in `parser`
 * @param value Stores the result on success
 *
 * @returns 1 on success, 0 if an error has been reported
 */
int
token_atoi(t_parser *parser, size_t pos, int *value);
/**
 * @brief Prints `pad` `n` times
 *
 * @param pad Padding string
 * @param n Number of times to print `pad`
 */
void
print_pad(const char *pad, size_t n);

#endif // PARSER_H
