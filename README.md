## Run in docker, follwing the steps below:
1. remove mini_Amazon/web-app/amazon/migrations directory.
2. make sure change database settings in mini_Amazon/web-app/ERSSHW5/settings.py
![image](https://user-images.githubusercontent.com/59811560/164951835-cd349241-9898-49b5-8b06-65241f3e2b93.png)
3.  make sure use correct statment in Server::connnectDB(), which is located at server.cpp. ![image](https://user-images.githubusercontent.com/59811560/164951866-9e78eeb9-35ea-4a66-85d8-12697e95588c.png)

4.  change world and ups hostname in the constructor of class Server, which is loacted at server.cpp.  ![image](https://user-images.githubusercontent.com/59811560/164951942-4941ee4f-beb2-4b2d-b69e-b84c4f748dba.png)


5. `sudo docker-compose down`
6. `sudo docker-compose build`
7. `sudo docker-compose up`
  
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

