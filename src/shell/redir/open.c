/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "shell/eval/eval.h"
#include "util/util.h"
#include <shell/shell.h>

/** @brief Checks whether a redirection is clobbering */
static int
	redir_is_clobbering(const t_redirection *redir)
{
	return (redir->type == R_OUTPUT_DIRECTION || redir->type == R_ERR_AND_OUT);
}

/** @brief Utility to expand a redirectee word */
static char
	*redir_expand(t_shell *shell, t_redirection *redir)
{
	char	*err;
	char	**result;
	char	*filename;

	result = word_expansion_single(shell, &redir->redirectee.filename);
	if (!result || !result[0] || result[1])
	{
		args_free(result);
		ft_asprintf(&err, "Ambiguous redirect");
		shell_error(shell, err, SRC_LOCATION);
		return (NULL);
	}
	filename = result[0];
	free(result);
	return (filename);
}

/** @brief Opens a file in noclobber moder, returns -1 and reports on failure */
static int
	noclobber_open(t_shell *shell, int flags, char *file)
{
	struct stat	sb[2];
	int			status;
	int			fd;
	char		*err;

	status = stat(file, &sb[0]);
	if (!status && (S_ISREG(sb[0].st_mode)))
		return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
			" mode", file), shell_error(shell, err, SRC_LOCATION), -1);
	flags &= ~O_TRUNC;
	if (status != 0)
	{
		fd = shell_open(shell, file, flags | O_EXCL, 0666);
		if (fd < 0 || errno == EEXIST)
			return (ft_asprintf(&err, "Failed to open `%s` for writing in "
				"noclobber mode", file),
				shell_error(shell, err, SRC_LOCATION), -1);
		return (fd);
	}
	fd = shell_open(shell, file, flags, 0666);
	if (fd < 0)
		return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
			" mode", file), shell_error(shell, err, SRC_LOCATION), -1);
	if ((fstat(fd, &sb[1]) == 0) && (S_ISREG(sb[1].st_mode) == 0) && status == 0
			&& (S_ISREG(sb[0].st_mode) == 0) &&
			sb[0].st_dev == sb[1].st_dev && sb[0].st_ino == sb[1].st_ino)
		return (fd);
	shell_close(shell, fd);
	errno = EEXIST;
	return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
		" mode", file), shell_error(shell, err, SRC_LOCATION), -1);
}

int
	redir_open(t_shell *shell, t_redirection *redir)
{
	char *const	filename = redir_expand(shell, redir);
	int			fd;
	char		*err;

	// TODO: Handle special filenames e.g /dev/std* and /dev/fd*
	if (!filename)
		return (-1);
	if (option_value(shell, "noclobber") && redir_is_clobbering(redir))
	{
		fd = noclobber_open(shell, redir->flags, filename);
		free(filename);
		return (fd);
	}
	fd = shell_open(shell, filename, redir->flags, 0666);
	if (fd < 0 && errno == EINTR)
		shell_perror(shell, "Interrupted by syscall", SRC_LOCATION);
	else if (fd < 0)
		return (ft_asprintf(&err, "Failed to open `%s`: %m", filename),
			shell_error(shell, err, SRC_LOCATION), -1);
	free(filename);
	return (fd);
}
