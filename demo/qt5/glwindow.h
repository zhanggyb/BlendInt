#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>

class GLWindow : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWindow(QWidget *parent = 0);
    ~GLWindow();

signals:

public slots:

protected:
    void initializeGL();
    void resizeGL (int w, int h);
    void paintGL ();
    void mouseMoveEvent (QMouseEvent* event);
    void mousePressEvent (QMouseEvent* event);
    void mouseReleaseEvent (QMouseEvent* event);

private:
};

#endif // GLWINDOW_H
