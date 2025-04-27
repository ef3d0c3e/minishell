/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>

t_ctx
	ctx_new(t_shell *shell, char *info)
{
	return ((t_ctx){
		.shell = shell,
		.info = info,
		.list = NULL,
		.parser = NULL,
		.prompt = NULL,
		.program = NULL,
	});
}

void
	ctx_free(t_ctx *context)
{
	if (context->list)
		token_list_free(context->list);
	if (context->parser)
		parser_free(context->parser);
	ast_free(context->program);
	free(context->prompt);
	free(context->info);
	context->info = NULL;
	context->prompt = NULL;
	context->list = NULL;
	context->parser = NULL;
	context->program = NULL;
}

int
	ctx_eval(
	t_ctx *ctx,
	char *prompt,
	void(*evaluator)(t_ctx *, void *),
	void *cookie)
{
	t_shell *const	shell = ctx->shell;
	t_string		input;
	t_token_list	list;
	t_parser		parser;

	if (ctx->info)
		ft_dprintf(2, "[Entering context %s]\n", ctx->info);
	ctx->list = &list;
	ctx->parser = &parser;
	ctx->prompt = prompt;
	input.str = prompt;
	input.len = ft_strlen(prompt);
	*ctx->list = tokenizer_tokenize(input);
	if (option_value(ctx->shell, "dbg_token"))
	{
		ft_dprintf(2, " -- Raw tokens --\n");
		token_list_print(input, &list);
	}
	if (!report_tokens(input, ctx->list) || !shell_error_flush(shell))
		return (shell->last_status = 2, 0);
	*ctx->list = token_expand(shell, *ctx->list);
	if (!report_tokens(input, ctx->list) || !shell_error_flush(shell))
		return (shell->last_status = 2, 0);
	if (option_value(shell, "dbg_token"))
	{
		ft_dprintf(2, " -- Expanded tokens --\n");
		token_list_print(input, &list);
	}
	*ctx->parser = parser_init(input, *ctx->list);
	ctx->program = parse(ctx->parser, 0, ctx->list->size, 0);
	if (option_value(shell, "dbg_ast"))
	{
		ft_dprintf(2, " -- Parsing --\n");
		ast_print(0, ctx->program);
	}
	if (!parser_error_flush(&parser))
		return (shell->last_status = 2, 0);
	evaluator(ctx, cookie);
	return (1);

}
