var mqtt = require('mqtt');

var client  = mqtt.connect('mqtt://api.mesosfer.com', { username:'[username]', password:'[password]' });

client.on('connect', function () {
    console.log("Connected. Waiting incoming message..");
    client.subscribe('device/[devicename]/[bucket]');    
})
client.on('message', function (topic, message) {
    console.log("message: " + message.toString())    
});
 
process.on('SIGINT', function() {
  client.end();  
  console.log("application exit");
  process.exit();
});
