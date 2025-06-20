/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capability.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:28:56 by thschnei          #+#    #+#             */
/*   Updated: 2025/05/13 09:34:34 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <term/capability.h>

static void
	fetch_num(t_numcap *nu)
{
	const char	*n[] = {"co", "it", "li", "lm", "sg", "pb", "vt", "ws", "Nl",
		"lh", "lw", "ma", "MW", "Co", "pa", "NC", "Ya", "Yb", "Yc", "Yd", "Ye",
		"Yf", "Yg", "Yh", "Yi", "Yj", "Yk", "Yl", "Ym", "Yn", "BT", "Yo", "Yp"};
	int			i;

	i = 0;
	while (i < (signed)(sizeof(n) / sizeof(n[0])))
	{
		nu->array[i] = tgetnum(n[i]);
		i++;
	}
}

static void
	fetch_flag(t_flagcap *f)
{
	const char	*n[] = {"bw", "am", "xb", "xs", "xn", "eo", "gn", "hc", "km",
		"hs", "in", "da", "db", "mi", "ms", "os", "es", "xt", "hz", "ul", "xo",
		"nx", "5i", "HC", "NR", "NP", "ND", "cc", "ut", "hl", "YA", "YB", "YC",
		"YD", "YE", "YF", "YG"};
	int			i;

	i = 0;
	while (i < (signed)(sizeof(n) / sizeof(n[0])))
	{
		f->array[i] = tgetflag(n[i]);
		i++;
	}
}

static void
	fetch_str2(t_strcap *s)
{
	const char	*n[] = {"#4", "%a", "%b", "%c", "%d", "%e", "%f", "%g", "%h",
		"%i", "%j", "!1", "!2", "!3", "RF", "F1", "F2", "F3", "F4", "F5", "F6",
		"F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF", "FG", "FH", "FI",
		"FJ", "FK", "FL", "FM", "FN", "FO", "FP", "FQ", "FR", "FS", "FT", "FU",
		"FV", "FW", "FX", "FY", "FZ", "Fa", "Fb", "Fc", "Fd", "Fe", "Ff", "Fg",
		"Fh", "Fi", "Fj", "Fk", "Fl", "Fm", "Fn", "Fo", "Fp", "Fq", "Fr", "cb",
		"MC", "ML", "MR", "Lf", "SC", "DK", "RC", "CW", "WG", "HU", "DI", "QD",
		"TO", "PU", "fh", "PA", "WA", "u0", "u1", "u2", "u3", "u4", "u5", "u6",
		"u7", "u8", "u9", "op", "oc", "Ic", "Ip", "sp", "Sf", "Sb", "ZA", "ZB",
		"ZC", "ZD", "ZE", "ZF", "ZG", "ZH", "ZI", "ZJ", "ZK", "ZL", "ZM", "ZN",
		"ZO", "ZP", "ZQ", "ZR", "ZS", "ZT", "ZU", "ZV", "ZW", "ZX", "ZY", "ZZ",
		"Za", "Zb", "Zc", "Zd", "Ze", "Zf", "Zg", "Zh", "Zi", "Zj", "Zk", "Zl",
		"Zm", "Zn", "Zo", "Zp", "Zq", "Zr", "Zs", "Zt", "Zu", "Zv", "Zw", "Zx",
		"Zy", "Km", "Mi", "RQ", "Gm", "AF", "AB", "xl", "dv", "ci", "s0", "s1",
		"s2", "s3", "ML", "MT", "Xy", "Zz", "Yv", "Yw", "Yx", "Yy", "Yz", "YZ",
		"S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "Xh", "Xl", "Xo", "Xr",
		"Xt", "Xv", "sA", "YI"};
	int			i;

	i = 0;
	while (i < (signed)(sizeof(n) / sizeof(n[0])))
	{
		s->array[201 + i] = tgetstr(n[i], NULL);
		i++;
	}
}

static void
	fetch_str1(t_strcap *s)
{
	const char	*n[] = {"bt", "bl", "cr", "cs", "ct", "cl", "ce", "cd", "ch",
		"CC", "cm", "do", "ho", "vi", "le", "CM", "ve", "nd", "ll", "up", "vs",
		"dc", "dl", "ds", "hd", "as", "mb", "md", "ti", "dm", "mh", "im", "mk",
		"mp", "mr", "so", "us", "ec", "ae", "me", "te", "ed", "ei", "se", "ue",
		"vb", "ff", "fs", "i1", "is", "i3", "if", "ic", "al", "ip", "kb", "ka",
		"kC", "kt", "kD", "kL", "kd", "kM", "kE", "kS", "k0", "k1", "k;", "k2",
		"k3", "k4", "k5", "k6", "k7", "k8", "k9", "kh", "kI", "kA", "kl", "kH",
		"kN", "kP", "kr", "kF", "kR", "kT", "ku", "ke", "ks", "l0", "l1", "la",
		"l2", "l3", "l4", "l5", "l6", "l7", "l8", "l9", "mo", "mm", "nw", "pc",
		"DC", "DL", "DO", "IC", "SF", "AL", "LE", "RI", "SR", "UP", "pk", "pl",
		"px", "ps", "pf", "po", "rp", "r1", "r2", "r3", "rf", "rc", "cv", "sc",
		"sf", "sr", "sa", "st", "wi", "ta", "ts", "uc", "hu", "iP", "K1", "K3",
		"K2", "K4", "K5", "pO", "rP", "ac", "pn", "kB", "SX", "RX", "SA", "RA",
		"XN", "XF", "eA", "LO", "LF", "@1", "@2", "@3", "@4", "@5", "@6", "@7",
		"@8", "@9", "@0", "%1", "%2", "%3", "%4", "%5", "%6", "%7", "%8", "%9",
		"%0", "&1", "&2", "&3", "&4", "&5", "&6", "&7", "&8", "&9", "&0", "*1",
		"*2", "*3", "*4", "*5", "*6", "*7", "*8", "*9", "*0", "#1", "#2", "#3"};
	int			i;

	i = 0;
	while (i < (signed)(sizeof(n) / sizeof(n[0])))
	{
		s->array[i] = tgetstr(n[i], NULL);
		i++;
	}
}

t_termcap
	fetch_capability(char *term)
{
	t_termcap	cap;

	ft_memset(&cap, 0, sizeof(t_termcap));
	cap.success = tgetent(NULL, term);
	if (cap.success == 0)
		cap.success = tgetent(NULL, "linux");
	if (cap.success > 0)
	{
		fetch_str1(&cap.strs);
		fetch_str2(&cap.strs);
		fetch_num(&cap.nums);
		fetch_flag(&cap.flags);
	}
	return (cap);
}
