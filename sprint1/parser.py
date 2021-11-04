#!/usr/bin/env python3

import sys

from ply import yacc
from lexer import Lexer, tokens
from thcast import *

import yaml
class Parser:
    def p_program(self, p):
        '''
        program : declaration_list
        '''
        p[0] = p[1]

    def p_list_builder_singleton(self, p):
        '''
        declaration_list : declaration SEMICOLON
        identifier_list : identifier
        lower_identifier_list : ID_LOWER
        typesum : constrdef
        type_list : type_atom
        case_list : case_statement SEMICOLON
        '''
        p[0] = [p[1]]

    def p_list_builder_recurse(self, p):
        '''
        declaration_list : declaration SEMICOLON declaration_list
        identifier_list : identifier identifier_list
        lower_identifier_list : ID_LOWER identifier_list
        typesum : constrdef BAR typesum
        type_list : type_atom type_list
        case_list : case_statement SEMICOLON case_list
        '''
        p[0] = p[len(p)-1]
        p[0].append(p[1])
    
    def p_declaration(self, p):
        '''
        declaration : datadef 
                    | funcdef
        '''
        p[0] = p[1]

    def p_function_type(self, p):
        '''
        function_type : type_constr_app ARROW function_type
        '''
        p[0] = Type(TypeConstrVar("->"), [p[1], p[3]])

    def p_type_constr_app(self, p):
        '''
        type_constr_app : type_constr_app type_atom
        '''
        if not isinstance(p[1], Type):
            p[1] = Type(p[1], [])
        if not isinstance(p[2], Type):
            p[2] = Type(p[2], [])

        p[0] = p[1]
        p[0].args.append(p[2])
       
    def p_type_atom(self, p):
        '''
        type_atom : identifier
        '''
        p[0] = TypeConstrVar(p[1])

    def p_type_id(self, p):
        '''
        function_type : type_constr_app
        type_constr_app : type_atom
        
        type : function_type
        type_atom : LPAREN type RPAREN
        '''
        if len(p) > 3:
            p[0] = p[2]
        else:
            p[0] = p[1]

    def p_datadef(self, p):
        '''
        datadef : DATA typename EQUAL typesum
        '''
        p[0] = DataDef(p[2], p[4])

    def p_typename(self, p):
        '''
        typename : ID_UPPER
                | ID_UPPER lower_identifier_list
        '''
        if len(p) < 3: args = []
        else: args = p[2]
        p[0] = Type(TypeConstrVar(p[1]), args)
 
    def p_constrdef(self, p):
        '''
        constrdef : ID_UPPER
                  | ID_UPPER type_list
        '''
        if len(p) < 3: args = []
        else: args = p[2]
        p[0] = ConstrDef(ConstrVar(p[1]), args)

    
    def p_funcdef(self, p):
        '''
        funcdef : functype SEMICOLON funcbody
        '''
        p[0] = FuncDef(p[1], p[3][0], p[3][1])

    def p_functype(self, p):
        '''
        functype : prefix_name DOUBLE_COLON type
        '''
        #TODO: give name as well
        p[0] = p[3]

    def p_funcbody(self, p):
        '''
        funcbody : func_sig EQUAL expr
        '''
        p[0] = p[1], p[3]

    def p_func_sig(self, p):
        '''
        func_sig : prefix_def
                 | infix_def
        '''
        p[0] = p[1]

    def p_prefix_def(self, p):
        '''
        prefix_def : prefix_name
                   | prefix_def ID_LOWER
        '''
        if len(p) == 2: p[0] = [p[1]]
        else: 
            p[0] = p[1]
            p[0].append(p[2])

    def p_infix_def(self, p):
        '''
        infix_def : ID_LOWER infix_name ID_LOWER
        '''
        p[0] = [p[2], p[1], p[3]]

    def p_infprename(self, p):
        '''
        prefix_name : ID_LOWER
                    | LPAREN infix_operator RPAREN

        infix_name : infix_operator
        '''
        if len(p) > 3: p[0] = p[2]
        else: p[0] = p[1]

    def p_expr(self, p):
        '''
        expr : infix_operation
             | case
        infix_operation : precedence_9
        precedence_neg1 : func_call
        '''
        p[0] = p[1]

    def p_func_call(self, p):
        '''
        func_call : atom
                  | func_call atom
        '''
        if len(p) == 2: p[0] = p[1]
        else: p[0] = FuncApp(p[1], p[2])


    def p_case(self, p):
        """
        case : CASE expr OF LBRACE case_list RBRACE
        """
        p[0] = CaseExpr(p[2], p[5])

    def p_case_statement(self, p):
        """
        case_statement : pattern_match ARROW expr
        """
        p[0] = CaseStatement(p[1], p[3])

    def p_pattern_match(self, p):
        '''
        pattern_match : ID_UPPER
                      | pattern_match ID_LOWER
        '''
        if len(p) == 2: 
            p[0] = PatternMatch(ConstrVar(p[1]))
        else: 
            p[0] = p[1]
            p[0].args.append(p[2])


    def p_atom(self, p):
        '''
        atom : literal
             | identifier
             | LPAREN expr RPAREN
        '''
        if len(p) > 3: p[0] = p[2]
        else: p[0] = p[1]
    
    def p_identifier(self, p):
        '''
        identifier : ID_UPPER 
                  | ID_LOWER
        '''
        p[0] = p[1]

    def p_literal(self, p):
        '''
        literal : STRING
                | NUMBER
                | CHAR
        '''
        p[0] = p[1]


    # Error handling rule
    def p_error(self, p):
        print(p)
        print("Syntax error in input!")

    def build_infixes(self):
        def p_infix_operator(p):
            'infix_operator : '
            p[0] = p[1]

        def p_infix_apply(p):
            """"""
            p[0] = FuncApp(FuncApp(p[2], p[1]), p[3])

        def p_infix_group(p):
            """"""
            p[0] = p[1]

        infix_classes = []

        for i in range(10):
            p = i-1 if i > 0 else 'neg1'
            for a in 'LIR':
                infix_classes.append(f'INFIX_{i}_{a}')

            p_infix_group.__doc__ += \
                    f"precedence_{i} : precedence_{i}_l\n"\
                    f"               | precedence_{i}_i\n"\
                    f"               | precedence_{i}_r\n"\
                    f"               | precedence_{p}\n"
            
            p_infix_apply.__doc__ += \
                    f"precedence_{i}_l : precedence_{p} INFIX_{i}_L precedence_{p}\n"\
                    f"                 | precedence_{i}_l INFIX_{i}_L precedence_{p}\n"\
                    f"precedence_{i}_i : precedence_{p} INFIX_{i}_I precedence_{p}\n"\
                    f"precedence_{i}_r : precedence_{p} INFIX_{i}_R precedence_{p}\n"\
                    f"                 | precedence_{p} INFIX_{i}_R precedence_{i}_r\n"

        p_infix_operator.__doc__ += '\n    | '.join(infix_classes)

        self.p_infix_operator = p_infix_operator
        self.p_infix_apply = p_infix_apply
        self.p_infix_group = p_infix_group



    # Build the parser
    def build(self, **kwargs):
        self.build_infixes()
        self.tokens = tokens
        self.lexer = Lexer()
        self.lexer.build()
        self.parser = yacc.yacc(module=self, **kwargs)

    # Show the prompt for user input
    def parse(self, text):
        lines = [statement.dictify() for statement in self.parser.parse(text)]
        print(yaml.dump(lines[::-1], sys.stdout))

        #print('\n'.join(map(str, self.parser.parse(text))))
        

if __name__ == "__main__":
    m = Parser()
    m.build()
    m.parse(open(sys.argv[1]).read())
