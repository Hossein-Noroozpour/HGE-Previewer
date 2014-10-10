#ifndef HGE_CONFIGURE_HPP
#define HGE_CONFIGURE_HPP
#define HGE_USE_OPENGL
/// Check bug conditions that may occur in program, it has perfomance penalty remove it in release
#define HGE_DEBUG_MODE
#ifdef HGE_DEBUG_MODE
/// Intensively check bug conditions that may occur in program, it has high perfomance penalty remore it in release
#define HGE_INTENSIVE_DEBUG_MODE
#endif
/// Show information about the excecution of program
#define HGE_TEST_MODE
#ifdef HGE_TEST_MODE
/// Show lots of information about the excecution of program
#define HGE_VERBOSE_TEST_MODE
#endif
#if !defined(HGE_USE_OPENGL) && (defined(_WIN64) || defined(_WIN32))
#define HGE_USE_DIRECTX_11 // do not use it it's not completed yet.
#ifndef HGE_USE_DIRECTX_11
#define HGE_USE_DIRECTX_10 // do not use it it's not completed yet.
#endif
#elif defined(HGE_USE_OPENGL) || defined(LINUX)
#define HGE_USE_OPENGL_43
#if !defined(HGE_USE_OPENGL_43)
#define HGE_USE_OPENGL_42
#endif
#if !defined(HGE_USE_OPENGL_43) && !defined(HGE_USE_OPENGL_42)
#define HGE_USE_OPENGL_41
#endif
#if !defined(HGE_USE_OPENGL_43) && !defined(HGE_USE_OPENGL_42) && !defined(HGE_USE_OPENGL_41)
#define HGE_USE_OPENGL_40
#endif
#if !defined(HGE_USE_OPENGL_43) && !defined(HGE_USE_OPENGL_42) && !defined(HGE_USE_OPENGL_41) && !defined(HGE_USE_OPENGL_40)
#define HGE_USE_OPENGL_33
#endif
#elif defined(ANDROID)
#define HGE_USE_OPENGL_ES_3
#ifndef HGE_USE_OPENGL_ES_3
#define HGE_USE_OPENGL_ES_2
#endif
#endif
#if defined(HGE_USE_OPENGL_33) || defined(HGE_USE_OPENGL_40) || defined(HGE_USE_OPENGL_41) || defined(HGE_USE_OPENGL_42) || defined(HGE_USE_OPENGL_43) || defined(HGE_USE_OPENGL_ES_3)
#define HGE_BASIC_QUERY_SUPPORT
#endif
#if defined(HGE_USE_OPENGL_33) || defined(HGE_USE_OPENGL_40) || defined(HGE_USE_OPENGL_41) || defined(HGE_USE_OPENGL_42) || defined(HGE_USE_OPENGL_43)
#define HGE_CONDITIONAL_RENDERING_SUPPORT
#define HGE_GEOMETRY_SHADER_SUPPORT
#endif
#define HGE_PRINT_CODE_LINE std::cout << __FILE__ << ": line:" << __LINE__ << std::endl;
#define HGE_FILE_LINE std::cout << __FILE__ << "(" << __LINE__ << "):\t\t\t" <<
/// Terrain configurator definitions
#define HGE_TERRAIN_COMPONENT_COUNT 12
#define HGE_TERRAIN_TEXTURE_COUNT 8
#define HGE_TERRAIN_TEXTURE_COUNT_STRING "8"
#define HGE_TERRAIN_HEIGHT_LIMIT_0 "5.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_1 "8.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_2 "10.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_3 "50.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_4 "100.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_5 "250.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_6 "300.0"
#define HGE_TERRAIN_HEIGHT_LIMIT_7 "350.0"
#define HGE_TERRAIN_HEIGHT_FADE_LIMIT "1.0"
#define HGE_TERRAIN_SLOP_LIMIT_0 "0.5"
#define HGE_TERRAIN_SLOP_FADE_LIMIT "0.1"
#endif // HGE_CONFIGURE_HPP
