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

typedef struct s_shell	t_shell;

# include <parser/parser.h>

/** @brief Context in the shell's evaluation */
typedef struct s_ctx
{
	/** @brief Reference to the owning shell */
	t_shell			*shell;
	/** @brief Context information string */
	char			*info;

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
/** @brief Cleans a context */
void
ctx_free(t_ctx *context);
/** @brief Pushes a new context to the shell and parse the given prompt in it */
int
ctx_eval(
	t_ctx *context,
	char *prompt,
	void(*evaluator)(t_ctx *, void *),
	void *cookie);

/** @brief Evaluates to stdout */
void
ctx_eval_stdout(t_shell *shell, char *prompt);
/** @brief Evaluates to a string */
t_string_buffer
ctx_eval_string(t_shell *shell, char *prompt, char *info);

#endif // CTX_H
