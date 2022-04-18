from enum import unique
from gettext import Catalog
from django.db import models
from django.utils.timezone import now

# Create your models here.

# 将itemId和seller做成复合主键？


class Item(models.Model):
    item_id = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=100)
    description = models.CharField(
        max_length=100, blank=False, null=False, unique=True)
    price = models.FloatField(max_length=1000, blank=False, null=False)
    catalog = models.CharField(max_length=100)
    seller = models.CharField(max_length=100)
    version = models.IntegerField(default=1)  # need default value.

    class Meta:
        db_table = 'item'


class Order(models.Model):
    pack_id = models.IntegerField(primary_key=True)
    addr_x = models.IntegerField()
    addr_y = models.IntegerField()
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
    ups_id = models.IntegerField()
    item = models.ForeignKey(Item, on_delete=models.SET_NULL, null=True)
    price = models.FloatField(max_length=1000)
    version = models.IntegerField(default=1)  # need default value.

    class Meta:
        db_table = 'order'


class Inventory(models.Model):
    item = models.ForeignKey(Item, on_delete=models.SET_NULL, null=True)
    item_amount = models.IntegerField()
    wh_id = models.IntegerField()
    version = models.IntegerField(default=1)  # need default value.

    class Meta:
        unique_together = ["item", "wh_id"]
        db_table = 'inventory'
