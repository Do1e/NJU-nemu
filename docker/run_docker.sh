export dockerid=`docker ps -a | grep nju-nemu-container | awk '{print $1}'`
docker start $dockerid
docker exec -it $dockerid /bin/bash