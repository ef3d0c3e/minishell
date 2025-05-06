#!/usr/bin/hsh

colored()
{
	printf "\x01%s\x02" $(tput setaf $1)
}

# set -o experr
prompt_left()
{
	ERR=$?
	STATUS=""
	if [ $ERR != 0 ]
	then
		STATUS=" $(colored 1)[CODE: $ERR]$(colored 7)"
	fi

	echo -n "$(colored 3)$USER$(colored 2)@$(colored 12)$PWD$(colored 7)$STATUS $(colored 13)󰘧$(colored 7) "
	return $ERR
}
