/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   profile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	try_source(t_shell *shell, const char *path)
{
	char	*cmd;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	ft_asprintf(&cmd, "source '%s'", path);
	ctx_eval_stdout(shell, cmd);
	return (1);
}

static void
	default_profile(t_shell *shell)
{
	char	*cmd;

	ft_asprintf(&cmd, "%s\n", "prompt_left() { echo -n \"$PWD\" '$ '; }");
	ctx_eval_stdout(shell, cmd);
}

void
	profile_source(t_shell *shell)
{
	t_pathbuf	buf;
	int			status;

	status = 0;
	default_profile(shell);
	pathbuf_init(&buf, 24);
	if (pathbuf_append(&buf, get_variable_value(shell, "SHELL_PROFILE"), 0))
		status = try_source(shell, pathbuf_cstr(&buf));
	if (!status && pathbuf_append(&buf, get_variable_value(shell, "HOME"), 1))
	{
		pathbuf_append(&buf, ".hshrc", 0);
		status = try_source(shell, pathbuf_cstr(&buf));
	}
	if (!status && pathbuf_append(&buf, get_variable_value(shell,
				"XDG_CONFIG_HOME"), 1))
	{
		pathbuf_append(&buf, "/hsh/profile.sh", 0);
		status = try_source(shell, pathbuf_cstr(&buf));
	}
	pathbuf_free(&buf);
}
