# game-backend

由 Go 编写的游戏后端服务。

## 部署

### 安装 Go

### 安装 protoc

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
