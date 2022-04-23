from curses.ascii import HT
from multiprocessing import context
import profile
from django.http import HttpResponse
from django.shortcuts import render, redirect
from amazon.utils import *
from django.contrib import messages
from .forms import UserRegisterForm, UpdateProfileForm
from django.contrib.auth.decorators import login_required
from .models import UserProfile


def register(request):
    if request.method == 'POST':
        form = UserRegisterForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            messages.success(
                request, f'You account have already created. You can login now!')
            return redirect('login')
    else:
        form = UserRegisterForm()
    return render(request, 'amazon/register.html', {'form': form})


@login_required
def home(request):
    # create profile in database for every new user
    list = UserProfile.objects.filter(userName=request.user.username)
    if list.exists() is not True:
        profile = UserProfile.objects.create()
        profile.userName = request.user.username
        profile.save()

    return render(request, 'amazon/home.html')


@login_required
def orderStatus(request):
    # TODO
    return render(request, 'amazon/orderStatus.html')


@login_required
def editProfile(request):
    profile = UserProfile.objects.filter(
        userName=request.user.username).first()
    if request.method == "POST":
        profile_form = UpdateProfileForm(request.POST, instance=profile)
        if profile_form.is_valid():  # 获取数据
            getAddrX = profile_form.cleaned_data['addrX']
            getAddrY = profile_form.cleaned_data['addrY']
            getUpsID = profile_form.cleaned_data['upsID']
            # create the ride
            profile = UserProfile.objects.filter(
                userName=request.user.username).first()
            profile.addrX = getAddrX
            profile.addrY = getAddrY
            profile.upsID = getUpsID
            profile.save()
            return redirect('home')  # 自动跳转回上一层
    else:  # GET
        profile_form = UpdateProfileForm(instance=profile)
        context = {
            'addrX': profile.addrX,
            'addrY': profile.addrY,
            'upsID': profile.upsID,
            'profile_form': profile_form
        }
        return render(request, 'amazon/editProfile.html', context)


@login_required
def shoppingCart(request):
    return render(request, 'amazon/home.html')


@login_required
def cataLog(request):
    if request.method == 'GET':
        return render(request, 'amazon/cataLog.html')


@login_required
def cataLogDetail(request, productID, productPrice, productDescription):
    context = {
        'productID': productID,
        'productPrice': productPrice,
        'productDescription': productDescription
    }
    if request.method == 'GET':
        return render(request, 'amazon/cataLogDetail.html', context)
    elif request.method == 'POST':
        ProductAddrX = request.POST['AddressX']
        ProductAddrY = request.POST['AddressY']
        ProductAmt = request.POST['Amount']
        ProductID = productID
        ProductPrice = productPrice
        ProductDescription = productDescription
        UPsId = request.POST['UPS_ID']
        # concatenate order information
        OrderInfo = ProductAddrX + ':' + ProductAddrY + ':' + \
            str(ProductAmt) + ':' + str(ProductID) + ':' + str(ProductPrice) + \
            ':' + ProductDescription + ':' + UPsId
        # For test orderInfo
        print('OrderInfo is: ' + OrderInfo)
        # Send the orderInfo to the server, and receive the server's response
        # if not receive, it will return false, else return true
        packid = sendOrder(OrderInfo)
        if packid == -1:
            return HttpResponse('PlaceOrder Fail')
        else:
            # return HttpResponse('The Order Packid is:' + packid)
            return render(request, 'amazon/placeOrderSuccess.html', {'packid': packid})


@login_required
def placeOrder(request):

    if request.method == 'GET':
        # GET request return index page
        return render(request, 'amazon/placeOrder.html')

    elif request.method == 'POST':
        ProductAddrX = request.POST['AddressX']
        ProductAddrY = request.POST['AddressY']
        ProductAmt = request.POST['Amount']
        ProductID = request.POST['ID']
        ProductPrice = request.POST['Price']
        ProductDescription = request.POST['Description']
        UPsId = request.POST['UPS_ID']

        # concatenate order information
        OrderInfo = ProductAddrX + ':' + ProductAddrY + ':' + \
            ProductAmt + ':' + ProductID + ':' + ProductPrice + \
            ':' + ProductDescription + ':' + UPsId
        # For test orderInfo
        print('OrderInfo is: ' + OrderInfo)

        # Send the orderInfo to the server, and receive the server's response
        # if not receive, it will return false, else return true
        packid = sendOrder(OrderInfo)
        if packid == -1:
            return HttpResponse('PlaceOrder Fail')
        else:
            return HttpResponse('The Order Packid is:' + packid)
