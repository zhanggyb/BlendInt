#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->hide();
    ui->statusBar->hide();
    
    glwindow_ = new GLWindow (this);
    setCentralWidget(glwindow_);
    
    resize (1200, 800);

    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about ()
{
    QMessageBox::about(this, tr("About"),
            tr("This is a demo of running Blender Interface Library in Qt5"));
}

