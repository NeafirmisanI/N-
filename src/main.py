import lexer
import parser_

def main():
    content = ""
    while True:
        content = promptForInput()
        lex = lexer.Lexer(content)
        tokens = lex.tokenize()
        parse = parser_.Parser(tokens)
        parse.parse()

def promptForInput():
    return str(input("N# > "))

main()