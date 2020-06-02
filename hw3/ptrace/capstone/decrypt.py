from pwn import *
import binascii as b

context.arch = 'x86_64'

ip = "aup.zoolab.org"
port = 2530
r = remote(ip,port)

for i in range(10):
    r.recvuntil('>>> ')
    case = r.recvuntil('Y')[0:-2]
    print(case)

    quiz = b.a2b_hex(case)
    res = disasm(quiz)
    print(res)
    lists = res.split('\n')
    print(lists)

    # print(lists[0].split('    '))

    result = ''
    for list in lists:
        instructions = list.split()
        # print(instructions)   
        if len(instructions) == 3:
            tmp = instructions[-1] + '\n'
        elif instructions[-2][-1] == ',':
            tmp = instructions[-3] + ' ' + instructions[-2] + ' ' + instructions[-1] + '\n'
        else:
            tmp = instructions[-2] + ' ' + instructions[-1] + '\n' 
        print(tmp)
        # print('\n')
        # result.append(tmp)
        result+=tmp
        
    print(result)
    payload = b.b2a_hex(result.encode('utf-8')).decode('utf-8')
    print(payload)
    r.sendline(payload)
r.interactive()