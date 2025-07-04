/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Saves the history */
static void
	save_history(t_getline *line, size_t num)
{
	int				fd;
	size_t			i;

	fd = -1;
	if (line->history.histfile)
		fd = open(line->history.histfile, O_APPEND | O_WRONLY);
	i = 0;
	while (i++ < num)
	{
		if (fd >= 0 && !line->history.entries[i - 1].saved)
			ft_dprintf(fd, "%s\n", line->history.entries[i - 1].input);
		free(line->history.entries[i - 1].input);
	}
	if (fd >= 0)
		close(fd);
	ft_memmove(line->history.entries, line->history.entries + num,
		(line->history.num_entries - num) * sizeof(t_history_ent));
	line->history.num_entries -= num;
}

t_history
	getline_history_init(size_t max_entries)
{
	return ((t_history){
		.entries = xmalloc(max_entries * sizeof(t_history_ent)),
		.num_entries = 0,
		.max_entries = max_entries,
		.histfile = NULL,
	});
}

void
	getline_history_free(t_getline *line)
{
	size_t	i;

	if (!line->shell->is_child)
		save_history(line, line->history.num_entries);
	else
	{
		i = 0;
		while (i < line->history.num_entries)
			free(line->history.entries[i++].input);
	}
	free(line->history.entries);
	free(line->history.histfile);
}

void
	getline_history_set_file(t_getline *line, char *histfile, int source)
{
	int		fd;
	char	*old;

	free(line->history.histfile);
	line->history.histfile = histfile;
	if (!source)
		return ;
	fd = open(histfile, O_RDONLY);
	if (fd < 0)
		return ;
	old = get_next_line(fd);
	while (old)
	{
		getline_history_add(line, ft_strdup(old), 1);
		old = get_next_line(fd);
	}
	close(fd);
}

void
	getline_history_add(t_getline *line, char *entry, int saved)
{
	size_t	i;

	if (line->history.num_entries >= line->history.max_entries)
		save_history(line, line->history.num_entries / 2);
	i = 0;
	while (entry[i])
	{
		if (entry[i] == '\n')
		{
			entry[i] = 0;
			break ;
		}
		++i;
	}
	if (line->history.num_entries && !ft_strcmp(entry,
			line->history.entries[line->history.num_entries - 1].input))
	{
		free(entry);
		return ;
	}
	line->history.entries[line->history.num_entries++] = (t_history_ent){
		.input = entry,
		.saved = saved,
	};
}
