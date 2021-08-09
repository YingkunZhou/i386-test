f = open('inslist.src')
lines = f.readlines()
f.close()
Toperand = set()
output = ''

for l in lines:
    w = l.split()
    if w and w[0] != ';':
        if l[:2] == '\\S':
            output += '\n## ' + ' '.join(w[1:]) + '\n\n'
        else:
            assert(w[0] == '\\c')
            if len(w) == 2:
                output += ' '.join(w[1:]) + '\n'
            else:
                t = w[-1].split(',')
                if ('ND' not in t) and ('PRIV' not in t) and ('NOLONG' not in t):
                    if len(w) > 3:
                        t = w[2].split(',')
                        for tt in t:
                            Toperand.add(tt)
                    output += ' '.join(w[1:-1]) + '\n'

f = open('inslist.md', 'w')
f.write(output)
f.close()
