/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

int
	token_pipeline(t_token_list *list, t_u8_iterator *it)
{
	static const char	*pipelines[] = {"|", "|&", NULL};
	const char			*pipeline;

	if (it->codepoint.str[0] != '|')
		return (0);
	pipeline = str_alternatives(it_substr(it, 2), pipelines);
	token_list_push(list, (t_token){
		.type = TOK_PIPELINE,
		.start = it->byte_pos,
		.end = it->byte_pos + ft_strlen(pipeline),
		.reserved_word = pipeline
	});
	it_advance(it, ft_strlen(pipeline));
	return (1);
}
