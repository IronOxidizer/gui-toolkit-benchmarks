#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>
#include <chrono>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

// Hook to paint event
bool MainWindow::event(QEvent *event)
{
    const bool ret_val = QMainWindow::event(event);
    if(!first_paint_occured && event->type() == QEvent::Paint)
    {
        first_paint_occured = true;
        // For startup benchmark
        std::cerr << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
    }
    return ret_val;
}

MainWindow::~MainWindow()
{
    delete ui;
}

