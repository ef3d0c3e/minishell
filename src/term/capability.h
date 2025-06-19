/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capability.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:10:01 by thschnei          #+#    #+#             */
/*   Updated: 2025/05/12 17:57:05 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPABILITY_H
# define CAPABILITY_H
# include <term.h>
# include <curses.h>
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
		int	bw;
		int	am;
		int	xsb;
		int	xhp;
		int	xenl;
		int	eo;
		int	gn;
		int	hc;
		int	km;
		int	hs;
		int	in;
		int	da;
		int	db;
		int	mir;
		int	msgr;
		int	os;
		int	eslok;
		int	xt;
		int	hz;
		int	ul;
		int	xon;
		int	nxon;
		int	mc5i;
		int	chts;
		int	nrrmc;
		int	npc;
		int	ndscr;
		int	ccc;
		int	bce;
		int	hls;
		int	xhpa;
		int	crxm;
		int	daisy;
		int	xvpa;
		int	sam;
		int	cpix;
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
		char	*cbt;
		char	*bel;
		char	*cr;
		char	*csr;
		char	*tbc;
		char	*clear;
		char	*el;
		char	*ed;
		char	*hpa;
		char	*cmdch;
		char	*cup;
		char	*cud1;
		char	*home;
		char	*civis;
		char	*cub1;
		char	*mrcup;
		char	*cnorm;
		char	*cuf1;
		char	*ll;
		char	*cuu1;
		char	*cvvis;
		char	*dch1;
		char	*dl1;
		char	*dsl;
		char	*hd;
		char	*smacs;
		char	*blink;
		char	*bold;
		char	*smcup;
		char	*smdc;
		char	*dim;
		char	*smir;
		char	*invis;
		char	*prot;
		char	*rev;
		char	*smso;
		char	*smul;
		char	*ech;
		char	*rmacs;
		char	*sgr0;
		char	*rmcup;
		char	*rmdc;
		char	*rmir;
		char	*rmso;
		char	*rmul;
		char	*flash;
		char	*ff;
		char	*fsl;
		char	*is1;
		char	*is2;
		char	*is3;
		char	*_if; /* if */
		char	*ich1;
		char	*il1;
		char	*ip;
		char	*kbs;
		char	*ktbc;
		char	*kclr;
		char	*kctab;
		char	*kdch1;
		char	*kdl1;
		char	*kcud1;
		char	*krmir;
		char	*kel;
		char	*ked;
		char	*kf0;
		char	*kf1;
		char	*kf10;
		char	*kf2;
		char	*kf3;
		char	*kf4;
		char	*kf5;
		char	*kf6;
		char	*kf7;
		char	*kf8;
		char	*kf9;
		char	*khome;
		char	*kich1;
		char	*kil1;
		char	*kcub1;
		char	*kll;
		char	*knp;
		char	*kpp;
		char	*kcuf1;
		char	*kind;
		char	*kri;
		char	*khts;
		char	*kcuu1;
		char	*rmkx;
		char	*smkx;
		char	*lf0;
		char	*lf1;
		char	*lf10;
		char	*lf2;
		char	*lf3;
		char	*lf4;
		char	*lf5;
		char	*lf6;
		char	*lf7;
		char	*lf8;
		char	*lf9;
		char	*rmm;
		char	*smm;
		char	*nel;
		char	*pad;
		char	*dch;
		char	*dl;
		char	*cud;
		char	*ich;
		char	*indn;
		char	*il;
		char	*cub;
		char	*cuf;
		char	*rin;
		char	*cuu;
		char	*pfkey;
		char	*pfloc;
		char	*pfx;
		char	*mc0;
		char	*mc4;
		char	*mc5;
		char	*rep;
		char	*rs1;
		char	*rs2;
		char	*rs3;
		char	*rf;
		char	*rc;
		char	*vpa;
		char	*sc;
		char	*ind;
		char	*ri;
		char	*sgr;
		char	*hts;
		char	*wind;
		char	*ht;
		char	*tsl;
		char	*uc;
		char	*hu;
		char	*iprog;
		char	*ka1;
		char	*ka3;
		char	*kb2;
		char	*kc1;
		char	*kc3;
		char	*mc5p;
		char	*rmp;
		char	*acsc;
		char	*pln;
		char	*kcbt;
		char	*smxon;
		char	*rmxon;
		char	*smam;
		char	*rmam;
		char	*xonc;
		char	*xoffc;
		char	*enacs;
		char	*smln;
		char	*rmln;
		char	*kbeg;
		char	*kcan;
		char	*kclo;
		char	*kcmd;
		char	*kcpy;
		char	*kcrt;
		char	*kend;
		char	*kent;
		char	*kext;
		char	*kfnd;
		char	*khlp;
		char	*kmrk;
		char	*kmsg;
		char	*kmov;
		char	*knxt;
		char	*kopn;
		char	*kopt;
		char	*kprv;
		char	*kprt;
		char	*krdo;
		char	*kref;
		char	*krfr;
		char	*krpl;
		char	*krst;
		char	*kres;
		char	*ksav;
		char	*kspd;
		char	*kund;
		char	*_kbeg; /* kBEG */
		char	*_kcan; /* kCAN */
		char	*_kcmd; /* kCMD */
		char	*_kcpy; /* kCPY */
		char	*_kcrt; /* kCRT */
		char	*kdc; /* kDC */
		char	*kdl; /* kDL */
		char	*kslt;
		char	*_kend; /* kEND */
		char	*keol; /* kEOL */
		char	*_kext; /* kEXT */
		char	*_kfnd; /* kFND */
		char	*_khlp; /* kHLP */
		char	*khom; /* kHOM */
		char	*kic; /* kIC */
		char	*klft; /* kLFT */
		char	*_kmsg; /* kMSG */
		char	*_kmov; /* kMOV */
		char	*_knxt; /* kNXT */
		char	*_kopt; /* kOPT */
		char	*_kprv; /* kPRV */
		char	*_kprt; /* kPRT */
		char	*_krdo; /* kRDO */
		char	*_krpl; /* kRPL */
		char	*krit; /* kRIT */
		char	*_kres; /* kRES */
		char	*_ksav; /* kSAV */
		char	*_kspd; /* kSPD */
		char	*_kund; /* kUND */
		char	*rfi;
		char	*kf11;
		char	*kf12;
		char	*kf13;
		char	*kf14;
		char	*kf15;
		char	*kf16;
		char	*kf17;
		char	*kf18;
		char	*kf19;
		char	*kf20;
		char	*kf21;
		char	*kf22;
		char	*kf23;
		char	*kf24;
		char	*kf25;
		char	*kf26;
		char	*kf27;
		char	*kf28;
		char	*kf29;
		char	*kf30;
		char	*kf31;
		char	*kf32;
		char	*kf33;
		char	*kf34;
		char	*kf35;
		char	*kf36;
		char	*kf37;
		char	*kf38;
		char	*kf39;
		char	*kf40;
		char	*kf41;
		char	*kf42;
		char	*kf43;
		char	*kf44;
		char	*kf45;
		char	*kf46;
		char	*kf47;
		char	*kf48;
		char	*kf49;
		char	*kf50;
		char	*kf51;
		char	*kf52;
		char	*kf53;
		char	*kf54;
		char	*kf55;
		char	*kf56;
		char	*kf57;
		char	*kf58;
		char	*kf59;
		char	*kf60;
		char	*kf61;
		char	*kf62;
		char	*kf63;
		char	*el1;
		char	*mgc;
		char	*smgl;
		char	*smgr;
		char	*fln;
		char	*sclk;
		char	*dclk;
		char	*rmclk;
		char	*cwin;
		char	*wingo;
		char	*hup;
		char	*dial;
		char	*qdial;
		char	*_tone; /* tone */
		char	*_pulse; /* pulse */
		char	*hook;
		char	*pause;
		char	*wait;
		char	*u0;
		char	*u1;
		char	*u2;
		char	*u3;
		char	*u4;
		char	*u5;
		char	*u6;
		char	*u7;
		char	*u8;
		char	*u9;
		char	*op;
		char	*oc;
		char	*initc;
		char	*initp;
		char	*scp;
		char	*setf;
		char	*setb;
		char	*cpi;
		char	*lpi;
		char	*chr;
		char	*cvr;
		char	*defc;
		char	*swidm;
		char	*sdrfq;
		char	*sitm;
		char	*slm;
		char	*smicm;
		char	*snlq;
		char	*snrmq;
		char	*sshm;
		char	*ssubm;
		char	*ssupm;
		char	*sum;
		char	*rwidm;
		char	*ritm;
		char	*rlm;
		char	*rmicm;
		char	*rshm;
		char	*rsubm;
		char	*rsupm;
		char	*rum;
		char	*mhpa;
		char	*mcud1;
		char	*mcub1;
		char	*mcuf1;
		char	*mvpa;
		char	*mcuu1;
		char	*porder;
		char	*mcud;
		char	*mcub;
		char	*mcuf;
		char	*mcuu;
		char	*scs;
		char	*smgb;
		char	*smgbp;
		char	*smglp;
		char	*smgrp;
		char	*smgt;
		char	*smgtp;
		char	*sbim;
		char	*scsd;
		char	*rbim;
		char	*rcsd;
		char	*subcs;
		char	*supcs;
		char	*docr;
		char	*zerom;
		char	*csnm;
		char	*kmous;
		char	*minfo;
		char	*reqmp;
		char	*getm;
		char	*setaf;
		char	*setab;
		char	*pfxl;
		char	*devt;
		char	*csin;
		char	*s0ds;
		char	*s1ds;
		char	*s2ds;
		char	*s3ds;
		char	*smglr;
		char	*smgtb;
		char	*birep;
		char	*binel;
		char	*bicr;
		char	*colornm;
		char	*defbi;
		char	*endbi;
		char	*setcolor;
		char	*slines;
		char	*dispc;
		char	*smpch;
		char	*rmpch;
		char	*smsc;
		char	*rmsc;
		char	*pctrm;
		char	*scesc;
		char	*scesa;
		char	*ehhlm;
		char	*elhlm;
		char	*elohlm;
		char	*erhlm;
		char	*ethlm;
		char	*evhlm;
		char	*sgr1;
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
		int	cols;
		int	it;
		int	_lines;
		int	lm;
		int	xmc;
		int	pb;
		int	vt;
		int	wsl;
		int	nlab;
		int	lh;
		int	lw;
		int	ma;
		int	wnum;
		int	colors;
		int	pairs;
		int	ncv;
		int	bufsz;
		int	spinv;
		int	spinh;
		int	maddr;
		int	mjump;
		int	mcs;
		int	mls;
		int	npins;
		int	orc;
		int	orl;
		int	orhi;
		int	orvi;
		int	cps;
		int	widcs;
		int	btns;
		int	bitwin;
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
*fetch_capability(char *term);

#endif
