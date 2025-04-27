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
#include <math.h>
#include <shell/shell.h>
#include <stdio.h>

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

static void
	cmd_arg_push(t_parser *parser, t_ast_node *cmd, size_t arg_pos, size_t pos)
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
	arg_push(parser, &cmd->cmd.args[arg_pos], pos);
}

t_ast_node
	*parse_simple_command(t_parser *parser)
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
	if (parser->pos < parser->list.size
		&& parser->list.tokens[parser->pos].type == TOK_SPACE)
		++parser->pos;
	while (parser->pos < parser->list.size)
	{
		parse_redir_repeat(parser, &cmd->cmd.redirs);
		if (parser->pos >= parser->list.size)
			break ;
		tok = &parser->list.tokens[parser->pos];

		if (tok->type == TOK_PARAM || tok->type == TOK_PARAM_SIMPLE
			|| tok->type == TOK_CMD_SUB
			|| (arg_pos != 0 && token_isword(tok->type))
			|| accept_word(parser, 0))
		{
			cmd_arg_push(parser, cmd, arg_pos, parser->pos);
			++parser->pos;
		}
		else if (tok->type == TOK_SPACE)
		{
			++parser->pos;
			++arg_pos;
		}
		else
			break ;
	}
	if (!cmd->cmd.args && !cmd->cmd.assigns && !cmd->cmd.redirs.redirs_size)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

t_ast_node
	*parse_cmdlist(t_parser *parser);


static void
	if_push(t_parser *parser, t_ast_node *stmt, int with_cond)
{
	t_ast_node	*cond;
	t_ast_node	*body;

	if (with_cond)
	{
		cond = parse_cmdlist(parser);
		expect(parser, 0, "then");
		++parser->pos;
	}
	body = parse_cmdlist(parser);

	if (with_cond)
	{
		stmt->st_if.conds = ft_realloc(stmt->st_if.conds,
				sizeof(t_ast_node **) * stmt->st_if.nconds,
				sizeof(t_ast_node **) * (stmt->st_if.nconds + 1));
		stmt->st_if.conds[stmt->st_if.nconds++] = cond;
	}
	stmt->st_if.bodies = ft_realloc(stmt->st_if.bodies,
		sizeof(t_ast_node **) * stmt->st_if.nbodies,
		sizeof(t_ast_node **) * (stmt->st_if.nbodies + 1));
	stmt->st_if.bodies[stmt->st_if.nbodies++] = body;
}

t_ast_node
	*parse_if_stmt(t_parser *parser)
{
	int			in_stmt = !parser->allow_reserved;
	t_ast_node	*stmt;

	++parser->pos;
	parser->allow_reserved = 0;
	stmt = make_if_node();
	if_push(parser, stmt, 1);
	while (accept(parser, 0, "elif"))
	{
		++parser->pos;
		if_push(parser, stmt, 1);
	}
	if (accept(parser, 0, "else"))
	{
		++parser->pos;
		if_push(parser, stmt, 0);
	}
	expect(parser, 0, "fi");
	++parser->pos;
	parser->allow_reserved = !in_stmt;
	return (stmt);
}

t_ast_node
	*parse_while_stmt(t_parser *parser)
{
	int			in_stmt = !parser->allow_reserved;
	t_ast_node	*cond;
	t_ast_node	*body;

	++parser->pos;
	parser->allow_reserved = 0;
	cond = parse_cmdlist(parser);
	expect(parser, 0, "do");
	++parser->pos;
	body = parse_cmdlist(parser);
	expect(parser, 0, "done");
	++parser->pos;
	parser->allow_reserved = !in_stmt;
	return (make_while_node(cond, body));
}

