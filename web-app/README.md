## Run in docker, follwing the steps below:
1. remove mini_Amazon/web-app/amazon/migrations directory.
2. `sudo docker-compose down`
3. `sudo docker-compose build`
4. `sudo docker-compose up`
  
Then Django framework will generate migration file and create database and related tables automatically.  
## To use the admin in Django, you need to create a superuser in docker, following the steps below:
1. after running the docker, use `sudo docker ps -a` to find the container ID for mini_amazon_web IMAGE.  
2. run `sudo docker exec -it ${CONTAINER_ID}  python3 manage.py createsuperuser`
3. follow the instruction to create a super user

## Run outside docker, follwing the steps below:
1. run initserver.sh
2. run runserver.sh

## If you want to clean the database in mini_amazon for each testing, follwing steps below:
1. `psql -U postgres mini_amazon` password is `passw0rd`
2. run `truncate item cascade;`
3. run `truncate shoppingcart;`
4. run `alter sequence orders_pack_id_seq restart with 1;`

