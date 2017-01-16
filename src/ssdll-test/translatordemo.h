#ifndef TRANSLATORDEMO_H
#define TRANSLATORDEMO_H

#include <QObject>
#include <lookupdictionary.h>

class TranslatorDemo : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorDemo(QObject *parent = 0);

public:
    bool loadDict(const QString &ifoFilepath);
    void process(const QString &line);

private:
    LookupDictionary m_LookupDictionary;
};

#endif // TRANSLATORDEMO_H
