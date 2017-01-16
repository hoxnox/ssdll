#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>
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
    } else {
        std::cout << "Dictionary loaded" << std::endl;
    }

    QString nextLine;
    QTextStream qtin(stdin);

    while (true) {
        std::cout << "> " << std::flush;
        QString line = qtin.readLine();
        demo.process(line);
    }

    return 0;
}
