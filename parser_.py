from Objects.varObject import VariableObject

class Parser(object):

    def __init__(self, tokens):
        self.tokens = tokens
        self.token_index = 0
        self.transpiled_code = ""
        self.varObj = VariableObject()
    
    def parse(self):
        while self.token_index < len(self.tokens):
            token_type = self.tokens[self.token_index][0]
            token_value = self.tokens[self.token_index][1]
            
            if token_type == "VAR_DECLARATION" and token_value == "var":
                self.parseVariableDeclaration(self.tokens[self.token_index:len(self.tokens)])

            self.token_index += 1

        print(self.varObj.variables)

    def parseVariableDeclaration(self, token_stream):
        tokens_checked = 0

        name = ""
        operator = ""
        value = ""

        for token in range(0, len(token_stream)):
            token_type = token_stream[tokens_checked][0]
            token_value = token_stream[tokens_checked][1]

            if token == 1 and token_type == "IDENTIFIER":
                name = token_value
            elif token == 1 and token_type != "IDENTIFIER":
                print("N# ERROR: Invalid variable name" + token_value)
                quit()
            elif token == 2 and token_type == "OPERATOR":
                operator = token_value
            elif token == 2 and token_type != "OPERATOR":
                print("N# ERROR: Invalid or missing assignment operator")
                quit()
            elif token == 3 and token_type in ["STRING", "INTEGER", "IDENTIFIER"]:
                value = token_value
            elif token == 3 and token_type not in ["STRING", "INTEGER", "IDENTIFIER"]:
                print("N# ERROR: Invalid variable assignment value " + token_type)
                quit()

            tokens_checked += 1
        
        self.varObj.set_variable(name, operator, value)

        self.token_index += tokens_checked