from nltk import sent_tokenize

f = open('tmp.md')
lines = f.readlines()
f.close()

newline = True
output = ''
for l in lines:
    if l == '\n':
        newline = True
        output += l
    elif newline or (l[0] == '-' or l[0] == '*') and l[1] == ' ' or l[0].isnumeric() and l[1] == '.':
        newline = False
        output += l
    else:
        output = output[:-1] + ' ' + l

f = open('tmp1.md', 'w')
f.write(output)
f.close()

f = open('tmp1.md')
lines = f.readlines()
f.close()

text = ''
for l in lines:
    if l[0] == '#' or l[0] == '$' or l[0] == '!' or l[0].isnumeric() and l[1] == '.' or (l[0] == '-' or l[0] == '*') and l[1] == ' ':
        text += l
    else:
        if l != '\n':
            text += '\n'.join(sent_tokenize(l))
        text += '\n'
f = open('tmp1.md', 'w')
f.write(text)
f.close()

f = open('tmp1.md')
text = f.read()
math = False
output = ''
for c in text:
    if c == '$':
        if math:
            math = False
            output += '\">'
        else:
            math = True
            output += '<img src=\"https://render.githubusercontent.com/render/math?math='
    else:
        output += c

f = open('tmp1.md', 'w')
f.write(output)
f.close()
