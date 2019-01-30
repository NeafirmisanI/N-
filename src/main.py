import lexer
import interpreter
import sys

interpret = interpreter.Parser()

def main():
    print("N# Shell")
    content = ""
    if len(sys.argv) == 1:
        promptForInput()
    elif len(sys.argv) == 2:
        openFile(sys.argv[1])

def promptForInput():
     while True:
        content = str(input("N# > "))
        run(content)

def run(code):
     lex = lexer.Lexer(content)
     tokens = lex.tokenize()
     interpret.parse(tokens)

def openFile(name):
     with open(name, "r") as sourceFile:
        run(sourceFile.read())

main()
