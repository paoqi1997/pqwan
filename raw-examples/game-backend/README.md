# game-backend

由 Go 编写的游戏后端服务。

## 部署

### 安装 [Go](https://go.dev/doc/install)

获取相应的包。

```
$ wget https://go.dev/dl/go1.23.2.linux-amd64.tar.gz
```

提取相应的包。

```
$ sudo tar -xzvf go1.23.2.linux-amd64.tar.gz -C /usr/local
```

添加 go/bin 到环境变量中。

```
$ echo "export PATH=$PATH:/usr/local/go/bin" >> $HOME/.profile
$ source $HOME/.profile
```

配置 Go 的环境变量。

```
$ go env -w GOPATH=/usr/local/go/ws
$ go env -w GO111MODULE=on
$ go env -w GOPROXY=https://goproxy.cn,direct
```

### 为 [grpc](https://grpc.io/docs/languages/go/quickstart/) 安装 [protoc](https://protobuf.dev/downloads/)

获取相应的包。

```
$ wget https://github.com/protocolbuffers/protobuf/releases/download/v28.3/protoc-28.3-linux-x86_64.zip
```

将 protoc 放在 /usr/local/bin 目录。

```
$ unzip protoc-28.3-linux-x86_64.zip -d protoc
$ cd protoc/bin
$ sudo cp protoc /usr/local/bin
```

安装 protoc 的 Go 插件。

```
$ go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
$ go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest
```

### 安装 [Docker](https://docs.docker.com/engine/install/ubuntu/)

在[此处](https://download.docker.com/linux/ubuntu/dists/jammy/pool/stable/amd64/)获取 deb 包并安装它们。

```
$ sudo dpkg -i containerd.io_1.7.22-1_amd64.deb \
    docker-ce_27.3.1-1~ubuntu.22.04~jammy_amd64.deb \
    docker-ce-cli_27.3.1-1~ubuntu.22.04~jammy_amd64.deb \
    docker-buildx-plugin_0.17.1-1~ubuntu.22.04~jammy_amd64.deb \
    docker-compose-plugin_2.29.7-1~ubuntu.22.04~jammy_amd64.deb
```

将当前用户添加到 docker 组中。

```
$ sudo groupadd docker
$ sudo usermod -aG docker $USER
```

### 部署 MongoDB

#### [mongodb](https://www.mongodb.com/zh-cn/docs/manual/tutorial/install-mongodb-community-with-docker/)

拉取镜像。

```
$ docker pull dockerpull.org/mongodb/mongodb-community-server:latest

$ docker tag dockerpull.org/mongodb/mongodb-community-server:latest mongodb/mongodb-community-server:latest
```

运行镜像。

```
$ docker run \
    -p 27017:27017 -d --rm --name mongodb \
    mongodb/mongodb-community-server:latest
```

#### [mongosh](https://www.mongodb.com/zh-cn/docs/mongodb-shell/install/)

安装 mongosh。

```
$ tar -xzvf mongosh-2.3.3-linux-x64.tgz

$ cd mongosh-2.3.3-linux-x64/bin

$ sudo cp mongosh /usr/local/bin
$ sudo cp mongosh_crypt_v1.so /usr/local/lib
```

连接到 mongodb。

```
$ mongosh --port 27017
```

执行以下命令：

```js
test> db
test> show dbs

// pq 为游戏项目代号
test> use pq

// db.collection.xxx 中的 collection 相当于表
pq> db.players.insertOne(
  {
    id: 10137135,
    name: "paoqi"
  }
)

pq> db.players.find()
```

#### [mongo-go-driver](https://www.mongodb.com/zh-cn/docs/drivers/go/current/quick-start/)

获取 MongoDB 的 Go 驱动。

```
$ go get go.mongodb.org/mongo-driver/mongo
```

## TPs

[Docker Hub 镜像加速器](https://gist.github.com/y0ngb1n/7e8f16af3242c7815e7ca2f0833d3ea6)

### GitHub repos

[bobohume/gonet](https://github.com/bobohume/gonet)

## 架构

v1.0 如下所示：

```
gameLogic-1:
|
|--- UNIX domain sockets ---> localCache --- rpc ---> db-proxy -> mongodb-cluster
|
|---> gameHub -> gameLogic-2
|
|---> redis-proxy -> redis-cluster

gateway:
|
|---> etcd-cluster
|
|---> gameLogic-1
|
|---> gameHub -> gameLogic-1

game-client:
|
|---> gateway -> gameLogic-1
|
|---> gateway -> gameHub -> gameLogic-1
|
|--- kcp ---> battle-server -> gameHub -> gameLogic-1
```

v1.1 如下所示：

```
gameLogic:
|
|--- UNIX domain sockets ---> localCache

# 读数据时若 localCache 中找不到，应向 db-proxy 发起请求
# 写数据时给对应的数据块打上脏标记，由存盘队列把数据刷到 db-proxy
# 写数据分为内置和外置两种方式

# 集成在 gameLogic
localCache:
|
|--- UNIX domain sockets ---> gameLogic(db-layer) --- rpc ---> db-proxy -> mongodb-cluster

# 划分出 db-sync
localCache:
|
|--- UNIX domain sockets ---> db-sync --- rpc ---> db-proxy -> mongodb-cluster
```
