import argparse
from ply import lex
from thcast import Variable, Literal

# List of token names. This is always required
tokens = [
    # Keywords
    'DATA',
    'CASE',
    'OF',
   
    #Symbols
    'LBRACE',
    'RBRACE',
    'SEMICOLON',
    'LPAREN',
    'RPAREN',
    'EQUAL',
    'BAR',
    'DOUBLE_COLON', # double colon
    'ARROW', 
    
    # IDs and literals
    'ID_UPPER',
    'ID_LOWER',
    'NUMBER',

    # Not used by parser rn, but may be useful in future
    'STRING',
    'CHAR',
]

# reserved keywords and symbols
# because PLY is fucking stupid and matches longest regexes first
# so i have to manually set these classes
reserved = {
    'case' : 'CASE',
    'of' : 'OF',
    'data' : 'DATA',

    '{': 'LBRACE',
    '}': 'RBRACE',
    ';': 'SEMICOLON',
    '(': 'LPAREN',
    ')': 'RPAREN',
    '=': 'EQUAL',
    '|': 'BAR',
    '::': 'DOUBLE_COLON',
    '->': 'ARROW', 
}

operators = {}

for i in range(10):
    for a in 'LIR':
        tokens.append(f'INFIX_{i}_{a}')

def escape_infix(s):
    for c in '\\$*+-?.^|/':
        s = s.replace(c, '\\'+c)

    return s

def add_operator(prec, ass, operator):
    operators[operator] = prec, ass.upper()

add_operator(6, 'l', '+')
add_operator(6, 'l', '-')
add_operator(7, 'l', '*')
add_operator(7, 'l', '/')

class Lexer():

    t_ignore = ' \t'

    # Keywords
    t_DATA = r'data'
    t_CASE = r'case'
    t_OF = r'of'
    
    # Reserved symbols. INFIX_9_L doesn't match all of them
    # so we still need these rules
    t_ARROW = r'->'
    t_DOUBLE_COLON = r'::'
    t_BAR = r'\|'
    t_EQUAL = r'='
    t_LPAREN = r'\('
    t_RPAREN = r'\)'
    t_SEMICOLON = r';'
    t_LBRACE = r'\{'
    t_RBRACE = r'\}'

    # Identifiers
    def t_ID_UPPER(self, t):
        r'[A-Z][a-zA-Z0-9_]*'
        t.value = Variable(t.value)
        return t

    def t_ID_LOWER(self, t):
        r'[a-z_][a-zA-Z0-9_]*'
        if t.value in reserved:
            t.type = reserved.get(t.value, 'ID_LOWER')
        else:
            t.value = Variable(t.value)
        return t

    # Literals
    def t_NUMBER(self, t):
        r'[0-9]\d*'
        t.value = Literal(int(t.value))
        return t

    def t_STRING(self, t):
        r'\"([^\\\n]|(\\.))*?\"'
        t.value = Literal(t.value[1:-1].encode('utf8').decode('unicode_escape'))
        return t

    def t_CHAR(self, t):
        r'\'([^\\\n]|(\\.))\''
        t.value = Literal(t.value[1:-1].encode('utf8').decode('unicode_escape'))
        return t
    
    @lex.TOKEN(r'--.*')
    def t_COMMENT(self, t):
        pass
    
    # Default infix rule

    # Define a rule so we can track line numbers
    def t_newline(self, t):
        r'\n+'
        t.lexer.lineno += len(t.value)

    # Error handling rule.
    def t_error(self, t):
        print("Illegal character '%s'" % t.value[0])
        t.lexer.skip(1)
    
    def t_INFIX_9_L(self, t):
        r'[\\\$\*\+\-\?\.\^\|\/!#%&<>=@~:]+'
        if t.value in reserved:
            t.type = reserved[t.value]
        elif t.value in operators:
            prec, ass = operators[t.value]
            t.type = f'INFIX_{prec}_{ass}'
        
        t.value = Variable(t.value)
        return t


    # Build the lexer.
    def build(self, **kwargs):
        self.tokens = tokens
        self.lexer = lex.lex(module=self, **kwargs)

    def test(self, data):
        self.lexer.input(data)
        while True:
            tok = self.lexer.token()
            if not tok:
                break
            print(tok)

# Main function
if __name__=="__main__":

    parser = argparse.ArgumentParser(description='THC Lexer')
    parser.add_argument('FILE', help="Input file with miniJava source code")
    args = parser.parse_args()

    f = open(args.FILE, 'r')
    data = f.read()
    f.close()

    m = Lexer()
    m.build()
    m.test(data)
