import webbrowser
import varObject
from error import NSError

class Interpreter:

    def __init__(self):
        self.varObj = varObject.VariableObject()
        self.tokens = []
    
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
            elif token_type == "IDENTIFIER" and token_value not in ["print", "var", "browse"]:
                if token_value in self.varObj.variables:
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
        webbrowser.open_new_tab(token_value)

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
            elif token == 1 and token_type == "IDENTIFIER" and token_value in self.varObj.variables:
                toPrint = self.varObj.get_variable(token_value)
            elif token == 2 and token_type == "IDENTIFIER" and token_value in ["uppercase", "lowercase", "camelcae", "swapcase"]:
                if token_value == "lowercase":
                    try:
                        toPrint = toPrint.casefold()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'lowercase'")
                if token_value == "uppercase":
                    try:
                        toPrint = toPrint.upper()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'uppercase'")
                if token_value == "camelcase":
                    try:
                        toPrint = toPrint.capitalize()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'camelase'")
                if token_value == "swapcase":
                    try:
                        toPrint = toPrint.swapcase()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'swapcase'")
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
                if token_value not in ["pi", "euler", "os", "os-version", "argv", "cwd", "tau"]:
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
            elif token == 4 and token_type == "IDENTIFIER" and token_value in ["uppercase", "lowercase", "camelcae", "swapcase"]:
                if token_value == "lowercase":
                    try:
                        value = value.casefold()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'lowercase'")
                if token_value == "uppercase":
                    try:
                        value = value.upper()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'uppercase'")
                if token_value == "camelcase":
                    try:
                        value = value.capitalize()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'camelase'")
                if token_value == "swapcase":
                    try:
                        value = value.swapcase()
                    except AttributeError:
                        raise NSError("Uncaught SyntaxError: Number object has no attribute 'swapcase'")

            tokens_checked += 1
        
        self.varObj.set_variable(name, value)
        #print(self.varObj.variables)
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
