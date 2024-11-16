package main

import (
    "context"
    "encoding/json"
    "fmt"
    "net"
    "strconv"

    "google.golang.org/grpc"

    pb "pprotos"
)

const (
    G_Port      uint32 = 10032
    G_MongoPort uint32 = 27017
)

var (
    G_DBLayer *DBLayer
)

type GreeterServer struct {
    pb.UnimplementedGreeterServer
}

func (s *GreeterServer) SayHello(_ context.Context, in *pb.HelloRequest) (*pb.HelloReply, error) {
    inName := in.GetName()

    fmt.Printf("[GreeterServer][SayHello] name: %s\n", inName)

    return &pb.HelloReply{
        Message: fmt.Sprintf("Hello %s", inName),
    }, nil
}

type PlayerServer struct {
    pb.UnimplementedPlayerServer
}

func (s *PlayerServer) GetName(_ context.Context, in *pb.GetNameRequest) (*pb.GetNameReply, error) {
    playerID := in.GetPlayerID()

    fmt.Printf("[PlayerServer][GetName] playerID: %s\n", playerID)

    pID, err := strconv.ParseUint(playerID, 10, 32)
    if err != nil {
        fmt.Printf("[PlayerServer][GetName] strconv.ParseUint err: %v\n", err)
        return &pb.GetNameReply{ StatusCode: 1, Name: "" }, err
    }

    collectionName := "players"
    jsonReply := G_DBLayer.GetOne(collectionName, uint32(pID))

    var getResult map[string]interface{}

    if err := json.Unmarshal(jsonReply, &getResult); err != nil {
        fmt.Printf("[PlayerServer][GetName] json.Unmarshal err: %v\n", err)
        return &pb.GetNameReply{ StatusCode: 2, Name: "" }, err
    }

    var name string

    if rawName, ok := getResult["name"]; ok {
        name = rawName.(string)
    } else {
        name = ""
    }

    return &pb.GetNameReply{ StatusCode: 0, Name: name }, err
}

func (s *PlayerServer) SetName(_ context.Context, in *pb.SetNameRequest) (*pb.SetNameReply, error) {
    playerID := in.GetPlayerID()
    newName := in.GetName()

    pID, err := strconv.ParseUint(playerID, 10, 32)
    if err != nil {
        fmt.Printf("[PlayerServer][SetName] strconv.ParseUint err: %v\n", err)
        return &pb.SetNameReply{ StatusCode: 1 }, err
    }

    fmt.Printf("[PlayerServer][SetName] playerID: %s, name: %s\n", playerID, newName)

    collectionName := "players"
    jsonGetReply := G_DBLayer.GetOne(collectionName, uint32(pID))

    if string(jsonGetReply) == "{}" {
        jsonData := fmt.Sprintf("{\"id\":%d,\"name\":\"%s\"}", pID, newName)
        inJsonData := []byte(jsonData)
        msg := G_DBLayer.InsertOne(collectionName, inJsonData)
        fmt.Printf("[PlayerServer][SetName] DBLayer.InsertOne: %s\n", msg)
    } else {
        jsonData := fmt.Sprintf("{\"name\":\"%s\"}", newName)
        inJsonData := []byte(jsonData)
        msg := G_DBLayer.UpdateOne(collectionName, uint32(pID), inJsonData)
        fmt.Printf("[PlayerServer][SetName] DBLayer.UpdateOne: %s\n", msg)
    }

    return &pb.SetNameReply{ StatusCode: 0 }, err
}

func main() {
    listener, err := net.Listen("tcp", fmt.Sprintf(":%d", G_Port))
    if err != nil {
        fmt.Printf("[main][net.Listen] err: %v\n", err)
        return
    }

    fmt.Printf("Listen at %v\n", listener.Addr())

    G_DBLayer = NewDBLayer(fmt.Sprintf("mongodb://127.0.0.1:%d", G_MongoPort))

    server := grpc.NewServer()
    pb.RegisterGreeterServer(server, &GreeterServer{})
    pb.RegisterPlayerServer(server, &PlayerServer{})

    if err := server.Serve(listener); err != nil {
        fmt.Printf("[main] grpc.Server.Serve err: %v\n", err)
    }
}