t_ast_node
	*parse_compound_command(t_parser *parser)
{
	const size_t	begin = parser->pos;
	t_ast_node		*inner;
	t_ast_node		*node;

	node = NULL;
	if (accept(parser, 0, "("))
	{
		++parser->pos;
		inner = parse_cmdlist(parser);
		expect(parser, 0, ")");
		++parser->pos;
		node = make_subshell_node(inner);
		parse_redir_repeat(parser, &node->sub.redirs);
	}
	else if (accept(parser, 0, "{"))
	{
		++parser->pos;
		inner = parse_cmdlist(parser);
		expect(parser, 0, "}");
		++parser->pos;
		node = make_block_node(inner);
	}
	else if (accept(parser, 0, "if"))
		return (parse_if_stmt(parser));
	else if (accept(parser, 0, "while"))
		return (parse_while_stmt(parser));
	else
	{
		parser_error(parser, ft_strdup("Unexpected token"),
				begin, parser->list.size);
	}
	return (node);
	/* TODO:
    if (accept("if"))      return parse_if_clause();
    if (accept("while"))   return parse_while_clause();
    if (accept("until"))   return parse_until_clause();
    if (accept("for"))     return parse_for_clause();
    if (accept("case"))    return parse_case_clause();
    error("unexpected token in compound_command");
	*/
}

t_ast_node
	*parse_function_def(t_parser *parser)
{
	t_string_buffer	name;
	t_ast_node		*body;

	stringbuf_init(&name, 64);
	token_wordcontent(&name, &parser->list.tokens[parser->pos]);
	parser->pos += 3;
	while (accept(parser, 0, "\n"))
		++parser->pos;
	body = parse_compound_command(parser);
	// TODO: Force node to be a subshell or block node
	return (make_function_node(name, body));
}

t_ast_node
	*parse_command(t_parser *parser)
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
	if (token_isword(parser->list.tokens[parser->pos].type)
		&& accept(parser, 1, "(")
		&& accept(parser, 2, ")"))
		return (parse_function_def(parser));
	if (accept(parser, 0, "(") || accept(parser, 0, "{")
		|| accept(parser, 0, "if")
		|| accept(parser, 0, "while")
		|| accept(parser, 0, "until")
		|| accept(parser, 0, "for")
		|| accept(parser, 0, "case"))
		return (parse_compound_command(parser));
	return (parse_simple_command(parser));
}

t_ast_node
	*parse_pipeline(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		op;

	left = parse_command(parser);
	while (accept(parser, 0, "|") || accept(parser, 0, "|&"))
	{
		op = parser->list.tokens[parser->pos];
		++parser->pos;
		right = parse_command(parser);
		left = make_logic_node(op, left, right);
	}
	return (left);
}

t_ast_node
	*parse_and_or(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		op;

	left = parse_pipeline(parser);
	while (accept(parser, 0, "&&") || accept(parser, 0, "||"))
	{
		op = parser->list.tokens[parser->pos];
		++parser->pos;
		right = parse_command(parser);
		left = make_logic_node(op, left, right);
	}
	return (left);
}

t_ast_node
	*parse_cmdlist(t_parser *parser)
{
	t_ast_node	*cmd;
	t_ast_node	*list;
	t_token		*sep;

	cmd = NULL;
	if (parser->pos < parser->list.size)
		cmd = parse_and_or(parser);
	list = make_list_node();
	list_node_push(list, cmd, '\0');
	while (accept(parser, 0, ";") || accept(parser, 0, "\n")
		|| accept(parser, 0, "&"))
	{
		sep = &parser->list.tokens[parser->pos];
		++parser->pos;
		if (parser->pos >= parser->list.size)
			break ;
		cmd = parse_and_or(parser);
		if (cmd)
			list_node_push(list, cmd, sep->reserved_word[0]);
	}

	/*
	t_ast_node*	left;
	t_ast_node*	right;
	t_token		op;
	
	if (parser->pos >= parser->list.size)
		return (NULL);
	left = parse_and_or(parser);
	if (accept(parser, 0, ";") || accept(parser, 0, "&")
		|| accept(parser, 0, "\n"))
	{
		op = parser->list.tokens[parser->pos];
		++parser->pos;
		right = parse_cmdlist(parser);
		return (make_logic_node(op, left, right));
	}
	return (left);
	*/
	return (list);
}

t_ast_node
	*parse(t_parser *parser, size_t start, size_t end, int min_prec)
{
	return (parse_cmdlist(parser));
}

