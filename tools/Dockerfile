FROM ubuntu:22.04

USER root

# 设置环境变量，时区为上海，东八区
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 拷贝.ssh内容到在root目录下，以便能够下载计算器的代码
ADD .ssh /root/.ssh
RUN chmod 0600 /root/.ssh/id_rsa*

# 设置国内源
RUN sed -E -i -e 's/(archive|ports).ubuntu.com/mirrors.ustc.edu.cn/g' -e '/security.ubuntu.com/d' /etc/apt/sources.list

# 安装软件
RUN apt-get update && \
	apt-get install -y zsh vim git wget curl python3 unzip sudo && \
	apt-get install -y software-properties-common apt-utils build-essential gcc-12 g++-12 clang clangd clang-format clang-tidy bear llvm libomp-dev libtool cmake ninja-build graphviz graphviz-dev dos2unix  && \
	apt-get install -y flex bison  && \
	apt-get install -y gdb lldb gdbserver gdb-multiarch  && \
	apt-get install -y openjdk-17-jdk dotnet-sdk-6.0  && \
	apt-get install -y gcc-mips-linux-gnu g++-mips-linux-gnu && \
	apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu && \
	apt-get install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf && \
	apt-get install -y gcc-riscv64-linux-gnu g++-riscv64-linux-gnu && \
	apt-get install -y qemu-user-static && \
	apt-get install -y doxygen texlive-lang-chinese texlive-lang-english texlive-latex-extra texlive-science texlive-plain-generic && \
	apt-get install -y openssh-server

# 安装ANTLR 4.12.0
RUN wget -O /usr/local/bin/antlr-4.12.0-complete.jar https://proxy.201704.xyz/https://github.com/antlr/website-antlr4/blob/gh-pages/download/antlr-4.12.0-complete.jar && \
	chmod +x /usr/local/bin/antlr-4.12.0-complete.jar && \
	wget -O ~/antlr4-cpp-runtime-4.12.0-source.zip https://proxy.201704.xyz/https://github.com/antlr/website-antlr4/blob/gh-pages/download/antlr4-cpp-runtime-4.12.0-source.zip && \
	unzip ~/antlr4-cpp-runtime-4.12.0-source.zip -d ~/antlr4-cpp-runtime-4.12.0-source && \
	cd ~/antlr4-cpp-runtime-4.12.0-source && \
	cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Debug -DANTLR_BUILD_CPP_TESTS=OFF && \
	cmake --build build --parallel && \
	cmake --install build

# 删除 apt/lists，可以减少最终镜像大小
RUN rm -rf /var/lib/apt/lists/*

# 安装oh-my-zsh
# 注意若代理git clone失败时，请去除https://proxy.201704.xyz/或者用国内gitee的网址加速下载
RUN git clone https://proxy.201704.xyz/https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh && \
	cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc && \
	git clone https://proxy.201704.xyz/https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions && \
	git clone https://proxy.201704.xyz/https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting && \
	sed -i 's/^plugins=(/plugins=(zsh-autosuggestions zsh-syntax-highlighting z /' ~/.zshrc && \
	chsh -s /bin/zsh root

# 创建code用户
RUN useradd --create-home --no-log-init --shell /bin/zsh -G sudo code && \
	adduser code sudo && echo 'code:password' | chpasswd

# 拷贝.ssh内容到在root目录下，以便能够下载计算器的代码
ADD .ssh /home/code/.ssh

# 改变用户和权限
RUN chown -R code:code /home/code/.ssh && chmod 0600 /home/code/.ssh/id_rsa*

# 切换用户code
USER code

# 安装oh-my-zsh
# 注意若代理git clone失败时，请去除https://proxy.201704.xyz/或者用国内gitee的网址加速下载
RUN git clone https://proxy.201704.xyz/https://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh && \
	cp ~/.oh-my-zsh/templates/zshrc.zsh-template ~/.zshrc && \
	git clone https://proxy.201704.xyz/https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions && \
	git clone https://proxy.201704.xyz/https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting && \
	sed -i 's/^plugins=(/plugins=(zsh-autosuggestions zsh-syntax-highlighting z /' ~/.zshrc

WORKDIR /home/code

ENTRYPOINT /bin/zsh
