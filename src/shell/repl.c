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
#include "parser/parser.h"
#include <shell/eval.h>
#include <expansion/expansion.h>

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
	repl(t_environ *env, char *s)
{
	t_string		input;
	t_token_list	list;
	t_parser		parser;

	env_parser_free(env);
	input.str = s;
	input.len = ft_strlen(s);
	env->prompt = s;
	env->token_list = &list;
	list = tokenizer_tokenize(input);
	if (option_value(env, "dbg_token"))
	{
		dprintf(2, " -- Raw tokens --\n");
		token_list_debug(input, &list);
	}
	if (!report_tokens(input, env->token_list) || !shell_error_flush(env))
		return (env_parser_free(env), -1);
	*env->token_list = token_expand(env, *env->token_list);
	if (!report_tokens(input, env->token_list) || !shell_error_flush(env)
		|| env->last_status != 0)
		return (env_parser_free(env), -1);
	if (option_value(env, "dbg_token"))
	{
		dprintf(2, " -- Expanded tokens --\n");
		token_list_debug(input, &list);
	}
	parser = parser_init(input, list);
	env->parser = &parser;
	env->program = parse(&parser, 0, list.size, 0);
	if (!parser_error_flush(&parser))
		return (env_parser_free(env), -1);
	if (option_value(env, "dbg_ast"))
	{
		printf(" -- Parsing --\n");
		ast_print_debug(input, env->program, 0);
	}
	eval(env, env->program);
	env_parser_free(env);
	return (env->last_status);
}
