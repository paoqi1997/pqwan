#!/bin/bash

protoc --go_out=./pprotos --go_opt=paths=source_relative \
  --go-grpc_out=./pprotos --go-grpc_opt=paths=source_relative \
  ./hello.proto

protoc --go_out=./pprotos --go_opt=paths=source_relative \
  --go-grpc_out=./pprotos --go-grpc_opt=paths=source_relative \
  ./player.proto
