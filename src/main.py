import sys
from lexer import Lexer
from interpreter import Interpreter
from error import NSError

interpreter = Interpreter()

def main():
    content = ""
    if len(sys.argv) == 1:
        print("N# Shell")
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
        except KeyboardInterrupt:
            print("\nQuitting...")
            sys.exit(1)
        except:
            print("Internal error, raising exception:")
            raise

def run(code):
     global interpreter 
     tokens = Lexer().lex(code)
     #print(tokens)
     interpreter.interpret(tokens)

def openFile(name):
     with open(name, "r") as sourceFile:
        try:
            run(sourceFile.read())
        except NSError as Error:
            print(Error)
            sys.exit(1)

main()
