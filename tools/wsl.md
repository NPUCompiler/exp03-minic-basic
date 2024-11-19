
# WSL安装Ubuntu-22.04并配置开发环境

## WSL安装运行Ubuntu-22.04

在通过可能需要管理员权限，若出现错误请以管理员权限运行此脚本。

通过如下的命令下载安装Ubuntu-22.04，最后会提示创建一个用户(假定code)并设置密码，最后输入exit退出Ubuntu系统

```powershell
wsl --install --web-download Ubuntu-22.04
```

## 以root用户进入Ubuntu并安装软件

执行如下的命令以root用户进入ubuntu系统

``` powershell
wsl --user root --distribution Ubuntu-22.04 --cd ~
```

在进入系统后拷贝ubuntu.sh到用户目录下运行，然后执行该脚本。

假定ubuntu.sh的位置在E:\GitDatabase\calculator\tools文件夹下，则对应的Linux系统的路径变更为/mnt/e/GitDatabase/calculator/tools/ubuntu.sh

具体的执行命令如下：

```bash
# 拷贝ubuntu.sh脚本
cp /mnt/e/GitDatabase/calculator/tools/ubuntu.sh ~
# 执行脚本安装软件
sh ~/ubuntu.sh
```

## 以普通用户进入ubuntu进行开发与测试

执行如下的命令以普通用户code进入ubuntu系统。要注意请根据用户名修改下面的命令。

``` powershell
wsl --user code --distribution Ubuntu-22.04 --cd ~
```

在VSCode IDE中借助WSL插件可连接ubuntu进行远程开发与调试。
