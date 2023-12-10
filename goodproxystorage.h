#ifndef GOODPROXYSTORAGE_H
#define GOODPROXYSTORAGE_H
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <algorithm>
#include "socket.h"
#include <QThread>
#include "getdata.h"
#include <QMutex>
class GoodProxyStorage : public QObject
{
    Q_OBJECT
public:
    explicit GoodProxyStorage(QObject *parent = nullptr);
    void saveToFile(QString proxReady, QString portReady);
    void startThreadforConnection(QString proxReady, QString portReady);

    QMutex *mutex;

signals:
    void scondNickStart(QString QproxyIP,
                        QString QproxyPort);
public slots:
    void storeReady(QString QproxyIP,
                    QString QproxyPort);

    void startThreadforConnection2(QString proxReady, QString portReady);
protected:
    QString proxReady;
    QString portReady;
private:

    QFile file;
    static std::vector<QString> IPs;
    static std::vector<QString> Ports;
    static std::vector<QString> nickDatabase;

    std::string mNick;
    GetData data;
    QString QmNick;
    std::vector<QString>nicks1;
    static int NickCounter;
    static int IPcounter;

    QString relativePathToWorkingProxies;
};

#endif // GOODPROXYSTORAGE_H
