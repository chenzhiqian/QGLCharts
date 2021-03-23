#ifndef QGLSHADER_H
#define QGLSHADER_H

#include <QObject>
#include <QMap>

class QOpenGLShaderProgram;
class QGLShader : public QObject
{
    Q_OBJECT
private:
    static QGLShader *s_sharedInstance;
    QMap<QObject *, QOpenGLShaderProgram *> mStandardShader;
    QMap<QObject *, QOpenGLShaderProgram *> mPointShader;

private:
    explicit QGLShader(QObject *parent = nullptr);
    QGLShader(const QGLShader &shader);
    QGLShader &operator=(const QGLShader &shader);

public:
    static QGLShader *defaultShader() {
        if (s_sharedInstance == nullptr) {
            s_sharedInstance = new QGLShader();
        }

        return s_sharedInstance;
    }

    void bindStandardShader(QObject *parent);
    void bindPointShader(QObject *parent);
    void unbindShader(QObject *parent);
};

#endif // QGLSHADER_H
