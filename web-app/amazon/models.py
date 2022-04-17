from enum import unique
from gettext import Catalog
from django.db import models
from django.utils.timezone import now

# Create your models here.

# 将itemId和seller做成复合主键？


class Item(models.Model):
    itemId = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=100)
    description = models.CharField(
        max_length=100, blank=False, null=False, unique=True)
    price = models.FloatField(max_length=1000, blank=False, null=False)
    catalog = models.CharField(max_length=100)
    seller = models.CharField(max_length=100)
    version = models.IntegerField(default=1)  # need default value.

    class Meta:
        db_table = 'Item'


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
    itemId = models.ForeignKey(Item, on_delete=models.SET_NULL, null=True)
    price = models.FloatField(max_length=1000)
    version = models.IntegerField(default=1)  # need default value.

    class Meta:
        db_table = 'Order'


class Inventory(models.Model):
    itemId = models.ForeignKey(Item, on_delete=models.SET_NULL, null=True)
    itemAmount = models.IntegerField()
    whID = models.IntegerField()
    version = models.IntegerField(default=1)  # need default value.

    class Meta:
        unique_together = ["itemId", "whID"]
        db_table = 'Inventory'
