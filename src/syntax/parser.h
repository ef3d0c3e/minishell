/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_H
# define PARSER_H

#include <util/util.h>

/**
 * @brief State of the parser
 */
typedef struct s_parser_state
{
	/**
	 * @brief Iterator
	 */
	t_u8_iterator	it;
	/**
	 * @brief Current word token
	 */
	t_string_view	word;
	/**
	 * @brief Quoting character
	 */
	t_string_view	quote;
	/**
	 * @brief Escape length
	 */
	size_t			escape_len;
}	t_parser_state;

#endif // PARSER_H
