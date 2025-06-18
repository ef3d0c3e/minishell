/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/18 08:39:57 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

void
	token_error(t_token_list *list, size_t start, size_t end, const char *msg)
{
	token_list_push(list, TOK_ERROR, start,
		end)->err = (t_string){
	.str = msg,
	.len = ft_strlen(msg)
};
}
