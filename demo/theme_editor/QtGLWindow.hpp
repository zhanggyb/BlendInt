#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class QtGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit QtGLWindow(QWindow *parent = 0);
    ~QtGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event);

    void exposeEvent(QExposeEvent *event);

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};
