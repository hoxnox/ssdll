#ifndef TRANSLATORDEMO_H
#define TRANSLATORDEMO_H

#include <QObject>
#include <QSocketNotifier>
#include <lookupdictionary.h>

class TranslatorDemo : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorDemo(QObject *parent = 0);

public:
    void connectEntities();
    bool loadDict(const QString &ifoFilepath);

signals:
    void quit();

private slots:
    void readWord();

private:
    LookupDictionary m_LookupDictionary;
    QSocketNotifier *m_SocketNotifier;
};

#endif // TRANSLATORDEMO_H
