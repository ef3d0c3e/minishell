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
#include <shell/shell.h>

/** @brief Checks whether a redirection is clobbering */
static int
	redir_is_clobbering(const t_redirection *redir)
{
	return (redir->type == R_OUTPUT_DIRECTION || redir->type == R_ERR_AND_OUT);
}

/** @brief Opens a file in noclobber moder, returns -1 and reports on failure */
static int
	noclobber_open(t_shell *shell, t_redirection *redir, int flags)
{
	const char	*filename = stringbuf_cstr(&redir->redirectee.filename);
	struct stat	sb[2];
	int			status;
	int			fd;
	char		*err;

	status = stat(filename, &sb[0]);
	if (!status && (S_ISREG(sb[0].st_mode)))
		return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
			" mode", filename), shell_error(shell, err, SRC_LOCATION), -1);
	flags &= ~O_TRUNC;
	if (status != 0)
	{
		fd = shell_open(shell, filename, flags | O_EXCL, 0666);
		if (fd < 0 || errno == EEXIST)
			return (ft_asprintf(&err, "Failed to open `%s` for writing in "
				"noclobber mode", filename),
				shell_error(shell, err, SRC_LOCATION), -1);
		return (fd);
	}
	fd = shell_open(shell, filename, flags, 0666);
	if (fd < 0)
		return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
			" mode", filename), shell_error(shell, err, SRC_LOCATION), -1);
	if ((fstat(fd, &sb[1]) == 0) && (S_ISREG(sb[1].st_mode) == 0) && status == 0
			&& (S_ISREG(sb[0].st_mode) == 0) &&
			sb[0].st_dev == sb[1].st_dev && sb[0].st_ino == sb[1].st_ino)
		return (fd);
	shell_close(shell, fd);
	errno = EEXIST;
	return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
		" mode", filename), shell_error(shell, err, SRC_LOCATION), -1);
}

int
	redir_open(t_shell *shell, t_redirection *redir)
{
	const char	*filename = stringbuf_cstr(&redir->redirectee.filename);
	int			fd;
	char		*err;

	// TODO: Handle special filenames e.g /dev/std* and /dev/fd*
	if (option_value(shell, "noclobber") && redir_is_clobbering(redir))
		return (noclobber_open(shell, redir, redir->flags));
	fd = shell_open(shell, filename, redir->flags, 0666);
	if (fd < 0 && errno == EINTR)
		shell_perror(shell, "Interrupted by syscall", SRC_LOCATION);
	//else if (fd < 0)
	//	return (ft_asprintf(&err, "Failed to open `%s`: %m", filename),
	//		shell_error(shell, err, SRC_LOCATION), -1);
	return (fd);
}
