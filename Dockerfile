FROM ubuntu:18.04
LABEL maintainer="Testov Boris (testov@protei.ru)"
RUN echo "deb http://archive.ubuntu.com/ubuntu xenial main universe" | tee -a /etc/apt/sources.list && \
apt-get update -qq && \
apt-get install -y \
qtbase5-dev \
qt5-default \
qttools5-dev \
libdbus-1-3 \
libxkbcommon-x11-0 \
xvfb \
gcc-7 \
g++-7 \
doxygen \ 
graphviz \
libgtest-dev \
cmake \
build-essential \
git \
software-properties-common \
wget \
meld
WORKDIR /usr/src/gtest
RUN env "PATH=$PATH" cmake . && env "PATH=$PATH" make && \
cp libgtest.a libgtest_main.a /usr/lib && \
add-apt-repository ppa:mhier/libboost-latest -y && \
apt-get install libboost1.70-dev -y && \
wget -O CLion.tar.gz https://cloud.protei.ru/s/LxGco8pVabMjxoc/download && \
tar xvzf CLion.tar.gz -C /opt/ && \
ln -s /opt/clion-2020.2/bin/clion.sh /usr/bin/clion && \
rm CLion.tar.gz && \
git clone https://github.com/BorisTestov/duplicate_finder /duplicate_finder
RUN echo "parse_git_branch() {" >> /root/.bashrc && \
echo "  git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\\\1)/'" >> /root/.bashrc && \
echo "}" >> /root/.bashrc && \
echo "export PS1=\"\u@\h \[\\\e[32m\]\w \[\\\e[91m\]\\\$(parse_git_branch)\[\\\e[00m\]$ \"" >> /root/.bashrc && \
echo "test -f /root/.git-completion.bash && . \$_" >> /root/.bashrc && \
echo "bind 'set bell-style none'" >> /root/.bashrc && \
wget https://raw.githubusercontent.com/git/git/master/contrib/completion/git-completion.bash -O /root/.git-completion.bash && \
git config --global core.editor "vim" && \
git config --global diff.tool meld && \
git config --global user.useConfigOnly true && \
git config --global credential.helper store
CMD ["clion","/duplicate_finder/CMakeLists.txt"]



