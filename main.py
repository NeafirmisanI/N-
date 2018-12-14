import os
import datetime

print ("N# Python Interpreter")

def promptForInput():
    return str(input("N# - "))

def promptForInputMultiline():
    return str(input("N# ~ "))

def isDigit(char):
    for d in "0123456789":
        if char == d:
            return True
    return False

def isNumber(string):
    for c in string:
        if not isDigit(c):
            return False
    return True

# Static assignment for isString()
stringLiteralIndexes = []
# End static assignment

def isString(index):
    return stringLiteralIndexes[index]

def sortStringIntoList(string):
    words = [""]
    index = 0
    
    for i, s in enumerate(string):
        if words[index] == "df":
            fnData = captureFn(string, i-2)
            parseFunction(fnData)
        if s != ' ' and s != '\"' and s != "\'":
            words[index] += s
            words.append("")
            stringLiteralIndexes.append(False)
            stringLiteralIndexes[index] = False
        elif s == '\"' or s == "\'":
            words[index] += getStringContents(i, string)
            i = getStringEndIndex(i, string) + 1
            words.append("")
            stringLiteralIndexes.append(False)
            stringLiteralIndexes[index] = True
            index += 1
        else:
            index += 1
    print(words)
    return words

def findCloseQuote(string):
    for i, s in enumerate(string):
        if s == '"' or s == "'":
            return i
    return 0

def getStringContents(index, string):
    string = string[index+1:]
    endIndex = int(findCloseQuote(string))
    string = string[:endIndex]
    return string

def getStringEndIndex(index, string):
    string = string[index+1:]
    return findCloseQuote(string)

def value(words, index):
    if isNumber(words[index]):
        return words[index]
    elif isString(index):
        return words[index]
    elif isFunction(words[index]):
        return parseFunctionCall(words, index)
    elif isVariable(words[index]):
        return getVariableData(words[index])
    elif isMath(words[3]):
        return evaluate(words[3], words[2], words[4])
    elif isMathPro(words[3]):
        if words[3] == "factorial":
            return evaluateFactorial(words[2])
        else:
            return evaluateFibonancci(words[2])
    else:
        print("N#\nSyntaxError: Unknown identifier \"" + words[index] + "\"")
        return ""

def validateOperation(operand, operator):
    validExpression = ((operand == "Log" or operand == "uppercase" or operand == "lowercase" or operand == "camelcase" or operand == "swapcase" or operand == "system") and (operator == "=" or operator == "+" or operator == "-" or operator == "*" or operator == "/" or operator == "fibonacci" or operator == "factorial"))
    
    if not validExpression:
        print("N#\nSyntaxError: Invalid operator \"" + operator + "\" on operand \"" + operand + "'")

def isKeyword(string):
    keywords = [
        "uppercase",
        "lowercase",
        "camelcase",
        "swapcase"
    ]

    for k in keywords:
        if string == k:
            return True
    return False

def isOperator(string):
    operators = [
        "=",
        "+",
        "-"
        "*",
        "/",
        "factorial",
        "fibonacci"
    ]

    for o in operators:
        if string == o:
            return True
    return False

def nPrint(words):
    validateOperation(words[0], words[1])
    if isKeyword(words[3]): validateOperation(words[3], words[5])
    if isVariable(words[2]):
        print(getVariableData(words[2]))
    if isMath(words[3]):
        print(evaluate(words[3], words[2], words[4]))
    elif isMathPro(words[3]):
        if words[3] == "factorial":
            print(evaluateFactorial(words[2]))
        else:
            print(evaluateFibonancci(words[2]))
    print(stringProcessing(3, words[5], words))

def nSystem(words):
    validateOperation(words[0], words[1])
    os.system(value(words, 2))

def executeList(words):
    if words[0] == "":
        print("N#\nSyntaxError: No Value Received")
        return
    elif words[0] == "Log":
        if not words[2] == "Input":
            nPrint(words)
        else:
            nPrint(askForInput)
    elif isFunction(words[0]):
        value(words, 0)
    elif words[0] == "system":
        nSystem(words)
    elif not isOperator(words[0]):
        if words[1] == "=":
            if isMath(words[3]):
                setVariable(words[0], evaluate(words[3], words[2], words[4]), words)
            else:
                if not words[2] == "Input":
                    setVariable(words[0], 2, words)
                else:
                    setVariable(words[0], askForInput(), words)
    else:
        value(words, 0)


