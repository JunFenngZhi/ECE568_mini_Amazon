# Generated by Django 4.0.4 on 2022-04-18 22:33

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('amazon', '0003_rename_item_id_inventory_item_and_more'),
    ]

    operations = [
        migrations.AlterModelTable(
            name='order',
            table='orders',
        ),
    ]
