import varObject
from error import NSError

class Interpreter(object):

    def __init__(self):
        self.transpiled_code = ""
        self.varObj = varObject.VariableObject()
    
    def parse(self, tokens):
        self.tokens = tokens
        self.token_index = 0
        while self.token_index < len(self.tokens):
            token_type = self.tokens[self.token_index][0]
            token_value = self.tokens[self.token_index][1]
            
            if token_type == "VAR_DECLARATION" and token_value == "var":
                advance_val = self.tokens[self.token_index + 1][1]
                if advance_val not in self.varObj.variables:
                    self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 0)
                else:
                    raise NSError("N# ERROR: Variable " + advance_val + " already referenced")
            elif token_type == "IDENTIFIER":
                if token_value in self.varObj.variables:
                    self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 1)
                else:
                    raise NSError("N# ERROR: Undefined variable " + token_value)
            elif token_type == "PRINT_STATEMENT" and token_value == "print":
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
            elif token == 1 and token_type == "INTEGER":
                toPrint = token_value
            elif token == 1 and token_type == "IDENTIFIER" and token_value in self.varObj.variables:
                toPrint = self.varObj.get_variable(token_value)
            elif token == 1 and token_type not in ["STRING", "INTEGER", "IDENTIFIER"]:
                raise NSError("N# ERROR: Invalid printing value")

            tokens_checked += 1
        
        print(toPrint)

        self.token_index += tokens_checked

    def parseVariableDeclaration(self, token_stream, index):
        tokens_checked = 0

        name = ""
        operator = ""
        value = ""

        for token in range(index, len(token_stream) + index):
            token_type = token_stream[tokens_checked][0]
            token_value = token_stream[tokens_checked][1]

            if token == 1 and token_type == "IDENTIFIER":
                if token_value not in ["pi", "euler", "os", "os-version", "argv", "cwd"]:
                    name = token_value
                else:
                    raise NSError("N# ERROR: Cannot change value of static variable " + token_value)
            elif token == 1 and token_type != "IDENTIFIER":
                raise NSError("N# ERROR: Invalid variable name " + token_value)
            elif token == 2 and token_type != "OPERATOR":
                try:
                    advance_val = token_stream[tokens_checked + 1][1]
                except IndexError:
                    advance_val = token_value
                if advance_val != token_value:
                    raise NSError("N# ERROR: Missing assignment operator")
                else:
                    value = ""
            elif token == 3 and token_type in ["STRING", "INTEGER", "IDENTIFIER"]:
                if token_stream == "IDENTIFIER" and token_value in self.varObj.variables:
                    raise NSError("N# ERROR: Variable not referenced")
                elif token_type == "IDENTIFIER":
                    value = self.varObj.get_variable(token_value)
                else:
                    value = token_value
            elif token == 3 and token_type not in ["STRING", "INTEGER"]:
                raise NSError("N# ERROR: Invalid variable assignment value " + token_type)

            tokens_checked += 1
        
        self.varObj.set_variable(name, value)
        #print(self.varObj.variables)
        self.token_index += tokens_checked
