from gettext import Catalog
from django.db import models
from django.utils.timezone import now

# Create your models here.


class Item(models.Model):
    package_id = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=100)
    description = models.CharField(
        max_length=100, blank=False, null=False, unique=True)
    price = models.FloatField(max_length=1000, blank=False, null=False)
    catalog = models.CharField(max_length=100)
    seller = models.CharField(max_length=100)
    version = models.IntegerField()  # need default value. 


class Order(models.Model):
    package_id = models.IntegerField(primary_key=True)
    address_x = models.IntegerField()
    address_y = models.IntegerField()
    creation_time = models.DateTimeField(default=now)
    STATUS_CHOICES = [
        ('PACKING', 'packing'),
        ('PACKED', 'packed'),
        ('LOADING', 'loading'),
        ('LOADED', 'loaded'),
        ('DELIVERING', 'delivering'),
        ('DELIVERED', 'delivered'),
    ]
    status = models.CharField(
        max_length=50, choices=STATUS_CHOICES, default='packing')
    amount = models.IntegerField()
    ups_id = models.IntegerField()
    item = models.ForeignKey(Item, on_delete=models.SET_NULL, null=True)  
    total_price = models.FloatField(max_length=1000)
    version = models.IntegerField() # need default value.


class Inventory(models.Model):
    item_name = models.CharField(max_length=100, primary_key=True)
    item_amount = models.IntegerField()
    warehouse_id = models.IntegerField()
    version = models.IntegerField() # need default value.


