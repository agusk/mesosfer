var mqtt = require('mqtt');

var client  = mqtt.connect('mqtt://api.mesosfer.com', { username:'[username]', password:'[password]' });

client.on('connect', function () {
    console.log('mqtt was connectted');
    setInterval(sendMsg, 10000);
});
client.on('packetsend', function () {  
  console.log("===> message was sent!");
});
client.on('error', function (error) {
    console.log('Error was occured');
    console.log(error);
});

var sendMsg = function sendTemperature(){
    console.log("sending message...");    
    var humidity = Math.floor((Math.random() * 60) + 50);
    var temperature = Math.floor((Math.random() * 30) + 15);

    client.publish('device/[devicename]/[bucket]', 
    '{"appId":"[appId]", "schemaId":"[schemaId]", "bucket":"[bucket]", "metadata":{"humidity":"' 
    + humidity.toString()+'","temperature":"' + temperature.toString()+'"}}');    
};
 
process.on('SIGINT', function() {
  client.end();
  clearInterval(sendMsg);  
  console.log("application exit");
  process.exit();
});



