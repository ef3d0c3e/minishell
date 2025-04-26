/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	token_list_debug(t_string input, const t_token_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		token_print_debug(2, input, &list->tokens[i]);
		++i;
	}
	ft_dprintf(2, "\n");
}

int
	repl(t_shell *shell, char *s)
{
	t_string		input;
	t_token_list	list;
	t_parser		parser;

	shell_parser_free(shell);
	input.str = s;
	input.len = ft_strlen(s);
	shell->prompt = s;
	shell->token_list = &list;
	list = tokenizer_tokenize(input);
	if (option_value(shell, "dbg_token"))
	{
		ft_dprintf(2, " -- Raw tokens --\n");
		token_list_debug(input, &list);
	}
	if (!report_tokens(input, shell->token_list) || !shell_error_flush(shell))
		return (shell_parser_free(shell), shell->last_status = 2, shell->last_status);
	*shell->token_list = token_expand(shell, *shell->token_list);
	if (!report_tokens(input, shell->token_list) || !shell_error_flush(shell)
		|| shell->last_status != 0)
		return (shell_parser_free(shell), shell->last_status = 2, shell->last_status);
	if (option_value(shell, "dbg_token"))
	{
		ft_dprintf(2, " -- Expanded tokens --\n");
		token_list_debug(input, &list);
	}
	parser = parser_init(input, *shell->token_list);
	shell->parser = &parser;
	shell->program = parse(shell->parser, 0, shell->token_list->size, 0);
	if (option_value(shell, "dbg_ast"))
	{
		ft_dprintf(2, " -- Parsing --\n");
		ast_print(0, shell->program);
	}
	if (!parser_error_flush(&parser))
		return (shell_parser_free(shell), shell->last_status = 2, shell->last_status);
	eval(shell, shell->program);
	shell_parser_free(shell);
	return (shell->last_status);
}
