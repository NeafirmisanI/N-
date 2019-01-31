import sys
import lexer
import interpreter
from error import NSError

interpret = interpreter.Parser()

def main():
    print("N# Shell")
    content = ""
    if len(sys.argv) == 1:
        promptForInput()
    elif len(sys.argv) == 2:
        openFile(sys.argv[1])
    else:
        raise RuntimeError("Usage: N# [filename]")

def promptForInput():
     while True:
        content = str(input("N# > "))
        try:
            run(content)
        except NSError as error:
            print(error)
            print()
        except KeyboardInterrupt:
            print("\nQuitting...")
            sys.exit(1)
        except:
            print("Internal error, raising exception:")
            raise

def run(code):
     lex = lexer.Lexer(code)
     tokens = lex.tokenize()
     interpret.parse(tokens)

def openFile(name):
     with open(name, "r") as sourceFile:
        try:
            run(sourceFile.read())
        except NSError as Error:
            print(Error)
            print()
            sys.exit(1)

main()
