from curses.ascii import HT
from django.http import HttpResponse
from django.shortcuts import render
from amazon.utils import *


# Create your views here.


def index_view(request):

    if request.method == 'GET':
        # GET request return index page
        return render(request, 'amazon/index.html')

    elif request.method == 'POST':
        ProductAddrX = request.POST['AddressX']
        ProductAddrY = request.POST['AddressY']
        ProductAmt = request.POST['Amount']
        ProductID = request.POST['ID']
        ProductPrice = request.POST['Price']
        UPsId = request.POST['UPS_ID']
        # concatenate order information
        OrderInfo = ProductAddrX + ':' + ProductAddrY + ':' + \
            ProductAmt + ':' + ProductID + ':' + ProductPrice + ':' + UPsId
        # For test orderInfo
        print('OrderInfo is: ' + OrderInfo)

        # Send the orderInfo to the server, and receive the server's response
        # if not receive, it will return false, else return true
        recvOrNot = sendOrder(OrderInfo)
        if recvOrNot == True:
            return HttpResponse('PlaceOrder Success')
        else:
            return HttpResponse('PlaceOrder Fail')
