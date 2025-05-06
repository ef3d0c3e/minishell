# Minishell -- A simple C shell

# Authors:
 - [ef3d0c3e](https://github.com/ef3d0c3e)
 - [Cheldux](https://github.com/Cheldux)

# Documentation

## Regex / Pattern matching

Pattern matching is partially implemented to follow bash's rules.
Notable exceptions include the following:
 - Character class range aren't implemented: `[a-z]`, `[aZ-0a]` (etc..) haven't been
 implemented
 - The regex is built on a byte-basis (not codepoint-basis), therefore escaping a unicode
 character can lead to surprising results. For instance `\Σ` aka `5c ce a3` leads only the byte after `5c` to be escaped.
 In practice this is not a problem because only ASCII characters are reserved words for the regex, the rest has to match literally.
 - Similarly, `nocaseglob` only applies to ASCII characters. Other characters (i.e unicode) have to match literally.
 - Complex regexes, e.g with `extglob` can mess up tokenization, a workaround is to
 store them in variables. For instance:
    - `echo !(foo|*.c)` will not be recognized, instead write the following:
    `PAT="!(foo|*.c); echo $PAT"`. This ugly workaround prevents `(|)` from being
    parsed.
 - Pattern exclusion rules, `GLOBIGNORE` and `FIGNORE` are not supported.
 - Changing rule `globskipdots` is not possible, but it is enabled by default.

The following option (available via the `shopt` builtin) are implemented:
 - `extglob`: Allows extended patterns, defaults: `0`
    - `?(PATTERN-LIST)`
        matches zero or one occurrence of the given patterns.
    - `*(PATTERN-LIST)`
        matches zero or more occurrences of the given patterns.
    - `+(PATTERN-LIST)`
        matches one or more occurrences of the given patterns.
    - `@(PATTERN-LIST)`
        matches one of the given patterns.
    - `!(PATTERN-LIST)`
        matches anything except one of the given patterns.
 - `globstar`: Allows the `**` pattern which will match subdirectory when followed by a `/`, defaults: `0`
 - `dotglob`: Allows files starting with `.` to match when using `*` or `**`, defaults: `0`
 - `nocaseglob`: **ASCII** characters comparison is done in a case-insensitive manner, defaults: `0`


# License

This project is licensed under the GNU GPL version 3.
See [LICENSE](./LICENSE) for more informations.
