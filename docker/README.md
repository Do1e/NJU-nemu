### 在根目录(NJU-nemu)下运行
1. `scp -P [port] -r [username]@[IPaddr]:~/pa_nju ./`，将在线平台文件复制到本地目录
2. 将本`docker`文件夹放入`pa_nju`目录下
3. 在`pa_nju`目录下运行`build_docker.sh`脚本(Linux)或`build_docker.ps1`脚本(Windows)。
4. 在`game`、`kernel`、`nemu`目录下的`Makefile`文件中，找到`CFLAGS`并在结尾加上`-m32`选项。在`nemu`目录下的`Makefile`文件中，找到`LDFLAGS`并在结尾加上`-m32`选项。
5. 若要使用Cmake可以attach vscode。
![](https://www.helloimg.com/images/2022/07/13/Z9KRhz.png)
6. 再次强调，`kernel`最好使用在线平台编译。`scp -P [port] -r [username]@[IPaddr]:~/pa_nju/kernel/kernel* ./kernel/`
7. 以及PA4需要X11转发，目前没有好的办法使用docker实现，个人是在自己的archlinux上运行的。