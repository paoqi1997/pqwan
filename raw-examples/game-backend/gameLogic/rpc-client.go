package main

import (
    "context"
    "fmt"
    "time"

    "google.golang.org/grpc"
    "google.golang.org/grpc/credentials/insecure"

    pb "pprotos"
)

type Client struct {
    conn *grpc.ClientConn
    c    pb.GreeterClient
}

func NewClient(port uint32) *Client {
    conn, err := grpc.NewClient(
        fmt.Sprintf("127.0.0.1:%d", port),
        grpc.WithTransportCredentials(insecure.NewCredentials()),
    )
    if err != nil {
        fmt.Printf("grpc.NewClient: %v\n", err)
        return nil
    }

    return &Client{
        conn: conn,
        c:    pb.NewGreeterClient(conn),
    }
}

func (cli *Client) Close() {
    cli.conn.Close()
}

func (cli *Client) SayHello(name string) (bool, string) {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.c.SayHello(ctx, &pb.HelloRequest{ Name: name })
    if err != nil {
        msg := fmt.Sprintf("SayHelloError: %v", err)
        fmt.Println(msg)
        return false, msg
    }

    return true, r.GetMessage()
}
