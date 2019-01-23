import lexer
import interpreter

def main():
    content = ""
    while True:
        content = promptForInput()
        lex = lexer.Lexer(content)
        tokens = lex.tokenize()
        interpret = interpreter.Parser(tokens)
        interpret.parse()

def promptForInput():
    return str(input("N# > "))

main()
