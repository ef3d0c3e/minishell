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

typedef struct s_shell	t_shell;

enum e_fd_type
{
	/** @brief FD comes from `open()` */
	FDT_OPEN,
	/** @brief FD comes from `dup()` */
	FDT_DUP,
	/** @brief FD comes from `pipe()` */
	FDT_PIPE,
};

typedef struct s_fd_data
{
	enum e_fd_type	type;
	/** @brief Original filename, may be NULL */
	char			*filename;
	/** @brief Open flags (unset when irrelevant) */
	int				flags;
	/** @brief Open mode (unset when irrelevant) */
	int				mode;
	/** @brief FD shadowed by this one, for `dup2` */
	int				duped_to;
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
 * @brief Checks that a fd is open and that it supports the following mask
 *
 * @param fd Fd to check for
 * @param mask Mask to check against
 *
 * @returns `fd_data->flag & mask` (>0) if the check succeeds.
 * -1 If the file descriptor is not found, or 0 if the mask fails.
 */
int
fd_check(t_shell *shell, int fd, int mask);

#endif // FDS_H
