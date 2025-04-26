/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/parser.h"
#include "parser/redir_parser.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <shell/shell.h>

/*
<script>				::= <list_of_commands>

<list_of_commands>		::= <and_or> ( (';' | '&' | NEWLINE) <list_of_commands> )?

<and_or>				::= <pipeline> ( ('&&' | '||') <pipeline> )*

<pipeline>				::= <command> ( '|' <command> )*

<command>				::= <simple_command>
						| <compound_command>
						| <function_def>

<simple_command>		::= cmd_prefix cmd_word? cmd_suffix?
<cmd_prefix>			::= io_redirect 
						| cmd_prefix io_redirect
						| ASSIGNMENT_WORD 
						| cmd_prefix ASSIGNMENT_WORD

<function_def>			::= WORD '()' <compound_command>

<compound_command>		::= '(' <list_of_commands> ')'
						| '{' <list_of_commands> '}'
						| <if_clause>
						| <while_clause>
| <until_clause>
						| <for_clause>
						| <case_clause>

<if_clause>				::= 'if' <list_of_commands> 'then' <list_of_commands>
						( 'elif' <list_of_commands>			'then' <list_of_commands> )*
						[ 'else' <list_of_commands>			]
						'fi'

<while_clause>			::= 'while' <list_of_commands> 'do' <list_of_commands> 'done'

<until_clause>			::= 'until' <list_of_commands> 'do' <list_of_commands> 'done'

<for_clause>			::= 'for' WORD 'in' WORD* 'do' <list_of_commands> 'done'

<case_clause>			::= 'case' WORD 'in'
						( PATTERN ')' <list_of_commands> ';;' )+
						'esac'
*/

/**
 * @brief Creates a new logic node
 */
t_ast_node
*make_logic_node(t_token op, t_ast_node *left, t_ast_node *right);
t_ast_node
*make_cmd_node();
t_ast_node
*make_subshell_node(t_ast_node *inner);

int
	accept(t_parser *parser, size_t start, size_t end, const char *word)
{
	const t_token	*tok;

	if (start >= end)
		return (0);
	tok = &parser->list.tokens[start];
	if (tok->type == TOK_KEYWORD || tok->type == TOK_SEQUENCE
		|| tok->type == TOK_OPERATOR || tok->type == TOK_PIPELINE
		|| tok->type == TOK_GROUPING)
		return (!ft_strcmp(tok->reserved_word, word));
	return (0);
}

static void
	cmd_arg_push(t_ast_node *cmd, size_t arg_pos, const t_token *tok)
{
	while (cmd->cmd.nargs <= arg_pos)
	{
		cmd->cmd.args = ft_realloc(cmd->cmd.args,
			sizeof(struct s_argument) * cmd->cmd.nargs,
			sizeof(struct s_argument) * (cmd->cmd.nargs + 1));
		cmd->cmd.args[cmd->cmd.nargs].items = NULL;
		cmd->cmd.args[cmd->cmd.nargs].nitems = 0;
		++cmd->cmd.nargs;
	}
	arg_push(&cmd->cmd.args[arg_pos], tok);
}

t_ast_node
	*parse_simple_command(t_parser *parser, size_t *start, size_t end)
{
	t_ast_node		*cmd;
	const t_token	*tok;
	size_t			arg_pos;

	cmd = make_cmd_node();
	/*
	TODO: Parse assignments
	while (peek() == TOK_ASSIGNMENT_WORD || peek_is_redirection()) {
		if (accept(TOK_ASSIGNMENT_WORD)) {
			Token t = last_token();            // e.g. "VAR=$?"
											   // split at the first '='
			char* eq = strchr(t.text, '=');    // guaranteed non-NULL
			*eq = '\0';
			char* name = t.text;
			char* rhs  = eq + 1;
			// record an assignment node (with unexpanded rhs)
			cmd->add_assignment(name, rhs, t.location);
		}
		else {
			cmd->add_redirection(parse_redirection());
		}
	}
	*/

	arg_pos = 0;
	while (*start < end)
	{
		*start += parse_redir_repeat(parser, *start, end, &cmd->cmd.redirs);
		if (*start >= end)
			break ;
		tok = &parser->list.tokens[*start];

		if (tok->type == TOK_PARAM || tok->type == TOK_PARAM_SIMPLE
			|| token_isword(tok->type))
		{
			cmd_arg_push(cmd, arg_pos, tok);
			++*start;
		}
		else if (tok->type == TOK_SPACE)
		{
			++*start;
			++arg_pos;
		}
		else
			break ;
	}
	return (cmd);
}

