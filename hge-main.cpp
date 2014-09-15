/**
 * Author: Hossein Noroozpour Thany Abady
 * Email: Hossein.Noroozpour@gmail.com
 */
#include "hge-main.hpp"
#ifdef ANDROID
#ifdef __cplusplus
extern "C"
{
#endif // End of __cplusplus
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_init (JNIEnv *pEnv, jclass clazz)
	{
		return 1;
	}
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_resize (JNIEnv *pEnv, jclass clazz, jint width, jint height)
	{
		return 1;
	}
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_render (JNIEnv *pEnv, jclass clazz)
	{
		return 1;
	}
#ifdef __cplusplus
}
#endif // End of __cplusplus
#else // End of ANDROID
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	return 0;
}
#endif
