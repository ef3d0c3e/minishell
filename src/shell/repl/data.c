/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

void
	*repl_data_new(t_getline *line)
{
	t_repl_data	*data;

	(void)line;
	data = xmalloc(sizeof(t_repl_data));
	data->list = (t_token_list){NULL, 0, 0};
	return (data);
}

/** @brief Frees REPL data for getline */
void
	repl_data_free(t_getline *line, void *data_)
{
	t_repl_data *const	data = data_;
	
	(void)line;
	token_list_free(&data->list);
	free(data);
}
