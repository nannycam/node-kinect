
#include <node/node.h>
#include <node/node_buffer.h>
#include <node/v8.h>

#include <libfreenect/libfreenect.h>

using namespace v8;

freenect_context *f_ctx;
freenect_device *f_dev;

uint8_t *rgb_back;

Persistent<Function> callback;

void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp) {
	HandleScope scope;
	const unsigned argc = 1;
	Local<Value> argv[argc] = { Local<Value>::New(node::Buffer::New((char*)rgb, 640*480*3)->handle_) };
	if (!callback.IsEmpty())
		callback->Call(Context::GetCurrent()->Global(), argc, argv);
}

Handle<Value> startKinect(const Arguments& args) {
	HandleScope scope;

	if (freenect_init(&f_ctx, NULL) != 0) {
		ThrowException(Exception::Error(String::New("Context not initialized")));
		return scope.Close(Undefined());
	}

	int numDevices = freenect_num_devices(f_ctx);
	printf("Found %d device(s)\n", numDevices);

	if (freenect_open_device(f_ctx, &f_dev, 0) != 0) {
		ThrowException(Exception::Error(String::New("Couldn't init device")));
		return scope.Close(Undefined());
	}

	freenect_set_video_callback(f_dev, rgb_cb);
	freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
	freenect_set_video_buffer(f_dev, rgb_back);
	freenect_start_video(f_dev);

	freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT));
	freenect_start_depth(f_dev);

	return scope.Close(Number::New(0));
}

Handle<Value> runKinect(const Arguments& args) {
	HandleScope scope;
	return scope.Close(Number::New(freenect_process_events(f_ctx)));
}

Handle<Value> stopKinect(const Arguments& args) {
	HandleScope scope;

	freenect_stop_depth(f_dev);
	freenect_stop_video(f_dev);

	freenect_close_device(f_dev);
	freenect_shutdown(f_ctx);

	return scope.Close(Undefined());
}

Handle<Value> setFrameCallback(const Arguments& args) {
	HandleScope scope;

	if (!args[0]->IsFunction()) {
		ThrowException(Exception::TypeError(String::New("Argument is not of type Function")));
		return scope.Close(Undefined());
	}

	callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));

	return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("startKinect"), FunctionTemplate::New(startKinect)->GetFunction());
	exports->Set(String::NewSymbol("runKinect"), FunctionTemplate::New(runKinect)->GetFunction());
	exports->Set(String::NewSymbol("stopKinect"), FunctionTemplate::New(stopKinect)->GetFunction());
	exports->Set(String::NewSymbol("setFrameCallback"), FunctionTemplate::New(setFrameCallback)->GetFunction());
}

NODE_MODULE(hello, init)
