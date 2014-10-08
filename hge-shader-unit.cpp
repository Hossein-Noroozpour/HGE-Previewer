#include "hge-shader-unit.hpp"
const std::string hge::shader::ShaderUnit::shaderLanguageVersion =
#ifdef HGE_USE_OPENGL_43
"#version 430\n";
#elif defined(HGE_USE_OPENGL_42)
"#version 420\n";
#elif defined(HGE_USE_OPENGL_41)
"#version 410\n";
#elif defined(HGE_USE_OPENGL_40)
"#version 400\n";
#elif defined(HGE_USE_OPENGL_33)
"#version 330\n";
#endif
#ifdef HGE_DEBUG_MODE
const std::string hge::shader::ShaderUnit::vsUvOutVarName("outUV");
const std::string hge::shader::ShaderUnit::vsWorldPositionOutVarName("worldPosition");
const std::string hge::shader::ShaderUnit::lightIntensityVarName("lightIntensity");
const std::string hge::shader::ShaderUnit::modelViewProjectionMatrixUniformName("mvpMatrix");
const std::string hge::shader::ShaderUnit::modelMatrixUniformName("modelMatrix");
const std::string hge::shader::ShaderUnit::sunDirectionUniformName("sunDirection");
const std::string hge::shader::ShaderUnit::shaderEndline("\n");
#else
const std::string hge::shader::ShaderUnit::vsUvOutVarName("ouv");
const std::string hge::shader::ShaderUnit::vsWorldPositionOutVarName("wp");
const std::string hge::shader::ShaderUnit::lightIntensityVarName("li");
const std::string hge::shader::ShaderUnit::modelViewProjectionMatrixUniformName("mvp");
const std::string hge::shader::ShaderUnit::modelMatrixUniformName("m");
const std::string hge::shader::ShaderUnit::sunDirectionUniformName("s");
const std::string hge::shader::ShaderUnit::shaderEndline("");
#endif
//hge::shader::ShaderUnit::ShaderUnit():
//shaderProgram(render::ShaderEngine::createProgram())
//{
//}
//void hge::shader::ShaderUnit::setModelMatrix(const glm::mat4& modelMatrix)
//{
//	glUniformMatrix4fv(model_matrix_uniform_loaction,
//		1, GL_FALSE, glm::value_ptr(modelMatrix));
//}
//void hge::shader::ShaderUnit::setModelViewProjectionMatrix
//(const glm::mat4& modelViewProjectionMatrix)
//{
//	glUniformMatrix4fv(model_view_projection_matrix_uniform_location, 1,
//		GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
//}
//hge::shader::ShaderUnit::~ShaderUnit()
//{
//	hge::render::ShaderEngine::endObject(vertexShader);
//	hge::render::ShaderEngine::endObject(fragmentShader);
//	hge::render::ShaderEngine::endProgram(shaderProgram);
//}
//void hge::shader::ShaderUnit::_use()
//{

//}
//void hge::shader::ShaderUnit::use()
//{
//	glUseProgram(shaderProgram);
//	_use();
//}
//GLuint hge::shader::ShaderUnit::getModelViewProjectionMatrixUniformLocation()
//{
//	return model_view_projection_matrix_uniform_location;
//}
//GLuint hge::shader::ShaderUnit::getModelMatrixUniformLoaction()
//{
//	return model_matrix_uniform_loaction;
//}
//GLuint hge::shader::ShaderUnit::getVertexShader()
//{
//	return vertexShader;
//}
//GLuint hge::shader::ShaderUnit::getFragmentShader()
//{
//	return fragmentShader;
//}
//GLuint hge::shader::ShaderUnit::getShaderProgram()
//{
//	return shaderProgram;
//}
