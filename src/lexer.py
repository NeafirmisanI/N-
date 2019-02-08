from error import NSError
import string

class Lexer:

    def __init__(self):
        self.tokens = []
        self.code = ""
        self.pos = 0
        self.digits = "0123456789"

    def lex(self, code):
        self.code = code
        self.pos = 0

        while self.pos < len(self.code):
            self.skipspace()
            char = self.code[self.pos]

            validName = (
                string.ascii_lowercase + string.ascii_uppercase +
                "$+-/*%=><!^"
            )

            if char in "+-*/=":
                self.addToken("OPERATOR", char)
                continue
            if char in '\'\"':
                self.string(char)
                continue
            if char in self.digits:
                self.number()
                continue
            if (char == '-'
                and self.current + 1 < len(self.inp)
                and self.inp[self.current + 1] in digits):
                        self.number()
                        continue
            if char in validName:
                self.name()
                continue
            raise NSError("Lexical error: Unrecognized character " + char)
        return self.tokens
            
    def skipspace(self):
        while self.pos <= len(self.code):
            if self.code[self.pos] in [' ', '\t', '\n', '\r']:
                self.pos += 1
                continue
            if self.code[self.pos] == '~':
                self.pos += 1
                try:
                    if self.code[self.pos] == '~':
                        self.pos += 1
                except IndexError:
                    raise NSError("Uncaught SyntaxError: Invalid comment syntax")
                self.pos += 1
                continue
            break

    def addToken(self, typ, value):
        self.tokens.append({
            "type": "OPERATOR",
            "value": value,
        })
        self.pos += len(str(value))

    def string(self, quote):
        self.pos += 1
        value = ""

        try:
            while self.code[self.pos] != quote:
                char = self.code[self.pos]

                if char == '\\':
                    self.pos += 1
                    if self.code[self.pos] == '"':
                        char = '"'
                    elif self.code[self.pos] == "'":
                        char = "'"
                    elif self.code[self.pos] == "\\":
                        char = "\\"
                    elif self.code[self.pos] == "n":
                        char = "\n"
                    elif self.code[self.pos] == "t":
                        char = "\t"
                    elif self.code[self.pos] == "b":
                        char = "\b"
                    elif self.code[self.pos] == "a":
                        char = "\a"

                value += char
                self.pos += 1
        except IndexError:
            raise NSError("Uncaught SyntaxError: Invalid string syntax")

        self.pos += 1
        self.tokens.append({
            "type": "STRING",
            "value": value,
        })
        
    def number(self):
        lexeme = ""
        while not self.atEnd() and self.code[self.pos] in self.digits:
            lexeme += self.code[self.pos]
            self.pos += 1
        value = None
        if not self.atEnd() and self.code[self.pos] == '.':
            lexeme += self.code[self.pos]
            self.pos += 1
            while not self.atEnd() and self.code[self.pos] in self.digits:
                lexeme += self.code[self.pos]
                self.pos += 1
            value = float(lexeme)
        else:
            value = int(lexeme)

        self.tokens.append({
            "type": "NUMBER",
            "value": value,
        })

    def name(self):
        validName = (
            string.ascii_lowercase + string.ascii_uppercase +
            "$+-/*%=><!^"
        )
        
        value = ""
        while not self.atEnd() and self.code[self.pos] in validName:
            value += self.code[self.pos]
            self.pos += 1
        
        self.tokens.append({
            "type": "IDENTIFIER",
            "value": value,
        })
    
    def atEnd(self):
        return self.pos >= len(self.code)
