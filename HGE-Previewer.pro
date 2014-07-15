TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LIBS += -lGLEW -lglfw -lGL -lfreeimage -lboost_system
SOURCES += \
	hge-camera-unit.cpp \
	hge-director.cpp \
	hge-geometry-unit.cpp \
	hge-main.cpp \
	hge-mesh-unit.cpp \
	hge-model-unit.cpp \
	hge-perspective-unit.cpp \
	hge-scene-unit.cpp \
	hge-shader-engine.cpp \
	hge-shader-unit.cpp \
	hge-skybox-shader-unit.cpp \
	hge-skybox-unit.cpp \
	hge-sun-shader.cpp \
	hge-terrain-sun-shader.cpp \
	hge-terrain-unit.cpp \
	hge-texture-unit.cpp \
	hge-texture-util.cpp \
	hge-vertex-data.cpp \
	hge-white-shader.cpp \
	hge-tcp-connector.cpp

OTHER_FILES += \
	LICENSE \
	README.md

HEADERS += \
	hge-actor-unit.hpp \
	hge-application-unit.hpp \
	hge-camera-unit.hpp \
	hge-director.hpp \
	hge-geometry-unit.hpp \
	hge-math.hpp \
	hge-mesh-unit.hpp \
	hge-model-unit.hpp \
	hge-perspective-unit.hpp \
	hge-scene-unit.hpp \
	hge-shader-engine.hpp \
	hge-shader-unit.hpp \
	hge-skybox-shader-unit.hpp \
	hge-skybox-unit.hpp \
	hge-sun-shader.hpp \
	hge-terrain-sun-shader.hpp \
	hge-terrain-unit.hpp \
	hge-texture-unit.hpp \
	hge-texture-util.hpp \
	hge-vertex-data.hpp \
	hge-white-shader.hpp \
	hge-protocol.hpp \
	hge-connector.hpp \
	hge-tcp-connector.hpp