#include "mainwindow.h"
#include "dynamiclayout.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(new Ui::MainWindow) {
    mUi->setupUi(this);

    setCentralWidget(new DynamicLayout(this));
}

MainWindow::~MainWindow() {
    delete mUi;
}