#!/usr/bin/env sh

make C=cc
SHELL_PROFILE=./default.sh SHELL_FORTUNES=./fortunes SHELL_COMPLETION=./completion ./minishell
