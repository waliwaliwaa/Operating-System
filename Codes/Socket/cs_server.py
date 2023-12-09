import socket
import threading
import time
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('127.0.0.1', 12345))
server_socket.listen(5)
print("Waiting for connection...")
def tcplink(sock, addr):
    print('Accept new connection from %s:%s...' % addr)
    sock.send(b'welcome!')
    name = sock.recv(1024).decode('utf-8')
    sname = str(name)
    time.sleep(1)
    psd = sock.recv(1024).decode('utf-8')
    spsd = str(psd)
    print("The loginname is %s has login, and loginpasswd is %s." % (sname, spsd))
    if sname == "admin" and  spsd == "123456":
        msg1 = bytes('accept', encoding = 'utf-8')
        time.sleep(1)
        sock.send(msg1)
        requ_num = sock.recv(1024)
        print(requ_num)
        ms3 = bytes('finsh', encoding = 'utf-8')
        if str(requ_num) == "b'1'":
            print('The operqtion is 1.')
            file_op = bytes('Simulate the operation that file transfer!', encoding = 'utf-8')
            time.sleep(1)
            sock.send(file_op)
            sock.send(ms3)
            order1 = sock.recv(1024).decode('utf-8')
        elif str(requ_num) == "b'2'":
            print('The operqtion is 2.')
            sql_op = bytes('Simulate the operation that sql select!', encoding = 'utf-8')
            sock.send(sql_op)
            sock.send(ms3)
            order1 = sock.recv(1024)
        if str(order1) == "byebye":
            print("Server get byebye!stop conn.")
            sock.close()
        else:
            pass
    else:
        msg2 = bytes('Refuse, plz check your loginname and passwd!', encoding = 'utf-8')
        sock.send(msg2)
        sock.close()
    while True:
        time.sleep(1)
        break
    sock.close()
    print('Connection from %s:%s closed.' % addr)
    print("****over****\n")
n = 0#loginer
while True:
    sock, addr = server_socket.accept()
    # 建立新线程来处理TCP链接:
    new_t = threading.Thread(target = tcplink, args=(sock, addr))
    new_t.start()
    n = n + 1
    print("Here are %d loginer." % n)
