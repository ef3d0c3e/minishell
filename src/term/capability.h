/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capability.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:10:01 by thschnei          #+#    #+#             */
/*   Updated: 2025/06/19 17:36:12 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPABILITY_H
# define CAPABILITY_H
# include <util/util.h>

/* Field names won't be described here.
 * For further details on what they do man terminfo */

/* Struct containing all bool fields of the db
 * plus truecolor support.*/

typedef union e_flagcap
{
	int	array[38];
	struct
	{
		/** @brief Terminfo `auto_left_margin`
		 *
		 * cub1 wraps from column 0 to last column
		 *
		 * TI Name: `bw`
		 * TC Name: `bw`
		 */
		int	bw;
		/** @brief Terminfo `auto_right_margin`
		 *
		 * terminal has automatic margins
		 *
		 * TI Name: `am`
		 * TC Name: `am`
		 */
		int	am;
		/** @brief Terminfo `no_esc_ctlc`
		 *
		 * beehive (f1=escape, f2=ctrl C)
		 *
		 * TI Name: `xsb`
		 * TC Name: `xb`
		 */
		int	xsb;
		/** @brief Terminfo `ceol_standout_glitch`
		 *
		 * standout not erased by overwriting (hp)
		 *
		 * TI Name: `xhp`
		 * TC Name: `xs`
		 */
		int	xhp;
		/** @brief Terminfo `eat_newline_glitch`
		 *
		 * newline ignored after 80 cols (concept)
		 *
		 * TI Name: `xenl`
		 * TC Name: `xn`
		 */
		int	xenl;
		/** @brief Terminfo `erase_overstrike`
		 *
		 * can erase overstrikes with a blank
		 *
		 * TI Name: `eo`
		 * TC Name: `eo`
		 */
		int	eo;
		/** @brief Terminfo `generic_type`
		 *
		 * generic line type
		 *
		 * TI Name: `gn`
		 * TC Name: `gn`
		 */
		int	gn;
		/** @brief Terminfo `hard_copy`
		 *
		 * hardcopy terminal
		 *
		 * TI Name: `hc`
		 * TC Name: `hc`
		 */
		int	hc;
		/** @brief Terminfo `has_meta_key`
		 *
		 * Has a meta key (i.e., sets 8th-bit)
		 *
		 * TI Name: `km`
		 * TC Name: `km`
		 */
		int	km;
		/** @brief Terminfo `has_status_line`
		 *
		 * has extra status line
		 *
		 * TI Name: `hs`
		 * TC Name: `hs`
		 */
		int	hs;
		/** @brief Terminfo `insert_null_glitch`
		 *
		 * insert mode distinguishes nulls
		 *
		 * TI Name: `in`
		 * TC Name: `in`
		 */
		int	in;
		/** @brief Terminfo `memory_above`
		 *
		 * display may be retained above the screen
		 *
		 * TI Name: `da`
		 * TC Name: `da`
		 */
		int	da;
		/** @brief Terminfo `memory_below`
		 *
		 * display may be retained below the screen
		 *
		 * TI Name: `db`
		 * TC Name: `db`
		 */
		int	db;
		/** @brief Terminfo `move_insert_mode`
		 *
		 * safe to move while in insert mode
		 *
		 * TI Name: `mir`
		 * TC Name: `mi`
		 */
		int	mir;
		/** @brief Terminfo `move_standout_mode`
		 *
		 * safe to move while in standout mode
		 *
		 * TI Name: `msgr`
		 * TC Name: `ms`
		 */
		int	msgr;
		/** @brief Terminfo `over_strike`
		 *
		 * terminal can overstrike
		 *
		 * TI Name: `os`
		 * TC Name: `os`
		 */
		int	os;
		/** @brief Terminfo `status_line_esc_ok`
		 *
		 * escape can be used on the status line
		 *
		 * TI Name: `eslok`
		 * TC Name: `es`
		 */
		int	eslok;
		/** @brief Terminfo `dest_tabs_magic_smso`
		 *
		 * tabs destructive, magic so char (t1061)
		 *
		 * TI Name: `xt`
		 * TC Name: `xt`
		 */
		int	xt;
		/** @brief Terminfo `tilde_glitch`
		 *
		 * cannot print ~'s (Hazeltine)
		 *
		 * TI Name: `hz`
		 * TC Name: `hz`
		 */
		int	hz;
		/** @brief Terminfo `transparent_underline`
		 *
		 * underline character overstrikes
		 *
		 * TI Name: `ul`
		 * TC Name: `ul`
		 */
		int	ul;
		/** @brief Terminfo `xon_xoff`
		 *
		 * terminal uses xon/xoff handshaking
		 *
		 * TI Name: `xon`
		 * TC Name: `xo`
		 */
		int	xon;
		/** @brief Terminfo `needs_xon_xoff`
		 *
		 * padding will not work, xon/xoff required
		 *
		 * TI Name: `nxon`
		 * TC Name: `nx`
		 */
		int	nxon;
		/** @brief Terminfo `prtr_silent`
		 *
		 * printer will not echo on screen
		 *
		 * TI Name: `mc5i`
		 * TC Name: `5i`
		 */
		int	mc5i;
		/** @brief Terminfo `hard_cursor`
		 *
		 * cursor is hard to see
		 *
		 * TI Name: `chts`
		 * TC Name: `HC`
		 */
		int	chts;
		/** @brief Terminfo `non_rev_rmcup`
		 *
		 * smcup does not reverse rmcup
		 *
		 * TI Name: `nrrmc`
		 * TC Name: `NR`
		 */
		int	nrrmc;
		/** @brief Terminfo `no_pad_char`
		 *
		 * pad character does not exist
		 *
		 * TI Name: `npc`
		 * TC Name: `NP`
		 */
		int	npc;
		/** @brief Terminfo `non_dest_scroll_region`
		 *
		 * scrolling region is non-destructive
		 *
		 * TI Name: `ndscr`
		 * TC Name: `ND`
		 */
		int	ndscr;
		/** @brief Terminfo `can_change`
		 *
		 * terminal can re-define existing colors
		 *
		 * TI Name: `ccc`
		 * TC Name: `cc`
		 */
		int	ccc;
		/** @brief Terminfo `back_color_erase`
		 *
		 * screen erased with background color
		 *
		 * TI Name: `bce`
		 * TC Name: `ut`
		 */
		int	bce;
		/** @brief Terminfo `hue_lightness_saturation`
		 *
		 * terminal uses only HLS color notation (Tektronix)
		 *
		 * TI Name: `hls`
		 * TC Name: `hl`
		 */
		int	hls;
		/** @brief Terminfo `col_addr_glitch`
		 *
		 * only positive motion for hpa/mhpa caps
		 *
		 * TI Name: `xhpa`
		 * TC Name: `YA`
		 */
		int	xhpa;
		/** @brief Terminfo `cr_cancels_micro_mode`
		 *
		 * using cr turns off micro mode
		 *
		 * TI Name: `crxm`
		 * TC Name: `YB`
		 */
		int	crxm;
		/** @brief Terminfo `has_print_wheel`
		 *
		 * printer needs operator to change character set
		 *
		 * TI Name: `daisy`
		 * TC Name: `YC`
		 */
		int	daisy;
		/** @brief Terminfo `row_addr_glitch`
		 *
		 * only positive motion for vpa/mvpa caps
		 *
		 * TI Name: `xvpa`
		 * TC Name: `YD`
		 */
		int	xvpa;
		/** @brief Terminfo `semi_auto_right_margin`
		 *
		 * printing in last column causes cr
		 *
		 * TI Name: `sam`
		 * TC Name: `YE`
		 */
		int	sam;
		/** @brief Terminfo `cpi_changes_res`
		 *
		 * changing character pitch changes resolution
		 *
		 * TI Name: `cpix`
		 * TC Name: `YF`
		 */
		int	cpix;
		/** @brief Terminfo `lpi_changes_res`
		 *
		 * changing line pitch changes resolution
		 *
		 * TI Name: `lpix`
		 * TC Name: `YG`
		 */
		int	lpix;
		int	truecolor;
	};
}	t_flagcap;

/* Struct containing all string fields of the db,
 * they should mostly be escape sequences.
 * Field should be NULL if unsupported.*/

