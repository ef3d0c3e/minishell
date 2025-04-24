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
#include <shell/redir/eval_redir.h>

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
			" mode", filename), shell_error(env, err, SRC_LOCATION), -1);
	flags &= ~O_TRUNC;
	if (status != 0)
	{
		fd = open (filename, flags | O_EXCL, 0666);
		if (fd < 0 || errno == EEXIST)
			return (ft_asprintf(&err, "Failed to open `%s` for writing in "
				"noclobber mode", filename),
				shell_error(env, err, SRC_LOCATION), -1);
		return (fd);
	}
	fd = open (filename, flags, 0666);
	if (fd < 0)
		return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
			" mode", filename), shell_error(env, err, SRC_LOCATION), -1);
	if ((fstat(fd, &sb[1]) == 0) && (S_ISREG(sb[1].st_mode) == 0) && status == 0
			&& (S_ISREG(sb[0].st_mode) == 0) &&
			sb[0].st_dev == sb[1].st_dev && sb[0].st_ino == sb[1].st_ino)
		return (fd);
	close(fd);
	errno = EEXIST;
	return (ft_asprintf(&err, "Failed to open `%s` for writing in noclobber"
		" mode", filename), shell_error(env, err, SRC_LOCATION), -1);
}

int
	redir_open(t_shell *shell, t_redirection *redir)
{
	const char	*filename = stringbuf_cstr(&redir->redirectee.filename);
	int			fd;
	char		*err;

	// TODO: Handle special filenames e.g /dev/std* and /dev/fd*
	
	if (option_value(env, "noclobber") && redir_is_clobbering(redir))
		return (noclobber_open(env, redir, redir->flags));
	fd = open(filename, redir->flags, 0666);
	if (fd < 0 && errno == EINTR)
		shell_perror(env, "Interrupted by syscall", SRC_LOCATION);
	//else if (fd < 0)
	//	return (ft_asprintf(&err, "Failed to open `%s`: %m", filename),
	//		shell_error(env, err, SRC_LOCATION), -1);
	return (fd);
}
