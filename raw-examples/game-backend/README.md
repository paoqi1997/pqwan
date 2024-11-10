# 游戏服务器架构

如下所示：

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
