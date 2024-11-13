package main

import (
    "context"
    "encoding/json"
    "fmt"

    "go.mongodb.org/mongo-driver/bson"
    "go.mongodb.org/mongo-driver/bson/primitive"
    "go.mongodb.org/mongo-driver/mongo"
    "go.mongodb.org/mongo-driver/mongo/options"
)

type DBLayer struct {
    client      *mongo.Client
    dbName      string
    collections map[string]*mongo.Collection
}

func NewDBLayer(uri string) *DBLayer {
    client, err := mongo.Connect(context.TODO(), options.Client().ApplyURI(uri))
    if err != nil {
        fmt.Printf("mongo.Connect: %v\n", err)
        return nil
    }

    layer := &DBLayer{
        client:      client,
        dbName:      "pq",
        collections: map[string]*mongo.Collection{},
    }

    layer.AddCollections()

    return layer
}

func (L *DBLayer) AddCollections() {
    L.AddCollection("players")
}

func (L *DBLayer) AddCollection(collectionName string) {
    L.collections[collectionName] = L.client.Database(L.dbName).Collection(collectionName)
}

func (L *DBLayer) Collection(collectionName string) *mongo.Collection {
    return L.collections[collectionName]
}

func (L *DBLayer) Disconnect() {
    if err := L.client.Disconnect(context.TODO()); err != nil {
        fmt.Printf("Disconnect: %v\n", err)
    }
}

func (L *DBLayer) GetOne(collectionName string, id uint32) []byte {
    var getResult bson.M

    paramD := bson.D{
        { Key: "id", Value: id },
    }

    err := L.Collection(collectionName).FindOne(context.TODO(), paramD).Decode(&getResult)
    if err != nil {
        if err == mongo.ErrNoDocuments {
            fmt.Printf("[DBLayer][GetOne] %s.FindOne: %d not found\n", collectionName, id)
        } else {
            fmt.Printf("[DBLayer][GetOne] %s.FindOne: %v\n", collectionName, err)
        }

        return []byte("{}")
    }

    jsonData, err := json.Marshal(getResult)
    if err != nil {
        fmt.Printf("[DBLayer][GetOne] json.Marshal: %v\n", err)
        return []byte("{}")
    }

    return jsonData
}

func (L *DBLayer) InsertOne(collectionName string, inJsonData []byte) []byte {
    var doc interface{}

    err := bson.UnmarshalExtJSON(inJsonData, true, &doc)
    if err != nil {
        fmt.Printf("[DBLayer][InsertOne] bson.Unmarshal: %v\n", err)
        return []byte("{}")
    }

    insertResult, err := L.Collection(collectionName).InsertOne(context.TODO(), &doc)
    if err != nil {
        fmt.Printf("[DBLayer][InsertOne] %s.InsertOne: %v\n", collectionName, err)
        return []byte("{}")
    }

    insertedID := insertResult.InsertedID.(primitive.ObjectID).String()

    return []byte(fmt.Sprintf("{\"insertedId\":%s}", insertedID))
}

func (L *DBLayer) UpdateOne(collectionName string, id uint32, inJsonData []byte) []byte {
    filter := bson.D{{ Key: "id", Value: id }}

    var inData map[string]interface{}

    err := json.Unmarshal(inJsonData, &inData)
    if err != nil {
        fmt.Printf("[DBLayer][UpdateOne] json.Unmarshal: %v\n", err)
        return []byte("{}")
    }

    updateKVs := bson.D{}

    for k, v := range inData {
        updateKVs = append(updateKVs, bson.E{ Key: k, Value: v })
    }

    updateExp := bson.D{{ Key: "$set", Value: updateKVs }}

    updateResult, err := L.Collection(collectionName).UpdateOne(context.TODO(), filter, updateExp)
    if err != nil {
        fmt.Printf("[DBLayer][UpdateOne] %s.UpdateOne: %v\n", collectionName, err)
        return []byte("{}")
    }

    myUpdateResult := map[string]interface{}{
        "MatchedCount":  updateResult.MatchedCount,
        "ModifiedCount": updateResult.ModifiedCount,
        "UpsertedCount": updateResult.UpsertedCount,
    }

    jsonReply, err := json.Marshal(myUpdateResult)
    if err != nil {
        fmt.Printf("[DBLayer][UpdateOne] json.Marshal: %v\n", err)
        return []byte("{}")
    }

    return jsonReply
}
