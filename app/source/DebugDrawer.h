#pragma once

#include <LinearMath/btIDebugDraw.h>
#include <Maths/mat4.h>
#include <Resources/Shader.h>

class DebugDrawer : public btIDebugDraw
{
    int m_debugMode;
    GLuint m_VBO;
    GLuint m_VAO;
    Resources::Shader* m_shader;

public:

    DebugDrawer();
    ~DebugDrawer();

    void SetMatrices(Core::Maths::mat4 pViewMatrix, Core::Maths::mat4 pProjectionMatrix);

    virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

    virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;

    virtual void   reportErrorWarning(const char* warningString) override;

    virtual void   draw3dText(const btVector3& location, const char* textString) override;

    virtual void   setDebugMode(int debugMode) override;

    virtual int    getDebugMode() const override { return m_debugMode; }

};