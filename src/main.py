import lexer
import interpreter

interpret = interpreter.Parser()

def main():
    content = ""
    while True:
        content = promptForInput()
        lex = lexer.Lexer(content)
        tokens = lex.tokenize()
        interpret.parse(tokens)

def promptForInput():
    return str(input("N# > "))

main()
