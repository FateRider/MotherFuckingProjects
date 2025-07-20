#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onButtonClicked(); // Slot function

    private:
        QPushButton *button; // Pointer for button
        QLabel *label; // Create label
        QWidget *centerWidget; // Create new widgets
        QVBoxLayout *mainLayout; // Create main Layout
};
