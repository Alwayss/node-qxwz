#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <node.h>
#include <iostream>
#include <string>
#include <stdint.h>

#include "qxwz_rtcm.h"

#undef QXLOGI
#define QXLOGI printf

namespace qxwz {
	using namespace std;
	using namespace v8;

	Isolate* isolate;
	Local<Function> statusCallbacks;
	Local<Function> dataCallbacks;
	const unsigned argc = 1;
	Local<Value> statusArgv[argc];	
	Local<Value> dataArgv[argc];

	void start(const FunctionCallbackInfo<Value>& args) {

	    isolate = args.GetIsolate();
	    Local<Object> options = Local<Object>::Cast(args[0]);
	    statusCallbacks = Local<Function>::Cast(args[1]);
	    dataCallbacks = Local<Function>::Cast(args[2]);

	    //获取qxwz的配置参数
	    Local<String> appkeyStr = Local<String>::Cast(options->Get(String::NewFromUtf8(isolate, "appkey")));
	    String::Utf8Value appkey(appkeyStr->ToString());

	    Local<String> appSecretStr = Local<String>::Cast(options->Get(String::NewFromUtf8(isolate, "appSecret")));
	    String::Utf8Value appSecret(appSecretStr->ToString());

	    Local<String> deviceIdStr = Local<String>::Cast(options->Get(String::NewFromUtf8(isolate, "deviceId")));
	    String::Utf8Value deviceId(deviceIdStr->ToString());

	    Local<String> deviceTypeStr = Local<String>::Cast(options->Get(String::NewFromUtf8(isolate, "deviceType")));
	    String::Utf8Value deviceType(deviceTypeStr->ToString());

	    qxwz_config config;
	    config.appkey = *appkey;
	    config.appSecret = *appSecret;
	    config.deviceId = *deviceId;
	    config.deviceType = *deviceType;
	    qxwz_setting(&config);
	
	    qxwz_rtcm_start([](qxwz_rtcm data){
		QXLOGI("QXWZ_RTCM_DATA:%s\n", data.buffer);
	        QXLOGI("QXWZ_RTCM_LENGTH:%ld\n", data.length);

		Local<ArrayBuffer> buffers = ArrayBuffer::New(isolate, data.length);
	    	for(size_t i = 0; i < data.length; i++){
		     buffers->Set(i, Number::New(isolate, data.buffer[i]));
	   	}
		dataArgv[0] = buffers;
		dataCallbacks->Call(Null(isolate), argc, dataArgv);  
	
	    }, [](qxwz_rtcm_status code){
		QXLOGI("QXWZ_RTCM_STATUS:%d\n", code);  
		statusArgv[0] = { Number::New(isolate, code) };
	    	statusCallbacks->Call(Null(isolate), argc, statusArgv);  
	    });	
	}

	void sendGGA(const FunctionCallbackInfo<Value>& args) {

	    Local<String> gga = Local<String>::Cast(args[0]);
	    String::Utf8Value ggastring(gga->ToString());

	    printf("Start send GGA done\r\n");
	    qxwz_rtcm_sendGGAWithGGAString(*ggastring);
	}

	void stop(const FunctionCallbackInfo<Value>& args){
	    //关闭rtcm sdk
	    printf("qxwz_rtcm_stop here\r\n");
	    //sleep(1);
	    qxwz_rtcm_stop();
	    printf("qxwz_rtcm_stop done\r\n");	
	}

	void init(Local<Object> exports) {
	    NODE_SET_METHOD(exports, "start", start);
	    NODE_SET_METHOD(exports, "sendGGA", sendGGA);
	    NODE_SET_METHOD(exports, "stop", stop);
	}

	NODE_MODULE(qxwz, init)
}
