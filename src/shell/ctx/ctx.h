/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CTX_H
# define CTX_H

#include "shell/eval/eval.h"
#include "util/util.h"
typedef struct s_shell	t_shell;

# include <parser/parser.h>

/** @brief Context in the shell's evaluation */
typedef struct s_ctx
{
	/** @brief Reference to the owning shell */
	t_shell			*shell;
	/** @brief Context information string */
	char			*info;
	/** @brief Previous active context, used for cleanup in forked shells */
	struct s_ctx	*prev;

	/** @brief Current program prompt */
	char			*prompt;
	/** @brief Current token list */
	t_token_list	*list;
	/** @brief Current parser */
	t_parser		*parser;
	/** @brief Parsed program */
	t_ast_node		*program;
}	t_ctx;

/** @brief Initializes an empty new context */
t_ctx
ctx_new(t_shell *shell, char *info);
/**
 * @brief Cleans a context
 *
 * If there were a previous active context, the shell's current context is
 * updated to it's previous contextx.
 *
 * @note In case the context being freed is not the active context, an exception
 * is thrown.
 *
 * @param context Context to free
 */
void
ctx_free(t_ctx *context);
/**
 * @brief Evaluates the given prompt in the given context
 *
 * The context is pushed into the shell, and it's previous field is set to the
 * previous active context (or NULL if none). When cleaning up, contexts have
 * to be popped and freed in order.
 *
 * In case an error happens, `evaluator` is still called, but the `ctx->program`
 * field will be NULL. This means that it will have to be cleaned up.
 *
 * @param context Context to tokenize+parse+evaluate in
 * @param prompt Prompt to evaluate
 * @param evaluator Evaluator function
 * @param cookie Cookie given to the evaluator function
 */
int
ctx_eval(
	t_ctx *context,
	char *prompt,
	void(*evaluator)(t_ctx *, void *),
	void *cookie);

/** @brief Evaluates to stdout */
t_eval_result
ctx_eval_stdout(t_shell *shell, char *prompt);

struct s_eval_string_result
{
	t_eval_result	result;
	t_string_buffer	stdout;
};

/** @brief Evaluates to a string */
struct s_eval_string_result
ctx_eval_string(t_shell *shell, char *prompt, char *info);

#endif // CTX_H
