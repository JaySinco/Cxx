const thrift = require('thrift');
const Client = require('./gen-nodejs/RemoteShellService');

let connection = thrift.createConnection('localhost', 9090)
let client = thrift.createClient(Client, connection);

connection.on('error', function (err) {
    console.error(err);
});

function test_GetComputerInfo() {
    client.GetComputerInfo(
        function (err, response) {
            console.log("[GetComputerInfo]", err, response);
            connection.end();
        }
    )
}

test_GetComputerInfo();
