#include "MainWindow.hpp"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle("My First OOP Qt App");
    this->resize(400, 300);

    // 1. Create to widgets
    button = new QPushButton("Click me!!", this);
    label = new QLabel("Welcome the Qt / Welcome the hell");

    // 2. Creates layout and add widgets step by step
    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(label);
    mainLayout->addWidget(button);

    // 3. Widget assign to layout
    centerWidget = new QWidget(this);
    centerWidget->setLayout(mainLayout);
    this->setCentralWidget(centerWidget);

    // signal-slot connection
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onButtonClicked() { qDebug() << "ButtonClicked"; }