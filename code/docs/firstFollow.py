eps = '_epsilon_'

rules = []  # Raw grammar rules

with open("complete_grammar_modified.txt", "r") as fp:
    for line in fp:
        # Splitting on newline and turning it into an array
        data = line.strip().split('\n')
        datas = data[0].split()
        if data!=['']:
            rules.append(datas)


# grab terminals & non terminals:
terminals = []
nonterminals = []

for rule in rules:
    for token in rule:
        if token=="===>":
            continue
        elif token[0]=='<': # nonterminal
            if token not in nonterminals:
                nonterminals.append(token)
        else:
            if token not in terminals:
                terminals.append(token)


def merge(l1, l2):
    L = l1
    for item in l2:
        L.append(item)

    return L


def first(X, rules, terminals, nonterminals):

    # print "FINDING FIRST OF", X

    if X in terminals:
        return [X]
    else:
        
        locations = []
        i = 0

        FIRSTS = []

        for rule in rules:
            # print rule[0]
            if rule[0] == X:
                locations.append(i)
            i += 1

        # print locations

        for idx in locations:
            rule = rules[idx]
            
            # print "IN RULE ", rule

            # for each occurance in LHS
            for j in range(2,len(rule)):
                # traverse through each non terminal on RHS
                if rule[j] == X: # drop scanning through it (infinite recursion)
                    continue
                else:
                    subfirst = first(rule[j], rules, terminals, nonterminals)
                    if eps in subfirst:
                        FIRSTS = merge(FIRSTS, subfirst)
                        continue

                    else:
                        FIRSTS = merge(FIRSTS, subfirst)
                        break

        # print "Found first(", X, ") = ", FIRSTS
        # print
        FIRSTS = sorted(list(set(FIRSTS)))
        return FIRSTS


def printFirst(rules, terminals, nonterminals):
    for nt in sorted(nonterminals):
        
        print nt, ": {", 
        
        firsts = first(nt, rules, terminals, nonterminals)
        i = 0
        l = len(firsts)

        for f in firsts:
            if i==l-1:
                print f,
            else:
                print f, ",",
            i += 1

        print "}"


def follow(X, rules, terminals, nonterminals):

    i = -1
    j = 0

    locations = []

    # print X

    for rule in rules:
        i += 1

        if X in rule:
            
            for j in range(2, len(rule)):
                if X == rule[j]:
                    locations.append((i,j))
        else:
            continue

    # print locations

    FOLLOW = []

    for loc in locations:
        
        (I,J) = loc
        rule = rules[I]

        flagBroken = False
        parentFollow = []

        for nexts in range(J+1, len(rule)):
            token = rule[nexts]
            subfirst = first(token, rules, terminals, nonterminals)

            if eps in subfirst:
                FOLLOW = merge(FOLLOW, subfirst)
                continue
            else:
                FOLLOW = merge(FOLLOW, subfirst)
                flagBroken = True
                break

        # picking the follow of the parents that derived X

        if flagBroken!=True: # Epsilon was found till the last token in the rule

            if X!= rule[0]:
                #print 'par:', rule[0]
                #print 'x:', X
                #print
                parentFollow = follow(rule[0], rules, terminals, nonterminals)

            FOLLOW = merge(FOLLOW, parentFollow)
    

    FOLLOWset = set(FOLLOW)
    FOLLOW = sorted(list(FOLLOWset))

    if eps in FOLLOW:
        FOLLOW.remove(eps)

    return FOLLOW


def printFollow(rules, terminals, nonterminals):

    for nt in sorted(nonterminals):
        
        print nt, ": {", 
        
        follows = follow(nt, rules, terminals, nonterminals)
        i = 0
        l = len(follows)

        for f in follows:
            if i==l-1:
                print f,
            else:
                print f, ",",
            i += 1

        print "}"


def firstRHS(rule, rules, terminals, nonterminals):

    FIRST = []

    for symbol in rule[2:]:

        subfirst = first(symbol, rules, terminals, nonterminals)
        FIRST = merge(FIRST, subfirst)

        if eps not in subfirst:
            break

    return sorted(list(set(FIRST)))



'''
# WORKING PYTHON OUTPUT

print follow("CL", rules, terminals, nonterminals)

def print_rules(rules):
    for (i,r) in enumerate(rules):
        print str(i+1) + ".",
        for item in r:
            print item,
        print

print "GRAMMAR RULES"
print
print_rules(rules)
print
print "FIRST SETS"
print
printFirst(rules, terminals, nonterminals)
print
print "FOLLOW SETS"
print
printFollow(rules, terminals, nonterminals)
'''


