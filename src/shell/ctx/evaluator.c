/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/ctx/ctx.h"
#include <shell/shell.h>


static void
	evaluator(t_ctx *ctx, void *cookie)
{
	t_shell *const	shell = ctx->shell;
	(void)cookie;
	eval(ctx->shell, ctx->program);
	ctx_free(ctx);
	if (!shell_error_flush(shell))
		shell->last_status = 1;
}

void
	ctx_eval_stdout(t_shell *shell, char *prompt)
{
	t_ctx	ctx;

	ctx = ctx_new(shell, NULL);
	ctx_eval(&ctx, prompt, evaluator, NULL);
}