def assignVariable(identifier, data):
    variableIdentifiers.append(identifier)
    variableData.append(data)

def getVariableData(identifier):
    for i, s in enumerate(variableIdentifiers):
        if s == identifier:
            return variableData[i]

def setVariable(identifier, num, words):
    if isNumber(identifier):
        print("N#\nSyntaxError: Number cannot be used as an identifier")
        return
    db = stringProcessing(num, words[5], words)
    if isVariable(identifier):
        variableData[getVariableIndex(identifier)] = db
    else:
        assignVariable(identifier, db)

def getVariableIndex(identifier):
    for i, s in enumerate(variableIdentifiers):
        if s == identifier:
            return i

def isVariable(identifier):
    for s in variableIdentifiers:
        if s == identifier:
            return True
    return False
            

# Static assignment for variable functions
variableIdentifiers = [
    "datetime",
    "datetime.time",
    "datetime.date",
    "datetime.week",
    "datetime.day.year",
    "datetime.timezone",
    "datetime.utc",
    "datetime.microsecnd",
    "datetime.second",
    "datetime.minute",
    "datetime.ampm",
    "datetime.hour",
    "datetime.year",
    "datetime.year.short",
    "datetime.month.number",
    "datetime.month.short",
    "datetime.month",
    "datetime.day.month",
    "datetime.weekday.number",
    "datetime.weekday",
    "datetime.weekday.short"
]  
variableData = [
    datetime.datetime.now().strftime("%c"),
    datetime.datetime.now().strftime("%X"),
    datetime.datetime.now().strftime("%x"),
    datetime.datetime.now().strftime("%W"),
    datetime.datetime.now().strftime("%j"),
    datetime.datetime.now().strftime("%Z"),
    datetime.datetime.now().strftime("%z"),
    datetime.datetime.now().strftime("%f"),
    datetime.datetime.now().strftime("%S"),
    datetime.datetime.now().strftime("%M"),
    datetime.datetime.now().strftime("%p"),
    datetime.datetime.now().strftime("%H"),
    datetime.datetime.now().strftime("%Y"),
    datetime.datetime.now().strftime("%y"),
    datetime.datetime.now().strftime("%m"),
    datetime.datetime.now().strftime("%b"),
    datetime.datetime.now().strftime("%B"),
    datetime.datetime.now().strftime("%d"),
    datetime.datetime.now().strftime("%w"),
    datetime.datetime.now().strftime("%A"),
    datetime.datetime.now().strftime("%a")
]

mathOperators = [
    "+",
    "-",
    "*",
    "/"
]

mathProperties = [
    "factorial",
    "fibonacci"
]

# End static assignment

# Static assignment for functions
functions = []
# End static assignment

class Function():
    def __init__(self, identifier):
        self.paramIdentifiers = []
        self.identifier = identifier
        self.contents = []
        
    def isParam(self, identifier):
        for s in self.paramIdentifiers:
            if s == identifier:
                return True
        return False

    def getParamData(self, identifier, data):
        for i, s in enumerate(self.paramIdentifiers):
            if s == identifier:
                return data[i]
    
    def getParamIndex(self, identifier):
        for i, s in enumerate(self.paramIdentifiers):
            if s == identifier:
                return i

    def newParam(self, identifier):
        self.paramIdentifiers.append(identifier)

    def appendStatement(self, statement):
        self.contents.append(statement)

    def execute(self, paramData, words):
        if bool(runFunction):
            global variableIdentifiers
            global variableData

            variableIdentifiersBackup = variableIdentifiers
            variableDataBackup = variableData

            if self.paramIdentifiers[0]:
                for index, identifier in enumerate(self.paramIdentifiers):
                    setVariable(identifier, paramData[index], words)

            for s in self.contents:
                executeList(s)

            variableIdentifiers = variableIdentifiersBackup
            variableData = variableDataBackup

def parseFunction(rawInput):
    rawMetadata = sortFunctionmetadataIntoList(rawInput)
    if len(rawMetadata) < 3: pass
    if rawMetadata[0] != "df": pass
    
    identifier = rawMetadata[1]
    if isVariable(identifier): pass

    rawContents = parseFunctionContents(rawInput)
    splitContents = rawContents.split("\n")

    contents = []

    for line in splitContents:
        contents.append(sortStringIntoList(line))
    contents = contents[1:]

    functions.append(Function(identifier))
    functionIndex = getFunctionIndex(identifier)

    paramNames = parseParamNames(rawMetadata, 2)
    if paramNames != None:
        for p in paramNames:
            functions[functionIndex].newParam(p)

    functions[functionIndex].contents = contents
    print(rawInput)