typedef union e_strcap
{
	char	*array[394];
	struct
	{
		/** @brief Terminfo `back_tab`
		 *
		 * back tab (P)
		 *
		 * TI Name: `cbt`
		 * TC Name: `bt`
		 */
		char	*cbt;
		/** @brief Terminfo `bell`
		 *
		 * audible signal (bell) (P)
		 *
		 * TI Name: `bel`
		 * TC Name: `bl`
		 */
		char	*bel;
		/** @brief Terminfo `carriage_return`
		 *
		 * carriage return (P*) (P*)
		 *
		 * TI Name: `cr`
		 * TC Name: `cr`
		 */
		char	*cr;
		/** @brief Terminfo `change_scroll_region`
		 *
		 * change region to line #1 to line #2 (P)
		 *
		 * TI Name: `csr`
		 * TC Name: `cs`
		 */
		char	*csr;
		/** @brief Terminfo `clear_all_tabs`
		 *
		 * clear all tab stops (P)
		 *
		 * TI Name: `tbc`
		 * TC Name: `ct`
		 */
		char	*tbc;
		/** @brief Terminfo `clear_screen`
		 *
		 * clear screen and home cursor (P*)
		 *
		 * TI Name: `clear`
		 * TC Name: `cl`
		 */
		char	*clear;
		/** @brief Terminfo `clr_eol`
		 *
		 * clear to end of line (P)
		 *
		 * TI Name: `el`
		 * TC Name: `ce`
		 */
		char	*el;
		/** @brief Terminfo `clr_eos`
		 *
		 * clear to end of screen (P*)
		 *
		 * TI Name: `ed`
		 * TC Name: `cd`
		 */
		char	*ed;
		/** @brief Terminfo `column_address`
		 *
		 * horizontal position #1, absolute (P)
		 *
		 * TI Name: `hpa`
		 * TC Name: `ch`
		 */
		char	*hpa;
		/** @brief Terminfo `command_character`
		 *
		 * terminal settable cmd character in prototype !?
		 *
		 * TI Name: `cmdch`
		 * TC Name: `CC`
		 */
		char	*cmdch;
		/** @brief Terminfo `cursor_address`
		 *
		 * move to row #1 columns #2
		 *
		 * TI Name: `cup`
		 * TC Name: `cm`
		 */
		char	*cup;
		/** @brief Terminfo `cursor_down`
		 *
		 * down one line
		 *
		 * TI Name: `cud1`
		 * TC Name: `do`
		 */
		char	*cud1;
		/** @brief Terminfo `cursor_home`
		 *
		 * home cursor (if no cup)
		 *
		 * TI Name: `home`
		 * TC Name: `ho`
		 */
		char	*home;
		/** @brief Terminfo `cursor_invisible`
		 *
		 * make cursor invisible
		 *
		 * TI Name: `civis`
		 * TC Name: `vi`
		 */
		char	*civis;
		/** @brief Terminfo `cursor_left`
		 *
		 * move left one space
		 *
		 * TI Name: `cub1`
		 * TC Name: `le`
		 */
		char	*cub1;
		/** @brief Terminfo `cursor_mem_address`
		 *
		 * memory relative cursor addressing, move to row #1 columns #2
		 *
		 * TI Name: `mrcup`
		 * TC Name: `CM`
		 */
		char	*mrcup;
		/** @brief Terminfo `cursor_normal`
		 *
		 * make cursor appear normal (undo civis/cvvis)
		 *
		 * TI Name: `cnorm`
		 * TC Name: `ve`
		 */
		char	*cnorm;
		/** @brief Terminfo `cursor_right`
		 *
		 * non-destructive space (move right one space)
		 *
		 * TI Name: `cuf1`
		 * TC Name: `nd`
		 */
		char	*cuf1;
		/** @brief Terminfo `cursor_to_ll`
		 *
		 * last line, first column (if no cup)
		 *
		 * TI Name: `ll`
		 * TC Name: `ll`
		 */
		char	*ll;
		/** @brief Terminfo `cursor_up`
		 *
		 * up one line
		 *
		 * TI Name: `cuu1`
		 * TC Name: `up`
		 */
		char	*cuu1;
		/** @brief Terminfo `cursor_visible`
		 *
		 * make cursor very visible
		 *
		 * TI Name: `cvvis`
		 * TC Name: `vs`
		 */
		char	*cvvis;
		/** @brief Terminfo `delete_character`
		 *
		 * delete character (P*)
		 *
		 * TI Name: `dch1`
		 * TC Name: `dc`
		 */
		char	*dch1;
		/** @brief Terminfo `delete_line`
		 *
		 * delete line (P*)
		 *
		 * TI Name: `dl1`
		 * TC Name: `dl`
		 */
		char	*dl1;
		/** @brief Terminfo `dis_status_line`
		 *
		 * disable status line
		 *
		 * TI Name: `dsl`
		 * TC Name: `ds`
		 */
		char	*dsl;
		/** @brief Terminfo `down_half_line`
		 *
		 * half a line down
		 *
		 * TI Name: `hd`
		 * TC Name: `hd`
		 */
		char	*hd;
		/** @brief Terminfo `enter_alt_charset_mode`
		 *
		 * start alternate character set (P)
		 *
		 * TI Name: `smacs`
		 * TC Name: `as`
		 */
		char	*smacs;
		/** @brief Terminfo `enter_blink_mode`
		 *
		 * turn on blinking
		 *
		 * TI Name: `blink`
		 * TC Name: `mb`
		 */
		char	*blink;
		/** @brief Terminfo `enter_bold_mode`
		 *
		 * turn on bold (extra bright) mode
		 *
		 * TI Name: `bold`
		 * TC Name: `md`
		 */
		char	*bold;
		/** @brief Terminfo `enter_ca_mode`
		 *
		 * string to start programs using cup
		 *
		 * TI Name: `smcup`
		 * TC Name: `ti`
		 */
		char	*smcup;
		/** @brief Terminfo `enter_delete_mode`
		 *
		 * enter delete mode
		 *
		 * TI Name: `smdc`
		 * TC Name: `dm`
		 */
		char	*smdc;
		/** @brief Terminfo `enter_dim_mode`
		 *
		 * turn on half-bright mode
		 *
		 * TI Name: `dim`
		 * TC Name: `mh`
		 */
		char	*dim;
		/** @brief Terminfo `enter_insert_mode`
		 *
		 * enter insert mode
		 *
		 * TI Name: `smir`
		 * TC Name: `im`
		 */
		char	*smir;
		/** @brief Terminfo `enter_secure_mode`
		 *
		 * turn on blank mode (characters invisible)
		 *
		 * TI Name: `invis`
		 * TC Name: `mk`
		 */
		char	*invis;
		/** @brief Terminfo `enter_protected_mode`
		 *
		 * turn on protected mode
		 *
		 * TI Name: `prot`
		 * TC Name: `mp`
		 */
		char	*prot;
		/** @brief Terminfo `enter_reverse_mode`
		 *
		 * turn on reverse video mode
		 *
		 * TI Name: `rev`
		 * TC Name: `mr`
		 */
		char	*rev;
		/** @brief Terminfo `enter_standout_mode`
		 *
		 * begin standout mode
		 *
		 * TI Name: `smso`
		 * TC Name: `so`
		 */
		char	*smso;
		/** @brief Terminfo `enter_underline_mode`
		 *
		 * begin underline mode
		 *
		 * TI Name: `smul`
		 * TC Name: `us`
		 */
		char	*smul;
		/** @brief Terminfo `erase_chars`
		 *
		 * erase #1 characters (P)
		 *
		 * TI Name: `ech`
		 * TC Name: `ec`
		 */
		char	*ech;
		/** @brief Terminfo `exit_alt_charset_mode`
		 *
		 * end alternate character set (P)
		 *
		 * TI Name: `rmacs`
		 * TC Name: `ae`
		 */
		char	*rmacs;
		/** @brief Terminfo `exit_attribute_mode`
		 *
		 * turn off all attributes
		 *
		 * TI Name: `sgr0`
		 * TC Name: `me`
		 */
		char	*sgr0;
		/** @brief Terminfo `exit_ca_mode`
		 *
		 * strings to end programs using cup
		 *
		 * TI Name: `rmcup`
		 * TC Name: `te`
		 */
		char	*rmcup;
		/** @brief Terminfo `exit_delete_mode`
		 *
		 * end delete mode
		 *
		 * TI Name: `rmdc`
		 * TC Name: `ed`
		 */
		char	*rmdc;
		/** @brief Terminfo `exit_insert_mode`
		 *
		 * exit insert mode
		 *
		 * TI Name: `rmir`
		 * TC Name: `ei`
		 */
		char	*rmir;
		/** @brief Terminfo `exit_standout_mode`
		 *
		 * exit standout mode
		 *
		 * TI Name: `rmso`
		 * TC Name: `se`
		 */
		char	*rmso;
		/** @brief Terminfo `exit_underline_mode`
		 *
		 * exit underline mode
		 *
		 * TI Name: `rmul`
		 * TC Name: `ue`
		 */
		char	*rmul;
		/** @brief Terminfo `flash_screen`
		 *
		 * visible bell (may not move cursor)
		 *
		 * TI Name: `flash`
		 * TC Name: `vb`
		 */
		char	*flash;
		/** @brief Terminfo `form_feed`
		 *
		 * hardcopy terminal page eject (P*)
		 *
		 * TI Name: `ff`
		 * TC Name: `ff`
		 */
		char	*ff;
		/** @brief Terminfo `from_status_line`
		 *
		 * return from status line
		 *
		 * TI Name: `fsl`
		 * TC Name: `fs`
		 */
		char	*fsl;
		/** @brief Terminfo `init_1string`
		 *
		 * initialization string
		 *
		 * TI Name: `is1`
		 * TC Name: `i1`
		 */
		char	*is1;
		/** @brief Terminfo `init_2string`
		 *
		 * initialization string
		 *
		 * TI Name: `is2`
		 * TC Name: `is`
		 */
		char	*is2;
		/** @brief Terminfo `init_3string`
		 *
		 * initialization string
		 *
		 * TI Name: `is3`
		 * TC Name: `i3`
		 */
		char	*is3;
		/** @brief Terminfo `init_file`
		 *
		 * name of initialization file
		 *
		 * TI Name: `if`
		 * TC Name: `if`
		 */
		char	*_if; /* if */
		/** @brief Terminfo `insert_character`
		 *
		 * insert character (P)
		 *
		 * TI Name: `ich1`
		 * TC Name: `ic`
		 */
		char	*ich1;
		/** @brief Terminfo `insert_line`
		 *
		 * insert line (P*)
		 *
		 * TI Name: `il1`
		 * TC Name: `al`
		 */
		char	*il1;
		/** @brief Terminfo `insert_padding`
		 *
		 * insert padding after inserted character
		 *
		 * TI Name: `ip`
		 * TC Name: `ip`
		 */
		char	*ip;
		/** @brief Terminfo `key_backspace`
		 *
		 * backspace key
		 *
		 * TI Name: `kbs`
		 * TC Name: `kb`
		 */
		char	*kbs;
		/** @brief Terminfo `key_catab`
		 *
		 * clear-all-tabs key
		 *
		 * TI Name: `ktbc`
		 * TC Name: `ka`
		 */
		char	*ktbc;
		/** @brief Terminfo `key_clear`
		 *
		 * clear-screen or erase key
		 *
		 * TI Name: `kclr`
		 * TC Name: `kC`
		 */
		char	*kclr;
		/** @brief Terminfo `key_ctab`
		 *
		 * clear-tab key
		 *
		 * TI Name: `kctab`
		 * TC Name: `kt`
		 */
		char	*kctab;
		/** @brief Terminfo `key_dc`
		 *
		 * delete-character key
		 *
		 * TI Name: `kdch1`
		 * TC Name: `kD`
		 */
		char	*kdch1;
		/** @brief Terminfo `key_dl`
		 *
		 * delete-line key
		 *
		 * TI Name: `kdl1`
		 * TC Name: `kL`
		 */
		char	*kdl1;
		/** @brief Terminfo `key_down`
		 *
		 * down-arrow key
		 *
		 * TI Name: `kcud1`
		 * TC Name: `kd`
		 */
		char	*kcud1;
		/** @brief Terminfo `key_eic`
		 *
		 * sent by rmir or smir in insert mode
		 *
		 * TI Name: `krmir`
		 * TC Name: `kM`
		 */
		char	*krmir;
		/** @brief Terminfo `key_eol`
		 *
		 * clear-to-end-of-line key
		 *
		 * TI Name: `kel`
		 * TC Name: `kE`
		 */
		char	*kel;
		/** @brief Terminfo `key_eos`
		 *
		 * clear-to-end-of-screen key
		 *
		 * TI Name: `ked`
		 * TC Name: `kS`
		 */
		char	*ked;
		/** @brief Terminfo `key_f0`
		 *
		 * F0 function key
		 *
		 * TI Name: `kf0`
		 * TC Name: `k0`
		 */
		char	*kf0;
		/** @brief Terminfo `key_f1`
		 *
		 * F1 function key
		 *
		 * TI Name: `kf1`
		 * TC Name: `k1`
		 */
		char	*kf1;
		/** @brief Terminfo `key_f10`
		 *
		 * F10 function key
		 *
		 * TI Name: `kf10`
		 * TC Name: `k;`
		 */
		char	*kf10;
		/** @brief Terminfo `key_f2`
		 *
		 * F2 function key
		 *
		 * TI Name: `kf2`
		 * TC Name: `k2`
		 */
		char	*kf2;
		/** @brief Terminfo `key_f3`
		 *
		 * F3 function key
		 *
		 * TI Name: `kf3`
		 * TC Name: `k3`
		 */
		char	*kf3;
		/** @brief Terminfo `key_f4`
		 *
		 * F4 function key
		 *
		 * TI Name: `kf4`
		 * TC Name: `k4`
		 */
		char	*kf4;
		/** @brief Terminfo `key_f5`
		 *
		 * F5 function key
		 *
		 * TI Name: `kf5`
		 * TC Name: `k5`
		 */
		char	*kf5;
		/** @brief Terminfo `key_f6`
		 *
		 * F6 function key
		 *
		 * TI Name: `kf6`
		 * TC Name: `k6`
		 */
		char	*kf6;
		/** @brief Terminfo `key_f7`
		 *
		 * F7 function key
		 *
		 * TI Name: `kf7`
		 * TC Name: `k7`
		 */
		char	*kf7;
		/** @brief Terminfo `key_f8`
		 *
		 * F8 function key
		 *
		 * TI Name: `kf8`
		 * TC Name: `k8`
		 */
		char	*kf8;
		/** @brief Terminfo `key_f9`
		 *
		 * F9 function key
		 *
		 * TI Name: `kf9`
		 * TC Name: `k9`
		 */
		char	*kf9;
		/** @brief Terminfo `key_home`
		 *
		 * home key
		 *
		 * TI Name: `khome`
		 * TC Name: `kh`
		 */
		char	*khome;
		/** @brief Terminfo `key_ic`
		 *
		 * insert-character key
		 *
		 * TI Name: `kich1`
		 * TC Name: `kI`
		 */
		char	*kich1;
		/** @brief Terminfo `key_il`
		 *
		 * insert-line key
		 *
		 * TI Name: `kil1`
		 * TC Name: `kA`
		 */
		char	*kil1;
		/** @brief Terminfo `key_left`
		 *
		 * left-arrow key
		 *
		 * TI Name: `kcub1`
		 * TC Name: `kl`
		 */
		char	*kcub1;
		/** @brief Terminfo `key_ll`
		 *
		 * lower-left key (home down)
		 *
		 * TI Name: `kll`
		 * TC Name: `kH`
		 */
		char	*kll;
		/** @brief Terminfo `key_npage`
		 *
		 * next-page key
		 *
		 * TI Name: `knp`
		 * TC Name: `kN`
		 */
		char	*knp;
		/** @brief Terminfo `key_ppage`
		 *
		 * previous-page key
		 *
		 * TI Name: `kpp`
		 * TC Name: `kP`
		 */
		char	*kpp;
		/** @brief Terminfo `key_right`
		 *
		 * right-arrow key
		 *
		 * TI Name: `kcuf1`
		 * TC Name: `kr`
		 */
		char	*kcuf1;
		/** @brief Terminfo `key_sf`
		 *
		 * scroll-forward key
		 *
		 * TI Name: `kind`
		 * TC Name: `kF`
		 */
		char	*kind;
		/** @brief Terminfo `key_sr`
		 *
		 * scroll-backward key
		 *
		 * TI Name: `kri`
		 * TC Name: `kR`
		 */
		char	*kri;
		/** @brief Terminfo `key_stab`
		 *
		 * set-tab key
		 *
		 * TI Name: `khts`
		 * TC Name: `kT`
		 */
		char	*khts;
		/** @brief Terminfo `key_up`
		 *
		 * up-arrow key
		 *
		 * TI Name: `kcuu1`
		 * TC Name: `ku`
		 */
		char	*kcuu1;
		/** @brief Terminfo `keypad_local`
		 *
		 * leave keyboard transmit mode
		 *
		 * TI Name: `rmkx`
		 * TC Name: `ke`
		 */
		char	*rmkx;
		/** @brief Terminfo `keypad_xmit`
		 *
		 * enter keyboard transmit mode
		 *
		 * TI Name: `smkx`
		 * TC Name: `ks`
		 */
		char	*smkx;
		/** @brief Terminfo `lab_f0`
		 *
		 * label on function key f0 if not f0
		 *
		 * TI Name: `lf0`
		 * TC Name: `l0`
		 */
		char	*lf0;
		/** @brief Terminfo `lab_f1`
		 *
		 * label on function key f1 if not f1
		 *
		 * TI Name: `lf1`
		 * TC Name: `l1`
		 */
		char	*lf1;
		/** @brief Terminfo `lab_f10`
		 *
		 * label on function key f10 if not f10
		 *
		 * TI Name: `lf10`
		 * TC Name: `la`
		 */
		char	*lf10;
		/** @brief Terminfo `lab_f2`
		 *
		 * label on function key f2 if not f2
		 *
		 * TI Name: `lf2`
		 * TC Name: `l2`
		 */
		char	*lf2;
		/** @brief Terminfo `lab_f3`
		 *
		 * label on function key f3 if not f3
		 *
		 * TI Name: `lf3`
		 * TC Name: `l3`
		 */
		char	*lf3;
		/** @brief Terminfo `lab_f4`
		 *
		 * label on function key f4 if not f4
		 *
		 * TI Name: `lf4`
		 * TC Name: `l4`
		 */
		char	*lf4;
		/** @brief Terminfo `lab_f5`
		 *
		 * label on function key f5 if not f5
		 *
		 * TI Name: `lf5`
		 * TC Name: `l5`
		 */
		char	*lf5;
		/** @brief Terminfo `lab_f6`
		 *
		 * label on function key f6 if not f6
		 *
		 * TI Name: `lf6`
		 * TC Name: `l6`
		 */
		char	*lf6;
		/** @brief Terminfo `lab_f7`
		 *
		 * label on function key f7 if not f7
		 *
		 * TI Name: `lf7`
		 * TC Name: `l7`
		 */
		char	*lf7;
		/** @brief Terminfo `lab_f8`
		 *
		 * label on function key f8 if not f8
		 *
		 * TI Name: `lf8`
		 * TC Name: `l8`
		 */
		char	*lf8;
		/** @brief Terminfo `lab_f9`
		 *
		 * label on function key f9 if not f9
		 *
		 * TI Name: `lf9`
		 * TC Name: `l9`
		 */
		char	*lf9;
		/** @brief Terminfo `meta_off`
		 *
		 * turn off meta mode
		 *
		 * TI Name: `rmm`
		 * TC Name: `mo`
		 */
		char	*rmm;
		/** @brief Terminfo `meta_on`
		 *
		 * turn on meta mode (8th-bit on)
		 *
		 * TI Name: `smm`
		 * TC Name: `mm`
		 */
		char	*smm;
		/** @brief Terminfo `newline`
		 *
		 * newline (behave like cr followed by lf)
		 *
		 * TI Name: `nel`
		 * TC Name: `nw`
		 */
		char	*nel;
		/** @brief Terminfo `pad_char`
		 *
		 * padding char (instead of null)
		 *
		 * TI Name: `pad`
		 * TC Name: `pc`
		 */
		char	*pad;
		/** @brief Terminfo `parm_dch`
		 *
		 * delete #1 characters (P*)
		 *
		 * TI Name: `dch`
		 * TC Name: `DC`
		 */
		char	*dch;
		/** @brief Terminfo `parm_delete_line`
		 *
		 * delete #1 lines (P*)
		 *
		 * TI Name: `dl`
		 * TC Name: `DL`
		 */
		char	*dl;
		/** @brief Terminfo `parm_down_cursor`
		 *
		 * down #1 lines (P*)
		 *
		 * TI Name: `cud`
		 * TC Name: `DO`
		 */
		char	*cud;
		/** @brief Terminfo `parm_ich`
		 *
		 * insert #1 characters (P*)
		 *
		 * TI Name: `ich`
		 * TC Name: `IC`
		 */
		char	*ich;
		/** @brief Terminfo `parm_index`
		 *
		 * scroll forward #1 lines (P)
		 *
		 * TI Name: `indn`
		 * TC Name: `SF`
		 */
		char	*indn;
		/** @brief Terminfo `parm_insert_line`
		 *
		 * insert #1 lines (P*)
		 *
		 * TI Name: `il`
		 * TC Name: `AL`
		 */
		char	*il;
		/** @brief Terminfo `parm_left_cursor`
		 *
		 * move #1 characters to the left (P)
		 *
		 * TI Name: `cub`
		 * TC Name: `LE`
		 */
		char	*cub;
		/** @brief Terminfo `parm_right_cursor`
		 *
		 * move #1 characters to the right (P*)
		 *
		 * TI Name: `cuf`
		 * TC Name: `RI`
		 */
		char	*cuf;
		/** @brief Terminfo `parm_rindex`
		 *
		 * scroll back #1 lines (P)
		 *
		 * TI Name: `rin`
		 * TC Name: `SR`
		 */
		char	*rin;
		/** @brief Terminfo `parm_up_cursor`
		 *
		 * up #1 lines (P*)
		 *
		 * TI Name: `cuu`
		 * TC Name: `UP`
		 */
		char	*cuu;
		/** @brief Terminfo `pkey_key`
		 *
		 * program function key #1 to type string #2
		 *
		 * TI Name: `pfkey`
		 * TC Name: `pk`
		 */
		char	*pfkey;
		/** @brief Terminfo `pkey_local`
		 *
		 * program function key #1 to execute string #2
		 *
		 * TI Name: `pfloc`
		 * TC Name: `pl`
		 */
		char	*pfloc;
		/** @brief Terminfo `pkey_xmit`
		 *
		 * program function key #1 to transmit string #2
		 *
		 * TI Name: `pfx`
		 * TC Name: `px`
		 */
		char	*pfx;
		/** @brief Terminfo `print_screen`
		 *
		 * print contents of screen
		 *
		 * TI Name: `mc0`
		 * TC Name: `ps`
		 */
		char	*mc0;
		/** @brief Terminfo `prtr_off`
		 *
		 * turn off printer
		 *
		 * TI Name: `mc4`
		 * TC Name: `pf`
		 */
		char	*mc4;
		/** @brief Terminfo `prtr_on`
		 *
		 * turn on printer
		 *
		 * TI Name: `mc5`
		 * TC Name: `po`
		 */
		char	*mc5;
		/** @brief Terminfo `repeat_char`
		 *
		 * repeat char #1 #2 times (P*)
		 *
		 * TI Name: `rep`
		 * TC Name: `rp`
		 */
		char	*rep;
		/** @brief Terminfo `reset_1string`
		 *
		 * reset string
		 *
		 * TI Name: `rs1`
		 * TC Name: `r1`
		 */
		char	*rs1;
		/** @brief Terminfo `reset_2string`
		 *
		 * reset string
		 *
		 * TI Name: `rs2`
		 * TC Name: `r2`
		 */
		char	*rs2;
		/** @brief Terminfo `reset_3string`
		 *
		 * reset string
		 *
		 * TI Name: `rs3`
		 * TC Name: `r3`
		 */
		char	*rs3;
		/** @brief Terminfo `reset_file`
		 *
		 * name of reset file
		 *
		 * TI Name: `rf`
		 * TC Name: `rf`
		 */
		char	*rf;
		/** @brief Terminfo `restore_cursor`
		 *
		 * restore cursor to position of last save_cursor
		 *
		 * TI Name: `rc`
		 * TC Name: `rc`
		 */
		char	*rc;
		/** @brief Terminfo `row_address`
		 *
		 * vertical position #1 absolute (P)
		 *
		 * TI Name: `vpa`
		 * TC Name: `cv`
		 */
		char	*vpa;
		/** @brief Terminfo `save_cursor`
		 *
		 * save current cursor position (P)
		 *
		 * TI Name: `sc`
		 * TC Name: `sc`
		 */
		char	*sc;
		/** @brief Terminfo `scroll_forward`
		 *
		 * scroll text up (P)
		 *
		 * TI Name: `ind`
		 * TC Name: `sf`
		 */
		char	*ind;
		/** @brief Terminfo `scroll_reverse`
		 *
		 * scroll text down (P)
		 *
		 * TI Name: `ri`
		 * TC Name: `sr`
		 */
		char	*ri;
		/** @brief Terminfo `set_attributes`
		 *
		 * define video attributes #1-#9 (PG9)
		 *
		 * TI Name: `sgr`
		 * TC Name: `sa`
		 */
		char	*sgr;
		/** @brief Terminfo `set_tab`
		 *
		 * set a tab in every row, current columns
		 *
		 * TI Name: `hts`
		 * TC Name: `st`
		 */
		char	*hts;
		/** @brief Terminfo `set_window`
		 *
		 * current window is lines #1-#2 cols #3-#4
		 *
		 * TI Name: `wind`
		 * TC Name: `wi`
		 */
		char	*wind;
		/** @brief Terminfo `tab`
		 *
		 * tab to next 8-space hardware tab stop
		 *
		 * TI Name: `ht`
		 * TC Name: `ta`
		 */
		char	*ht;
		/** @brief Terminfo `to_status_line`
		 *
		 * move to status line, column #1
		 *
		 * TI Name: `tsl`
		 * TC Name: `ts`
		 */
		char	*tsl;
		/** @brief Terminfo `underline_char`
		 *
		 * underline char and move past it
		 *
		 * TI Name: `uc`
		 * TC Name: `uc`
		 */
		char	*uc;
		/** @brief Terminfo `up_half_line`
		 *
		 * half a line up
		 *
		 * TI Name: `hu`
		 * TC Name: `hu`
		 */
		char	*hu;
		/** @brief Terminfo `init_prog`
		 *
		 * path name of program for initialization
		 *
		 * TI Name: `iprog`
		 * TC Name: `iP`
		 */
		char	*iprog;
		/** @brief Terminfo `key_a1`
		 *
		 * upper left of keypad
		 *
		 * TI Name: `ka1`
		 * TC Name: `K1`
		 */
		char	*ka1;
		/** @brief Terminfo `key_a3`
		 *
		 * upper right of keypad
		 *
		 * TI Name: `ka3`
		 * TC Name: `K3`
		 */
		char	*ka3;
		/** @brief Terminfo `key_b2`
		 *
		 * center of keypad
		 *
		 * TI Name: `kb2`
		 * TC Name: `K2`
		 */
		char	*kb2;
		/** @brief Terminfo `key_c1`
		 *
		 * lower left of keypad
		 *
		 * TI Name: `kc1`
		 * TC Name: `K4`
		 */
		char	*kc1;
		/** @brief Terminfo `key_c3`
		 *
		 * lower right of keypad
		 *
		 * TI Name: `kc3`
		 * TC Name: `K5`
		 */
		char	*kc3;
		/** @brief Terminfo `prtr_non`
		 *
		 * turn on printer for #1 bytes
		 *
		 * TI Name: `mc5p`
		 * TC Name: `pO`
		 */
		char	*mc5p;
		/** @brief Terminfo `char_padding`
		 *
		 * like ip but when in insert mode
		 *
		 * TI Name: `rmp`
		 * TC Name: `rP`
		 */
		char	*rmp;
		/** @brief Terminfo `acs_chars`
		 *
		 * graphics charset pairs, based on vt100
		 *
		 * TI Name: `acsc`
		 * TC Name: `ac`
		 */
		char	*acsc;
		/** @brief Terminfo `plab_norm`
		 *
		 * program label #1 to show string #2
		 *
		 * TI Name: `pln`
		 * TC Name: `pn`
		 */
		char	*pln;
		/** @brief Terminfo `key_btab`
		 *
		 * back-tab key
		 *
		 * TI Name: `kcbt`
		 * TC Name: `kB`
		 */
		char	*kcbt;
		/** @brief Terminfo `enter_xon_mode`
		 *
		 * turn on xon/xoff handshaking
		 *
		 * TI Name: `smxon`
		 * TC Name: `SX`
		 */
		char	*smxon;
		/** @brief Terminfo `exit_xon_mode`
		 *
		 * turn off xon/xoff handshaking
		 *
		 * TI Name: `rmxon`
		 * TC Name: `RX`
		 */
		char	*rmxon;
		/** @brief Terminfo `enter_am_mode`
		 *
		 * turn on automatic margins
		 *
		 * TI Name: `smam`
		 * TC Name: `SA`
		 */
		char	*smam;
		/** @brief Terminfo `exit_am_mode`
		 *
		 * turn off automatic margins
		 *
		 * TI Name: `rmam`
		 * TC Name: `RA`
		 */
		char	*rmam;
		/** @brief Terminfo `xon_character`
		 *
		 * XON character
		 *
		 * TI Name: `xonc`
		 * TC Name: `XN`
		 */
		char	*xonc;
		/** @brief Terminfo `xoff_character`
		 *
		 * XOFF character
		 *
		 * TI Name: `xoffc`
		 * TC Name: `XF`
		 */
		char	*xoffc;
		/** @brief Terminfo `ena_acs`
		 *
		 * enable alternate char set
		 *
		 * TI Name: `enacs`
		 * TC Name: `eA`
		 */
		char	*enacs;
		/** @brief Terminfo `label_on`
		 *
		 * turn on soft labels
		 *
		 * TI Name: `smln`
		 * TC Name: `LO`
		 */
		char	*smln;
		/** @brief Terminfo `label_off`
		 *
		 * turn off soft labels
		 *
		 * TI Name: `rmln`
		 * TC Name: `LF`
		 */
		char	*rmln;
		/** @brief Terminfo `key_beg`
		 *
		 * begin key
		 *
		 * TI Name: `kbeg`
		 * TC Name: `@1`
		 */
		char	*kbeg;
		/** @brief Terminfo `key_cancel`
		 *
		 * cancel key
		 *
		 * TI Name: `kcan`
		 * TC Name: `@2`
		 */
		char	*kcan;
		/** @brief Terminfo `key_close`
		 *
		 * close key
		 *
		 * TI Name: `kclo`
		 * TC Name: `@3`
		 */
		char	*kclo;
		/** @brief Terminfo `key_command`
		 *
		 * command key
		 *
		 * TI Name: `kcmd`
		 * TC Name: `@4`
		 */
		char	*kcmd;
		/** @brief Terminfo `key_copy`
		 *
		 * copy key
		 *
		 * TI Name: `kcpy`
		 * TC Name: `@5`
		 */
		char	*kcpy;
		/** @brief Terminfo `key_create`
		 *
		 * create key
		 *
		 * TI Name: `kcrt`
		 * TC Name: `@6`
		 */
		char	*kcrt;
		/** @brief Terminfo `key_end`
		 *
		 * end key
		 *
		 * TI Name: `kend`
		 * TC Name: `@7`
		 */
		char	*kend;
		/** @brief Terminfo `key_enter`
		 *
		 * enter/send key
		 *
		 * TI Name: `kent`
		 * TC Name: `@8`
		 */
		char	*kent;
		/** @brief Terminfo `key_exit`
		 *
		 * exit key
		 *
		 * TI Name: `kext`
		 * TC Name: `@9`
		 */
		char	*kext;
		/** @brief Terminfo `key_find`
		 *
		 * find key
		 *
		 * TI Name: `kfnd`
		 * TC Name: `@0`
		 */
		char	*kfnd;
		/** @brief Terminfo `key_help`
		 *
		 * help key
		 *
		 * TI Name: `khlp`
		 * TC Name: `%1`
		 */
		char	*khlp;
		/** @brief Terminfo `key_mark`
		 *
		 * mark key
		 *
		 * TI Name: `kmrk`
		 * TC Name: `%2`
		 */
		char	*kmrk;
		/** @brief Terminfo `key_message`
		 *
		 * message key
		 *
		 * TI Name: `kmsg`
		 * TC Name: `%3`
		 */
		char	*kmsg;
		/** @brief Terminfo `key_move`
		 *
		 * move key
		 *
		 * TI Name: `kmov`
		 * TC Name: `%4`
		 */
		char	*kmov;
		/** @brief Terminfo `key_next`
		 *
		 * next key
		 *
		 * TI Name: `knxt`
		 * TC Name: `%5`
		 */
		char	*knxt;
		/** @brief Terminfo `key_open`
		 *
		 * open key
		 *
		 * TI Name: `kopn`
		 * TC Name: `%6`
		 */
		char	*kopn;
		/** @brief Terminfo `key_options`
		 *
		 * options key
		 *
		 * TI Name: `kopt`
		 * TC Name: `%7`
		 */
		char	*kopt;
		/** @brief Terminfo `key_previous`
		 *
		 * previous key
		 *
		 * TI Name: `kprv`
		 * TC Name: `%8`
		 */
		char	*kprv;
		/** @brief Terminfo `key_print`
		 *
		 * print key
		 *
		 * TI Name: `kprt`
		 * TC Name: `%9`
		 */
		char	*kprt;
		/** @brief Terminfo `key_redo`
		 *
		 * redo key
		 *
		 * TI Name: `krdo`
		 * TC Name: `%0`
		 */
		char	*krdo;
		/** @brief Terminfo `key_reference`
		 *
		 * reference key
		 *
		 * TI Name: `kref`
		 * TC Name: `&1`
		 */
		char	*kref;
		/** @brief Terminfo `key_refresh`
		 *
		 * refresh key
		 *
		 * TI Name: `krfr`
		 * TC Name: `&2`
		 */
		char	*krfr;
		/** @brief Terminfo `key_replace`
		 *
		 * replace key
		 *
		 * TI Name: `krpl`
		 * TC Name: `&3`
		 */
		char	*krpl;
		/** @brief Terminfo `key_restart`
		 *
		 * restart key
		 *
		 * TI Name: `krst`
		 * TC Name: `&4`
		 */
		char	*krst;
		/** @brief Terminfo `key_resume`
		 *
		 * resume key
		 *
		 * TI Name: `kres`
		 * TC Name: `&5`
		 */
		char	*kres;
		/** @brief Terminfo `key_save`
		 *
		 * save key
		 *
		 * TI Name: `ksav`
		 * TC Name: `&6`
		 */
		char	*ksav;
		/** @brief Terminfo `key_suspend`
		 *
		 * suspend key
		 *
		 * TI Name: `kspd`
		 * TC Name: `&7`
		 */
		char	*kspd;
		/** @brief Terminfo `key_undo`
		 *
		 * undo key
		 *
		 * TI Name: `kund`
		 * TC Name: `&8`
		 */
		char	*kund;
		/** @brief Terminfo `key_sbeg`
		 *
		 * shifted begin key
		 *
		 * TI Name: `kBEG`
		 * TC Name: `&9`
		 */
		char	*_kbeg; /* kBEG */
		/** @brief Terminfo `key_scancel`
		 *
		 * shifted cancel key
		 *
		 * TI Name: `kCAN`
		 * TC Name: `&0`
		 */
		char	*_kcan; /* kCAN */
		/** @brief Terminfo `key_scommand`
		 *
		 * shifted command key
		 *
		 * TI Name: `kCMD`
		 * TC Name: `*1`
		 */
		char	*_kcmd; /* kCMD */
		/** @brief Terminfo `key_scopy`
		 *
		 * shifted copy key
		 *
		 * TI Name: `kCPY`
		 * TC Name: `*2`
		 */
		char	*_kcpy; /* kCPY */
		/** @brief Terminfo `key_screate`
		 *
		 * shifted create key
		 *
		 * TI Name: `kCRT`
		 * TC Name: `*3`
		 */
		char	*_kcrt; /* kCRT */
		/** @brief Terminfo `key_sdc`
		 *
		 * shifted delete-character key
		 *
		 * TI Name: `kDC`
		 * TC Name: `*4`
		 */
		char	*kdc; /* kDC */
		/** @brief Terminfo `key_sdl`
		 *
		 * shifted delete-line key
		 *
		 * TI Name: `kDL`
		 * TC Name: `*5`
		 */
		char	*kdl; /* kDL */
		/** @brief Terminfo `key_select`
		 *
		 * select key
		 *
		 * TI Name: `kslt`
		 * TC Name: `*6`
		 */
		char	*kslt;
		/** @brief Terminfo `key_send`
		 *
		 * shifted end key
		 *
		 * TI Name: `kEND`
		 * TC Name: `*7`
		 */
		char	*_kend; /* kEND */
		/** @brief Terminfo `key_seol`
		 *
		 * shifted clear-to-end-of-line key
		 *
		 * TI Name: `kEOL`
		 * TC Name: `*8`
		 */
		char	*keol; /* kEOL */
		/** @brief Terminfo `key_sexit`
		 *
		 * shifted exit key
		 *
		 * TI Name: `kEXT`
		 * TC Name: `*9`
		 */
		char	*_kext; /* kEXT */
		/** @brief Terminfo `key_sfind`
		 *
		 * shifted find key
		 *
		 * TI Name: `kFND`
		 * TC Name: `*0`
		 */
		char	*_kfnd; /* kFND */
		/** @brief Terminfo `key_shelp`
		 *
		 * shifted help key
		 *
		 * TI Name: `kHLP`
		 * TC Name: `#1`
		 */
		char	*_khlp; /* kHLP */
		/** @brief Terminfo `key_shome`
		 *
		 * shifted home key
		 *
		 * TI Name: `kHOM`
		 * TC Name: `#2`
		 */
		char	*khom; /* kHOM */
		/** @brief Terminfo `key_sic`
		 *
		 * shifted insert-character key
		 *
		 * TI Name: `kIC`
		 * TC Name: `#3`
		 */
		char	*kic; /* kIC */
		/** @brief Terminfo `key_sleft`
		 *
		 * shifted left-arrow key
		 *
		 * TI Name: `kLFT`
		 * TC Name: `#4`
		 */
		char	*klft; /* kLFT */
		/** @brief Terminfo `key_smessage`
		 *
		 * shifted message key
		 *
		 * TI Name: `kMSG`
		 * TC Name: `%a`
		 */
		char	*_kmsg; /* kMSG */
		/** @brief Terminfo `key_smove`
		 *
		 * shifted move key
		 *
		 * TI Name: `kMOV`
		 * TC Name: `%b`
		 */
		char	*_kmov; /* kMOV */
		/** @brief Terminfo `key_snext`
		 *
		 * shifted next key
		 *
		 * TI Name: `kNXT`
		 * TC Name: `%c`
		 */
		char	*_knxt; /* kNXT */
		/** @brief Terminfo `key_soptions`
		 *
		 * shifted options key
		 *
		 * TI Name: `kOPT`
		 * TC Name: `%d`
		 */
		char	*_kopt; /* kOPT */
		/** @brief Terminfo `key_sprevious`
		 *
		 * shifted previous key
		 *
		 * TI Name: `kPRV`
		 * TC Name: `%e`
		 */
		char	*_kprv; /* kPRV */
		/** @brief Terminfo `key_sprint`
		 *
		 * shifted print key
		 *
		 * TI Name: `kPRT`
		 * TC Name: `%f`
		 */
		char	*_kprt; /* kPRT */
		/** @brief Terminfo `key_sredo`
		 *
		 * shifted redo key
		 *
		 * TI Name: `kRDO`
		 * TC Name: `%g`
		 */
		char	*_krdo; /* kRDO */
		/** @brief Terminfo `key_sreplace`
		 *
		 * shifted replace key
		 *
		 * TI Name: `kRPL`
		 * TC Name: `%h`
		 */
		char	*_krpl; /* kRPL */
		/** @brief Terminfo `key_sright`
		 *
		 * shifted right-arrow key
		 *
		 * TI Name: `kRIT`
		 * TC Name: `%i`
		 */
		char	*krit; /* kRIT */
		/** @brief Terminfo `key_srsume`
		 *
		 * shifted resume key
		 *
		 * TI Name: `kRES`
		 * TC Name: `%j`
		 */
		char	*_kres; /* kRES */
		/** @brief Terminfo `key_ssave`
		 *
		 * shifted save key
		 *
		 * TI Name: `kSAV`
		 * TC Name: `!1`
		 */
		char	*_ksav; /* kSAV */
		/** @brief Terminfo `key_ssuspend`
		 *
		 * shifted suspend key
		 *
		 * TI Name: `kSPD`
		 * TC Name: `!2`
		 */
		char	*_kspd; /* kSPD */
		/** @brief Terminfo `key_sundo`
		 *
		 * shifted undo key
		 *
		 * TI Name: `kUND`
		 * TC Name: `!3`
		 */
		char	*_kund; /* kUND */
		/** @brief Terminfo `req_for_input`
		 *
		 * send next input char (for ptys)
		 *
		 * TI Name: `rfi`
		 * TC Name: `RF`
		 */
		char	*rfi;
		/** @brief Terminfo `key_f11`
		 *
		 * F11 function key
		 *
		 * TI Name: `kf11`
		 * TC Name: `F1`
		 */
		char	*kf11;
		/** @brief Terminfo `key_f12`
		 *
		 * F12 function key
		 *
		 * TI Name: `kf12`
		 * TC Name: `F2`
		 */
		char	*kf12;
		/** @brief Terminfo `key_f13`
		 *
		 * F13 function key
		 *
		 * TI Name: `kf13`
		 * TC Name: `F3`
		 */
		char	*kf13;
		/** @brief Terminfo `key_f14`
		 *
		 * F14 function key
		 *
		 * TI Name: `kf14`
		 * TC Name: `F4`
		 */
		char	*kf14;
		/** @brief Terminfo `key_f15`
		 *
		 * F15 function key
		 *
		 * TI Name: `kf15`
		 * TC Name: `F5`
		 */
		char	*kf15;
		/** @brief Terminfo `key_f16`
		 *
		 * F16 function key
		 *
		 * TI Name: `kf16`
		 * TC Name: `F6`
		 */
		char	*kf16;
		/** @brief Terminfo `key_f17`
		 *
		 * F17 function key
		 *
		 * TI Name: `kf17`
		 * TC Name: `F7`
		 */
		char	*kf17;
		/** @brief Terminfo `key_f18`
		 *
		 * F18 function key
		 *
		 * TI Name: `kf18`
		 * TC Name: `F8`
		 */
		char	*kf18;
		/** @brief Terminfo `key_f19`
		 *
		 * F19 function key
		 *
		 * TI Name: `kf19`
		 * TC Name: `F9`
		 */
		char	*kf19;
		/** @brief Terminfo `key_f20`
		 *
		 * F20 function key
		 *
		 * TI Name: `kf20`
		 * TC Name: `FA`
		 */
		char	*kf20;
		/** @brief Terminfo `key_f21`
		 *
		 * F21 function key
		 *
		 * TI Name: `kf21`
		 * TC Name: `FB`
		 */
		char	*kf21;
		/** @brief Terminfo `key_f22`
		 *
		 * F22 function key
		 *
		 * TI Name: `kf22`
		 * TC Name: `FC`
		 */
		char	*kf22;
		/** @brief Terminfo `key_f23`
		 *
		 * F23 function key
		 *
		 * TI Name: `kf23`
		 * TC Name: `FD`
		 */
		char	*kf23;
		/** @brief Terminfo `key_f24`
		 *
		 * F24 function key
		 *
		 * TI Name: `kf24`
		 * TC Name: `FE`
		 */
		char	*kf24;
		/** @brief Terminfo `key_f25`
		 *
		 * F25 function key
		 *
		 * TI Name: `kf25`
		 * TC Name: `FF`
		 */
		char	*kf25;
		/** @brief Terminfo `key_f26`
		 *
		 * F26 function key
		 *
		 * TI Name: `kf26`
		 * TC Name: `FG`
		 */
		char	*kf26;
		/** @brief Terminfo `key_f27`
		 *
		 * F27 function key
		 *
		 * TI Name: `kf27`
		 * TC Name: `FH`
		 */
		char	*kf27;
		/** @brief Terminfo `key_f28`
		 *
		 * F28 function key
		 *
		 * TI Name: `kf28`
		 * TC Name: `FI`
		 */
		char	*kf28;
		/** @brief Terminfo `key_f29`
		 *
		 * F29 function key
		 *
		 * TI Name: `kf29`
		 * TC Name: `FJ`
		 */
		char	*kf29;
		/** @brief Terminfo `key_f30`
		 *
		 * F30 function key
		 *
		 * TI Name: `kf30`
		 * TC Name: `FK`
		 */
		char	*kf30;
		/** @brief Terminfo `key_f31`
		 *
		 * F31 function key
		 *
		 * TI Name: `kf31`
		 * TC Name: `FL`
		 */
		char	*kf31;
		/** @brief Terminfo `key_f32`
		 *
		 * F32 function key
		 *
		 * TI Name: `kf32`
		 * TC Name: `FM`
		 */
		char	*kf32;
		/** @brief Terminfo `key_f33`
		 *
		 * F33 function key
		 *
		 * TI Name: `kf33`
		 * TC Name: `FN`
		 */
		char	*kf33;
		/** @brief Terminfo `key_f34`
		 *
		 * F34 function key
		 *
		 * TI Name: `kf34`
		 * TC Name: `FO`
		 */
		char	*kf34;
		/** @brief Terminfo `key_f35`
		 *
		 * F35 function key
		 *
		 * TI Name: `kf35`
		 * TC Name: `FP`
		 */
		char	*kf35;
		/** @brief Terminfo `key_f36`
		 *
		 * F36 function key
		 *
		 * TI Name: `kf36`
		 * TC Name: `FQ`
		 */
		char	*kf36;
		/** @brief Terminfo `key_f37`
		 *
		 * F37 function key
		 *
		 * TI Name: `kf37`
		 * TC Name: `FR`
		 */
		char	*kf37;
		/** @brief Terminfo `key_f38`
		 *
		 * F38 function key
		 *
		 * TI Name: `kf38`
		 * TC Name: `FS`
		 */
		char	*kf38;
		/** @brief Terminfo `key_f39`
		 *
		 * F39 function key
		 *
		 * TI Name: `kf39`
		 * TC Name: `FT`
		 */
		char	*kf39;
		/** @brief Terminfo `key_f40`
		 *
		 * F40 function key
		 *
		 * TI Name: `kf40`
		 * TC Name: `FU`
		 */
		char	*kf40;
		/** @brief Terminfo `key_f41`
		 *
		 * F41 function key
		 *
		 * TI Name: `kf41`
		 * TC Name: `FV`
		 */
		char	*kf41;
		/** @brief Terminfo `key_f42`
		 *
		 * F42 function key
		 *
		 * TI Name: `kf42`
		 * TC Name: `FW`
		 */
		char	*kf42;
		/** @brief Terminfo `key_f43`
		 *
		 * F43 function key
		 *
		 * TI Name: `kf43`
		 * TC Name: `FX`
		 */
		char	*kf43;
		/** @brief Terminfo `key_f44`
		 *
		 * F44 function key
		 *
		 * TI Name: `kf44`
		 * TC Name: `FY`
		 */
		char	*kf44;
		/** @brief Terminfo `key_f45`
		 *
		 * F45 function key
		 *
		 * TI Name: `kf45`
		 * TC Name: `FZ`
		 */
		char	*kf45;
		/** @brief Terminfo `key_f46`
		 *
		 * F46 function key
		 *
		 * TI Name: `kf46`
		 * TC Name: `Fa`
		 */
		char	*kf46;
		/** @brief Terminfo `key_f47`
		 *
		 * F47 function key
		 *
		 * TI Name: `kf47`
		 * TC Name: `Fb`
		 */
		char	*kf47;
		/** @brief Terminfo `key_f48`
		 *
		 * F48 function key
		 *
		 * TI Name: `kf48`
		 * TC Name: `Fc`
		 */
		char	*kf48;
		/** @brief Terminfo `key_f49`
		 *
		 * F49 function key
		 *
		 * TI Name: `kf49`
		 * TC Name: `Fd`
		 */
		char	*kf49;
		/** @brief Terminfo `key_f50`
		 *
		 * F50 function key
		 *
		 * TI Name: `kf50`
		 * TC Name: `Fe`
		 */
		char	*kf50;
		/** @brief Terminfo `key_f51`
		 *
		 * F51 function key
		 *
		 * TI Name: `kf51`
		 * TC Name: `Ff`
		 */
		char	*kf51;
		/** @brief Terminfo `key_f52`
		 *
		 * F52 function key
		 *
		 * TI Name: `kf52`
		 * TC Name: `Fg`
		 */
		char	*kf52;
		/** @brief Terminfo `key_f53`
		 *
		 * F53 function key
		 *
		 * TI Name: `kf53`
		 * TC Name: `Fh`
		 */
		char	*kf53;
		/** @brief Terminfo `key_f54`
		 *
		 * F54 function key
		 *
		 * TI Name: `kf54`
		 * TC Name: `Fi`
		 */
		char	*kf54;
		/** @brief Terminfo `key_f55`
		 *
		 * F55 function key
		 *
		 * TI Name: `kf55`
		 * TC Name: `Fj`
		 */
		char	*kf55;
		/** @brief Terminfo `key_f56`
		 *
		 * F56 function key
		 *
		 * TI Name: `kf56`
		 * TC Name: `Fk`
		 */
		char	*kf56;
		/** @brief Terminfo `key_f57`
		 *
		 * F57 function key
		 *
		 * TI Name: `kf57`
		 * TC Name: `Fl`
		 */
		char	*kf57;
		/** @brief Terminfo `key_f58`
		 *
		 * F58 function key
		 *
		 * TI Name: `kf58`
		 * TC Name: `Fm`
		 */
		char	*kf58;
		/** @brief Terminfo `key_f59`
		 *
		 * F59 function key
		 *
		 * TI Name: `kf59`
		 * TC Name: `Fn`
		 */
		char	*kf59;
		/** @brief Terminfo `key_f60`
		 *
		 * F60 function key
		 *
		 * TI Name: `kf60`
		 * TC Name: `Fo`
		 */
		char	*kf60;
		/** @brief Terminfo `key_f61`
		 *
		 * F61 function key
		 *
		 * TI Name: `kf61`
		 * TC Name: `Fp`
		 */
		char	*kf61;
		/** @brief Terminfo `key_f62`
		 *
		 * F62 function key
		 *
		 * TI Name: `kf62`
		 * TC Name: `Fq`
		 */
		char	*kf62;
		/** @brief Terminfo `key_f63`
		 *
		 * F63 function key
		 *
		 * TI Name: `kf63`
		 * TC Name: `Fr`
		 */
		char	*kf63;
		/** @brief Terminfo `clr_bol`
		 *
		 * Clear to beginning of line
		 *
		 * TI Name: `el1`
		 * TC Name: `cb`
		 */
		char	*el1;
		/** @brief Terminfo `clear_margins`
		 *
		 * clear right and left soft margins
		 *
		 * TI Name: `mgc`
		 * TC Name: `MC`
		 */
		char	*mgc;
		/** @brief Terminfo `set_left_margin`
		 *
		 * set left soft margin at current column (not in BSD termcap)
		 *
		 * TI Name: `smgl`
		 * TC Name: `ML`
		 */
		char	*smgl;
		/** @brief Terminfo `set_right_margin`
		 *
		 * set right soft margin at current column
		 *
		 * TI Name: `smgr`
		 * TC Name: `MR`
		 */
		char	*smgr;
		/** @brief Terminfo `label_format`
		 *
		 * label format
		 *
		 * TI Name: `fln`
		 * TC Name: `Lf`
		 */
		char	*fln;
		/** @brief Terminfo `set_clock`
		 *
		 * set clock, #1 hrs #2 mins #3 secs
		 *
		 * TI Name: `sclk`
		 * TC Name: `SC`
		 */
		char	*sclk;
		/** @brief Terminfo `display_clock`
		 *
		 * display clock
		 *
		 * TI Name: `dclk`
		 * TC Name: `DK`
		 */
		char	*dclk;
		/** @brief Terminfo `remove_clock`
		 *
		 * remove clock
		 *
		 * TI Name: `rmclk`
		 * TC Name: `RC`
		 */
		char	*rmclk;
		/** @brief Terminfo `create_window`
		 *
		 * define a window #1 from #2,#3 to #4,#5
		 *
		 * TI Name: `cwin`
		 * TC Name: `CW`
		 */
		char	*cwin;
		/** @brief Terminfo `goto_window`
		 *
		 * go to window #1
		 *
		 * TI Name: `wingo`
		 * TC Name: `WG`
		 */
		char	*wingo;
		/** @brief Terminfo `hangup`
		 *
		 * hang-up phone
		 *
		 * TI Name: `hup`
		 * TC Name: `HU`
		 */
		char	*hup;
		/** @brief Terminfo `dial_phone`
		 *
		 * dial number #1
		 *
		 * TI Name: `dial`
		 * TC Name: `DI`
		 */
		char	*dial;
		/** @brief Terminfo `quick_dial`
		 *
		 * dial number #1 without checking
		 *
		 * TI Name: `qdial`
		 * TC Name: `QD`
		 */
		char	*qdial;
		/** @brief Terminfo `tone`
		 *
		 * select touch tone dialing
		 *
		 * TI Name: `tone`
		 * TC Name: `TO`
		 */
		char	*_tone; /* tone */
		/** @brief Terminfo `pulse`
		 *
		 * select pulse dialing
		 *
		 * TI Name: `pulse`
		 * TC Name: `PU`
		 */
		char	*_pulse; /* pulse */
		/** @brief Terminfo `flash_hook`
		 *
		 * flash switch hook
		 *
		 * TI Name: `hook`
		 * TC Name: `fh`
		 */
		char	*hook;
		/** @brief Terminfo `fixed_pause`
		 *
		 * pause for 2-3 seconds
		 *
		 * TI Name: `pause`
		 * TC Name: `PA`
		 */
		char	*pause;
		/** @brief Terminfo `wait_tone`
		 *
		 * wait for dial-tone
		 *
		 * TI Name: `wait`
		 * TC Name: `WA`
		 */
		char	*wait;
		/** @brief Terminfo `user0`
		 *
		 * User string #0
		 *
		 * TI Name: `u0`
		 * TC Name: `u0`
		 */
		char	*u0;
		/** @brief Terminfo `user1`
		 *
		 * User string #1
		 *
		 * TI Name: `u1`
		 * TC Name: `u1`
		 */
		char	*u1;
		/** @brief Terminfo `user2`
		 *
		 * User string #2
		 *
		 * TI Name: `u2`
		 * TC Name: `u2`
		 */
		char	*u2;
		/** @brief Terminfo `user3`
		 *
		 * User string #3
		 *
		 * TI Name: `u3`
		 * TC Name: `u3`
		 */
		char	*u3;
		/** @brief Terminfo `user4`
		 *
		 * User string #4
		 *
		 * TI Name: `u4`
		 * TC Name: `u4`
		 */
		char	*u4;
		/** @brief Terminfo `user5`
		 *
		 * User string #5
		 *
		 * TI Name: `u5`
		 * TC Name: `u5`
		 */
		char	*u5;
		/** @brief Terminfo `user6`
		 *
		 * User string #6
		 *
		 * TI Name: `u6`
		 * TC Name: `u6`
		 */
		char	*u6;
		/** @brief Terminfo `user7`
		 *
		 * User string #7
		 *
		 * TI Name: `u7`
		 * TC Name: `u7`
		 */
		char	*u7;
		/** @brief Terminfo `user8`
		 *
		 * User string #8
		 *
		 * TI Name: `u8`
		 * TC Name: `u8`
		 */
		char	*u8;
		/** @brief Terminfo `user9`
		 *
		 * User string #9
		 *
		 * TI Name: `u9`
		 * TC Name: `u9`
		 */
		char	*u9;
		/** @brief Terminfo `orig_pair`
		 *
		 * Set default pair to its original value
		 *
		 * TI Name: `op`
		 * TC Name: `op`
		 */
		char	*op;
		/** @brief Terminfo `orig_colors`
		 *
		 * Set all color pairs to the original ones
		 *
		 * TI Name: `oc`
		 * TC Name: `oc`
		 */
		char	*oc;
		/** @brief Terminfo `initialize_color`
		 *
		 * initialize color #1 to (#2,#3,#4)
		 *
		 * TI Name: `initc`
		 * TC Name: `Ic`
		 */
		char	*initc;
		/** @brief Terminfo `initialize_pair`
		 *
		 * Initialize color pair #1 to fg=(#2,#3,#4), bg=(#5,#6,#7)
		 *
		 * TI Name: `initp`
		 * TC Name: `Ip`
		 */
		char	*initp;
		/** @brief Terminfo `set_color_pair`
		 *
		 * Set current color pair to #1
		 *
		 * TI Name: `scp`
		 * TC Name: `sp`
		 */
		char	*scp;
		/** @brief Terminfo `set_foreground`
		 *
		 * Set foreground color #1
		 *
		 * TI Name: `setf`
		 * TC Name: `Sf`
		 */
		char	*setf;
		/** @brief Terminfo `set_background`
		 *
		 * Set background color #1
		 *
		 * TI Name: `setb`
		 * TC Name: `Sb`
		 */
		char	*setb;
		/** @brief Terminfo `change_char_pitch`
		 *
		 * Change number of characters per inch to #1
		 *
		 * TI Name: `cpi`
		 * TC Name: `ZA`
		 */
		char	*cpi;
		/** @brief Terminfo `change_line_pitch`
		 *
		 * Change number of lines per inch to #1
		 *
		 * TI Name: `lpi`
		 * TC Name: `ZB`
		 */
		char	*lpi;
		/** @brief Terminfo `change_res_horz`
		 *
		 * Change horizontal resolution to #1
		 *
		 * TI Name: `chr`
		 * TC Name: `ZC`
		 */
		char	*chr;
		/** @brief Terminfo `change_res_vert`
		 *
		 * Change vertical resolution to #1
		 *
		 * TI Name: `cvr`
		 * TC Name: `ZD`
		 */
		char	*cvr;
		/** @brief Terminfo `define_char`
		 *
		 * Define a character #1, #2 dots wide, descender #3
		 *
		 * TI Name: `defc`
		 * TC Name: `ZE`
		 */
		char	*defc;
		/** @brief Terminfo `enter_doublewide_mode`
		 *
		 * Enter double-wide mode
		 *
		 * TI Name: `swidm`
		 * TC Name: `ZF`
		 */
		char	*swidm;
		/** @brief Terminfo `enter_draft_quality`
		 *
		 * Enter draft-quality mode
		 *
		 * TI Name: `sdrfq`
		 * TC Name: `ZG`
		 */
		char	*sdrfq;
		/** @brief Terminfo `enter_italics_mode`
		 *
		 * Enter italic mode
		 *
		 * TI Name: `sitm`
		 * TC Name: `ZH`
		 */
		char	*sitm;
		/** @brief Terminfo `enter_leftward_mode`
		 *
		 * Start leftward carriage motion
		 *
		 * TI Name: `slm`
		 * TC Name: `ZI`
		 */
		char	*slm;
		/** @brief Terminfo `enter_micro_mode`
		 *
		 * Start micro-motion mode
		 *
		 * TI Name: `smicm`
		 * TC Name: `ZJ`
		 */
		char	*smicm;
		/** @brief Terminfo `enter_near_letter_quality`
		 *
		 * Enter NLQ mode
		 *
		 * TI Name: `snlq`
		 * TC Name: `ZK`
		 */
		char	*snlq;
		/** @brief Terminfo `enter_normal_quality`
		 *
		 * Enter normal-quality mode
		 *
		 * TI Name: `snrmq`
		 * TC Name: `ZL`
		 */
		char	*snrmq;
		/** @brief Terminfo `enter_shadow_mode`
		 *
		 * Enter shadow-print mode
		 *
		 * TI Name: `sshm`
		 * TC Name: `ZM`
		 */
		char	*sshm;
		/** @brief Terminfo `enter_subscript_mode`
		 *
		 * Enter subscript mode
		 *
		 * TI Name: `ssubm`
		 * TC Name: `ZN`
		 */
		char	*ssubm;
		/** @brief Terminfo `enter_superscript_mode`
		 *
		 * Enter superscript mode
		 *
		 * TI Name: `ssupm`
		 * TC Name: `ZO`
		 */
		char	*ssupm;
		/** @brief Terminfo `enter_upward_mode`
		 *
		 * Start upward carriage motion
		 *
		 * TI Name: `sum`
		 * TC Name: `ZP`
		 */
		char	*sum;
		/** @brief Terminfo `exit_doublewide_mode`
		 *
		 * End double-wide mode
		 *
		 * TI Name: `rwidm`
		 * TC Name: `ZQ`
		 */
		char	*rwidm;
		/** @brief Terminfo `exit_italics_mode`
		 *
		 * End italic mode
		 *
		 * TI Name: `ritm`
		 * TC Name: `ZR`
		 */
		char	*ritm;
		/** @brief Terminfo `exit_leftward_mode`
		 *
		 * End left-motion mode
		 *
		 * TI Name: `rlm`
		 * TC Name: `ZS`
		 */
		char	*rlm;
		/** @brief Terminfo `exit_micro_mode`
		 *
		 * End micro-motion mode
		 *
		 * TI Name: `rmicm`
		 * TC Name: `ZT`
		 */
		char	*rmicm;
		/** @brief Terminfo `exit_shadow_mode`
		 *
		 * End shadow-print mode
		 *
		 * TI Name: `rshm`
		 * TC Name: `ZU`
		 */
		char	*rshm;
		/** @brief Terminfo `exit_subscript_mode`
		 *
		 * End subscript mode
		 *
		 * TI Name: `rsubm`
		 * TC Name: `ZV`
		 */
		char	*rsubm;
		/** @brief Terminfo `exit_superscript_mode`
		 *
		 * End superscript mode
		 *
		 * TI Name: `rsupm`
		 * TC Name: `ZW`
		 */
		char	*rsupm;
		/** @brief Terminfo `exit_upward_mode`
		 *
		 * End reverse character motion
		 *
		 * TI Name: `rum`
		 * TC Name: `ZX`
		 */
		char	*rum;
		/** @brief Terminfo `micro_column_address`
		 *
		 * Like column_address in micro mode
		 *
		 * TI Name: `mhpa`
		 * TC Name: `ZY`
		 */
		char	*mhpa;
		/** @brief Terminfo `micro_down`
		 *
		 * Like cursor_down in micro mode
		 *
		 * TI Name: `mcud1`
		 * TC Name: `ZZ`
		 */
		char	*mcud1;
		/** @brief Terminfo `micro_left`
		 *
		 * Like cursor_left in micro mode
		 *
		 * TI Name: `mcub1`
		 * TC Name: `Za`
		 */
		char	*mcub1;
		/** @brief Terminfo `micro_right`
		 *
		 * Like cursor_right in micro mode
		 *
		 * TI Name: `mcuf1`
		 * TC Name: `Zb`
		 */
		char	*mcuf1;
		/** @brief Terminfo `micro_row_address`
		 *
		 * Like row_address #1 in micro mode
		 *
		 * TI Name: `mvpa`
		 * TC Name: `Zc`
		 */
		char	*mvpa;
		/** @brief Terminfo `micro_up`
		 *
		 * Like cursor_up in micro mode
		 *
		 * TI Name: `mcuu1`
		 * TC Name: `Zd`
		 */
		char	*mcuu1;
		/** @brief Terminfo `order_of_pins`
		 *
		 * Match software bits to print-head pins
		 *
		 * TI Name: `porder`
		 * TC Name: `Ze`
		 */
		char	*porder;
		/** @brief Terminfo `parm_down_micro`
		 *
		 * Like parm_down_cursor in micro mode
		 *
		 * TI Name: `mcud`
		 * TC Name: `Zf`
		 */
		char	*mcud;
		/** @brief Terminfo `parm_left_micro`
		 *
		 * Like parm_left_cursor in micro mode
		 *
		 * TI Name: `mcub`
		 * TC Name: `Zg`
		 */
		char	*mcub;
		/** @brief Terminfo `parm_right_micro`
		 *
		 * Like parm_right_cursor in micro mode
		 *
		 * TI Name: `mcuf`
		 * TC Name: `Zh`
		 */
		char	*mcuf;
		/** @brief Terminfo `parm_up_micro`
		 *
		 * Like parm_up_cursor in micro mode
		 *
		 * TI Name: `mcuu`
		 * TC Name: `Zi`
		 */
		char	*mcuu;
		/** @brief Terminfo `select_char_set`
		 *
		 * Select character set, #1
		 *
		 * TI Name: `scs`
		 * TC Name: `Zj`
		 */
		char	*scs;
		/** @brief Terminfo `set_bottom_margin`
		 *
		 * Set bottom margin at current line
		 *
		 * TI Name: `smgb`
		 * TC Name: `Zk`
		 */
		char	*smgb;
		/** @brief Terminfo `set_bottom_margin_parm`
		 *
		 * Set bottom margin at line #1 or (if smgtp is not given) #2 lines
		 * from bottom
		 *
		 * TI Name: `smgbp`
		 * TC Name: `Zl`
		 */
		char	*smgbp;
		/** @brief Terminfo `set_left_margin_parm`
		 *
		 * Set left (right) margin at column #1
		 *
		 * TI Name: `smglp`
		 * TC Name: `Zm`
		 */
		char	*smglp;
		/** @brief Terminfo `set_right_margin_parm`
		 *
		 * Set right margin at column #1
		 *
		 * TI Name: `smgrp`
		 * TC Name: `Zn`
		 */
		char	*smgrp;
		/** @brief Terminfo `set_top_margin`
		 *
		 * Set top margin at current line
		 *
		 * TI Name: `smgt`
		 * TC Name: `Zo`
		 */
		char	*smgt;
		/** @brief Terminfo `set_top_margin_parm`
		 *
		 * Set top (bottom) margin at row #1
		 *
		 * TI Name: `smgtp`
		 * TC Name: `Zp`
		 */
		char	*smgtp;
		/** @brief Terminfo `start_bit_image`
		 *
		 * Start printing bit image graphics
		 *
		 * TI Name: `sbim`
		 * TC Name: `Zq`
		 */
		char	*sbim;
		/** @brief Terminfo `start_char_set_def`
		 *
		 * Start character set definition #1, with #2 characters in the set
		 *
		 * TI Name: `scsd`
		 * TC Name: `Zr`
		 */
		char	*scsd;
		/** @brief Terminfo `stop_bit_image`
		 *
		 * Stop printing bit image graphics
		 *
		 * TI Name: `rbim`
		 * TC Name: `Zs`
		 */
		char	*rbim;
		/** @brief Terminfo `stop_char_set_def`
		 *
		 * End definition of character set #1
		 *
		 * TI Name: `rcsd`
		 * TC Name: `Zt`
		 */
		char	*rcsd;
		/** @brief Terminfo `subscript_characters`
		 *
		 * List of subscriptable characters
		 *
		 * TI Name: `subcs`
		 * TC Name: `Zu`
		 */
		char	*subcs;
		/** @brief Terminfo `superscript_characters`
		 *
		 * List of superscriptable characters
		 *
		 * TI Name: `supcs`
		 * TC Name: `Zv`
		 */
		char	*supcs;
		/** @brief Terminfo `these_cause_cr`
		 *
		 * Printing any of these characters causes CR
		 *
		 * TI Name: `docr`
		 * TC Name: `Zw`
		 */
		char	*docr;
		/** @brief Terminfo `zero_motion`
		 *
		 * No motion for subsequent character
		 *
		 * TI Name: `zerom`
		 * TC Name: `Zx`
		 */
		char	*zerom;
		/** @brief Terminfo `char_set_names`
		 *
		 * Produce #1'th item from list of character set names
		 *
		 * TI Name: `csnm`
		 * TC Name: `Zy`
		 */
		char	*csnm;
		/** @brief Terminfo `key_mouse`
		 *
		 * Mouse event has occurred
		 *
		 * TI Name: `kmous`
		 * TC Name: `Km`
		 */
		char	*kmous;
		/** @brief Terminfo `mouse_info`
		 *
		 * Mouse status information
		 *
		 * TI Name: `minfo`
		 * TC Name: `Mi`
		 */
		char	*minfo;
		/** @brief Terminfo `req_mouse_pos`
		 *
		 * Request mouse position
		 *
		 * TI Name: `reqmp`
		 * TC Name: `RQ`
		 */
		char	*reqmp;
		/** @brief Terminfo `get_mouse`
		 *
		 * Curses should get button events, parameter #1 not documented.
		 *
		 * TI Name: `getm`
		 * TC Name: `Gm`
		 */
		char	*getm;
		/** @brief Terminfo `set_a_foreground`
		 *
		 * Set foreground color to #1, using ANSI escape
		 *
		 * TI Name: `setaf`
		 * TC Name: `AF`
		 */
		char	*setaf;
		/** @brief Terminfo `set_a_background`
		 *
		 * Set background color to #1, using ANSI escape
		 *
		 * TI Name: `setab`
		 * TC Name: `AB`
		 */
		char	*setab;
		/** @brief Terminfo `pkey_plab`
		 *
		 * Program function key #1 to type string #2 and show string #3
		 *
		 * TI Name: `pfxl`
		 * TC Name: `xl`
		 */
		char	*pfxl;
		/** @brief Terminfo `device_type`
		 *
		 * Indicate language, codeset support
		 *
		 * TI Name: `devt`
		 * TC Name: `dv`
		 */
		char	*devt;
		/** @brief Terminfo `code_set_init`
		 *
		 * Init sequence for multiple codesets
		 *
		 * TI Name: `csin`
		 * TC Name: `ci`
		 */
		char	*csin;
		/** @brief Terminfo `set0_des_seq`
		 *
		 * Shift to codeset 0 (EUC set 0, ASCII)
		 *
		 * TI Name: `s0ds`
		 * TC Name: `s0`
		 */
		char	*s0ds;
		/** @brief Terminfo `set1_des_seq`
		 *
		 * Shift to codeset 1
		 *
		 * TI Name: `s1ds`
		 * TC Name: `s1`
		 */
		char	*s1ds;
		/** @brief Terminfo `set2_des_seq`
		 *
		 * Shift to codeset 2
		 *
		 * TI Name: `s2ds`
		 * TC Name: `s2`
		 */
		char	*s2ds;
		/** @brief Terminfo `set3_des_seq`
		 *
		 * Shift to codeset 3
		 *
		 * TI Name: `s3ds`
		 * TC Name: `s3`
		 */
		char	*s3ds;
		/** @brief Terminfo `set_lr_margin`
		 *
		 * Set both left and right margins to #1, #2.
		 * (ML is not in BSD termcap).
		 *
		 * TI Name: `smglr`
		 * TC Name: `ML`
		 */
		char	*smglr;
		/** @brief Terminfo `set_tb_margin`
		 *
		 * Sets both top and bottom margins to #1, #2
		 *
		 * TI Name: `smgtb`
		 * TC Name: `MT`
		 */
		char	*smgtb;
		/** @brief Terminfo `bit_image_repeat`
		 *
		 * Repeat bit image cell #1 #2 times
		 *
		 * TI Name: `birep`
		 * TC Name: `Xy`
		 */
		char	*birep;
		/** @brief Terminfo `bit_image_newline`
		 *
		 * Move to next row of the bit image
		 *
		 * TI Name: `binel`
		 * TC Name: `Zz`
		 */
		char	*binel;
		/** @brief Terminfo `bit_image_carriage_return`
		 *
		 * Move to beginning of same row
		 *
		 * TI Name: `bicr`
		 * TC Name: `Yv`
		 */
		char	*bicr;
		/** @brief Terminfo `color_names`
		 *
		 * Give name for color #1
		 *
		 * TI Name: `colornm`
		 * TC Name: `Yw`
		 */
		char	*colornm;
		/** @brief Terminfo `define_bit_image_region`
		 *
		 * Define rectangular bit image region
		 *
		 * TI Name: `defbi`
		 * TC Name: `Yx`
		 */
		char	*defbi;
		/** @brief Terminfo `end_bit_image_region`
		 *
		 * End a bit-image region
		 *
		 * TI Name: `endbi`
		 * TC Name: `Yy`
		 */
		char	*endbi;
		/** @brief Terminfo `set_color_band`
		 *
		 * Change to ribbon color #1
		 *
		 * TI Name: `setcolor`
		 * TC Name: `Yz`
		 */
		char	*setcolor;
		/** @brief Terminfo `set_page_length`
		 *
		 * Set page length to #1 lines
		 *
		 * TI Name: `slines`
		 * TC Name: `YZ`
		 */
		char	*slines;
		/** @brief Terminfo `display_pc_char`
		 *
		 * Display PC character #1
		 *
		 * TI Name: `dispc`
		 * TC Name: `S1`
		 */
		char	*dispc;
		/** @brief Terminfo `enter_pc_charset_mode`
		 *
		 * Enter PC character display mode
		 *
		 * TI Name: `smpch`
		 * TC Name: `S2`
		 */
		char	*smpch;
		/** @brief Terminfo `exit_pc_charset_mode`
		 *
		 * Exit PC character display mode
		 *
		 * TI Name: `rmpch`
		 * TC Name: `S3`
		 */
		char	*rmpch;
		/** @brief Terminfo `enter_scancode_mode`
		 *
		 * Enter PC scancode mode
		 *
		 * TI Name: `smsc`
		 * TC Name: `S4`
		 */
		char	*smsc;
		/** @brief Terminfo `exit_scancode_mode`
		 *
		 * Exit PC scancode mode
		 *
		 * TI Name: `rmsc`
		 * TC Name: `S5`
		 */
		char	*rmsc;
		/** @brief Terminfo `pc_term_options`
		 *
		 * PC terminal options
		 *
		 * TI Name: `pctrm`
		 * TC Name: `S6`
		 */
		char	*pctrm;
		/** @brief Terminfo `scancode_escape`
		 *
		 * Escape for scancode emulation
		 *
		 * TI Name: `scesc`
		 * TC Name: `S7`
		 */
		char	*scesc;
		/** @brief Terminfo `alt_scancode_esc`
		 *
		 * Alternate escape for scancode emulation
		 *
		 * TI Name: `scesa`
		 * TC Name: `S8`
		 */
		char	*scesa;
		/** @brief Terminfo `enter_horizontal_hl_mode`
		 *
		 * Enter horizontal highlight mode
		 *
		 * TI Name: `ehhlm`
		 * TC Name: `Xh`
		 */
		char	*ehhlm;
		/** @brief Terminfo `enter_left_hl_mode`
		 *
		 * Enter left highlight mode
		 *
		 * TI Name: `elhlm`
		 * TC Name: `Xl`
		 */
		char	*elhlm;
		/** @brief Terminfo `enter_low_hl_mode`
		 *
		 * Enter low highlight mode
		 *
		 * TI Name: `elohlm`
		 * TC Name: `Xo`
		 */
		char	*elohlm;
		/** @brief Terminfo `enter_right_hl_mode`
		 *
		 * Enter right highlight mode
		 *
		 * TI Name: `erhlm`
		 * TC Name: `Xr`
		 */
		char	*erhlm;
		/** @brief Terminfo `enter_top_hl_mode`
		 *
		 * Enter top highlight mode
		 *
		 * TI Name: `ethlm`
		 * TC Name: `Xt`
		 */
		char	*ethlm;
		/** @brief Terminfo `enter_vertical_hl_mode`
		 *
		 * Enter vertical highlight mode
		 *
		 * TI Name: `evhlm`
		 * TC Name: `Xv`
		 */
		char	*evhlm;
		/** @brief Terminfo `set_a_attributes`
		 *
		 * Define second set of video attributes #1-#6
		 *
		 * TI Name: `sgr1`
		 * TC Name: `sA`
		 */
		char	*sgr1;
		/** @brief Terminfo `set_pglen_inch`
		 *
		 * Set page length to #1 hundredth of an inch
		 * (some implementations use sL for termcap).
		 *
		 * TI Name: `slength`
		 * TC Name: `YI`
		 */
		char	*slength;
	};
}	t_strcap;

