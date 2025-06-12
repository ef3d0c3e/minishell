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
#include <shell/shell.h>

static void
	evaluator(t_ctx *ctx, void *cookie)
{
	t_shell *const			shell = ctx->shell;
	t_eval_result *const	result = cookie;
	static const char		*fmt = "%s: only meaningful in a 'for', 'while', "
		"or 'until' loop\n";

	(void)cookie;
	*result = (t_eval_result){RES_NONE, 0};
	*result = eval(ctx->shell, ctx->program);
	ctx_free(ctx);
	if (!shell_error_flush(shell))
		shell->last_status = 1;
	else if (result->type == RES_EXIT)
		shell->last_status = result->param;
	else if (result->type == RES_CONTINUE)
		ft_dprintf(2, fmt, "continue");
	else if (result->type == RES_BREAK)
		ft_dprintf(2, fmt, "break");
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