def captureFn(string, startIndex):
    string = string[startIndex:]
    while True:
        for c in string:
            if c == ")":
                return string
        string += "\n"
        string += promptForInputMultiline()

def getFunctionIndex(identifier):
    for i, f in enumerate(functions):
        if f.identifier == identifier:
            return i
    return 0

def isFunction(identifier):
    for f in functions:
        if f.identifier == identifier:
            return True
    return False

def parseFunctionContents(rawInput):
    startIndex = 0
    for i, c in enumerate(rawInput):
        if c == "(":
            startIndex = i
    
    contents = rawInput[startIndex+1:len(rawInput)-2]
    return contents

def getFunctionCallParams(words, idIndex, function):
    global runFunction
    runFunction = 1
    rawWords = unsplitWords(words[idIndex + 2:])
    params = rawWords.split(",")
    functionCorrectLastParam(params)

    if len(params) != len(function.paramIdentifiers) or (function.paramIdentifiers[0] == "" and params[0] != ""):
        print("N#\nSyntaxError: Invalid parameter amount provided")
        runFunction = 0
    return params

def parseParamNames(metadata, startIndex):
    names = [""]
    index = 0

    metadata = metadata[startIndex:]

    for m in metadata:
        for c in m:
            if c == " " or c == "{":
                continue

            if c == ",":
                index += 1
                names.append("")
            elif c == "}":
                return names
            else:
                names[index] += c

def sortFunctionmetadataIntoList(rawInput):
    words = [""]
    index = 0

    for i, c in enumerate(rawInput):
        if c == " ":
            words.append("")
            index += 1
        elif c == "(":
            return words
        else:
            words.append("")
            words[index] += c 
    
    print("N#\nSyntaxError: Invalid function definition")
    
def callFunction(identifier, paramData, words):
    function = functions[getFunctionIndex(identifier)]
    return function.execute(paramData, words)

def parseFunctionCall(words, idIndex):
    function = functions[getFunctionIndex(words[idIndex])]
    if not function.paramIdentifiers:
        callFunction(words[idIndex], [], words)
    else:
        callFunction(words[idIndex], getFunctionCallParams(words, idIndex, function), words)
        
def isMath(operator):
    for i in mathOperators:
        if operator == i:
            return True
    return False

def evaluate(operator, numo, numt):
    if operator == "+":
        return int(numo) + int(numt)
    elif operator == "-":
        return int(numo) - int(numt)
    elif operator == "*":
        return int(numo) * int(numt)
    else:
        return int(numo) / int(numt)
    
def unsplitWords(words):
    string = ""
    for w in words:
        for c in w:
            string += c
    return string

def valueStr(string):
    if isNumber(string):
        return string
    if isStringLiteral(string):
        return string
    elif isVariable(string):
        return getVariableData(string)
    else:
        print("N#\nSyntaxError: Unknown VSTR identifier \"" + string + "\"")
        return ""

def isStringLiteral(string):
    if string[0] != "\"": return False
    if string[len(string)-1] != "\"": return False
    return True

def main():
    while True:
        executeList(sortStringIntoList(promptForInput()))

def functionCorrectLastParam(list):
    paramsLength = len(list)
    pId = 1
    for p in list:
        if(paramsLength == pId):
            list[pId-1] = correct(str(p))
        pId += 1

def correct(string):
    strLength = len(string)
    strToCorrect = strLength - 1
    return string[0:strToCorrect]

def isMathPro(words):
    for i in mathProperties:
        if words == i:
            return True
    return False

def evaluateFactorial(n):
    num = int(n)
    if num == 0:
        return 1
    else:
        return num * evaluateFactorial(num-1)
    
def evaluateFibonancci(n):
    num = int(n)
    if num == 0: 
        return 0
    elif num == 1: 
        return 1
    else: 
        return evaluateFibonancci(num-1)+evaluateFibonancci(num-2)
    
def askForInput():
    inp = input("")
    return inp

def stringProcessing(item, effect, words):
    if effect == "uppercase":
        return value(words, item).upper()
    elif effect == "lowercase":
        return value(words, item).casefold()
    elif effect == "camelcase":
        return value(words, item).title()
    elif effect == "swapcase":
        return value(words, item).swapcase()
    elif effect == "":
        return value(words, item)

main()
