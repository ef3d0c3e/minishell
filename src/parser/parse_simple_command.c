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

/** @brief Puses an argument to a command */
static void
	cmd_arg_push(t_parser *parser, t_ast_node *cmd, size_t arg_pos)
{
	while (cmd->cmd.args.size <= arg_pos)
	{
		cmd->cmd.args.list = ft_realloc(cmd->cmd.args.list,
			sizeof(t_word) * cmd->cmd.args.size,
			sizeof(t_word) * (cmd->cmd.args.size + 1));
		cmd->cmd.args.list[cmd->cmd.args.size].atoms = NULL;
		cmd->cmd.args.list[cmd->cmd.args.size].natoms = 0;
		++cmd->cmd.args.size;
	}
	word_push(parser, &cmd->cmd.args.list[arg_pos]);
}

/** @brief Pushes into a command's assignments list */
static void
	push_assign(
	t_ast_node *cmd,
	t_string_buffer ident,
	struct s_word arg)
{
	cmd->cmd.assigns = ft_realloc(cmd->cmd.assigns,
		sizeof(struct s_assignment) * cmd->cmd.nassigns,
		sizeof(struct s_assignment) * (cmd->cmd.nassigns + 1));
	cmd->cmd.assigns[cmd->cmd.nassigns].variable =
		stringbuf_from_range(ident.str, ident.str + ident.len);
	cmd->cmd.assigns[cmd->cmd.nassigns].value = arg;
	++cmd->cmd.nassigns;
}

/** @brief Parses a list of assignments */
static void
	parse_assignments(t_parser *parser, t_ast_node *cmd)
{
	const t_token		*tok;
	size_t				assign_ident;
	struct s_word	arg;

	if (parser->pos < parser->list.size
		&& parser->list.tokens[parser->pos].type == TOK_SPACE)
		++parser->pos;
	while (parser->pos < parser->list.size)
	{
		parse_redir_repeat(parser, &cmd->cmd.redirs);
		if (!accept_tok(parser, 0, TOK_ASSIGN))
			break ;
		assign_ident = parser->pos++;
		arg.atoms = NULL;
		arg.natoms = 0;
		tok = &parser->list.tokens[parser->pos];
		while (parser->pos < parser->list.size
			&& (tok->type == TOK_PARAM || tok->type == TOK_PARAM_SIMPLE
				|| tok->type == TOK_CMD_SUB
				|| token_isword(tok->type)))
		{
			word_push(parser, &arg);
			++parser->pos;
			tok = &parser->list.tokens[parser->pos];
		}
		push_assign(cmd, parser->list.tokens[assign_ident].word, arg);
		if (parser->pos < parser->list.size
			&& parser->list.tokens[parser->pos].type == TOK_SPACE)
			++parser->pos;
	}
}

t_ast_node
	*parse_simple_command(t_parser *parser)
{
	t_ast_node *const	cmd = make_cmd_node();
	const t_token		*tok;
	size_t				arg_pos;

	parse_assignments(parser, cmd);
	arg_pos = 0;
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
			cmd_arg_push(parser, cmd, arg_pos);
		else if (tok->type == TOK_SPACE)
			++arg_pos;
		else
			break ;
		++parser->pos;
	}
	if (!cmd->cmd.args.size && !cmd->cmd.assigns
		&& !cmd->cmd.redirs.redirs_size)
		return (free(cmd), NULL);
	return (cmd);
}
