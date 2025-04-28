/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	cmd_arg_push(t_parser *parser, t_ast_node *cmd, size_t arg_pos)
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
	arg_push(parser, &cmd->cmd.args[arg_pos]);
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
			cmd_arg_push(parser, cmd, arg_pos);
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
