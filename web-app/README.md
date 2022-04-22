## Run in docker, follwing the steps below:
1. remove mini_Amazon/web-app/amazon/migrations directory.
2. `sudo docker-compose down`
3. `sudo docker-compose build`
4. `sudo docker-compose up`
  
Then, Django framework will generate migration file and create database and related tables automatically.

## Run outside docker, follwing the steps below:
1. run initserver.sh
2. run runserver.sh

