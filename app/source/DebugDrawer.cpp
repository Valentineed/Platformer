#include <glad/glad.h>
#include "DebugDrawer.h"

#include <Resources/Shader.h>
#include <stdio.h> //printf debugging


DebugDrawer::DebugDrawer()
    :m_debugMode(0),m_VBO(0), m_VAO(0)
{
	m_shader = new Resources::Shader;
	std::string fragmentShaderText = m_shader->LoadFileGLSL("Shader/DebugBullet.frag");
	std::string vertexShaderText = m_shader->LoadFileGLSL("Shader/DebugBullet.vert");
	m_shader->CreateShaderProgram(fragmentShaderText.c_str(), vertexShaderText.c_str());
}

DebugDrawer::~DebugDrawer()
{
	delete m_shader;
}

void DebugDrawer::SetMatrices(Core::Maths::mat4 pViewMatrix, Core::Maths::mat4 pProjectionMatrix)
{
	glProgramUniformMatrix4fv(m_shader->GetID(), glGetUniformLocation(m_shader->GetID(), "projection"), 1, GL_FALSE, pProjectionMatrix.m_matrix);
	glProgramUniformMatrix4fv(m_shader->GetID(), glGetUniformLocation(m_shader->GetID(), "view"), 1, GL_FALSE, pViewMatrix.m_matrix);
}


void    DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
		// Vertex data
		float points[12];

		points[0] = from.x();
		points[1] = from.y();
		points[2] = from.z();
		points[3] = color.x();
		points[4] = color.y();
		points[5] = color.z();

		points[6] = to.x();
		points[7] = to.y();
		points[8] = to.z();
		points[9] = color.x();
		points[10] = color.y();
		points[11] = color.z();

		
		
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);

		glUseProgram(m_shader->GetID());

		glGenBuffers(1, &m_VBO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
    
}

void    DebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

void    DebugDrawer::draw3dText(const btVector3& /*location*/, const char* /*textString*/)
{
    //glRasterPos3f(location.x(),  location.y(),  location.z());
    //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void    DebugDrawer::reportErrorWarning(const char* warningString)
{
    printf(warningString);
}

void    DebugDrawer::drawContactPoint(const btVector3& /*pointOnB*/, const btVector3& /*normalOnB*/, btScalar /*distance*/, int /*lifeTime*/, const btVector3& /*color*/)
{
    {
        //btVector3 to=pointOnB+normalOnB*distance;
        //const btVector3&from = pointOnB;
        //glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

        //DebugDrawer::drawLine(from, to, color);

        //glRasterPos3f(from.x(),  from.y(),  from.z());
        //char buf[12];
        //sprintf(buf," %d",lifeTime);
        //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
    }
}