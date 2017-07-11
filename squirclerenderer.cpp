#include "squirclerenderer.h"

SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
    delete mpImage;
    delete mTexture;

    for (int i = 0; i < 5; ++i)
    {
        delete mTextures[i];
    }
}

void SquircleRenderer::initTexture()
{
    mpImage = new QImage();
    mpImage->load(":/images/Resource/Zero0.jpg");

    mTexture = new QOpenGLTexture(*mpImage);

    memset(mTextures, 0, sizeof(mTextures));
}

void SquircleRenderer::paint()
{
    if (!m_program) {
        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();
//        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
//                                                    "attribute highp vec4 vertices;"
//                                                    "varying highp vec2 coords;"
//                                                    "void main() {"
//                                                    "    gl_Position = vertices;"
//                                                    "    coords = vertices.xy;"
//                                                    "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "attribute highp vec4 vertices;"
                                                    "attribute vec2 coord_texture;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_Position = vertices;"
                                                    "    coords = coord_texture;"
                                                    "}");
//        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
//                                                    "uniform lowp float t;"
//                                                    "varying highp vec2 coords;"
//                                                    "void main() {"
//                                                    "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
//                                                    "    i = smoothstep(t - 0.8, t + 0.8, i);"
//                                                    "    i = floor(i * 20.) / 20.;"
//                                                    "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
//                                                    "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "uniform sampler2D texture0;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    "
                                                    "    gl_FragColor = texture2D(texture0, coords);"
                                                    "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->bindAttributeLocation("coord_texture", 1);
        m_program->link();

        this->initTexture();

    }

    m_program->bind();

    m_program->setUniformValue("texture0", 0);

    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);

    float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    float coords[] = {
        0, 1,
        1, 1,
        0, 0,
        1, 0,
    };

    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setAttributeArray(1, GL_FLOAT, coords, 2);

//    m_program->setUniformValue("t", (float) m_t);

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glDisable(GL_DEPTH_TEST);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    mTexture->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program->disableAttributeArray(1);
    m_program->disableAttributeArray(0);
    m_program->release();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}
