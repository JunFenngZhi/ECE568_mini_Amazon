from django.urls import path
from . import views
from django.contrib.auth import views as auth_views


urlpatterns = [
    # system login and sign up
    path('', auth_views.LoginView.as_view(
        template_name='amazon/login.html'), name='login'),
    path('logout/', auth_views.LogoutView.as_view(template_name='amazon/logout.html'), name='logout'),
    path('register/', views.register, name='register'),

    # home page
    path('home/', views.home, name='home'),

    # orderStatus
    path('orderStatus/', views.orderStatus, name='orderStatus'),

    # edit profile
    path('editProfile/', views.editProfile, name='editProfile'),

    # shopping cart
    path('shoppingCart/', views.shoppingCart, name='shoppingCart'),

    # place Order
    path('placeOrder/', views.placeOrder, name='placeOrder'),

    # product cataLog
    path('cataLog/', views.cataLog, name='cataLog'),

    # product detail
    path('cataLog/detail/<int:productID>/<int:productPrice>/<str:productDescription>/',
         views.cataLogDetail, name='detail'),

]
