$dockerid=(docker ps -a | findstr nju-nemu-container).split(' ')[0]
docker start $dockerid
docker exec -it $dockerid /bin/bash