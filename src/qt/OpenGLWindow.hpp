#pragma once

#include <QWindow>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLPaintDevice>
#include <QMouseEvent>

namespace postrisc {

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
Q_OBJECT

public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating;
    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};


class TriangleWindow : public OpenGLWindow {
public:
    TriangleWindow(QWindow *parent = 0);

    void initialize() override;
    void render() override;

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
    bool m_is_mouse_left_button_pressed;
};

} // namespace postrisc
