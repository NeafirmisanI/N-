import platform
import sys
import os

class VariableObject(object):
    
    def __init__(self):
        self.variables = {"pi": 3.1415926535897, "euler": 2.718281828459, "os": platform.system(), "os-version": platform.release(), "argv": sys.argv, "cwd": os.getcwd(), "tau": 6.28318530718}

    def set_variable(self, name, value):
        self.variables[name] = value

    def get_variable(self, name):
        return self.variables[name]