t_ast_node
	*parse_cmdlist(t_parser *parser, size_t *start, size_t end);

t_ast_node
	*parse_compound_command(t_parser *parser, size_t *start, size_t end)
{
	const size_t	begin = *start;
	t_ast_node		*inner;
	t_ast_node		*node;

	node = NULL;
	if (accept(parser, *start, end, "("))
	{
		++*start;
		inner = parse_cmdlist(parser, start, end);
		if (!accept(parser, *start, end, ")"))
			parser_error(parser, ft_strdup("Expected `)` token"),
					begin, end);
		++*start;
		node = make_subshell_node(inner);
		*start += parse_redir_repeat(parser, *start, end, &node->expr.redirs);
	}
	else
	{
		parser_error(parser, ft_strdup("Unexpected token"),
				begin, end);
	}
	return (node);
	/* TODO:
    if (accept("{")) {
        AST* inner = parse_list_of_commands();
        expect("}");
        return make_block_node(inner);
    }
    if (accept("if"))      return parse_if_clause();
    if (accept("while"))   return parse_while_clause();
    if (accept("until"))   return parse_until_clause();
    if (accept("for"))     return parse_for_clause();
    if (accept("case"))    return parse_case_clause();
    error("unexpected token in compound_command");
	*/
}

t_ast_node
	*parse_command(t_parser *parser, size_t *start, size_t end)
{
	/*
	TODO: [WORD] '()' -> function parser
    if (peek_is_word() && peek_next_is("()")) {
        return parse_function_def();
    }
	TODO: [keyword start stmt] -> compound parser
    if (peek_is_one_of("if","while","until","for","case","{","(")) {
        return parse_compound_command();
    }
	*/
	if (accept(parser, *start, end, "("))
		return (parse_compound_command(parser, start, end));

	return (parse_simple_command(parser, start, end));
}

t_ast_node
	*parse_pipeline(t_parser *parser, size_t *start, size_t end)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		op;

	left = parse_command(parser, start, end);
	while (accept(parser, *start, end, "|") || accept(parser, *start, end, "|&"))
	{
		op = parser->list.tokens[*start];
		++*start;
		right = parse_command(parser, start, end);
		left = make_logic_node(op, left, right);
	}
	return (left);
}

t_ast_node
	*parse_and_or(t_parser *parser, size_t *start, size_t end)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		op;

	left = parse_pipeline(parser, start, end);
	while (accept(parser, *start, end, "&&") || accept(parser, *start, end, "||"))
	{
		op = parser->list.tokens[*start];
		++*start;
		right = parse_command(parser, start, end);
		left = make_logic_node(op, left, right);
	}
	return (left);
}

t_ast_node
	*parse_cmdlist(t_parser *parser, size_t *start, size_t end)
{
	t_ast_node*	left;
	t_ast_node*	right;
	t_token		op;
	
	left = parse_and_or(parser, start, end);
	if (accept(parser, *start, end, ";") || accept(parser, *start, end, "&")
		|| accept(parser, *start, end, "\n"))
	{
		op = parser->list.tokens[*start];
		++*start;
		right = parse_cmdlist(parser, start, end);
		return (make_logic_node(op, left, right));
	}
	return (left);
}

t_ast_node
	*parse(t_parser *parser, size_t start, size_t end, int min_prec)
{
	return (parse_cmdlist(parser, &start, end));
}

