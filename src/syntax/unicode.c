#include "unicode.h"
#include <sys/types.h>

/* Codepoint length in bytes */
static inline ssize_t
	cp_len(t_string_view s)
{
	if (!s.len)
		return (0);
	if (~(s.s[0]) & 0x80) 
		return (1);
	else if (~(s.s[0] & 0xE0) & 0x20)
	{
		if (s.len < 1)
			return (0);
		else if (~(s.s[1] & 0xB0) & 0x40)
			return (2);
		else
			return (-1);
	}
	else if (~(s.s[0] & 0xF0) & 0x10)
	{
		if (s.len < 2)
			return (0);
		else if ((~(s.s[1] & 0xB0) & 0x40) && (~(s.s[2] & 0xB0) & 0x40))
			return (3);
		else
			return (-1);
	}
	else if (~(s.s[0] & 0xF8) & 0x08)
	{
		if (s.len < 3)
			return (0);
		else if ((~(s.s[1] & 0xB0) & 0x40) && ( ~(s.s[2] & 0xB0) & 0x40) && (~(s.s[3] & 0xB0) & 0x40))
			return (4);
		else
			return (-1);
	}
}

t_u8_iterator
	iterator_new(const char *s)
{
	const t_string_view	sv = {
		.s = s,
		.len = ft_strlen(s),
	};

	return ((t_u8_iterator){
		.sv = sv,
		.byte_pos = 0,
		.cp_pos = 0,
		.cp_len = cp_len(sv),
	});
}

t_string_view
	iterator_next(t_u8_iterator *it)
{

}
