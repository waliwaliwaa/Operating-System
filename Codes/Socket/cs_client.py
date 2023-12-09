import socket
import time

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client_socket.connect(('127.0.0.1', 12345))

print(client_socket.recv(1024).decode('utf-8'))
name = input('Plz input your loginname:')
bname = bytes(name, encoding = 'utf-8')
psd = input('Plz input your passward:')
bpsd = bytes(psd, encoding = 'utf-8')
client_socket.send(bname)
time.sleep(1)
client_socket.send(bpsd)
order1 = client_socket.recv(1024).decode('utf-8')
print(order1)
time.sleep(1)
if order1 =='accept':
    requests_ser_num = input("operation('1'表示请求文件传输服务, '2'表示请求数据库查询服务):")
    b_requests_ser_num = bytes(requests_ser_num, encoding = 'utf-8')
    client_socket.send(b_requests_ser_num)
    op = client_socket.recv(1024).decode('utf-8')
    print(op)
    order2 = str(client_socket.recv(1024).decode('utf-8'))
    print("Receive the server's order: %s ." % (order2))
    if str(order2) == "finsh":
        ms1 = bytes('byebye', encoding='utf-8')
        print('Send the order "byebye" to server.')
        client_socket.send(ms1)
        # client_socket.send(b'exit')
        client_socket.close()
    else:
        pass
else:
    client_socket.close()
client_socket.close()
