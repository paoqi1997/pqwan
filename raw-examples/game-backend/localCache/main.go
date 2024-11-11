package main

import (
    "fmt"
    "io"
    "net"
    "os"
    "os/signal"
    "syscall"
)

const (
    G_Sock string = "/tmp/local-cache.sock"
)

var (
    G_Counter uint32 = 0
)

func OnAccept(conn net.Conn, connID uint32) {
    defer conn.Close()

    size := 8192 // 8K
    buffer := make([]byte, size)

    for {
        nBytes, err := conn.Read(buffer)

        if err != nil {
            if err == io.EOF {
                fmt.Printf("Connection %s/%d closed.\n", conn.RemoteAddr().String(), connID)
            } else {
                fmt.Printf("Read: %v\n", err)
            }

            break
        }

        msg := string(buffer[:nBytes])

        fmt.Printf("Connection %s/%d: %s\n", conn.RemoteAddr().String(), connID, msg)
    }
}

func OnExit() {
    ch := make(chan os.Signal, 1)

    signal.Notify(ch, os.Interrupt, syscall.SIGTERM)

    go func(ch chan os.Signal) {
        <-ch

        err := os.Remove(G_Sock)

        if err != nil {
            fmt.Printf("os.Remove: %v\n", err)
        }

        fmt.Println("Exited.")

        os.Exit(0)
    }(ch)
}

func main() {
    listener, err := net.Listen("unix", G_Sock)

    if err != nil {
        fmt.Printf("net.Listen: %v\n", err)
        return
    }

    defer listener.Close()

    OnExit()

    for {
        conn, err := listener.Accept()

        if err != nil {
            fmt.Printf("Accept: %v\n", err)
            continue
        }

        G_Counter++

        go OnAccept(conn, G_Counter)
    }
}
