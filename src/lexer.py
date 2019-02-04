from error import NSError

TT_PLUS = "OPERATOR"
TT_MINUS = "OPERATOR"
TT_MUL = "OPERATOR"
TT_DIV = "OPERATOR"
TT_EQU = "OPERATOR"
TT_INT = "INT"
TT_FLOAT = "FLOAT"
TT_STRING = "STRING"
TT_IDENTIFIER = "IDENTIFIER"
TT_PRINT = "PRINT_STATEMENT"
TT_VAR = "VAR_DECLARATION"
DIGITS = "0123456789"
ALPHABET = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM\'\""

class Lexer(object):

    def __init__(self, text):
        self.text = text
        self.pos = -1
        self.current_char = None
        self.advance()

    def advance(self):
        self.pos += 1
        self.current_char = self.text[self.pos] if self.pos < len(self.text) else None

    def tokenize(self):
        tokens = []

        while self.current_char != None:
            if self.current_char in ' \t':
                self.advance()
            elif self.current_char in DIGITS:
                tokens.append(self.number())
            elif self.current_char in ALPHABET:
                tokens.append(self.alphabet())
            elif self.current_char == '=':
                tokens.append([TT_EQU, self.current_char])
                self.advance()
            elif self.current_char == '+':
                tokens.append([TT_PLUS, self.current_char])
                self.advance()
            elif self.current_char == '-':
                tokens.append([TT_MINUS, self.current_char])
                self.advance()
            elif self.current_char == '*':
                tokens.append([TT_MUL, self.current_char])
                self.advance()
            elif self.current_char == '/':
                tokens.append([TT_DIV, self.current_char])
                self.advance()
            else:
                raise NSError("N# ERROR: Invalid character " + self.current_char)

        return tokens

    def number(self):
        num_str = ''
        dot_count = 0
        token = []

        while self.current_char != None and self.current_char in DIGITS + '.':
            if self.current_char == '.':
                if dot_count == 1: break
                dot_count += 1
                num_str += '.'
            else:
                num_str += self.current_char
            self.advance()

        if dot_count == 0:
            token = [TT_INT, int(num_str)]
        else:
            token = [TTT_FLOAT, float(num_str)]

        return token

    def alphabet(self):
        alphabet_str = ''
        quotation_count = 0
        token = []

        while self.current_char != None and self.current_char in ALPHABET + "\'\"":
            if self.current_char in "\'\"":
                quotation_count += 1
                alphabet_str += self.current_char
            else:
                alphabet_str += self.current_char
            self.advance()

        if quotation_count == 0:
            if alphabet_str == "print":
                token = [TT_PRINT, alphabet_str]
            elif alphabet_str == "var":
                token = [TT_VAR, alphabet_str]
            else:
                token = [TT_IDENTIFIER, alphabet_str]
        else:
            token = [TT_STRING, alphabet_str]

        return token
