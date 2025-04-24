/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tester.h"
#include "util/util.h"
#include <ftw.h>

static int
	delete(
	const char *pathname,
	const struct stat *sbuf,
	int type,
	struct FTW *ftwb)
{
	(void)sbuf;
	(void)type;
	(void)ftwb;
	if (ft_strncmp(pathname, "/tmp/shelltest-", 15))
	{
		ft_dprintf(2, "FATAL: Attempted to remove '%s'\n", pathname);
		return (-1);
	}
	if (remove(pathname) < 0)
	{
		perror("remove");
		return (-1);
	}
	return (0);
}

void
	eval_in_tempdir(const char *name, void (*fn)(void *), void *cookie)
{
	char	*dirname;
	char	*curpath;

	curpath = getcwd(NULL, 0);
	if (!curpath)
		perror("getcwd");
	ft_asprintf(&dirname, "/tmp/shelltest-%s", name);
	if (mkdir(dirname, 0700) < 0)
		perror("mkdir");
	if (chdir(dirname) < 0)
		perror("chdir");
	fn(cookie);
	if (chdir(curpath) < 0)
		perror("chdir");
	if (nftw(dirname, delete, 2, FTW_DEPTH | FTW_MOUNT | FTW_PHYS) < 0)
		perror("nftw");
	free(dirname);
	free(curpath);
}

static int
	test_content(
	t_file_test *test)
{
	int				fd;
	t_string_buffer	original;
	t_string_buffer	read;
	int				status;

	ft_dprintf(2, "(%s): ", test->path);
	original = stringbuf_from_range(test->content, test->content + test->size);
	fd = open(test->path, O_RDONLY);
	stringbuf_init(&read, 1024);
	read_incoming(fd, &read);
	status = stringbuf_compare("", &original, &read);
	if (status)
		ft_dprintf(2, "\033[0;32mOK\033[0;0m\n");
	else
		ft_dprintf(2, "\033[0;31mFailed\033[0;0m (%s#%zu)\n",
			test->file, test->line);
	stringbuf_free(&original);
	stringbuf_free(&read);
	close(fd);
	return (status);
}

int
	test_file_run(t_file_test *test, size_t id)
{
	ft_dprintf(2, "#%zu ", id);
	return (test_content(test));
}
