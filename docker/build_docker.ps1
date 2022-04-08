docker build --network=host -t nju-nemu:latest ./docker
$nowpath = ($PWD).Path.replace("\", "/").replace(":", "")
docker run --network=host -ti --name nju-nemu-container -v /${nowpath}:/root/NJU-nemu:rw --workdir=/root/NJU-nemu nju-nemu:latest /bin/bash
