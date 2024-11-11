package main

import (
    "bufio"
    "fmt"
    "io"
    "net"
    "os"
    "os/signal"
    "strings"
    "syscall"
)

const (
    G_Sock string = "/tmp/local-cache.sock"
)

func OnRead(conn net.Conn) {
    size := 1024 // 1K
    buffer := make([]byte, size)

    for {
        _, err := conn.Read(buffer)

        if err != nil {
            if err == io.EOF {
                fmt.Printf("Server %s closed.\n", conn.RemoteAddr().String())
                os.Exit(0)
            } else {
                fmt.Printf("Read: %v\n", err)
            }

            break
        }
    }
}

func OnExit() {
    ch := make(chan os.Signal, 1)

    signal.Notify(ch, os.Interrupt, syscall.SIGTERM)

    go func(ch chan os.Signal) {
        <-ch

        fmt.Println("Exited.")

        os.Exit(0)
    }(ch)
}

func main() {
    conn, err := net.Dial("unix", G_Sock)

    if err != nil {
        fmt.Printf("net.Dial: %v\n", err)
        return
    }

    defer conn.Close()

    OnExit()

    go OnRead(conn)

    input := bufio.NewReader(os.Stdin)

    for {
        line, err := input.ReadString('\n')

        if err != nil {
            fmt.Printf("ReadString: %v\n", err)
            break
        }

        line = strings.TrimRight(line, "\r\n")

        _, err = conn.Write([]byte(line))

        if err != nil {
            fmt.Printf("Write: %v\n", err)
            break
        }
    }
}
