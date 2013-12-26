#include <jni.h>
#include "Hello.h"
#include <stdio.h>
JNIEXPORT void JNICALL Java_Hello_sayHello
  (JNIEnv *env, jobject obj, jint ms)
{
    printf("Hello world %d\n",ms );
    return;
}
