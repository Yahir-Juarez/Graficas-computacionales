#include "CGrhaphicsManager.h"

#ifdef OGL_GRAPH_SYS

#include "GL/glew.h"
////#include "GL/eglew.h"
////#include "GL/glxew.h"
#include <GL/GLU.h>


void test()
{
	//glBindBuffer()
	//glBufferData(aaas, asasas, assas, GL_STATIC_DRAW);
}

SPtr<VertexShader> CGraphicsManager::createVertexShader(const Path& fileName, const String& entryPoint, const String& shaderModel)
{
	auto myShader = std::make_shared<VertexShader>();

	/*std::string shaderSource = readShaderFile(fileName);
	if (shaderSource.empty()) {
		return nullptr;
	}

	glGetProgramiv()*/

	return myShader;
}

#endif // OGL_GRAPH_SYS