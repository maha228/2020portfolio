#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oknotablitsa.h"
#include <iostream>
MainWindow::MainWindow(uint nodes_num,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1200,700);
    nodes_num_=nodes_num;
}

MainWindow::~MainWindow()
{
    delete ui;
}