# INTERFACING FOR C
ENUM_ID = [

# TERMINALS
"ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", 
"INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", 
"CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", 
"FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", 
"NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "_epsilon_", "ENUM_SEP_TERMINALS",

# CONTROL SYMBOLS
"EXIT", "ABRUPTEND", "ENUM_SEP_CONTROL", 

# NON TERMINALS
"mainFunction", "stmtsAndFunctionDefs", "other1", "stmtOrFunctionDef", "stmt", 
"functionDef", "parameter_list", "remainingList", "type", "declarationStmt", 
"var_list", "more_ids", "assignmentStmt_type1", "assignmentStmt_type2", 
"leftHandSide_singleVar", "leftHandSide_listVar", "rightHandSide_type1", 
"rightHandSide_type2", "sizeExpression", "ifStmt", "other2", "otherStmts", 
"ioStmt", "funCallStmt", "inputParameterList", "listVar", "arithmeticExpression", 
"other3", "arithmeticTerm", "other4", "factor", "operator_lowPrecedence", 
"operator_highPrecedence", "booleanExpression", "constrainedVars", "var", "matrix", 
"rows", "other5", "row", "other6", "remainingColElements", "matrixElement", 
"logicalOp", "relationalOp", "ENUM_SEP_NONTERMINALS"
]


def publishFirstRHS(firstFile, ENUM_ID, rules, terminals, nonterminals):

    print "FIRST - RULES"

    with open(firstFile, 'w') as file:

        file.write( str(len(rules)) + '\n' )
        
        for i,r in enumerate(rules):

            lhs = r[0][1:-1] # extract the term within the angular brackets
            firsts = firstRHS(r, rules, terminals, nonterminals)
            print i+1, r[0], "(", ENUM_ID.index(lhs), "):", firsts
            
            firstsStr = ''

            for f in firsts:
                firstsStr = firstsStr + ' ' +  str(ENUM_ID.index(f))

            file.write( 
                str(ENUM_ID.index(lhs)) + 
                ' ' + 
                str(len(firsts)) +
                firstsStr + '\n'
                )


def publishFirstNT(firstFile, ENUM_ID, rules, terminals, nonterminals):

    print
    print "FIRST - NON TERMINALS"

    with open(firstFile, 'w') as file:

        file.write( str(len(nonterminals)) + '\n' )

        for (i,nt) in enumerate(sorted(nonterminals)):
            
            firsts = first(nt, rules, terminals, nonterminals)
            print i+1, nt, "(", ENUM_ID.index(nt[1:-1]), "):", firsts

            firstsStr = ''

            for f in firsts:
                firstsStr = firstsStr + ' ' +  str(ENUM_ID.index(f))

            file.write( 
                str(ENUM_ID.index(nt[1:-1])) + 
                ' ' + 
                str(len(firsts)) +
                firstsStr + '\n'
                )


def publishFollowNT(followFile, ENUM_ID, rules, terminals, nonterminals):

    print
    print "FOLLOW - NON TERMINALS"

    with open(followFile, 'w') as file:

        file.write( str(len(nonterminals)) + '\n' )

        for (i,nt) in enumerate(sorted(nonterminals)):
            
            follows = follow(nt, rules, terminals, nonterminals)
            print i+1, nt, "(", ENUM_ID.index(nt[1:-1]), "):", follows

            followsStr = ''

            for f in follows:
                followsStr = followsStr + ' ' +  str(ENUM_ID.index(f))

            file.write( 
                str(ENUM_ID.index(nt[1:-1])) + 
                ' ' + 
                str(len(follows)) +
                followsStr + '\n'
                )



def convertGrammarToIntStream(grammarFile, rules, ENUM_ID, terminals, nonterminals):

    with open(grammarFile, 'w') as file:

        file.write( str(len(rules)) + ' ' + str(len(nonterminals)) + ' ' + str(len(terminals)) + '\n'  )

        for rule in rules:

            file.write( str(len(rule)-1) + ' ' )

            for term in rule:

                if term in nonterminals:
                    file.write(str(ENUM_ID.index(term[1:-1])) + ' ')
                elif term in terminals:
                    file.write(str(ENUM_ID.index(term)) + ' ')

            file.write('\n')


publishFirstRHS('firstRules.txt', ENUM_ID, rules, terminals, nonterminals)
publishFirstNT('firstNT.txt', ENUM_ID, rules, terminals, nonterminals)
publishFollowNT('followNT.txt', ENUM_ID, rules, terminals, nonterminals)
convertGrammarToIntStream('grammarFile.txt', rules, ENUM_ID, terminals, nonterminals)

# print follow('<var>', rules, terminals, nonterminals)