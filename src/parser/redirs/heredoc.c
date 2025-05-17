/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>


void
	push_heredoc(t_parser *parser, t_redirection *redir)
{
	if (parser->heredoc_count == HEREDOC_MAX)
	{
		parser_error(parser, ft_strdup("Too many heredocs"), 0,
			parser->list.tokens[parser->list.size - 1].end);
		return ;
	}
	parser->heredocs[parser->heredoc_count++] = redir;
}

static char
	*get_delim(const t_redirection *heredoc)
{
	t_string_buffer	buf;
	size_t			i;

	stringbuf_init(&buf, 24);
	i = 0;
	while (i < heredoc->redirector.filename.natoms)
	{
		atom_wordcontent(&buf, &heredoc->redirector.filename.atoms[i]);
		++i;
	}
	return (stringbuf_cstr(&buf));
}

static int
	read_heredoc(t_shell *shell, t_redirection *heredoc)
{
	char *const	delim = get_delim(heredoc);
	t_getline	line;
	char		*res;

	stringbuf_init(&heredoc->heredoc, 24);
	line = getline_setup(shell);
	while (1)
	{
		res = getline_read(&line, "> ");
		if (res == NULL)
			return (free(delim), getline_cleanup(&line), 0);
		if (!ft_strcmp(res, delim))
			break ;
		if (heredoc->heredoc.len)
			stringbuf_append(&heredoc->heredoc, (t_string){"\n", 1});
		stringbuf_append(&heredoc->heredoc, (t_string){res, ft_strlen(res)});
		free(res);
	}
	free(res);
	free(delim);
	getline_cleanup(&line);
	return (1);
}

int
	read_heredocs(t_shell *shell, t_parser *parser)
{
	size_t	i;

	i = 0;
	while (i < parser->heredoc_count)
	{
		if (!read_heredoc(shell, parser->heredocs[i]))
			break ;
		++i;
	}
	return (i == parser->heredoc_count);
}
