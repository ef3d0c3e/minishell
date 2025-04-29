/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/eval/eval.h"
#include <shell/shell.h>


t_eval_result
	eval_exec(t_shell *shell, t_ast_node *cmd, char *path, char **argv)
{
	free(path);
	args_free(argv);
	return ((t_eval_result){RES_NONE, 0});
}
