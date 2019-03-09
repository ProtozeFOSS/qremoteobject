import qbs
Project{
    name:"jagwire_example"
    SubProject{
        filePath: "../../qremoteobject.qbs"
    }
    SubProject{
        filePath: "key_client/key_client.qbs"
    }
    SubProject{
        filePath: "car_server/car_server.qbs"
    }

}
