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

    ACK_str = client.recv(1024)
    ACK_packid = ACK_str.split(":")
    if ACK_packid == '':
        return -1
    else:
        return ACK_packid[1]
