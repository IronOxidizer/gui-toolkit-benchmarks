#include <QApplication>
#include <QWidget>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.resize(512, 512);
    QPushButton button("Hello, World!", &window);
    window.show();
    return app.exec();
}
