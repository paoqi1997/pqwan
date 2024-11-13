package main

import (
    "fmt"
    "io"
    "net"
    "os"
    "os/signal"
    "strings"
    "syscall"

    "github.com/dgraph-io/ristretto/v2"
)

const (
    G_Sock string = "/tmp/local-cache.sock"
)

var (
    G_Counter uint32 = 0
    G_Cache   *ristretto.Cache[string, string]
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

        cacheGetPrefix := "c:g:"
        cacheSetPrefix := "c:s:"

        if strings.HasPrefix(msg, cacheGetPrefix) {
            keyIndex := len(cacheGetPrefix)
            key := msg[keyIndex:]

            fmt.Printf("Get key:<%s>\n", key)

            value, _ := G_Cache.Get(key)
            output := fmt.Sprintf("value:%s", value)

            _, _ = conn.Write([]byte(output))
        } else if strings.HasPrefix(msg, cacheSetPrefix) {
            kvIndex := len(cacheSetPrefix)
            rawKV := msg[kvIndex:]
            kv := strings.Split(rawKV, ":")
            key, value := kv[0], kv[1]

            fmt.Printf("Set key:<%s>/value:<%s>\n", key, value)

            G_Cache.Set(key, value, 0)
            _, _ = conn.Write([]byte("ok"))
            G_Cache.Wait()
        }
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

        G_Cache.Close()

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

    G_Cache, err = ristretto.NewCache(&ristretto.Config[string, string]{
        NumCounters: 1e7,
        MaxCost:     1 << 30,
        BufferItems: 64,
    })
    if err != nil {
        fmt.Printf("ristretto.NewCache: %v\n", err)
        return
    }

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
