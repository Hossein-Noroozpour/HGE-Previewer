#include "hge-configure.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-shader-engine.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#define HGE_u_sea 0
#define HGE_u_sea_str "0"
#define HGE_sure 1
#define HGE_sure_str "1"
#define HGE_grass1 2
#define HGE_grass1_str "2"
#define HGE_grass2 3
#define HGE_grass2_str "3"
#define HGE_soil 4
#define HGE_soil_str "4"
#define HGE_rock 5
#define HGE_rock_str "5"
#define HGE_snow 6
#define HGE_snow_str "6"
#define HGE_ice 7
#define HGE_ice_str "7"
#define HGE_1tab + shaderEndline + shaderTabCharacter +
#define HGE_2tab + shaderEndline + shaderTabCharacter + shaderTabCharacter +
#define HGE_3tab + shaderEndline + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter +
#define HGE_4tab + shaderEndline + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter +
#define HGE_5tab + shaderEndline + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter +
#define HGE_6tab + shaderEndline + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + shaderTabCharacter + s
hge::shader::TerrainSunShader::TerrainSunShader():
	shaderProgram(render::ShaderEngine::createProgram())
{
	std::string pVS =
			shaderLanguageVersion +
			"layout (location=0) in vec3 " + vertexAttribute + ";" + shaderEndline +
			//All of the normals must be normalized.
			"layout (location=1) in vec3 " + normalAttribute + ";" + shaderEndline +
			//All of the tangents must be normalized.
			"layout (location=2) in vec3 " + tangentAttribute + ";" + shaderEndline +
			//All of the bitangents must be normalized.
			"layout (location=3) in vec3 " + bitangentAttribute + ";" + shaderEndline +
#define VSVERTEXDATA \
			"VertexData" + shaderEndline +\
			"{"\
			HGE_1tab "float " + zAttribute + ";"\
			HGE_1tab "vec2 " + uvAttribute + ";"\
			HGE_1tab "vec3 " + normalAttribute + ";"\
			HGE_1tab "vec3 " + tangentAttribute + ";"\
			HGE_1tab "vec3 " + bitangentAttribute + ";"\
			"} "
			"out " VSVERTEXDATA  + vertexOutName + ";" + shaderEndline +
			"uniform mat4 " + modelViewProjectionMatrixUniformName + ";" + shaderEndline +
#define HGE_START_SHDR_FUN "void main()" + shaderEndline + "{"
			HGE_START_SHDR_FUN
			//HGE_1tab vertexOutName + "." + lightIntensityVarName + " = float(max(float(dot(" + sunDirectionUniformName + ", " + normalAttribute + ")), 0.1));" + shaderEndline +
			HGE_1tab "gl_Position = " + modelViewProjectionMatrixUniformName + " * vec4(" + vertexAttribute + ", 1.0);"
			HGE_1tab vertexOutName + "." + uvAttribute + " = vec2(" + vertexAttribute + ".x + " + vertexAttribute + ".z, " +
				 vertexAttribute + ".y + " + vertexAttribute + ".z);"
			HGE_1tab vertexOutName + "." + normalAttribute + " = " + normalAttribute + ";"
			HGE_1tab vertexOutName + "." + tangentAttribute + " = " + tangentAttribute + ";"
			HGE_1tab vertexOutName + "." + bitangentAttribute + " = " + bitangentAttribute + ";" + shaderEndline +
			"}";
#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "vertext shader is:" << std::endl << pVS << std::endl;
#endif
#ifdef HGE_GEOMETRY_SHADER_SUPPORT
	std::string pGS =
			shaderLanguageVersion +
			"layout (triangles) in;" + shaderEndline +
			"layout (triangle_strip, max_vertices=3) out;" + shaderEndline +
			"in " VSVERTEXDATA  + vertexInName + "[];" + shaderEndline +
#define GSVERTEXDATA \
			"VertexData" + shaderEndline +\
			"{"\
			HGE_1tab "float " + zAttribute + ";"\
			HGE_1tab "vec2 " + uvAttribute + ";"\
			HGE_1tab "vec3 " + normalAttribute + ";"\
			HGE_1tab "vec3 " + tangentAttribute + ";"\
			HGE_1tab "vec3 " + bitangentAttribute + ";"\
			"} "
			"out " GSVERTEXDATA  + vertexOutName + ";" + shaderEndline +
			HGE_START_SHDR_FUN
			HGE_1tab "for(int i = 0; i < gl_in.length(); i++)"
			HGE_1tab "{"
				HGE_2tab "gl_Position = gl_in[i].gl_Position;"
				HGE_2tab vertexOutName + "." + uvAttribute + " = " + vertexInName + "[i]." + uvAttribute + ";"
				HGE_2tab vertexOutName + "." + normalAttribute + " = " + vertexInName + "[i]." + normalAttribute + ";"
				HGE_2tab vertexOutName + "." + tangentAttribute + " = " + vertexInName + "[i]." + tangentAttribute + ";"
				HGE_2tab vertexOutName + "." + bitangentAttribute + " = " + vertexInName + "[i]." + bitangentAttribute + ";"
				HGE_2tab "EmitVertex();"
			HGE_1tab "}"
			HGE_1tab "EndPrimitive();" + shaderEndline +
			"}";

#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "geometry shader is:" << std::endl << pGS << std::endl;
#endif
#endif
	std::string pFS =
			shaderLanguageVersion +
#ifdef HGE_GEOMETRY_SHADER_SUPPORT
			"in " GSVERTEXDATA + vertexInName + ";" + shaderEndline +
#else
			"in " VSVERTEXDATA + vertexInName + ";" + shaderEndline +
#endif
#define HGE_V_I vertexInName + "." +
#define HGE_V_I_z   HGE_V_I zAttribute +
#define HGE_V_I_uv  HGE_V_I uvAttribute +
#define HGE_V_I_nrm HGE_V_I normalAttribute +
#define HGE_V_I_tng HGE_V_I tangentAttribute +
#define HGE_V_I_btg HGE_V_I bitangentAttribute +
			"const float limits [] = " + shaderEndline +
			"{"
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_0 ","
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_1 ","
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_2 ","
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_3 ","
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_4 ","
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_5 ","
			HGE_1tab HGE_TERRAIN_HEIGHT_LIMIT_6 + shaderEndline +
			"};" + shaderEndline +
			"out vec4 fragColor;" + shaderEndline +
			"uniform sampler2D " + textureSampler + "[" HGE_TERRAIN_TEXTURE_COUNT_STRING "];" + shaderEndline +
			"uniform mat4 " + sunDirectionUniformName + ";" + shaderEndline +
			HGE_START_SHDR_FUN
			HGE_1tab "if(" + HGE_V_I_nrm + ".z > " HGE_TERRAIN_SLOP_LIMIT_0 ")"
			HGE_1tab "{"
				HGE_2tab "fragColor = texture2D(" + textureSampler + "[" HGE_rock_str "], " + HGE_V_I_uv ");"
			HGE_1tab "}"
			HGE_1tab "else"
			HGE_1tab "{"
				HGE_2tab "vec4 tmpcolor;"
				HGE_2tab "if(" + HGE_V_I_z " >= " HGE_TERRAIN_HEIGHT_LIMIT_6 " + " HGE_TERRAIN_HEIGHT_FADE_LIMIT ")"
				HGE_2tab "{"
					HGE_3tab "fragColor = texture2D(" + textureSampler + "[" HGE_ice_str "], " + HGE_V_I_uv ");"
				HGE_2tab "}"
				HGE_2tab "else"
				HGE_2tab "{"
					HGE_3tab "for(int i = 0; i < " HGE_TERRAIN_TEXTURE_COUNT_STRING " - 1; i++)"
					HGE_3tab "{"
						HGE_4tab "if(" + HGE_V_I_z " < limits[i])"
						HGE_4tab "{"
							HGE_5tab "tmpcolor = texture2D(" + textureSampler + "[i], " + HGE_V_I_uv ");"
							HGE_5tab "break;"
						HGE_4tab "}"
						HGE_4tab "if(" + HGE_V_I_z " < limits[i] + " HGE_TERRAIN_HEIGHT_FADE_LIMIT ")"
						HGE_4tab "{"
							HGE_5tab "float r = (" + HGE_V_I_z " - limits[i]) / " HGE_TERRAIN_HEIGHT_FADE_LIMIT ";"
							HGE_5tab "vec4 c1 = texture2D(" + textureSampler + "[i], " + HGE_V_I_uv ");"
							HGE_5tab "vec4 c2 = texture2D(" + textureSampler + "[i+1], " + HGE_V_I_uv ");"
							HGE_5tab "tmpcolor = mix(c1, c2, r);"
							HGE_5tab "break;"
						HGE_4tab "}"
					HGE_3tab "}"
				HGE_2tab "}"
				HGE_2tab "if(" + HGE_V_I_nrm + ".z > " HGE_TERRAIN_SLOP_LIMIT_0 " + " HGE_TERRAIN_SLOP_FADE_LIMIT ")"
				HGE_2tab "{"
					HGE_3tab "float r = (" + HGE_V_I_nrm + ".z  - " HGE_TERRAIN_SLOP_LIMIT_0 ") / " HGE_TERRAIN_SLOP_FADE_LIMIT ";"
					HGE_3tab "vec4 c = texture2D(" + textureSampler + "[" HGE_rock_str "], " + HGE_V_I_uv ");"
					HGE_3tab "fragColor = mix(c, tmpcolor, r);"
				HGE_2tab "}"
				HGE_2tab "else"
				HGE_2tab "{"
					HGE_3tab "fragColor = tmpcolor;"
				HGE_2tab "}"
			HGE_1tab "}"
			HGE_1tab "if(" + HGE_V_I_nrm + ".z > -200000)"
			HGE_1tab "{"
				HGE_2tab "fragColor = vec4(0.0);"
			HGE_1tab "}" + shaderEndline +
			"}";

#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "fragment shader is:" << std::endl << pFS << std::endl;
#endif
#undef HGE_V_I_z
#undef HGE_V_I_uv
#undef HGE_V_I_nrm
#undef HGE_V_I_tng
#undef HGE_V_I_btg
#undef HGE_V_I
#undef HGE_START_SHDR_FUN
	vertexShaderProgram = render::ShaderEngine::addShaderToProgram(pVS, GL_VERTEX_SHADER, shaderProgram);
#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "Vertext shader compiled." << std::endl;
#endif
#ifdef HGE_GEOMETRY_SHADER_SUPPORT
	geometryShaderProgram = render::ShaderEngine::addShaderToProgram(pGS, GL_GEOMETRY_SHADER, shaderProgram);
#endif
#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "Geometry shader compiled." << std::endl;
#endif
	fragmentShaderProgram = render::ShaderEngine::addShaderToProgram(pFS, GL_FRAGMENT_SHADER, shaderProgram);
#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "Fragment shader compiled." << std::endl;
#endif
	hge::render::ShaderEngine::run(shaderProgram);
	mvpmul = render::ShaderEngine::getUniformLocation(modelViewProjectionMatrixUniformName, shaderProgram);
#ifdef HGE_DEBUG_MODE
	assert(mvpmul != 0xFFFFFFFF);
#endif
	glUniformMatrix4fv(mvpmul, 1, GL_FALSE, math::Matrix4D<>(1.0f).mat);
	/// \todo
//	cprtmul = render::ShaderEngine::getUniformLocation(std::string("cprtm"), shaderProgram);
//#ifdef HGE_DEBUG_MODE
//	assert(cprtmul != 0xFFFFFFFF);
//#endif
//	glUniform3fv(cprtmul, 1, math::Vector3D<>(1.0f).vec);
//	lodnul = render::ShaderEngine::getUniformLocation(std::string("lodn"), shaderProgram);
//#ifdef HGE_DEBUG_MODE
//	assert(lodnul != 0xFFFFFFFF);
//#endif
//	glUniform1i(lodnul, 0);
	for(GLuint i = 0; i < HGE_TERRAIN_TEXTURE_COUNT; i++)
	{
		tsl[i] = render::ShaderEngine::getUniformLocation(textureSampler + "[" + std::to_string(i) + "]", shaderProgram);
#ifdef HGE_DEBUG_MODE
		assert(tsl[i] != 0xFFFFFFFF);
#endif
		glUniform1i(tsl[i], i);
	}
}
hge::shader::TerrainSunShader::~TerrainSunShader()
{
	hge::render::ShaderEngine::endObject(vertexShaderProgram);
	hge::render::ShaderEngine::endObject(geometryShaderProgram);
	hge::render::ShaderEngine::endObject(fragmentShaderProgram);
	hge::render::ShaderEngine::endProgram(shaderProgram);
}
void hge::shader::TerrainSunShader::setModelMatrix(const math::Matrix4D<> &modelMatrix)
{
	glUniformMatrix4fv(mmul, 1, GL_FALSE, modelMatrix.mat);
}
void hge::shader::TerrainSunShader::setModelViewProjectionMatrix(const math::Matrix4D<> &modelViewProjectionMatrix)
{
	glUniformMatrix4fv(mvpmul, 1, GL_FALSE, modelViewProjectionMatrix.mat);
}
void hge::shader::TerrainSunShader::setCameraPositionRelativeToModel(const math::Vector3D<>& p)
{
	glUniform3fv(cprtmul, 1, p.vec);
}
void hge::shader::TerrainSunShader::setLODNumber(const GLuint& lodNumber)
{
	glUniform1i(lodnul, lodNumber);
}
void hge::shader::TerrainSunShader::use()
{
	glUseProgram(shaderProgram);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, HGE_TERRAIN_COMPONENT_COUNT * sizeof(GLfloat), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, HGE_TERRAIN_COMPONENT_COUNT * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, HGE_TERRAIN_COMPONENT_COUNT * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, HGE_TERRAIN_COMPONENT_COUNT * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
}
GLuint hge::shader::TerrainSunShader::getModelViewProjectionMatrixUniformLocation()
{
	return mvpmul;
}
GLuint hge::shader::TerrainSunShader::getModelMatrixUniformLoaction()
{
	return mmul;
}
GLuint hge::shader::TerrainSunShader::getVertexShaderProgram()
{
	return vertexShaderProgram;
}
GLuint hge::shader::TerrainSunShader::getGeometryShaderProgram()
{
	return geometryShaderProgram;
}
GLuint hge::shader::TerrainSunShader::getFragmentShaderProgram()
{
	return fragmentShaderProgram;
}
GLuint hge::shader::TerrainSunShader::getShaderProgram()
{
	return shaderProgram;
}
GLuint* hge::shader::TerrainSunShader::getTextureSamplerLocations()
{
	return tsl;
}
std::string hge::shader::TerrainSunShader::readShaderFile(const std::string &fileName)
{
	std::string result;
	std::ifstream in(fileName, std::ios::in);
	if(in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
		return result;
	}
	std::terminate();
}
