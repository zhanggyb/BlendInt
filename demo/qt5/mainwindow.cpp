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
}

MainWindow::~MainWindow()
{
    delete ui;
}
