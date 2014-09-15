#if defined(WIN64) || defined(WIN32) || defined(LINUX)
#define HGE_USE_OPENGL_43
//#define HGE_USE_OPENGL_42
//#define HGE_USE_OPENGL_41
//#define HGE_USE_OPENGL_40
//#define HGE_USE_OPENGL_33
#elif defined(ANDROID)
//#define HGE_USE_OPENGL_ES_3
//#define HGE_USE_OPENGL_ES_2
#endif
#if defined(HGE_USE_OPENGL_33) || defined(HGE_USE_OPENGL_40) || defined(HGE_USE_OPENGL_41) || defined(HGE_USE_OPENGL_42) || defined(HGE_USE_OPENGL_43) || defined(HGE_USE_OPENGL_ES_3)
#define HGE_BASIC_QUERY_SUPPORT 
#endif
#if defined(HGE_USE_OPENGL_33) || defined(HGE_USE_OPENGL_40) || defined(HGE_USE_OPENGL_41) || defined(HGE_USE_OPENGL_42) || defined(HGE_USE_OPENGL_43)
#define HGE_CONDITIONAL_RENDERING_SUPPORT
#endif