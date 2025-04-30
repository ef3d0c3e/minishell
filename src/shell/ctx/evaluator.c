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
#include "shell/eval/eval.h"
#include <shell/shell.h>


static void
	evaluator(t_ctx *ctx, void *cookie)
{
	t_shell *const			shell = ctx->shell;
	t_eval_result *const	result = cookie;

	(void)cookie;
	*result = eval(ctx->shell, ctx->program);
	ctx_free(ctx);
	if (!shell_error_flush(shell))
		shell->last_status = 1;
	else if (result->type == RES_EXIT)
		shell->last_status = result->param;
}

t_eval_result
	ctx_eval_stdout(t_shell *shell, char *prompt)
{
	t_ctx			ctx;
	t_eval_result	result;

	ctx = ctx_new(shell, NULL);
	ctx_eval(&ctx, prompt, evaluator, &result);
	return (result);
}

