#!/bin/sh

set -e

sudo apt-get update

enable_root()
{
    # sudo passwd root

    # change for /usr/share/lightdm/lightdm.conf.d/50-unity-greeter.conf
    # +greeter-show-manual-login=true

    # comment the below line in /etc/pam.d/gdm-password
    # +# auth  required        pam_succeed_if.so user != root quiet_success
    # comment the below line in /etc/pam.d/gdm-password
    # +# auth  required        pam_succeed_if.so user != root quiet_success

    # change for /root/.profile
    # -mesg n || true
    # +tty -s && mesg n || true
}

# enable to login as root, changes for /etc/ssh/sshd_config are shown as below
# #PermitRootLogin prohibit-password
# +PermitRootLogin yes
install_ssh()
{
    sudo apt-get install -y openssh-server
}
# start the ssh service
# sudo service ssh restart
# check the ssh service status
# sudo service ssh status

install_samba()
{
    sudo apt-get install -y samba samba-common
    # sudo smbpasswd -a smbuser

    # config for samba, in /etc/samba/smb.conf
    # [root]
	# comment = share folder with username and password
	# path = /root
	# # users that are allowed to login to this service
	# # Default: valid users = # No valid users list (anyone can login)
	# valid users = root
	# # default: create mask = 0744
	# create mask = 0700
	# # octal modes that are used when converting DOS modes to UNIX modes when creating UNIX directories
	# # default: directory mask = 0755
	# directory mask = 0700
	# # default: public = no
	# public = yes
	# # default: writable = no
	# writable = yes
	# # default: available = yes
	# available = yes
	# # default: browseable = yes
	# browseable = yes

	# [gunix]
	# comment = share folder with username and password
	# path = /home/gunix
	# valid users = gunix
	# create mask = 0777
	# directory mask = 0777
	# public = yes
	# writable = yes
	# available = yes
	# browseable = yes
}

install_ctags()
{
    sudo apt-get install -y autoconf pkg-config
    git clone https://github.com/universal-ctags/ctags.git
    cd ctags
    ./autogen.sh
    ./configure
    make
    sudo make install
}

install_gcc()
{
    # install gcc compiler
    sudo apt-get install -y gcc
    # Another way to install gcc compiler is to install it as part of build-essential package.
    # build-essential package will also install additional libraries as well as g++ compiler.
    # In most cases or if unsure this is exactly what you need.
    sudo apt-get install -y build-essential
    # build-essential package contains an informational list of packages which are considered
    # essential for building Ubuntu packages including gcc compiler, make and other required tools.
    # This package also depends on the packages on that list,
    # to make it easy to have the build-essential packages installed.
}

install_basic_software()
{
    # basic tool
    sudo apt-get install -y preload lzop tree dos2unix flex openssl libreadline-dev
    # version control
    # old linux version need to install git-core instead of git
    sudo apt-get install -y git subversion
    # editor and tags
    sudo apt-get install -y vim exuberant-ctags
    # build, compile and debug tools
    sudo apt-get install -y gcc build-essential gdb ddd
    # man pages
    sudo apt-get install -y manpages-dev man-db manpages-posix-dev
    # pip
    sudo apt-get install -y python-pip python3-pip
    # database
    sudo apt-get install -y mysql-server mysql-client libmysqlclient-dev apache2 mysql-workbench
    # nginx
    sudo apt-get install -y nginx
    # redis
    sudo apt-get install -y redis-server
    sudo apt-get install -y libx11-dev make libssl-dev libghc-zlib-dev unzip
}

build_android_environment()
{
    sudo apt-get install -y git ccache lzop bison gperf build-essential zip curl zlib1g-dev g++-multilib
    sudo apt-get install -y python-networkx libxml2-utils bzip2 libbz2-dev libghc-bzlib-dev squashfs-tools
    sudo apt-get install -y pngcrush liblz4-tool optipng libc6-dev-i386 gcc-multilib libssl-dev gnupg flex
    sudo apt-get install -y lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z1-dev libgl1-mesa-dev xsltproc
    sudo apt-get install -y unzip python-pip python-dev libffi-dev libxml2-dev libxslt1-dev libjpeg8-dev openjdk-8-jdk
}

set_java_env()
{
    sudo apt-get install -y openjdk-8-jdk
    export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
    export JRE_HOME=${JAVA_HOME}/jre
    export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib
    export PATH=${JAVA_HOME}/bin:${JRE_HOME}/bin:${PATH}
}
# set the default version of java or python
# sudo update-alternatives --config java
# sudo update-alternatives --config python

install_anaconda()
{
    sudo ./Anaconda3-2018.12-Linux-x86_64.sh
    # setup the environment for anaconda
    # add the below line to ~/.bashrc or
    # add it to the independent file in /etc/profile.d/anacond_env.sh
    export PATH=/path/to/anaconda/bin:$PATH
}

set_alias()
{
    alias vi='vim'
    alias ifconfig='ip -c a'
}

git_setting()
{
    export GIT_EDITOR=vim
}

python_tab()
{
    export PYTHONSTARTUP=/usr/lib/python2.7/tab.py
}

set_iot_toolchain()
{
    PATH=$PATH:/opt/toolchain-i386_gcc-4.8_glibc-2.19/usr/bin
}

set_repo_env()
{
    PATH=$PATH:~/bin
}


set_toolchain()
{
    TOOLCHAIN_PATH=$(echo "$PATH" | grep arm-2010.09)
    if [  "$TOOLCHAIN_PATH" = "" ]; then
        TOOLCHAIN_PATH=/opt/arm-2010.09
        PATH=$PATH:${TOOLCHAIN_PATH}/bin
    fi
}



