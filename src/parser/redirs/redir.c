/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	redir_free(t_redirection *redir)
{
	if (redir_dest_word(redir))
		arg_free(&redir->redirectee.filename);
}

void
	redirs_free(t_redirections *redirs)
{
	size_t	i;

	i = 0;
	while (i < redirs->redirs_size)
		redir_free(&redirs->redirs[i++]);
	free(redirs->redirs);
}

static void
	add_redir(t_redirections *redirs, t_redirection redir)
{
	size_t	new_cap;

	new_cap = redirs->redirs_capacity + !redirs->redirs_capacity * 4;
	while (new_cap < redirs->redirs_size + 1)
		new_cap *= 2;
	redirs->redirs = ft_realloc(redirs->redirs,
			redirs->redirs_capacity * sizeof(redir),
			new_cap * sizeof(redir));
	redirs->redirs_capacity = new_cap;
	redirs->redirs[redirs->redirs_size++] = redir;
}

void
	make_redirection(
	t_redirections *redirs,
	t_redirectee source,
	t_redirectee dest,
	enum e_redir_type type)
{
	t_redirection	redir;

	redir.redirector = source;
	redir.redirectee = dest;
	redir.type = type;
	redir.flags = 0;
	redir.here_doc_eof = NULL;
	if (type == R_ERR_AND_OUT || type == R_OUTPUT_FORCE
		|| type == R_OUTPUT_DIRECTION)
		redir.flags = O_TRUNC | O_WRONLY | O_CREAT;
	else if (type == R_APPEND_ERR_AND_OUT || type == R_APPENDING_TO)
		redir.flags = O_APPEND | O_WRONLY | O_CREAT;
	else if (type == R_INPUT_OUTPUT)
		redir.flags = O_RDWR | O_CREAT;
	else if (R_INPUT_DIRECTION)
		redir.flags = O_RDONLY;
	add_redir(redirs, redir);
}
