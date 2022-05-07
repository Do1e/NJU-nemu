docker build --network=host -t nju-nemu:latest ./docker
docker run --network=host -ti --name nju-nemu-container -p 3464:22 -v "$PWD":/root/NJU-nemu:rw --workdir=/root/NJU-nemu nju-nemu:latest /bin/bash
