/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDS_H
# define FDS_H

# include <util/util.h>

typedef struct s_shell	t_shell;

enum e_fd_type
{
	/** @brief FD comes from `open()` */
	FDT_OPEN,
	/** @brief FD comes from `dup()` */
	FDT_DUP,
	/** @brief FD comes from `pipe()` (read end) */
	FDT_PIPE_R,
	/** @brief FD comes from `pipe()` (write end) */
	FDT_PIPE_W,
};

/** @brief Per file-descriptor data */
typedef struct s_fd_data
{
	enum e_fd_type	type;
	/** @brief Original filename, may be NULL */
	char			*filename;
	/** @brief Open flags (unset when irrelevant) */
	int				flags;
	/** @brief Open mode (unset when irrelevant) */
	int				mode;
	/** @brief FD shadowed by this one, for `dup2` (-1 terminated) */
	int				*duped_to;
	/** @brief Original file descriptor for `dup`ed fds */
	int				duped_from;
	/** @brief Other end of the pipe for `pipe` fds */
	int				pipe;
}	t_fd_data;

/**
 * @brief Initializes fd-related data for the shell
 *
 * @param shell The shell session
 */
void
fd_data_init(t_shell *shell);
/**
 * @brief Frees fd-related data
 *
 * @param fd Fd data to free
 */
void
fd_data_free(t_fd_data *fd);
/**
 * @brief Creates a new fd data
 *
 * @param type The type of file descriptor
 * @param filename The fd's filename (NULL for none)
 * @param flags The flags for `open` (0 for none)
 * @param mode The mode for `open` (0 for none)
 *
 * @returns The newly created fd data
 */
t_fd_data
fd_data_from(enum e_fd_type type, char *filename, int flags, int mode);
/**
 * @brief Creates a new fd data from an existing one
 *
 * @param data The existing fd data
 *
 * @returns A copy of data
 */
t_fd_data
fd_data_clone(t_fd_data *data);
/**
 * @brief Checks that a fd is open and that it supports given operations
 *
 * @param shell The shell session
 * @param fd Fd to check for
 * @param read Check if read is supported
 * @param write Check if write is supported
 *
 * @returns Returns -1 if the fd is not found, 0 if one of the operation is
 * not supported, >0 if all operations are supported and the fd exists
 */
int
fd_check(t_shell *shell, int fd, int read, int write);
/**
 * @brief Prints fd data to string
 *
 * @param data Data to print
 *
 * @returns Data printed to string
 */
char
*fd_print(const t_fd_data *data);
/**
 * @brief Ensures uniqueness of a file descriptor
 *
 * @param shell Shell session
 * @param fd File descriptor to check for uniqueness
 * @param data Data associated with `fd`
 * @param loc Check location
 */
void
fd_unique(t_shell *shell, int fd, const t_fd_data *data, const char *loc);

/******************************************************************************/
/* Wrappers                                                                   */
/******************************************************************************/

/**
 * @brief Wrapper for `open`
 *
 * On success, this function will register the opened file descriptor
 * in the shell's fd registry.
 *
 * @warning In case the file descriptor is already registered, an error is
 * thrown
 *
 * @param shell The shell session
 * @param filename Filename to `open`
 * @param flags Open flags
 * @param mode Open mode
 *
 * @returns The opened file descriptor, -1 on error and set errno.
 */
int
shell_open(t_shell *shell, const char *filename, int flags, int mode);
/**
 * @brief Wrapper for `close`
 *
 * On success, this function will unregister the opened file descriptor
 * in the shell's fd registry.
 *
 * @warning In case the file descriptor was not previously registered, an error
 * is thrown.
 *
 * @param shell The shell session
 * @param fd File descriptor to `close`
 *
 * @returns 0 on success, -1 on error and set errno.
 */
int
shell_close(t_shell *shell, int fd);
/**
 * @brief Wrapper for `dup`
 *
 * On success, this function will register the newly created copy of `fd`.
 *
 * @warning In case the file descriptor was not previously registered, an error
 * is thrown.
 *
 * @param shell The shell session
 * @param fd File descriptor to `dup`
 *
 * @returns the new file descriptor on success, -1 on error and set errno.
 */
int
shell_dup(t_shell *shell, int fd);
/**
 * @brief Wrapper for `dup2`
 *
 * On success, this function will mark newfd as shadowing oldfd.
 *
 * @warning In case the file descriptor was not previously registered, an error
 * is thrown.
 *
 * @param shell The shell session
 * @param fd File descriptor to `dup`
 *
 * @returns the new file descriptor on success, -1 on error and set errno.
 */
int
shell_dup2(t_shell *shell, int oldfd, int newfd);
/**
 * @brief Wrapper for `pipe`
 *
 * @param shell The shel lsession
 * @param fds Pipe result
 * @return 0 on success, -1 on failure
 */
int
shell_pipe(t_shell *shell, int fds[2]);

#endif // FDS_H
