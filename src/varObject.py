class VariableObject(object):
    
    def __init__(self):
        self.variables = {}

    def set_variable(self, name, value):
        self.variables[name] = value
