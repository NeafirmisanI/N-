#HOUSTON, WE HAVE A PROBLEM HERE!

DIGITS = ["0123456789"]
ALPHABET = ["QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"]

class Lexer(object):
    
    def __init__(self, text):
        self.text = text
        self.pos = -1
        self.current_char = None
        self.advance()

    def advance(self):
        self.pos += 1
        try:
            self.current_char = self.text[self.pos]
        except IndexError:
            self.current_char = None
    
    def tokenize(self):
        #TODO: Update because it sucked
