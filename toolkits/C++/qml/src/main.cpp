#include <QApplication>
#include <QtQml/QQmlApplicationEngine>

QByteArray gui = R"lit(import QtQuick 2.0
import QtQuick.Controls 2.0

ApplicationWindow {
   id: rootWindow
   visible: true
   width: 512; height: 512
   title: "Qml App"

   Button {
      id: mybutton

      text: "Click me!"
      onClicked: {
         console.log("Clicked!")
      }
   }
})lit";

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadData(gui);

    return app.exec();
}