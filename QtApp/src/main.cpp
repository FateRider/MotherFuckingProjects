#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Hello Qt , I hate you");
    window.setWindowIcon(QIcon("../images/octopi_red.png"));
    window.setFixedSize(900, 600);

    QLabel *image = new QLabel(&window);
    image->setPixmap(QPixmap("../images/9SFjWW0.png"));
    image->setGeometry(550, 20, 400, 600);
    image->setScaledContents(true);

    QPushButton *btn = new QPushButton("Senpai pls click me UwU", &window);
    btn->move(500, 450);

    QPushButton *btn2 = new QPushButton("Restart", &window);
    btn2->move(400, 450);

    QLabel *label = new QLabel("Not clicked button pls click senpai PLSSS NYAAAA OwO", &window);
    label->move(300, 250);

    QObject::connect(btn, &QPushButton::clicked, [label]() { label->setText("UMMMHHH SENPAI CLICKED MEEE"); });
    QObject::connect(btn2, &QPushButton::clicked, [label]() { label->setText("Senpai pls again click me UwU"); });

    window.show();
    return app.exec();
}