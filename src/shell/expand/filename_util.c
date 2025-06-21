/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 14:31:37 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Checks if a fragment range needs filename expansion by scanning for
 * reserved words */
int
	filename_needs_expansion(
	t_shell *shell,
	const t_fragment *start,
	const t_fragment *end)
{
	static const char		*alts[] = {"*", "?", NULL};
	static const char		*glob[] = {"@", "!", "+", NULL};
	const t_string_buffer	*word;

	while (start != end)
	{
		if (start->flags & (FL_SQUOTED | FL_DQUOTED) && ++start)
			continue ;
		word = &start->word;
		if (str_find_alternatives((t_string){word->str, word->len}, alts))
			return (1);
		else if (option_value(shell, "extglob")
			&& str_find_alternatives((t_string){word->str, word->len}, glob))
			return (1);
		++start;
	}
	return (0);
}

// TODO: Error handling for failglob
int
	filename_make_regex(
	const t_globopts *opts,
	t_fragment *start,
	t_fragment *end,
	t_regex *regex)
{
	t_regex_builder		builder;

	builder = regex_builder_new();
	while (start != end)
	{
		if (start->flags & (FL_SQUOTED | FL_DQUOTED))
		{
			if (!regex_builder_literal(opts, &builder,
					stringbuf_cstr(&start->word)))
				return (0);
		}
		else if (!regex_builder_expr(opts, &builder,
				stringbuf_cstr(&start->word)))
			return (0);
		++start;
	}
	*regex = builder.regex;
	return (1);
}
