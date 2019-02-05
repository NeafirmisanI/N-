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
            elif token_type == "IDENTIFIER":
                if token_value in self.varObj.variables:
                    self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 1)
                else:
                    raise NSError("Uncaught ReferenceError: Undefined variable " + token_value)
            elif token_type == "IDENTIFIER" and token_value == "print":
                self.parsePrintStatement(self.tokens[self.token_index:len(self.tokens)])
            self.token_index += 1

    def parsePrintStatement(self, token_stream):
        tokens_checked = 0

        toPrint = ""

        for token in range(0, len(token_stream)):
            token_type = token_stream[tokens_checked][0]
            token_value = token_stream[tokens_checked][1]

            if token == 1 and token_type == "STRING":
                toPrint = token_value[1:-1]
            elif token == 1 and token_type == "NUMBER":
                toPrint = token_value
            elif token == 1 and token_type == "IDENTIFIER" and token_value in self.varObj.variables:
                toPrint = self.varObj.get_variable(token_value)
            elif token == 1 and token_type not in ["STRING", "NUMBER", "IDENTIFIER"]:
                raise NSError("Uncaught ReferenceError: " + token_value + " is not defined")

            tokens_checked += 1
        
        print(toPrint)

        self.token_index += tokens_checked

    def parseVariableDeclaration(self, token_stream, index):
        tokens_checked = 0

        name = ""
        operator = ""
        value = ""

        for token in range(index, len(token_stream) + index):
            token = self.tokens[self.token_index]
            token_type = token["type"]
            token_value = token["value"]
            print(token_type) #TODO: Fix error
            print(token_value)

            if token == 1 and token_type == "IDENTIFIER":
                if token_value not in ["pi", "euler", "os", "os-version", "argv", "cwd"]:
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
                    advance_val = token_stream[tokens_checked + 1][1]
                except IndexError:
                    advance_val = token_value
                if advance_val != token_value:
                    raise NSError("Uncaught SyntaxError: Missing assignment operator")
                else:
                    value = ""
            elif token == 3 and token_type in ["STRING", "NUMBER", "IDENTIFIER"]:
                if token_stream == "IDENTIFIER" and token_value in self.varObj.variables:
                    raise NSError("Uncaught ReferenceError: " + token_value + " is not defined")
                elif token_type == "IDENTIFIER":
                    value = self.varObj.get_variable(token_value)
                else:
                    value = token_value
            elif token == 3 and token_type not in ["STRING", "NUMBER"]:
                raise NSError("Uncaught SyntaxError: Invalid variable assignment value " + token_type)

            tokens_checked += 1
        
        self.varObj.set_variable(name, value)
        print(self.varObj.variables)
        self.token_index += tokens_checked
