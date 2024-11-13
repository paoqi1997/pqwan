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
    cg   pb.GreeterClient
    cp   pb.PlayerClient
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
        cg:   pb.NewGreeterClient(conn),
        cp:   pb.NewPlayerClient(conn),
    }
}

func (cli *Client) Close() {
    cli.conn.Close()
}

func (cli *Client) SayHello(name string) (bool, string) {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.cg.SayHello(ctx, &pb.HelloRequest{ Name: name })
    if err != nil {
        msg := fmt.Sprintf("SayHelloError: %v", err)
        fmt.Println(msg)
        return false, msg
    }

    return true, r.GetMessage()
}

func (cli *Client) GetName(playerID string) (bool, string) {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.cp.GetName(ctx, &pb.GetNameRequest{ PlayerID: playerID })
    if err != nil {
        msg := fmt.Sprintf("GetNameError: %v", err)
        fmt.Println(msg)
        return false, msg
    }

    return true, r.GetName()
}

func (cli *Client) SetName(playerID, name string) (bool, int32) {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.cp.SetName(ctx, &pb.SetNameRequest{ PlayerID: playerID, Name: name })
    if err != nil {
        msg := fmt.Sprintf("SetNameError: %v", err)
        fmt.Println(msg)
        return false, 1
    }

    return true, r.GetStatusCode()
}
