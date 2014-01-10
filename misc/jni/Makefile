libhello.so: Hello.c Hello.h
	gcc -I/usr/lib/jvm/java-7-openjdk-i386/include -o $@  -shared $^ ;\
	sudo cp libhello.so /usr/lib/.; \
	sudo ldconfig;


