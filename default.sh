#!/usr/bin/hsh

# set -o experr
prompt_left()
{
	ERR=$?
	STATUS=""
	if [ $ERR != 0 ]
	then
		STATUS=" $(tput setaf 1)[CODE: $ERR]$(tput setaf 7)"
	fi

	echo -n "$(tput setaf 3)$USER$(tput setaf 2)@$(tput setaf 12)$PWD$(tput setaf 7)$STATUS $(tput setaf 13)󰘧$(tput setaf 7) "
	return $ERR
}
