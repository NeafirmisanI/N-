import varObject

class Parser(object):

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
                if token_value not in self.varObj.variables:
                    self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 0)
                else:
                    print("N# ERROR: Variable already referenced")
            elif token_type == "IDENTIFIER" and token_value in self.varObj.variables:
                self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)], 1)
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
                print("N# ERROR: Invalid printing value")
                quit()

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
                name = token_value
            elif token == 1 and token_type != "IDENTIFIER":
                print("N# ERROR: Invalid variable name " + token_value)
                quit()
            elif token == 2 and token_type != "OPERATOR":
                print("N# ERROR: Invalid or missing assignment operator")
                quit()
            elif token == 3 and token_type in ["STRING", "INTEGER", "IDENTIFIER"]:
                if token_stream == "IDENTIFIER" and token_value in self.varObj.variables:
                    print("N# ERROR: Variable not referenced")
                    quit()
                elif token_type == "IDENTIFIER":
                    value = self.varObj.get_variable(token_value)
                else:
                    value = token_value
            elif token == 3 and token_type not in ["STRING", "INTEGER"]:
                print("N# ERROR: Invalid variable assignment value " + token_type)
                quit()

            tokens_checked += 1
        
        self.varObj.set_variable(name, value)
        #print(self.varObj.variables)
        self.token_index += tokens_checked
