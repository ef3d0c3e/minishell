/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "token.h"

int
token_seperator(t_parser_state *s, t_string_view cp)
{
	const int	is_sep = !(sv_cmp(cp, " ") * sv_cmp(cp, "\t"));

	return (is_sep && s->escape_len == 0 && s->quote.len == 0);
}
