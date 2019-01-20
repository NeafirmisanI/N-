from sys import *

tokens = []
symbols = {}

def lex(filecontents):
    tok = ""
    string = ""
    state = 0
    isexpr = 0
    expr = ""
    n = ""
    var = ""
    varStarted = 0
    filecontents = list(filecontents)
    for char in filecontents:
        tok += char
        if tok == " ":
            if state == 0:
                tok = ""
            else:
                tok = " "
        elif tok == "\n" or tok == "<EOF>":
            tok = ""
            if expr != "" and isexpr == 1:
                tokens.append("EXPR:" + expr)
                expr = ""
            elif expr != "" and isexpr == 0:
                tokens.append("NUM:" + expr)
                expr = ""
            elif var != "":
                tokens.append("VAR:" + var)
                var = ""
        elif tok == "=" and state == 0:
            if var != "":
                tokens.append("VAR:" + var)
                var = ""
                varStarted = 0
            tokens.append("EQUALS")
            tok = ""
        elif tok == "var" and state == 0:
            varStarted = 1
            var += tok
            tok = ""
        elif varStarted == 1:
            var += tok
            tok = ""
        elif tok == "print" or tok == "Print":
            tokens.append("PRINT")
            tok = ""
        elif tok in ["1","2","3","4","5","6","7","8","9","0"]:
            expr  += tok
            tok = ""
        elif tok in ["+","-","*","/","(",")","%"]:
            isexpr = 1
            expr += tok
            tok = ""
        elif tok == "\"":
            if state == 0:
                state = 1
            elif state == 1:
                tokens.append("STRING:" + string + "\"")
                string = ""
                state = 0
                tok = ""
        elif state == 1:
            string += tok
            tok = ""
    return tokens
    #print(tokens)
   
def interpret(toks):
    i = 0
    while(i < len(toks)):
        if toks[i] + " " + toks[i+1][0:6] == "PRINT STRING" or toks[i] + " " + toks[i+1][0:3] == "PRINT NUM" or toks[i] + " " + toks[i+1][0:4] == "PRINT EXPR":
            if toks[i+1][0:6] == "STRING":
                doPrint(toks[i+1])
            elif toks[i+1][0:3] == "NUM":
                doPrint(toks[i+1])
            elif toks[i+1][0:4] == "EXPR":
                doPrint(toks[i+1])
            i += 2
        if toks[i][0:3] + " " + toks[i+1] + " " + toks[i+2][0:6] == "VAR EQUALS STRING":
            doAssign(toks[i], toks[i+2])
            i += 3

def doPrint(toPrint):
    if toPrint[0:6] == "STRING":
        toPrint = toPrint[8:]
        toPrint = toPrint[:-1]
    elif toPrint[0:3] == "NUM":
        toPrint = toPrint[4:]
    elif toPrint[0:4] == "EXPR":
        toPrint = evalExpression(toPrint[5:])
    print(toPrint)

def evalExpression(expr):
    return eval(expr)

def doAssign(varname, varvalue):
    name = "$" + varname[7:]
    symbols[name] = varvalue
      
def open_file(filename):
    data = open(filename, "r").read()
    return data
   
def promptForInput():
    return str(input("> "))

def run():
    #data = open_file(argv[1]) #For console
    #data = promptForInput() #For use inside IDE
    with open("source.txt") as n:
        data = n.read() #For testing
        data += "<EOF>"
        toks = lex(data)
        interpret(toks)

run()