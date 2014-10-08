#include "hge-configure.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-shader-engine.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
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
			"{" + shaderEndline +\
				"vec2 " + uvAttribute + ";" + shaderEndline +\
				"vec3 " + normalAttribute + ";" + shaderEndline +\
				"vec3 " + tangentAttribute + ";" + shaderEndline +\
				"vec3 " + bitangentAttribute + ";" + shaderEndline +\
			"} "
			"out " VSVERTEXDATA  + vertexOutName + ";" + shaderEndline +
			"uniform mat4 " + modelViewProjectionMatrixUniformName + ";" + shaderEndline +
			"uniform mat4 " + sunDirectionUniformName + ";" + shaderEndline +
			"void main()" + shaderEndline +
			"{" + shaderEndline +
				// vertexOutName + "." + lightIntensityVarName + " = float(max(float(dot(" + sunDirectionUniformName + ", " + normalAttribute + ")), 0.1));" + shaderEndline +
				"gl_Position = " + modelViewProjectionMatrixUniformName + " * vec4(" + vertexAttribute + ", 1.0);" + shaderEndline +
				vertexOutName + "." + uvAttribute + " = vec2(" + vertexAttribute + ".x + " + vertexAttribute + ".z, " +
					 + vertexAttribute + ".y + " + vertexAttribute + ".z);" + shaderEndline +
				vertexOutName + "." + normalAttribute + " = " + normalAttribute + ";" + shaderEndline +
				vertexOutName + "." + tangentAttribute + " = " + tangentAttribute + ";" + shaderEndline +
				vertexOutName + "." + bitangentAttribute + " = " + bitangentAttribute + ";" + shaderEndline +
			"}";
#ifdef HGE_GEOMETRY_SHADER_SUPPORT
	std::string pGS =
			shaderLanguageVersion +
			"layout (triangles) in;" + shaderEndline +
			"layout (triangle_strip, max_vertices=3) out;" + shaderEndline +
			"in " VSVERTEXDATA  + vertexInName + ";" + shaderEndline +
#define GSVERTEXDATA \
			"VertexData" + shaderEndline +\
			"{" + shaderEndline +\
			shaderTabCharacter + "vec2 " + uvAttribute + ";" + shaderEndline +\
			shaderTabCharacter + "vec3 " + normalAttribute + ";" + shaderEndline +\
			shaderTabCharacter + "vec3 " + tangentAttribute + ";" + shaderEndline +\
			shaderTabCharacter + "vec3 " + bitangentAttribute + ";" + shaderEndline +\
			"} "
			"out " GSVERTEXDATA  + vertexOutName + ";" + shaderEndline +
			"void main()" + shaderEndline +
			"{" + shaderEndline +
			shaderTabCharacter + "for(int i = 0; i < gl_in.length(); i++)" + shaderEndline +
			shaderTabCharacter + "{" + shaderEndline +
			shaderTabCharacter + shaderTabCharacter + "gl_Position = gl_in[i].gl_Position;" + shaderEndline +
			shaderTabCharacter + shaderTabCharacter + vertexOutName + "." + uvAttribute + " = " + vertexInName + "." + uvAttribute + ";" + shaderEndline +
			shaderTabCharacter + shaderTabCharacter + vertexOutName + "." + normalAttribute + " = " + vertexInName + "." + normalAttribute + ";" + shaderEndline +
			shaderTabCharacter + shaderTabCharacter + vertexOutName + "." + tangentAttribute + " = " + vertexInName + "." + tangentAttribute + ";" + shaderEndline +
			shaderTabCharacter + shaderTabCharacter + vertexOutName + "." + bitangentAttribute + " = " + vertexInName + "." + bitangentAttribute + ";" + shaderEndline +
			shaderTabCharacter + shaderTabCharacter + "EmitVertex();" + shaderEndline +
			shaderTabCharacter + "}" +shaderEndline +
			shaderTabCharacter + "EndPrimitive();" + shaderEndline +
			"}";
#endif
	std::string pFS = readShaderFile(shLoc + "hge-terrain-shader-string.fragmentShader");
	vertexShaderProgram = render::ShaderEngine::addShaderToProgram(pVS, GL_VERTEX_SHADER, shaderProgram);
#ifdef HGE_TEST_MODE
	HGE_FILE_LINE "Vertext shader compiled." << std::endl;
#endif
#ifdef ANDROID
#else
	geometryShaderProgram = render::ShaderEngine::addShaderToProgram(pGS, GL_GEOMETRY_SHADER, shaderProgram);
#endif
#ifdef HGETERRAINSUNSHADERDEBUGMODE
	std::cout << "Geometry shader compiled." << std::endl;
	HGEPRINTCODELINE
#endif
	fragmentShaderProgram = render::ShaderEngine::addShaderToProgram(pFS, GL_FRAGMENT_SHADER, shaderProgram);
#ifdef HGETERRAINSUNSHADERDEBUGMODE
	std::cout << "Fragment shader compiled." << std::endl;
	HGEPRINTCODELINE
#endif
	hge::render::ShaderEngine::run(shaderProgram);
	mvpmul = render::ShaderEngine::getUniformLocation(std::string("mvpm"), shaderProgram);
#ifdef HGETERRAINSUNSHADERDEBUGMODE
	assert(mvpmul != 0xFFFFFFFF);
#endif
	glUniformMatrix4fv(mvpmul, 1, GL_FALSE, math::Matrix4D<>(1.0f).mat);
	cprtmul = render::ShaderEngine::getUniformLocation(std::string("cprtm"), shaderProgram);
#ifdef HGETERRAINSUNSHADERDEBUGMODE
	assert(cprtmul != 0xFFFFFFFF);
#endif
	glUniform3fv(cprtmul, 1, math::Vector3D<>(1.0f).vec);
	lodnul = render::ShaderEngine::getUniformLocation(std::string("lodn"), shaderProgram);
#ifdef HGETERRAINSUNSHADERDEBUGMODE
	assert(lodnul != 0xFFFFFFFF);
#endif
	glUniform1i(lodnul, 0);
	for(GLuint i = 0; i < HGETERRAINSUNSHADERNUMBEROFTEXTURES; i++)
	{
		tsl[i] = render::ShaderEngine::getUniformLocation(
				std::string("ts[") + std::to_string(i) + "]", shaderProgram);
#ifdef HGETERRAINSUNSHADERDEBUGMODE
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NUMBEROFBUFFERCOMPONENTS * sizeof(GLfloat), (void*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NUMBEROFBUFFERCOMPONENTS * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NUMBEROFBUFFERCOMPONENTS * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, NUMBEROFBUFFERCOMPONENTS * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
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
