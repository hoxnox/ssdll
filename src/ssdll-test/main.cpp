#include <QCoreApplication>
#include <QStringList>
#include "translatordemo.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QStringList arguments = app.arguments();

    if (arguments.length() < 2) {
        std::cout << "Please provide path to .ifo file" << std::endl;
        return 1;
    }

    QString ifoFilepath = arguments[1];
    TranslatorDemo demo;
    if (!demo.loadDict(ifoFilepath)) {
        std::cout << "Can't load dictionary" << std::endl;
        return 2;
    }

    demo.connectEntities();
    QObject::connect(&demo, SIGNAL(quit()), &app, SLOT(quit()));

    std::cout << "Ready!" << std::endl << "> " << std::flush;
    return app.exec();
}
