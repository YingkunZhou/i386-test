PR['registerLangHandler'](
  PR['createSimpleLexer'](
   [
     [PR['PR_COMMENT'], /^;[^\r\n]*/, null, ';'],
     [PR['PR_PLAIN'], /^[\t\n\r \xA0]+/, null, '\t\n\r \xA0'],
   ],
   [
     [PR['PR_KEYWORD'], /^\b(?:[e|r]?(ax|bx|cx|dx|si|di|sp|bp)|[a-d](h|l)|(r|xmm|ymm)([0-9]|1[0-5])|st[0-7]?)\b/, null],
    ]),
  ['asm']);
