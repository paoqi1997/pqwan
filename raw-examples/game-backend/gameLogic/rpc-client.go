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
        fmt.Printf("[NewClient][grpc.NewClient] err: %v\n", err)
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

func (cli *Client) SayHello(name string) string {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.cg.SayHello(ctx, &pb.HelloRequest{ Name: name })
    if err != nil {
        msg := fmt.Sprintf("[Client][SayHello] err: %v", err)
        fmt.Println(msg)
        return msg
    }

    return r.GetMessage()
}

func (cli *Client) GetName(playerID string) string {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.cp.GetName(ctx, &pb.GetNameRequest{ PlayerID: playerID })
    if err != nil {
        msg := fmt.Sprintf("[Client][GetName] err: %v", err)
        fmt.Println(msg)
        return msg
    }

    return r.GetName()
}

func (cli *Client) SetName(playerID, name string) int32 {
    ctx, cancel := context.WithTimeout(context.Background(), time.Second)

    defer cancel()

    r, err := cli.cp.SetName(ctx, &pb.SetNameRequest{ PlayerID: playerID, Name: name })
    if err != nil {
        msg := fmt.Sprintf("[Client][SetName] err: %v", err)
        fmt.Println(msg)
        return -1
    }

    return r.GetStatusCode()
}
