#include "qglshader.h"
#include "VertexShader.inc"
#include "FragmentShader.inc"

#include <QOpenGLShaderProgram>

QGLShader *QGLShader::s_sharedInstance = nullptr;

QGLShader::QGLShader(QObject *parent) : QObject(parent)
{

}

QGLShader::QGLShader(const QGLShader &shader)
{
    Q_UNUSED(shader)
}

QGLShader &QGLShader::operator=(const QGLShader &shader)
{
    Q_UNUSED(shader)
    return *this;
}

void QGLShader::bindStandardShader(QObject *parent)
{
    QOpenGLShaderProgram *shader = mStandardShader[parent];

    if (shader != nullptr) {
        shader->bind();
        return;
    }

    QOpenGLShader *vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!vertexShader->compileSourceCode(vertexShaderSource))
    {
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << vertexShader->log();
        return;
    }

    QOpenGLShader *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!fragmentShader->compileSourceCode(standardFgSource))
    {
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << fragmentShader->log();
        return;
    }

    shader = new QOpenGLShaderProgram(parent);
    shader->addShader(vertexShader);
    shader->addShader(fragmentShader);
    if (!shader->link())
    {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << shader->log() ;
        return;
    }

    shader->bind();

    mStandardShader.insert(parent, shader);
    delete vertexShader;
    delete fragmentShader;
}

void QGLShader::bindPointShader(QObject *parent)
{
    QOpenGLShaderProgram *shader = mPointShader[parent];
    if (shader != nullptr) {
        shader->bind();
        return;
    }

    QOpenGLShader *vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!vertexShader->compileSourceCode(vertexShaderSource))
    {
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << vertexShader->log();
        return;
    }

    QOpenGLShader *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!fragmentShader->compileSourceCode(pointFgSource))
    {
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << fragmentShader->log();
        return;
    }

    shader = new QOpenGLShaderProgram(parent);
    shader->addShader(vertexShader);
    shader->addShader(fragmentShader);
    if (!shader->link())
    {
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << shader->log() ;
        return;
    }

    shader->bind();

    mPointShader.insert(parent, shader);

    delete vertexShader;
    delete fragmentShader;
}

void QGLShader::unbindShader(QObject *parent)
{
    QOpenGLShaderProgram *shader = mPointShader[parent];
    if (shader != nullptr) {
        shader->release();
        mPointShader.remove(shader);
        delete shader;
    }

    shader = mStandardShader[parent];
    if (shader != nullptr) {
        shader->release();
        mStandardShader.remove(shader);
        delete shader;
    }
}
