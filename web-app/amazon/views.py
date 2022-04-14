from django.http import HttpResponse
from django.shortcuts import render
from amazon.utils import *


# Create your views here.


def index_view(request):

    if request.method == 'GET':
        # GET请求 返回页面
        return render(request, 'amazon/index.html')

    elif request.method == 'POST':
        ProductAddr = request.POST['Address']
        ProductAmt = request.POST['Amount']
        ProductID = request.POST['ID']
        ProductPrice = request.POST['Price']
        UPsId = request.POST['UPS_ID']
        sendOrder()
