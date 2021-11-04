import thcast

def get_variable(args, name):
    for n, arg in enumerate(args):
        if isinstance(arg, list) and name in arg:
            pos = arg.index(name)
            return n, pos
        elif isinstance(arg, str) and name == arg:
            return n;
    else:
        return None
def funcname(name):
    return "Func_" + escape(str(name))

def funcheader(name):
    return f"void {funcname(name)}(THCObject * this, THCList * args)"

def tofunc(name):
    return f"void {name}(THCObject * this, THCList * args)"

def contname(name, i):
    return "Cont" + str(i) + escape(str(name))

def contheader(name, i):
    return f"void {contname(name, i)}(THCObject * cont, THCList * args)"

def gencont(app, args, next_cont=None):
    if next_cont is None: next_cont = []
    if isinstance(app, thcast.Variable): 
        return Variable(next_cont, args, app)
        
    elif isinstance(app, thcast.Literal):
        return Literal(next_cont, args, app)

    elif isinstance(app, thcast.CaseExpr): 
        return Case(next_cont, args, app)

    elif isinstance(app, thcast.FuncApp):
        return Application(next_cont, args, app)

def escape(s):
    return s.replace(':', 'Colon')\
            .replace('+', 'Add')\
            .replace('$', 'Dollar')\
            .replace('\\', 'BSlash')\
            .replace('*', 'Star')\
            .replace('-', 'Dash')\
            .replace('.', 'Dot')\
            .replace('^', 'Carat')\
            .replace('|', 'Bar')\
            .replace('/', 'Slash')\
            .replace('!', 'Bang')

def make_cont_chain(conts):
    last_cont = 'cont'
    nextcontcode = ''
    for n, nc in enumerate(reversed(conts)):
        prev = 'cont' if n == 0 else f'&nc{n-1}'
        nextcontcode += f"""
        THCObject nc{n};
        constructContinuation(&nc{n}, {nc.name}_obj, {prev});
        setArgs(&nc{n}, args);
        """
        last_cont = f'&nc{n}'

    nextcontcode += f'THCObject * next_cont = {last_cont};'
    return nextcontcode

def cont_decl(name, num, obj):
    #if name == 'Func_len' and num == 1: 
    #    raise Exception
    #print(name, num)
    contname = f'{name}_C{num}'
    fdef = tofunc(contname)
    defs = f'THCObject * {contname}_obj;\n'
    init = f'{contname}_obj = constructFunction({contname}, {len(obj.scope)});\n'
    header = fdef + '{\n'
    header += f"if(!checkStackSize()) THC_minor_gc(this);\n"
    header += f"THCObject * cont = this->contents.f.cont;\n"
    
    obj.name = contname
    return defs, init, header

class Application:
    def __init__(self, next_conts, scope, s):
        self.func = gencont(s.func, scope, [self])
        self.arg = gencont(s.value, scope) 
        self.next_conts = next_conts

        # there are no function literals
        if isinstance(self.func, Variable):
            self.scope = scope
            self.prev = None
            self.arity =  len(scope)
        else:
            self.scope = [''] + scope
            self.prev = self.func
            self.arity = len(scope) + 1


    def gencode(self, symbol_table, enum_table, name, num):
        defs, init, header = cont_decl(name, num, self) 
        funcs = ''
        nextcontcode = make_cont_chain(self.next_conts)
        num += 1

        ld = lf = li = ''
        if self.prev is not None:
            nd, nf, ni, num, entry = self.prev.gencode(symbol_table, enum_table, name, num)
            defs += nd
            funcs += nf
            init += ni
            get_func = '''
            THCObject * func = args->head->contents.n.value;
            // get tail of args because htose are the ones we pass to the thing
            THCList new_args;
            new_args.head = args->head->contents.n.next;
            new_args.size = args->size-1;
            args = &new_args;
            '''
        else:
            get_func = self.func.do_assignment(symbol_table, 'THCObject * func')
            entry = self.name

        if isinstance(self.arg, Literal):
            call_arg = f"""
            THCObject lit;
            {self.arg.do_assignment(symbol_table, '&lit')}
            callCont(&funccont, &lit);
            """

        elif isinstance(self.arg, Variable):
            call_arg = f"""
            {self.arg.do_assignment(symbol_table, 'THCObject * arg')}
            callCont(&funccont, arg);
            """
        else:
            nd, nf, ni, num, nxt = self.arg.gencode(symbol_table, enum_table, name, num)
            defs += nd
            funcs += nf
            init += ni
            call_arg = f"""
            // reassign funccont
            THCObject argcont;
            constructContinuation(&argcont, {nxt}_obj, &funccont);
            setArgs(&argcont, args);
            callFunction(&argcont);
            """

        funcs += header + f"""
            {get_func}
            
            {nextcontcode}
            THCObject funccont;
            constructContinuation(&funccont, func, next_cont);
            {call_arg}
        """ + "}\n"

        return defs, funcs, init, num, entry
        

class Literal:
    def __init__(self, next_conts, scope, s):
        self.next_conts = next_conts
        self.prev = None
        self.scope = scope

        self.value = s.value
        self.arity = len(scope)

    def do_assignment(self, symbol_table, name):
        return f"constructInt({name}, {self.value});\n"

    def gencode(self, symbol_table, enum_table, name, num):
        defs, init, header = cont_decl(name, num, self) 
        nextcontcode = make_cont_chain(self.next_conts)
        # TODO: integer pooling maybe?
        func = header + f"""
            {nextcontcode}

            THCObject object;
            constructInt(&object, {self.value});
            callCont(next_cont, &object);""" + "}\n"
            
        return defs, func, init, num+1, self.name

