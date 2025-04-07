# Minishell -- A simple C shell

# Authors:
 - [ef3d0c3e](https://github.com/ef3d0c3e)
 - [Cheldux](https://github.com/Cheldux)

# Documentation

## Pipeline

When evaluating a pipeline the following is done:
 1. Resolve all programs that will be executed in chain (e.g pipe separated, `&&` and `||` lazily resolve when the execution is actually requested):
    - Check if the program is a `function` (not yet supported)
    - Check if the program is a `builtin` (using the builtin storage structure)
    - Check if the program is an executable in the `$PATH` (using the builtin storage structure)
    - If not found, repopulate the `$PATH` entries and try to resolve again
    - If not found, throw an error


# License

This project is licensed under the GNU GPL version 3.
