const util = require('util');
const thrift = require('thrift');
const Client = require('./gen-nodejs/RemoteShellService');

let connection = thrift.createConnection('localhost', 9090)
let client = thrift.createClient(Client, connection);

async function runClientMethod(client, methodName, ...args) {
    process.stdout.write("RUN [" + methodName + "] => ");
    try {
        let fn = util.promisify(client[methodName]);
        let res = await fn.call(client, ...args);
        process.stdout.write(JSON.stringify(res));
    } catch (err) {
        process.stdout.write("" + err);
        process.exit(0);
    }
    console.log("");
}

async function run_all_test() {
    await runClientMethod(client, "GetComputerInfo");
    await runClientMethod(client, "GetCpuInfo");
    await runClientMethod(client, "GetMemoryInfo");
    await runClientMethod(client, "GetDiskInfo", "c:/");
    await runClientMethod(client, "GetNetworkInfo");
    await runClientMethod(client, "Execute", "date /T");
    connection.end();
}

run_all_test();