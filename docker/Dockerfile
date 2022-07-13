FROM archlinux
RUN echo -e "Server = http://mirrors.aliyun.com/archlinux/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist
RUN echo -e "[multilib]\nInclude = /etc/pacman.d/mirrorlist" >> /etc/pacman.conf
RUN echo -e "[archlinuxcn]\nServer = https://mirrors.ustc.edu.cn/archlinuxcn/\$arch" >> /etc/pacman.conf
RUN pacman -Syu --noconfirm
RUN pacman -S vim gcc gdb clang cmake make git --noconfirm
RUN pacman -S glibc lib32-glibc sdl lib32-sdl readline lib32-readline --noconfirm
RUN pacman -S openssh --noconfirm
RUN systemctl enable sshd
RUN pacman-key --init
# RUN pacman-key -r F9F9FA97A403F63E
# RUN pacman-key -f F9F9FA97A403F63E
# RUN pacman-key --lsign-key F9F9FA97A403F63E
# RUN gpg --keyserver keys.gnupg.net --recv-keys F9F9FA97A403F63E
# RUN gpg --keyserver keys.gnupg.net --recv-keys 6C35B99309B5FA62
# RUN pacman -S yaourt --noconfirm
# RUN sed -i '1189,1201d' /usr/bin/makepkg
# RUN yaourt -S gcc8 --noconfirm
# RUN yaourt -S zsh-theme-powerlevel10k --noconfirm
# RUN yaourt -S zsh-autosuggestions --noconfirm
# RUN yaourt -S zsh-syntax-highlighting --noconfirm
RUN pacman -S base-devel --noconfirm
RUN pacman -S zsh --noconfirm