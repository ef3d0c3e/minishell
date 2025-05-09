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
#include <shell/shell.h>

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

/// Read exactly one byte, with the VMIN/VTIME you’ve configured in raw mode.
/// Returns:  1 on success (byte read into *out), 0 on timeout, –1 on error.
static int read_byte(int fd, char *out) {
    int n = read(fd, out, 1);
    if (n == 1)        return 1;
    if (n == 0)        return 0;    // timeout (VTIME expired)
    if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) return 0;
    return -1;                       // real error
}

/// Wait until we see the CSI introducer `ESC [`
/// Returns 1 if seen, 0 on timeout, –1 on error.
static int await_csi(int fd) {
    char c;
    // Scan until we see ESC
    while (1) {
        int r = read_byte(fd, &c);
        if (r <= 0) return r;
        if (c == '\x1b') break;
    }
    // Next must be '['
    if (read_byte(fd, &c) != 1) return 0;
    if (c != '[') return await_csi(fd);  // stray ESC something — keep looking
    return 1;
}

/// Read the numeric row and column from the stream after CSI has been seen.
/// Expects digits, then ‘;’, then digits, then ‘R’.
static int read_cursor_report(int fd, int *out_row, int *out_col) {
    enum { BUF_SZ = 32 };
    char buf[BUF_SZ];
    size_t i = 0;
    char c;

    // Read until ‘R’ or buffer full
    while (i < BUF_SZ - 1) {
        int r = read_byte(fd, &c);
        if (r <= 0) return -1;   // timeout or error
        buf[i++] = c;
        if (c == 'R') break;
    }
    buf[i] = '\0';

    // Now buf should look like: "<digits>;<digits>R"
    int row = 0, col = 0;
    if (sscanf(buf, "%d;%dR", &row, &col) != 2) {
        return -1;
    }
    *out_row = row;
    *out_col = col;
    return 0;
}

/// Public API: move cursor to a safe spot, ask for position, parse column, restore line above
int read_cursor_column(int in_fd) {
    int row, col;
    // Ask terminal to report: ESC[6n
    if (write(in_fd, "\x1b[6n", 4) != 4) return -1;

    // Wait for CSI
    if (await_csi(in_fd) != 1) return -1;

    // Read the `row;colR` and parse
    if (read_cursor_report(in_fd, &row, &col) != 0) return -1;

    return col;
}




int measure_terminal_width(t_getline *line,
		const char *utf8,
		size_t byte_len) {
	// 1) move to a known safe spot (e.g. start of spare line)
	write(line->out_fd, "\r\n", 2);
	write(line->out_fd, "\x1b[6n", 4);
	if (write(line->in_fd, "\x1b[6n", 4) != 4) return -1;
	int col0 = read_cursor_column(line->in_fd);

	// 2) write the test sequence
	write(line->out_fd, utf8, byte_len);

	// 3) ask again
	write(line->out_fd, "\x1b[6n", 4);
	int col1 = read_cursor_column(line->in_fd);

	// 4) restore up one line
	write(line->out_fd, "\x1b[1A\r", 4);

	return col1 - col0;
}

char
	*getline_read(t_getline *line, char *prompt)
{
	int				c;
	t_keybind_fn	callback;

	getline_raw_mode(line, 1);
	ft_dprintf(2, "Width =%d\n\r", read_cursor_column(line->in_fd));
	ft_dprintf(0, "🧑‍🌾");
	ft_dprintf(2, "Width =%d\n\r", read_cursor_column(line->in_fd));
	//while (1)
	//{
	//	c = line->getc(line->in_fd);
	//	if (c == -1)
	//		break ;
	//	if (c == '\003')
	//	{
	//		write(0, "\033[6n", 4);
	//		continue ;
	//	}
	//	if (c == '\004')
	//		break ;
	//	if (getline_handle_key(line, c))
	//		continue ;
	//	getline_buffer_insert(line, c);
	//	ft_dprintf(2, "Width =%d\n\r", read_cursor_column(line->in_fd));
	//	ft_dprintf(2, "Buffer='%.*s'\n\r", line->buffer.buffer.len, line->buffer.buffer.str);

	//}
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
