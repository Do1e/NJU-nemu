FROM archlinux
RUN echo -e "Server = http://mirrors.aliyun.com/archlinux/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist
RUN echo -e "[multilib]\nInclude = /etc/pacman.d/mirrorlist" >> /etc/pacman.conf
RUN echo -e "[archlinuxcn]\nServer = https://mirrors.ustc.edu.cn/archlinuxcn/\$arch" >> /etc/pacman.conf
RUN pacman -Syu --noconfirm
RUN pacman -S vim gcc gdb clang cmake make git --noconfirm
RUN pacman -S glibc lib32-glibc sdl lib32-sdl readline lib32-readline --noconfirm
RUN pacman openssh --noconfirm
RUN systemctl enable sshd
