import socket


def sendOrder(orderInfo):
    # For test print
    print('This is test for orderInfo')
    print('orderInfo is: ' + orderInfo)
    print(orderInfo.encode())
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('vcm-26124.vm.duke.edu', 9999))
    # client.connect(('vcm-24026.vm.duke.edu', 9999)) #for junfeng testing
    # For test print
    print('connect to server success!')
    try:
        client.send(orderInfo.encode('utf-8'))
    except socket.error as e:
        print('Error sending data: %s' % e)

    print('after send')

    ACK_flag = client.recv(1024)
    if ACK_flag != 'ACK':
        return True
    else:
        return False
