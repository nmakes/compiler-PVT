eps = '_epsilon_'

rules = []  # Raw grammar rules

with open("CFG.txt", "r") as fp:
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

# print "terminals"
# print terminals
# print
# print "nonterminals"
# print nonterminals
# print 
# print

def merge(l1, l2):
    L = l1
    for item in l2:
        L.append(item)

    return L

def first(X, rules, terminals, nonterminals):

    # print "FINDING FIRST OF", X

    if X in terminals:
        # print "Found first(", X, ") = ", [X]
        # print
        return [X]
    else:
        
        locations = []
        i = 0

        FIRSTS = []

        for rule in rules:
            if rule[0] == X:
                locations.append(i)
            i += 1

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
        return FIRSTS

for nt in sorted(nonterminals):
    
    print nt, ":", 
    
    firsts = first(nt, rules, terminals, nonterminals)
    i = 0
    l = len(firsts)

    for f in firsts:
        if i==l-1:
            print f,
        else:
            print f, ",",
        i += 1

    print


def follow(X, rules, terminals, nonterminals):

    i = -1
    j = 0

    for rule in rules:
        i += 1
        j = 0
        rhs = rule[2:]
        if X in rhs:
            pass