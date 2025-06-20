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
	if (context != context->shell->context)
		shell_fail(context->shell, "Context popped in the wrong order",
			SRC_LOCATION);
	context->shell->context = context->prev;
	if (context->list)
		token_list_free(context->list);
	if (context->parser)
		parser_free(context->parser);
	ast_free(context->program, 0);
	free(context->prompt);
	free(context->info);
	context->info = NULL;
	context->prompt = NULL;
	context->list = NULL;
	context->parser = NULL;
	context->program = NULL;
}

static void
	ctx_set(t_shell *shell, t_ctx *ctx)
{
	ctx->prev = shell->context;
	shell->context = ctx;
}

static int
	ctx_parse_eval(
	t_ctx *ctx,
	void (*evaluator)(t_ctx *, void *),
	void *cookie,
	t_string input)
{
	t_shell *const	shell = ctx->shell;
	t_parser		parser;

	if (option_value(shell, "dbg_parser"))
	{
		ft_dprintf(2, " -- Expanded tokens --\n");
		token_list_print(input, ctx->list);
	}
	ctx->parser = &parser;
	*ctx->parser = parser_init(input, *ctx->list);
	ctx->program = parse(ctx->parser);
	if (option_value(shell, "dbg_parser"))
	{
		ft_dprintf(2, " -- Parsing --\n");
		ast_print(0, ctx->program);
	}
	if (!parser_error_flush(&parser) || !read_heredocs(shell, ctx->parser))
	{
		ast_free(ctx->program, 1);
		ctx->program = NULL;
		return (shell->last_status = 2, evaluator(ctx, cookie), 0);
	}
	evaluator(ctx, cookie);
	return (1);
}

int
	ctx_eval(
	t_ctx *ctx,
	char *prompt,
	void (*evaluator)(t_ctx *, void *),
	void *cookie)
{
	t_shell *const	shell = ctx->shell;
	t_string		input;
	t_token_list	list;

	ctx_set(shell, ctx);
	if (option_value(ctx->shell, "dbg_parser") && ctx->info)
		ft_dprintf(2, "[Entering context %s]\n", ctx->info);
	ctx->list = &list;
	ctx->prompt = prompt;
	input.str = prompt;
	input.len = ft_strlen(prompt);
	*ctx->list = tokenizer_tokenize(input);
	if (option_value(ctx->shell, "dbg_parser"))
	{
		ft_dprintf(2, " -- Raw tokens --\n");
		token_list_print(input, &list);
	}
	if (!report_tokens(input, ctx->list) || !shell_error_flush(shell))
		return (shell->last_status = 2, evaluator(ctx, cookie), 0);
	*ctx->list = token_expand(shell, *ctx->list);
	if (!report_tokens(input, ctx->list) || !shell_error_flush(shell))
		return (shell->last_status = 2, evaluator(ctx, cookie), 0);
	return (ctx_parse_eval(ctx, evaluator, cookie, input));
}
