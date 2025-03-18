import re

# Token types that cover many Bash constructs.
TOKEN_TYPES = (
    "WORD",                     # A plain word or command/argument.
    "SINGLE_QUOTED_STRING",     # '...'
    "DOUBLE_QUOTED_STRING",     # "..."
    "PARAM_EXPANSION",          # ${...}
    "COMMAND_SUBSTITUTION",     # $(...) or `...`
    "ARITH_SUBSTITUTION",       # $((...))
    "REDIRECTION",              # >, <, >>, <<, <<-, <<<, 2>&1, etc.
    "OPERATOR",                 # &&, ||, ;, |, &, etc.
    "NEWLINE",                  # Newline is significant in Bash.
    "COMMENT",                  # Comment starting with #
    "HEREDOC",                  # A heredoc block
    "HERESTRING",               # A here-string (<<<)
    "ERROR",                    # Error token
    "EOF",                      # End-of-file marker
)

class Token:
    def __init__(self, typ, value, pos):
        self.typ = typ    # One of TOKEN_TYPES
        self.value = value
        self.pos = pos    # Position (index) in the input

    def __repr__(self):
        return f"Token({self.typ!r}, {self.value!r}, pos={self.pos})"

class BashTokenizer:
    def __init__(self, input_str):
        self.input = input_str
        self.pos = 0
        self.length = len(input_str)
        self.tokens = []
    
    def tokenize(self):
        """Main entry point: returns a list of tokens."""
        while self.pos < self.length:
            token = self.next_token()
            if token:
                self.tokens.append(token)
        self.tokens.append(Token("EOF", "", self.pos))
        return self.tokens

    def next_token(self):
        """Determines the next token starting at the current position."""
        self.skip_whitespace()
        if self.pos >= self.length:
            return None

        ch = self.current_char()

        # NEWLINE (always a token on its own)
        if ch == "\n":
            self.pos += 1
            return Token("NEWLINE", "\n", self.pos-1)

        # Comments: '#' outside of words and not within quotes.
        if ch == "#":
            return self._read_comment()

        # Redirections and operators may share characters.
        op = self._match_operator_or_redirection()
        if op:
            # For heredoc or here-string, we need to read extra lines.
            if op.value in ("<<", "<<-"):
                heredoc_token = self._read_heredoc(op)
                return heredoc_token
            if op.value == "<<<":
                # Here-string: the following word is taken as content.
                hs_token = self._read_here_string(op)
                return hs_token
            return op

        # If we see a '$', it might be one of several expansions.
        if ch == "$":
            return self._read_dollar_expansion()

        # Quoted strings.
        if ch in ("'", '"'):
            return self._read_quoted_string(ch)

        # Process substitution: <( ... ) or >( ... )
        if ch in "<>" and self.peek() == "(":
            return self._read_process_substitution()

        # Otherwise, treat it as a WORD.
        return self._read_word()

    def skip_whitespace(self):
        """Skip spaces and tabs, but not newlines (which are tokens)."""
        while self.pos < self.length and self.input[self.pos] in " \t":
            self.pos += 1

    def current_char(self):
        return self.input[self.pos] if self.pos < self.length else ""

    def peek(self, n=1):
        pos = self.pos + n
        if pos < self.length:
            return self.input[pos]
        return ""

    def _read_comment(self):
        start = self.pos
        # Consume until newline or end-of-input.
        while self.pos < self.length and self.input[self.pos] != "\n":
            self.pos += 1
        return Token("COMMENT", self.input[start:self.pos], start)

    def _match_operator_or_redirection(self):
        """
        Match the longest possible operator or redirection token.
        Bash has many such tokens.
        """
        candidates = [
            # Redirections (order matters: longer first)
            "2>&1", ">>", "<<-", "<<<", "<<", ">&", "<&", ">|",
            ">", "<",
            # Operators:
            "&&", "||", ";", "&", "|",
        ]
        for candidate in sorted(candidates, key=len, reverse=True):
            end = self.pos + len(candidate)
            if self.input[self.pos:end] == candidate:
                token_type = "REDIRECTION" if candidate[0] in "<>" or candidate in ("2>&1", ">&", "<&", ">|", "<<", "<<-", "<<<") else "OPERATOR"
                token = Token(token_type, candidate, self.pos)
                self.pos = end
                return token
        return None

    def _read_dollar_expansion(self):
        """Handle tokens that start with $: parameter, arithmetic, or command substitution."""
        start = self.pos
        self.pos += 1  # skip the '$'
        if self.pos >= self.length:
            return Token("ERROR", "Unexpected end after $", start)

        ch = self.current_char()
        # Arithmetic substitution: $(( ... ))
        if ch == "(" and self.peek() == "(":
            self.pos += 2  # skip ((
            content, err = self._read_until_matching("))")
            if err:
                return Token("ERROR", err, start)
            return Token("ARITH_SUBSTITUTION", content, start)
        # Command substitution: $( ... )
        elif ch == "(":
            self.pos += 1  # skip (
            content, err = self._read_until_matching(")")
            if err:
                return Token("ERROR", err, start)
            return Token("COMMAND_SUBSTITUTION", content, start)
        # Parameter expansion: ${...}
        elif ch == "{":
            self.pos += 1  # skip {
            content, err = self._read_until_matching("}")
            if err:
                return Token("ERROR", err, start)
            return Token("PARAM_EXPANSION", content, start)
        else:
            # Simple variable expansion: $VAR or $1 etc.
            var = ""
            while self.pos < self.length and re.match(r"[A-Za-z0-9_]", self.input[self.pos]):
                var += self.input[self.pos]
                self.pos += 1
            if var == "":
                return Token("ERROR", "Invalid variable expansion", start)
            return Token("WORD", "$" + var, start)

    def _read_until_matching(self, closing):
        """
        Read until the closing delimiter is found.
        This is a simplified routine that does not fully handle nested constructs
        for all types. For arithmetic or command substitution, a more
        elaborate state machine is needed.
        Returns (content, error_message). error_message is None if all is OK.
        """
        content = ""
        closing_len = len(closing)
        while self.pos < self.length:
            if self.input[self.pos:self.pos+closing_len] == closing:
                self.pos += closing_len
                return content, None
            # Support nesting for $(...) and $((...)) in a simple way:
            if self.input[self.pos] in ("(", "{") and closing in (")", "}", "))"):
                # We attempt a recursive read.
                nest, err = self._read_until_matching(")" if self.input[self.pos]=="(" else "}")
                if err:
                    return content, err
                content += nest
                continue
            content += self.input[self.pos]
            self.pos += 1
        return content, f"Unterminated expansion; expected '{closing}'"

    def _read_quoted_string(self, quote_char):
        """Read a string literal delimited by quote_char (' or ") with escapes."""
        start = self.pos
        self.pos += 1  # skip the opening quote
        content = ""
        while self.pos < self.length:
            ch = self.input[self.pos]
            if ch == quote_char:
                self.pos += 1
                typ = "SINGLE_QUOTED_STRING" if quote_char == "'" else "DOUBLE_QUOTED_STRING"
                return Token(typ, content, start)
            if ch == "\\":
                self.pos += 1
                if self.pos < self.length:
                    content += self.input[self.pos]
                    self.pos += 1
                else:
                    return Token("ERROR", "Incomplete escape sequence", start)
            else:
                content += ch
                self.pos += 1
        return Token("ERROR", f"Unterminated quoted string starting at {start}", start)

    def _read_word(self):
        """Read a word token that stops at whitespace or operator boundaries.
           Handles embedded parameter expansions and command substitutions by delegating
           to next_token if needed.
        """
        start = self.pos
        word = ""
        while self.pos < self.length:
            ch = self.current_char()
            # Stop at whitespace, newline, operator or redirection characters.
            if ch in " \t\n":
                break
            # If we hit a comment marker not within a word, break.
            if ch == "#" and (not word):
                break
            # Special characters that signal a new token.
            if ch in "'\"$<>&|;":
                # If we already collected some characters, return the WORD.
                if word:
                    break
                # Otherwise, delegate to the appropriate handler.
                if ch in "'\"":
                    return self._read_quoted_string(ch)
                if ch == "$":
                    return self._read_dollar_expansion()
                # Operators/redirections are handled separately.
                break
            word += ch
            self.pos += 1
        return Token("WORD", word, start)

    def _read_process_substitution(self):
        """Process substitutions look like <( ... ) or >( ... )"""
        start = self.pos
        op = self.current_char()  # '<' or '>'
        self.pos += 1  # consume < or >
        if self.pos < self.length and self.current_char() == "(":
            self.pos += 1  # skip '('
            content, err = self._read_until_matching(")")
            if err:
                return Token("ERROR", err, start)
            return Token("COMMAND_SUBSTITUTION", f"{op}({content})", start)
        else:
            return Token("ERROR", "Malformed process substitution", start)

    def _read_heredoc(self, op_token):
        """
        Handles heredoc syntax after encountering << or <<-.
        It reads the delimiter, then collects lines until one exactly matching the delimiter.
        """
        start = op_token.pos
        # Skip any spaces after << or <<-
        while self.pos < self.length and self.input[self.pos] in " \t":
            self.pos += 1

        # Read delimiter: it may be quoted or unquoted.
        delimiter = ""
        quoted = False
        if self.pos < self.length and self.input[self.pos] in ("'", '"'):
            quoted = True
            quote_char = self.input[self.pos]
            self.pos += 1
            while self.pos < self.length and self.input[self.pos] != quote_char:
                delimiter += self.input[self.pos]
                self.pos += 1
            if self.pos < self.length and self.input[self.pos] == quote_char:
                self.pos += 1
            else:
                return Token("ERROR", "Unterminated heredoc delimiter quote", start)
        else:
            # Unquoted: read until whitespace or newline.
            while self.pos < self.length and not self.input[self.pos].isspace():
                delimiter += self.input[self.pos]
                self.pos += 1

        if not delimiter:
            return Token("ERROR", "Missing heredoc delimiter", start)

        # Skip to end of line.
        while self.pos < self.length and self.input[self.pos] != "\n":
            self.pos += 1
        if self.pos < self.length and self.input[self.pos] == "\n":
            self.pos += 1

        # Collect heredoc content until a line exactly matching the delimiter.
        heredoc_content = ""
        found = False
        while self.pos < self.length:
            line_start = self.pos
            while self.pos < self.length and self.input[self.pos] != "\n":
                self.pos += 1
            line = self.input[line_start:self.pos]
            if self.pos < self.length and self.input[self.pos] == "\n":
                self.pos += 1
            # For <<-, ignore leading tabs.
            cmp_line = line.lstrip("\t") if op_token.value == "<<-" else line
            if cmp_line == delimiter:
                found = True
                break
            heredoc_content += line + "\n"
        if not found:
            return Token("ERROR", "Unterminated heredoc", start)
        return Token("HEREDOC", heredoc_content.rstrip("\n"), start)

    def _read_here_string(self, op_token):
        """
        Handles here-string syntax (<<<).
        The next non-whitespace word is taken as the string content.
        """
        start = op_token.pos
        self.skip_whitespace()
        content_token = self._read_word()
        if content_token.typ == "ERROR":
            return content_token
        return Token("HERESTRING", content_token.value, start)

# Example usage:
if __name__ == "__main__":
    sample_input = (
        "# This is a comment\n"
        "echo \"Hello, $USER\" && cat <<EOF\n"
        "This is a heredoc.\n"
        "It can include $variables and `commands`.\n"
        "EOF\n"
        "echo $(date)\n"
        "var=${HOME}\n"
        "result=$(( 3 + 4 * 2 ))\n"
        "process=<(grep foo file)\n"
        "a | b\n"
        "here <<< 'A here-string example'\n"
    )
    tokenizer = BashTokenizer(sample_input)
    tokens = tokenizer.tokenize()
    for token in tokens:
        print(token)
