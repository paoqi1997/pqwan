#!/bin/bash

protoc --go_out=./protos --go_opt=paths=source_relative \
  --go-grpc_out=./protos --go-grpc_opt=paths=source_relative \
  ./hello.proto

protoc --go_out=./protos --go_opt=paths=source_relative \
  --go-grpc_out=./protos --go-grpc_opt=paths=source_relative \
  ./player.proto
