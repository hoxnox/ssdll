#include "translatordemo.h"
#include <iostream>

TranslatorDemo::TranslatorDemo(QObject *parent) :
    QObject(parent)
{
    m_SocketNotifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
}

void TranslatorDemo::connectEntities() {
    QObject::connect(m_SocketNotifier, SIGNAL(activated(int)), this, SLOT(readWord()));
}

bool TranslatorDemo::loadDict(const QString &ifoFilepath) {
#ifdef Q_OS_WIN
    bool result = m_LookupDictionary.loadDictionary(ifoFilepath.toStdWString());
#else
    bool result = m_LookupDictionary.loadDictionary(ifoFilepath.toStdString());
#endif

    return result;
}

void TranslatorDemo::readWord() {
    std::string line;
    std::getline(std::cin, line);

    if (std::cin.eof() || (line == "quit") || (line == "exit")) {
        emit quit();
    } else {
        std::string translation;
        if (m_LookupDictionary.translate(line, translation)) {
            std::cout << translation << std::endl;
        } else {
            std::cout << "Not found";
        }

        std::cout << "> " << std::flush;
    }
}
