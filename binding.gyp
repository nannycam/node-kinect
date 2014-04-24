{
	"targets": [
		{
			"target_name": "hello",
			"sources": [ "src/hello.cc" ],
			"libraries": [ "-lfreenect" ],
			"cflags!": [ "-fno-exceptions" ],
			"cflags_cc!": [ "-fno-exceptions" ]
		}
	]
}
