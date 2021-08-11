f = open('inslist.src')
lines = f.readlines()
f.close()
Toperand = set()
output = ''
marks = {}
label_insn = ['LO', 'CA']
cnt = 0

for l in lines:
    w = l.split()
    if w :
        if w[0] == ';':
            if w[1] == '#':
                continue
            key_value = l[2:].strip().split(' = ')
            print(key_value)
            value = key_value[1]
            if value[0] == '(':
                value = value[1:-1].split(',')
            marks[key_value[0]] = value
        elif l[:2] == '\\S':
            continue
            output += '\n## ' + ' '.join(w[1:]) + '\n\n'
        else:
            assert(w[0] == '\\c')
            if len(w) == 2:
                output += ' '.join(w[1:]) + '\n'
            else:
                t = w[-1].split(',')
                if 'ND' not in t and 'PRIV' not in t and 'NOLONG' not in t \
                        and 'OBSOLETE' not in t and 'NEVER' not in t:
                    insn = w[1]
                    oprd = w[2]
                    t = oprd.split(',')
                    valid = False
                    for tt in t:
                        if tt in marks:
                            valid = True
                            break
                    w[1] = insn.lower()
                    if valid:
                        oprd = ''
                        for tt in t:
                            Toperand.add(tt)
                            value = marks[tt]
                            if type(value) is list:
                                for v in value:
                                    if v not in oprd:
                                        oprd += (v + ',')
                                        break
                            elif (insn[0:2] in label_insn or insn[0] == 'J' or insn == 'XBEGIN') and 'imm' in tt and '|' not in tt:
                                label = ' label' + str(cnt)
                                cnt += 1
                                output += (label + ':\n')
                                oprd += (' '.join(value.split()[:-1]) + label + ',')
                            else:
                                oprd += (value + ',')
                        w[2] = oprd[:-1]

                        if 'cc' == insn[-2:]:
                            insn = insn[:-2]
                            for i in marks['cc']:
                                w[1] = (insn + i).lower()
                                output += ' '.join(w[1:3]) + '\n'
                            continue
                        output += ' '.join(w[1:3]) + '\n'
                    else:
                        output += w[1] + '\n'

f = open('inslist.md', 'w')
f.write(output)
f.close()
