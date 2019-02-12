import webbrowser
import varObject
from error import NSError

class Interpreter:

    def __init__(self):
        self.varObj = varObject.VariableObject()
        self.tokens = []
        self.stringProc = ["lowercase", "uppercase", "camelcase", "swapcase"]
        self.keywords = ["var", "print", "browse"]
        self.staticVars = ["pi", "euler", "os", "os-version", "argv", "cwd", "tau"]
    
    def interpret(self, tokens):
        self.tokens = tokens
        self.token_index = 0
        while self.token_index < len(self.tokens):
            token = self.tokens[self.token_index]
            token_type = token["type"]
            token_value = token["value"]
            
            if token_type == "IDENTIFIER" and token_value == "var":
                advance = self.tokens[self.token_index + 1]
                advance_val = advance["value"]
                if advance_val not in self.varObj.variables:
                    self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 0)
                else:
                    raise NSError("Uncaught ReferenceError: Variable " + advance_val + " already referenced")
            elif token_type == "IDENTIFIER" and token_value not in self.keywords:
                if self.isVariable(token_value):
                    self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 1)
                else:
                    raise NSError("Uncaught ReferenceError: Undefined variable " + token_value)
            elif token_type == "IDENTIFIER" and token_value == "print":
                self.parsePrintStatement(self.tokens[self.token_index:len(self.tokens)])
            elif token_type == "IDENTIFIER" and token_value == "browse":
                self.parseBrowseDeclaration(self.tokens[self.token_index:len(self.tokens)])
            self.token_index += 1

    def parseBrowseDeclaration(self, token_stream):
        token = self.tokens[1]
        token_value = token["value"]
        token_type = token["type"]
        if token_type == "STRING":
            webbrowser.open_new_tab(token_value)
        else:
            if token_type == "IDENTIFIER":
                raise NSError("Uncaught SyntaxError: Unexpected identifier")
            else:
                raise NSError("Uncaught SyntaxError: Unexpected number")

    def parsePrintStatement(self, token_stream):
        tokens_checked = 0

        toPrint = ""

        for token in range(0, len(token_stream)):
            token_i = self.tokens[tokens_checked]
            token_type = token_i["type"]
            token_value = token_i["value"]

            if token == 1 and token_value == "input":
                toPrint = self.parseInputDeclaration(token)
            elif token == 1 and token_type == "STRING":
                toPrint = token_value
            elif token == 1 and token_type == "NUMBER":
                toPrint = token_value
            elif token == 1 and token_type == "IDENTIFIER" and self.isVariable(token_value):
                toPrint = self.varObj.get_variable(token_value)
            elif token == 2 and token_type == "IDENTIFIER" and token_value in self.stringProc:
                if token_value == "lowercase":
                    toPrint = self.convertToLowercase(toPrint)
                elif token_value == "uppercase":
                    toPrint = self.convertToUppercase(toPrint)
                elif token_value == "camelcase":
                    toPrint = self.convertToCamelcase(toPrint)
                if token_value == "swapcase":
                    toPrint = self.convertToSwapcase(toPrint)
            elif token == 1 and token_type not in ["STRING", "NUMBER", "IDENTIFIER"]:
                raise NSError("Uncaught ReferenceError: " + token_value + " is not defined")

            tokens_checked += 1
        
        print(toPrint)

        self.token_index += tokens_checked

    def parseVariableDeclaration(self, token_stream, index):
        tokens_checked = 0

        name = ""
        value = ""

        for token in range(index, len(token_stream) + index):
            token_i = self.tokens[tokens_checked]
            token_type = token_i["type"]
            token_value = token_i["value"]

            if token == 1 and token_type == "IDENTIFIER":
                if token_value not in self.staticVars:
                    name = token_value
                else:
                    raise NSError("Uncaught SyntaxError: Cannot change value of static variable " + token_value)
            elif token == 1 and token_type != "IDENTIFIER":
                if token_type == "STRING":
                    raise NSError("Uncaught SyntaxError: Unexpected string")
                elif token_type == "NUMBER":
                    raise NSError("Uncaught SyntaxError: Unexpected number")
            elif token == 2 and token_type != "OPERATOR":
                try:
                    token_advance = self.tokens[token + 1]
                    advance_val = token_advance["value"]
                except IndexError:
                    advance_val = token_value
                if advance_val != token_value:
                    raise NSError("Uncaught SyntaxError: Missing assignment operator")
                else:
                    value = "undefined"
            elif token == 3 and token_type in ["STRING", "NUMBER", "IDENTIFIER"]:
                if token_type == "IDENTIFIER":
                    if token_value == "input":
                        value = self.parseInputDeclaration(token)
                    else:
                        value = self.varObj.get_variable(token_value)
                else:
                    value = token_value
            elif token == 3 and token_type not in ["STRING", "NUMBER"]:
                raise NSError("Uncaught SyntaxError: Invalid variable assignment value " + token_type)
            elif token == 4 and token_type == "IDENTIFIER" and token_value in self.stringProc:
                if token_value == "lowercase":
                    value = self.convertToLowercase(value)
                elif token_value == "uppercase":
                    value = self.convertToUppercase(value)
                elif token_value == "camelcase":
                    value = self.convertToCamelcase(value)
                if token_value == "swapcase":
                    value = self.convertToSwapcase(value)

            tokens_checked += 1
        
        self.varObj.set_variable(name, value)
        self.token_index += tokens_checked

    def parseInputDeclaration(self, token):
        try:
            token_advance = self.tokens[token + 1]
            advance_val = token_advance["value"]
            if advance_val == "(":
                token_advance = self.tokens[token + 2]
                advance_val = token_advance["value"]
                advance_type = token_advance["type"]
                if advance_type == "STRING":
                    value = str(input(advance_val))
                elif advance_val == ")":
                    value = str(input(""))
                else:
                    raise NSError("Uncaught SyntaxError: Invalid input() function syntax")
            else:
                raise NSError("Uncaught SyntaxError: Invalid input() function syntax")
        except IndexError:
            value = str(input(""))

        return value

    def isVariable(self, name):
        return name in self.varObj.variables
    
    def convertToLowercase(self, string):
        try:
            return string.casefold()
        except AttributeError:
            raise NSError("Uncaught SyntaxError: Object has no attribute 'lowercase'")
            return

    def convertToUppercase(self, string):
        try:
            return string.upper()
        except AttributeError:
            raise NSError("Uncaught SyntaxError: Object has no attribute 'uppercase'")
            return

    def convertToSwapcase(self, string):
        try:
            return string.swapcase()
        except AttributeError:
            raise NSError("Uncaught SyntaxError: Object has no attribute 'swapcase'")
            return

    def convertToCamelcase(self, string):
        try:
            return string.title()
        except AttributeError:
            raise NSError("Uncaught SyntaxError: Object has no attribute 'camelcase'")
            return
