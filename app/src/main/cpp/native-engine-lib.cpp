#include <jni.h>

#include "connections/engine.h"

#include <map>

int id = 0;

std::map<int, Engine::Engine*> engines;

extern "C" {

JNIEXPORT jint JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_create(JNIEnv* pEnv, jobject /*thiz*/, jobject assetManager)
{
	Connections::ConnectionsEngine* pEngine = new Connections::ConnectionsEngine;
	AAssetManager* pAssetManager = AAssetManager_fromJava(pEnv, assetManager);
	if(!pEngine->init(pAssetManager))
	{
		SAFE_DELETE(pEngine);
		return -1;
	}

	engines[id] = pEngine;
	int i = id;
	++id;
	return static_cast<jint>(i);
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_destroy(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it == engines.end())
		return;

	SAFE_DELETE(it->second);
	engines.erase(it);
	LOGI("Engine destroyed");
}

JNIEXPORT jboolean JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_render(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it != engines.end())
		return static_cast<jboolean>(it->second->render());
	else
		LOGI("No Engine to render");
	return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_resume(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it != engines.end())
		engines[id]->resume();
	else
		LOGE("No Engine to resume");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_pause(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->pause();
	else
		LOGE("No Engine to pause");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_setSize(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id, int w, int h)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->updateSize(w,h);
	else
		LOGE("No Engine to set size to");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_connections_NativeEngine_onTouch(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id, float x, float y)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->touch(x,y);
	else
		LOGE("No Engine to touch");
}

}