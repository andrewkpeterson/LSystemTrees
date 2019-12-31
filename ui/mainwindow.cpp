#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include <QGLFormat>
#include <QSettings>
#include <Settings.h>
#include <QKeyEvent>
#include <iostream>
#include "gl/GLDebug.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up settings
    settings.cameraFar = 100.0f;
    settings.cameraNear = .1f;
    settings.cameraFov = .5;
}

MainWindow::~MainWindow()
{
    delete ui;
}