/* Struct containing all numeric fields of the db,
 * Field should be -1 if unsupported.*/

typedef union e_numcap
{
	int	array[33];
	struct
	{
		/** @brief Terminfo `columns`
		 *
		 * number of columns in a line
		 *
		 * TI Name: `cols`
		 * TC Name: `co`
		 */
		int	cols;
		/** @brief Terminfo `init_tabs`
		 *
		 * tabs initially every # spaces
		 *
		 * TI Name: `it`
		 * TC Name: `it`
		 */
		int	it;
		/** @brief Terminfo `lines`
		 *
		 * number of lines on screen or page
		 *
		 * TI Name: `lines`
		 * TC Name: `li`
		 */
		int	_lines; /* lines */
		/** @brief Terminfo `lines_of_memory`
		 *
		 * lines of memory if > line. 0 means varies
		 *
		 * TI Name: `lm`
		 * TC Name: `lm`
		 */
		int	lm;
		/** @brief Terminfo `magic_cookie_glitch`
		 *
		 * number of blank characters left by smso or rmso
		 *
		 * TI Name: `xmc`
		 * TC Name: `sg`
		 */
		int	xmc;
		/** @brief Terminfo `padding_baud_rate`
		 *
		 * lowest baud rate where padding needed
		 *
		 * TI Name: `pb`
		 * TC Name: `pb`
		 */
		int	pb;
		/** @brief Terminfo `virtual_terminal`
		 *
		 * virtual terminal number (CB/unix)
		 *
		 * TI Name: `vt`
		 * TC Name: `vt`
		 */
		int	vt;
		/** @brief Terminfo `width_status_line`
		 *
		 * number of columns in status line
		 *
		 * TI Name: `wsl`
		 * TC Name: `ws`
		 */
		int	wsl;
		/** @brief Terminfo `num_labels`
		 *
		 * number of labels on screen
		 *
		 * TI Name: `nlab`
		 * TC Name: `Nl`
		 */
		int	nlab;
		/** @brief Terminfo `label_height`
		 *
		 * rows in each label
		 *
		 * TI Name: `lh`
		 * TC Name: `lh`
		 */
		int	lh;
		/** @brief Terminfo `label_width`
		 *
		 * columns in each label
		 *
		 * TI Name: `lw`
		 * TC Name: `lw`
		 */
		int	lw;
		/** @brief Terminfo `max_attributes`
		 *
		 * maximum combined attributes terminal can handle
		 *
		 * TI Name: `ma`
		 * TC Name: `ma`
		 */
		int	ma;
		/** @brief Terminfo `maximum_windows`
		 *
		 * maximum number of definable windows
		 *
		 * TI Name: `wnum`
		 * TC Name: `MW`
		 */
		int	wnum;
		/** @brief Terminfo `max_colors`
		 *
		 * maximum number of colors on screen
		 *
		 * TI Name: `colors`
		 * TC Name: `Co`
		 */
		int	colors;
		/** @brief Terminfo `max_pairs`
		 *
		 * maximum number of color-pairs on the screen
		 *
		 * TI Name: `pairs`
		 * TC Name: `pa`
		 */
		int	pairs;
		/** @brief Terminfo `no_color_video`
		 *
		 * video attributes that cannot be used with colors
		 *
		 * TI Name: `ncv`
		 * TC Name: `NC`
		 */
		int	ncv;
		/** @brief Terminfo `buffer_capacity`
		 *
		 * numbers of bytes buffered before printing
		 *
		 * TI Name: `bufsz`
		 * TC Name: `Ya`
		 */
		int	bufsz;
		/** @brief Terminfo `dot_vert_spacing`
		 *
		 * spacing of pins vertically in pins per inch
		 *
		 * TI Name: `spinv`
		 * TC Name: `Yb`
		 */
		int	spinv;
		/** @brief Terminfo `dot_horz_spacing`
		 *
		 * spacing of dots horizontally in dots per inch
		 *
		 * TI Name: `spinh`
		 * TC Name: `Yc`
		 */
		int	spinh;
		/** @brief Terminfo `max_micro_address`
		 *
		 * maximum value in micro_..._address
		 *
		 * TI Name: `maddr`
		 * TC Name: `Yd`
		 */
		int	maddr;
		/** @brief Terminfo `max_micro_jump`
		 *
		 * maximum value in parm_..._micro
		 *
		 * TI Name: `mjump`
		 * TC Name: `Ye`
		 */
		int	mjump;
		/** @brief Terminfo `micro_col_size`
		 *
		 * character step size when in micro mode
		 *
		 * TI Name: `mcs`
		 * TC Name: `Yf`
		 */
		int	mcs;
		/** @brief Terminfo `micro_line_size`
		 *
		 * line step size when in micro mode
		 *
		 * TI Name: `mls`
		 * TC Name: `Yg`
		 */
		int	mls;
		/** @brief Terminfo `number_of_pins`
		 *
		 * numbers of pins in print-head
		 *
		 * TI Name: `npins`
		 * TC Name: `Yh`
		 */
		int	npins;
		/** @brief Terminfo `output_res_char`
		 *
		 * horizontal resolution in units per line
		 *
		 * TI Name: `orc`
		 * TC Name: `Yi`
		 */
		int	orc;
		/** @brief Terminfo `output_res_line`
		 *
		 * vertical resolution in units per line
		 *
		 * TI Name: `orl`
		 * TC Name: `Yj`
		 */
		int	orl;
		/** @brief Terminfo `output_res_horz_inch`
		 *
		 * horizontal resolution in units per inch
		 *
		 * TI Name: `orhi`
		 * TC Name: `Yk`
		 */
		int	orhi;
		/** @brief Terminfo `output_res_vert_inch`
		 *
		 * vertical resolution in units per inch
		 *
		 * TI Name: `orvi`
		 * TC Name: `Yl`
		 */
		int	orvi;
		/** @brief Terminfo `print_rate`
		 *
		 * print rate in characters per second
		 *
		 * TI Name: `cps`
		 * TC Name: `Ym`
		 */
		int	cps;
		/** @brief Terminfo `wide_char_size`
		 *
		 * character step size when in double wide mode
		 *
		 * TI Name: `widcs`
		 * TC Name: `Yn`
		 */
		int	widcs;
		/** @brief Terminfo `buttons`
		 *
		 * number of buttons on mouse
		 *
		 * TI Name: `btns`
		 * TC Name: `BT`
		 */
		int	btns;
		/** @brief Terminfo `bit_image_entwining`
		 *
		 * number of passes for each bit-image row
		 *
		 * TI Name: `bitwin`
		 * TC Name: `Yo`
		 */
		int	bitwin;
		/** @brief Terminfo `bit_image_type`
		 *
		 * type of bit-image device
		 *
		 * TI Name: `bitype`
		 * TC Name: `Yp`
		 */
		int	bitype;
	};
}	t_numcap;

/* Struct to hold every capabilities of
 * the current terminal running the shell,
 * it shouldn't be used by anything else than the
 * shell itself or its builtins.*/
typedef struct s_termcap
{
	int			success;
	t_flagcap	flags;
	t_strcap	strs;
	t_numcap	nums;
}	t_termcap;

/* In case of an unknown TERM will fallback to "dumb" termtype */
t_termcap
fetch_capability(char *term);

#endif
