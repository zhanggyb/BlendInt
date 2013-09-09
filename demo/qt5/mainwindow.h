#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "glwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

 protected:

	//void mouseMoveEvent (QMouseEvent* event);

	//void mousePressEvent (QMouseEvent* event);

	//void mouseReleaseEvent (QMouseEvent* event);

private slots:
    void about();

private:
    Ui::MainWindow *ui;
    
    GLWindow *glwindow_;
};

#endif // MAINWINDOW_H
