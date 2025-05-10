/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "getline/getline.h"
#include <shell/shell.h>
#include <unistd.h>

t_getline
	getline_setup(t_shell *shell)
{
	t_getline	line;

	line.shell = shell;
	line.in_fd = STDIN_FILENO;
	line.out_fd = STDOUT_FILENO;
	line.getc = getline_getc;
	line.prompt = NULL;
	line.buffer = getline_buffer_new();
	line.cursor_index = 0;
	line.display_width = 0;
	stringbuf_init(&line.input_queue, 24);
	getline_setup_keys(&line);
	return (line);
}

void
	getline_cleanup(t_getline *line)
{

}

void
	getline_raw_mode(t_getline *line, int set)
{
	struct termios	raw;

	if (!set)
	{
		tcsetattr(line->in_fd, TCSAFLUSH, &line->tio);
		return ;
	}
	if (!isatty(line->in_fd))
		return ;
	if (tcgetattr(line->in_fd, &line->tio) == -1)
	{
		shell_perror(line->shell, "tcgetattr fail", SRC_LOCATION);
		return ;
	}

	raw = line->tio;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN]  = 1;
	raw.c_cc[VTIME] = 1;
	if (tcsetattr(line->in_fd, TCSAFLUSH, &raw) == -1)
	{
		shell_perror(line->shell, "tcsetattr fail", SRC_LOCATION);
	}
}

int
	measure_terminal_width(t_getline *line, const char *utf8, size_t byte_len)
{
	int	col0;
	int	col1;

	//write(line->out_fd, "\r\n", 2);
	if (getline_cursor_pos(line, &col0, NULL) == -1)
		return (-1);

	write(line->in_fd, utf8, byte_len);
	if (getline_cursor_pos(line, &col1, NULL) == -1)
		return (-1);
	//write(line->out_fd, "\x1b[1A\r", 5); 

	// col0=1 col1=1
	//ft_dprintf(2, "[%d %d]", col0, col1);
	return (col1 - col0);
}

/*
void	recluster_around(t_getline *line, t_u8_iterator it)
{
	t_u8_iterator	lo;
	t_u8_iterator	hi;
	int				base_sum;
	int				changed;

	lo = it;
	hi = it;
	it_next(&hi);
	base_sum = measure_terminal_width(line, lo.str.str + lo.byte_pos,
			hi.byte_pos - lo.byte_pos);

	// Now attempt to expand outward while grouping reduces width
	while (1)
	{
		if (lo.cp_pos > 0)
		{
			int sum = sum_width(lo-1, hi);
			int grp = measure_width(lo-1, hi);
			if (grp < sum)
			{
				lo--; base_sum = grp; changed = true;
			}
		}
		// try extending right
		if (hi.codepoint.len)
		{
			int sum = base_sum + sum_width(hi, hi+1);
			int grp = measure_width(lo, hi+1);
			if (grp < sum) {
				hi++; base_sum = grp; changed = true;
			}
		}
	}
	do {
		changed = 0;
		// try extending left
		if (lo > 0) {
			int sum = sum_width(lo-1, hi);
			int grp = measure_width(lo-1, hi);
			if (grp < sum) {
				lo--; base_sum = grp; changed = true;
			}
		}
		// try extending right
		if (hi < cp_count) {
			int sum = base_sum + sum_width(hi, hi+1);
			int grp = measure_width(lo, hi+1);
			if (grp < sum) {
				hi++; base_sum = grp; changed = true;
			}
		}
	} while (changed);

	// Now [lo..hi) is the maximal cluster around k
	record_cluster(lo, hi, base_sum);
	// Anything inside [lo..hi) will not be reclustered again.
}
*/

void recluster_around(t_getline *line, size_t k);
static size_t
find_cluster_index_and_offset(t_getline *line,
                              size_t byte_pos,
                              size_t *out_cluster_start)
{
    size_t pos = 0;
    size_t i   = 0;
    while (i < line->buffer.s_clusters.size)
    {
        size_t sz = line->buffer.s_clusters.data[i].size;
        if (byte_pos < pos + sz)
        {
            if (out_cluster_start) *out_cluster_start = pos;
            return i;
        }
        pos += sz;
        i++;
    }
    if (out_cluster_start) *out_cluster_start = pos;
    return line->buffer.s_clusters.size; // past‑end
}

void
	getline_input_add(t_getline *line, int c)
{
	t_u8_iterator	it;
	getline_buffer_insert(line, c);
	if (line->buffer.cp_len) /* Unterminated codepoint */
		return ;

	ft_dprintf(2, "Needs cluster\n\r");
	it = it_new((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	it_next(&it);
	while (it.byte_next < line->cursor_index)
		it_next(&it);
	ft_dprintf(2, "at=%zu\n\r", it.byte_pos);

	//size_t cluster_start;
	//size_t ci = find_cluster_index_and_offset(line, it.byte_pos, &cluster_start);
	size_t k = getline_cluster_insert(line, it.byte_pos, line->cursor_index, 1);
	//recluster_around(line, ci);
	//getline_cluster_print(line);
}


char
	*getline_read(t_getline *line, char *prompt)
{
	int				c;

	getline_raw_mode(line, 1);

	while (1)
	{
		c = getline_read_char(line);
		if (c == -1)
			break ;
		if (c == '\003')
			break ;
		if (c == '\004')
			break ;
		if (getline_handle_key(line, c))
			continue ;
		getline_input_add(line, c);
		int w = measure_terminal_width(line, line->buffer.buffer.str, line->buffer.buffer.len);
		ft_dprintf(2, "Width =%d\n\r", w);
		ft_dprintf(2, "Buffer='%.*s'\n\r", line->buffer.buffer.len, line->buffer.buffer.str);
	}
	getline_raw_mode(line, 0);

	return (NULL);
}

int main(int ac, const char **av, const char **envp)
{
	t_shell shell = shell_new(envp);

	t_getline line = getline_setup(&shell);

	getline_read(&line, ft_strdup(" > "));

	shell_free(&shell);
	return (0);
}