class Variable:
    def __init__(self, next_conts, scope, s):
        self.next_conts = next_conts
        self.prev = None
        self.scope = scope

        self.name = s.name
        self.varname = s.name
        self.position = get_variable(scope, s.name)
        self.is_global = self.position is None

        self.arity = len(scope)
    
    def do_assignment(self, symbol_table, name, offset=0):
        if self.is_global:
            return f"{name} = {symbol_table[self.varname]}_obj;\n"
        elif isinstance(self.position, int):
            return f"{name} = indexList(args, {self.position+offset});\n"
        else:
            n = name.split()[-1]
            return f"THCObject * {n}p = indexList(args, {self.position[0]+offset});\n" + \
                   f"{name} = indexList(&({n}p->contents.l), {self.position[1]});\n"


    def gencode(self, symbol_table, enum_table, name, num):
        defs, init, header = cont_decl(name, num, self) 
        nextcontcode = make_cont_chain(self.next_conts)
        func =  header + f"""
            {nextcontcode}
            THCObject * v0;
            {self.do_assignment(symbol_table, 'v0')}
            callCont(next_cont, v0);
        """ + "}\n"
            
        return defs, func, init, num+1, self.name
        
 
class Case:
    def __init__(self, next_conts, scope, s):
        self.next_conts = next_conts
        self.expr = gencont(s.expr, scope, [self])
        
        self.scope = scope
        self.cases = []
        for case in s.cases:
            constr = case.pattern.constr
            new_args = [[x.name for x in reversed(case.pattern.args)]] + scope
            body = gencont(case.body, new_args, self.next_conts)
            self.cases.append((constr.name, body))

        if isinstance(self.expr, Variable) or isinstance(self.expr, Literal):
            self.scope = scope
            self.prev = None
            self.arity =  len(scope)
        else:
            self.scope = [''] + scope
            self.prev = self.expr
            self.arity = len(scope) + 1
        
    def gencode(self, symbol_table, enum_table, name, num):
        defs, init, header = cont_decl(name, num, self) 
        funcs = ''
        nextcontcode = make_cont_chain(self.next_conts)
        num += 1
        entry = self.name

        last = "callCont(&next, expr);"
        if isinstance(self.expr, Literal):
            get_expr = f"""
            THCObject exprobj;
            THCObject * expr = &exprobj;
            {self.expr.do_assignment(symbol_table, 'expr')}
            """
        elif isinstance(self.expr, Variable):
            get_expr = f"""
            {self.expr.do_assignment(symbol_table, 'THCObject * expr')}
            """
        else:
            nd, nf, ni, num, entry = self.prev.gencode(symbol_table, enum_table, name, num)
            defs += nd
            funcs += nf
            init += ni
            get_expr = 'THCObject * expr = args->head->contents.n.value;'
            last = "callFunction(&next);"
        
        cases = []
        for constr, body in self.cases:
            enum = enum_table[constr]
            
            nd, nf, ni, num, nxt = body.gencode(symbol_table, enum_table, name, num)
            defs += nd
            funcs += nf
            init += ni
            code = f"""
                case {enum}:
                    constructContinuation(&next, {nxt}_obj, cont);
                    break;\n"""
            cases.append(code)

        funcs += header + f"""
            {get_expr}
            evaluate(this, expr);
            THCObject next;
            switch(expr->constructor){{
                {''.join(cases)}\n
                default:
                    fprintf(stderr, "Non-exhaustive pattern matching in `{self.name.lstrip('Func_')}'.\\n");
                    printf("%d\\n", expr->constructor);
                    //raise segfault for asan stack trace
                    //int *x=NULL;
                    //printf("%d\\n", *x);
                    exit(-1);
            }}
            setArgs(&next, args);
            {last}
        """ + "}\n"

        return defs, funcs, init, num, entry

class DataDef:
    def __init__(self, s):
        self.constrs = [] # name arity pairs
        self.name = s.type_constr.constr.name
    
        for constr in s.constructors:
            # type erasure means types don't need to be stored
            constrname = constr.constr.name
            arity = len(constr.args);
            self.constrs.append((constrname, arity))
            
    def gencode(self, symbol_table, enum_table):
        enum = f"enum THCType{escape(self.name)}Constrs"+ "{\n"
    
        funcdefs = ""
        funcs = ""
        init = ""
    
        for name, arity in self.constrs:
            if name in enum_table:
                raise NameError(f"Duplicate constructors named {name}"
                                "This should have been caught during type check.")
    
            symbol_table[name] = f"Constr_{escape(name)}"
            enum_table[name] = f"E_TYPE_{escape(self.name)}_CONSTR_{escape(name)}"
            enum += f" {enum_table[name]},\n"
    
            funcdefs += f"THCObject * {symbol_table[name]}_obj;\n"
            
            if arity == 0:
                init += f"""
                {symbol_table[name]}_obj = malloc(sizeof(THCObject));
                constructAlgebraic({symbol_table[name]}_obj, {enum_table[name]});
                {symbol_table[name]}_obj->gcflag = HEAP;
                {symbol_table[name]}_obj->contents.l.head = NULL;
                {symbol_table[name]}_obj->contents.l.size = 0;\n
                """
                continue
    
            funcs += f"{tofunc(symbol_table[name])}" + "{" + f"""
                if(!checkStackSize()) THC_minor_gc(this);
                THCObject * cont = this->contents.f.cont;
                THCObject value;
                constructAlgebraic(&value, {enum_table[name]});
                value.contents.l.head = args->head;
                assert(args->size == {arity});
                value.contents.l.size = {arity};
                callCont(cont, &value);""" + "\n}\n"
    
            init += f"{symbol_table[name]}_obj = constructFunction({symbol_table[name]}, {arity});\n" 
    
    
        enum += "};\n"
        return enum, funcdefs, funcs, init
