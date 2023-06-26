#include <QApplication>
#include <QWidget>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    QPushButton button("Hello, World!", &window);
    window.show();
    return app.exec();
}
