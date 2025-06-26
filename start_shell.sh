#!/usr/bin/env sh

make C=cc
SHELL_PROFILE=$(realpath default.sh) SHELL_FORTUNES=$(realpath fortunes) SHELL_COMPLETION=$(realpath completion) ./minishell
