# 如何使用 SSH Key 登录 GitHub

## 1. 检查是否已有 SSH Key

```
cd ~/.ssh
ls
```

若目录下已有：

* `id_rsa`（私钥）
* `id_rsa.pub`（公钥）

则无需生成，直接跳到步骤 3。


## 2. 生成 SSH Key

执行命令：`ssh-keygen -t rsa -b 4096 -C "你的 GitHub 注册邮箱"`

参数说明：

| 参数     | 含义                       |
| -------- | -------------------------- |
| -t rsa   | 生成 RSA 类型密钥          |
| -b 4096  | 密钥长度 4096 位（更安全） |
| -C "xxx" | 注释，一般填写 GitHub 邮箱 |


一路回车即可，默认生成路径：

```
/home/用户名/.ssh/id_rsa
/home/用户名/.ssh/id_rsa.pub
```


## 3. 添加 SSH 公钥到 GitHub

查看公钥内容：`cat ~/.ssh/id_rsa.pub`


复制输出内容。

然后在 GitHub 网页操作：

1. 登录 GitHub
2. 点击头像 → Settings
3. SSH and GPG keys → New SSH key
4. 粘贴公钥内容
5. 点击 Add SSH key 保存


## 4. 修改本地仓库为 SSH 地址

进入你的项目目录：`cd /mnt/sdb/hh/hhe_cpp/Prj/CppHttpFileServer`

查看远程仓库地址：`git remote -v`

修改为 SSH 格式：`git remote set-url origin git@github.com:hehao678/CppHttpFileServer.git`

再次查看确认：`git remote -v`


## 5. 测试 SSH 连接 GitHub

执行命令: `ssh -T git@github.com`

如果输出如下，说明 SSH 配置成功：`Hi hehao678! You've successfully authenticated, but GitHub does not provide shell access.`


## 6. 提交和推送代码

执行正常的 Git 操作：

```
git add .
git commit -m "提交说明"
git push
```

无需输入密码，自动推送成功。


## 7. 常见问题

| 问题                   | 解决方案                         |
| ---------------------- | -------------------------------- |
| 提示 permission denied | 检查 SSH key 是否添加到 GitHub   |
| 没有 SSH key 文件      | 执行 ssh-keygen 生成             |
| 推送失败还是要密码     | 确认仓库地址是 SSH 格式          |
| 多账号冲突             | 使用 ~/.ssh/config 配置多个 Host |

## 参考命令速查

```bash
# 查看 SSH Key
ls ~/.ssh

# 生成 SSH Key
ssh-keygen -t rsa -b 4096 -C "xxx@xxx.com"

# 查看公钥
cat ~/.ssh/id_rsa.pub

# 设置远程仓库为 SSH
git remote set-url origin git@github.com:用户名/仓库名.git

# 测试 SSH 连接
ssh -T git@github.com

# 提交代码
git add .
git commit -m "说明"
git push

```
