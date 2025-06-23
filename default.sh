#!/usr/bin/hsh

colored()
{
	printf "%s" $(tput setaf $1)
}

# set -o experr
prompt_left()
{
	ERR=$?
	STATUS=""
	if [ $ERR != 0 ]
	then
		STATUS=" $(colored 1)[CODE: $ERR]"
	fi
	GIT="$(git branch --show-current 2&>/dev/null)"
	if [ $? == 0 ] && [ ! -z "$GIT" ]
	then
		GIT=" $(colored 2)($GIT)"
	else
		GIT=""
	fi

	echo -n "$(colored 3)${USER}$(colored 2)@$(colored 12)${PWD}${STATUS}${GIT}$(colored 13) 󰘧$(colored 7) "
	return $ERR
}

[ ! -z "${SHELL_FORTUNES}" ] &&
	[ -f "${SHELL_FORTUNES}" ] &&
	type shuf 2&>/dev/null 1&>/dev/null &&
	type head 2&>/dev/null 1&>/dev/null &&
	shuf "${SHELL_FORTUNES}" | head -n 1 | cowsay
