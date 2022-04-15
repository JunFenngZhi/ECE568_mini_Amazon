from gettext import Catalog
from django.db import models
from django.utils.timezone import now

# Create your models here.


class Item(models.Model):
    itemId = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=100)
    description = models.CharField(
        max_length=100, blank=False, null=False, unique=True)
    price = models.FloatField(max_length=1000, blank=False, null=False)
    catalog = models.CharField(max_length=100)
    seller = models.CharField(max_length=100)
    version = models.IntegerField(default=1)  # need default value.


class Order(models.Model):
    packId = models.IntegerField(primary_key=True)
    addrX = models.IntegerField()
    addrY = models.IntegerField()
    time = models.DateTimeField(default=now)
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
    upsId = models.IntegerField()
    item = models.ForeignKey(Item, on_delete=models.SET_NULL, null=True)
    totalPrice = models.FloatField(max_length=1000)
    version = models.IntegerField(default=1)  # need default value.


class Inventory(models.Model):
    itemID = models.IntegerField()
    itemName = models.CharField(max_length=100, primary_key=True)
    itemAmount = models.IntegerField()
    whID = models.IntegerField()
    version = models.IntegerField(default=1)  # need default value.
