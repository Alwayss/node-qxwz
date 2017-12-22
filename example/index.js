const addon = require('./build/release/qxwz');

const config = {};
config.appkey="xxxxxxx";
config.appSecret="xxxxx";
config.deviceId="xxxx";
config.deviceType="xxxxxx";

addon.start(config, function(code){
   console.log(code); 
   if(code === 1014){
   	addon.sendGGA("$GNGGA,062740.320,3028.854498,N,11424.141741,E,0,00,127.000,-62.620,M,0,M,,*73\r\n");
   }
}, function(buf){
   console.log(buf.lengths);
   addon.stop();
});
