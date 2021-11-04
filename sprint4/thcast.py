#TODO: Create different classes for constructors, type constructors, and values, don't just use Variable

class ASTNode:
    def generate_code(self):
        raise NotImplemented

    def __repr__(self):
        return str(self)

    def dictify(self):
        raise NotImplemented

class DataDef(ASTNode):
    def __init__(self, tc, constrs):
        self.type_constr = tc
        self.constructors = constrs

    def __str__(self):
        return f'data {self.type_constr} = {" | ".join(map(str, self.constructors))}'

    def dictify(self):
        return {'node': 'data', 'constr': self.type_constr.dictify(), 
                'constructors': [x.dictify() for x in self.constructors]}

class Type(ASTNode):
    def __init__(self, constr, args):
        self.constr = constr
        self.args = args
        self.arity = None

    def __str__(self):
        if not self.args: return str(self.constr)
        if isinstance(self.args, list):
            args = ") (".join(map(str, self.args))
        else:
            args = self.args
        return f'{self.constr} ({args})'

    def dictify(self):
        if isinstance(self.args, list):
            args = [x.dictify() for x in self.args]
        else:
            args = self.args.dictify()
        return {'node': 'type', 'constructor': self.constr.dictify(), 'args': args}

    def __eq__(self, other):
        return isinstance(other, type(self)) \
           and self.constr == other.constr \
           and len(self.args) == len(other.args) \
           and all(x == y for x, y in zip(self.args, other.args))

class ConstrDef(ASTNode):
    def __init__(self, constr, args):
        self.constr = constr
        self.args = args

    def __str__(self):
        if not self.args: return str(self.constr)
        return f'{self.constr} ({") (".join(map(str, self.args))})'
    
    def dictify(self):
        return {'node': 'constrdef', 'constructor': self.constr.dictify(), 'args': [x.dictify() for x in self.args]}

class FuncDef(ASTNode):
    def __init__(self, sig, args, body):
        self.type = sig
        self.name = args[0]
        self.args = args[1:]
        self.body = body

    def __str__(self):
        return f'{self.type} ; {self.name} {" ".join(map(str, self.args))} = {self.body}'
    
    def dictify(self):
        return {'node': 'funcdef', 'type': self.type.dictify(), 'name': self.name.dictify(),
                'args': [x.dictify() for x in self.args], 'body': self.body.dictify()}

class FuncApp(ASTNode):
    def __init__(self, func, value):
        self.func = func
        self.value = value

    def __str__(self):
        return f'({self.func}) ({self.value})'

    def dictify(self):
        return {'node': 'funcapp', 'func': self.func.dictify(), 'arg': self.value.dictify()}

class CaseExpr(ASTNode):
    def __init__(self, expr, cases):
        self.expr = expr
        self.cases = cases

    def __str__(self):
        return f'case {self.expr} ' + '{' + ";".join(map(str, self.cases)) + '}'

    def dictify(self):
        return {'node': 'case', 'expr': self.expr.dictify(), 'cases': [x.dictify() for x in self.cases]}

class CaseStatement(ASTNode):
    def __init__(self, pattern, body):
        self.pattern = pattern
        self.body = body

    def __str__(self):
        return f'{self.pattern} -> {self.body}'

    def dictify(self):
        return {'node': 'case_statement', 'expr': self.pattern.dictify(), 'expr': self.body.dictify()}

class PatternMatch(ASTNode):
    def __init__(self, constr):
        self.constr = constr
        self.args = []

    def __str__(self):
        return f'{self.constr} {" ".join(map(str, self.args))}'
    
    def dictify(self):
        return {'node': 'match', 'constr': self.constr.dictify(), 'args': [x.dictify() for x in self.args]}

class Variable(ASTNode):
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return str(self.name)

    def dictify(self):
        return {'node': 'variable', 'name': self.name}

    def __eq__(self, other):
        return isinstance(other, type(self)) and self.name == other.name

class TypeConstrVar(ASTNode):
    def __init__(self, name, arity=None):
        if isinstance(name, Variable):
            self.name = name.name
        else:
            self.name = name

        self.arity = arity
    
    def __str__(self):
        return str(self.name)

    def dictify(self):
        return {'node': 'type_constr', 'name': self.name}
    
    def __eq__(self, other):
        return isinstance(other, type(self)) and self.name == other.name

class ConstrVar(ASTNode):
    def __init__(self, name):
        if isinstance(name, Variable):
            self.name = name.name
        else:
            self.name - name
    
    def __str__(self):
        return str(self.name)

    def dictify(self):
        return {'node': 'constructor', 'id': self.name}
    
    def __eq__(self, other):
        return isinstance(other, type(self)) and self.name == other.name

class Literal(ASTNode):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return str(self.value)

    def dictify(self):
        return self.value
    
    def __eq__(self, other):
        return isinstance(other, type(self)) and self.name == other.name
