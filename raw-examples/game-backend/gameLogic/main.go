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
    G_Sock     string = "/tmp/local-cache.sock"
    G_RPC_Port uint32 = 10032
)

var (
    G_RPC *Client
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

func OnInput(line string) bool {
    helloPrefix := "h:"
    mongoGetPrefix := "m:g:"
    mongoUpdatePrefix := "m:u:"

    if strings.HasPrefix(line, helloPrefix) {
        helloMsgIndex := len(helloPrefix)
        _, msg := G_RPC.SayHello(line[helloMsgIndex:])
        fmt.Printf("SayHello: %s\n", msg)
        return true
    } else if strings.HasPrefix(line, mongoGetPrefix) {
        mongoGetIndex := len(mongoGetPrefix)
        playerID := line[mongoGetIndex:]
        _, playerName := G_RPC.GetName(playerID)
        fmt.Printf("GetName: <%s>\n", playerName)
        return true
    } else if strings.HasPrefix(line, mongoUpdatePrefix) {
        mongoUpdateIndex := len(mongoUpdatePrefix)
        IDAndName := strings.Split(line[mongoUpdateIndex:], ":")
        playerID, playerName := IDAndName[0], IDAndName[1]
        _, statusCode := G_RPC.SetName(playerID, playerName)
        fmt.Printf("SetName: %d\n", statusCode)
        return true
    }

    return false
}

func OnExit() {
    ch := make(chan os.Signal, 1)

    signal.Notify(ch, os.Interrupt, syscall.SIGTERM)

    go func(ch chan os.Signal) {
        <-ch

        fmt.Println("Exited.")

        G_RPC.Close()

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

    G_RPC = NewClient(G_RPC_Port)

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

        if OnInput(line) {
            continue
        }

        _, err = conn.Write([]byte(line))

        if err != nil {
            fmt.Printf("Write: %v\n", err)
            break
        }
    }
}
