package main

import (
    "context"
    "fmt"
    "net"

    "google.golang.org/grpc"

    pb "db-proxy/protos"
)

const (
    G_Port uint32 = 10032
)

type Server struct {
    pb.UnimplementedGreeterServer
}

func (s *Server) SayHello(_ context.Context, in *pb.HelloRequest) (*pb.HelloReply, error) {
    inName := in.GetName()

    fmt.Printf("funcName: SayHello, param_name: %s\n", inName)

    return &pb.HelloReply{
        Message: fmt.Sprintf("Hello %s", inName),
    }, nil
}

func main() {
    listener, err := net.Listen("tcp", fmt.Sprintf(":%d", G_Port))
    if err != nil {
        fmt.Printf("net.Listen: %v\n", err)
        return
    }

    fmt.Printf("Listen at %v\n", listener.Addr())

    server := grpc.NewServer()
    pb.RegisterGreeterServer(server, &Server{})

    if err := server.Serve(listener); err != nil {
        fmt.Printf("Serve: %v\n", err)
    }
}
