#include "tester.h"
#include <QEventLoop>
#include "serveraddresses.h"
//this tester is to connect to irc and test proxies
static QMutex  mutex;



Tester::Tester(QThread &mThread,
               std::string nick_,
               std::vector<std::string> ircProxies,
               std::vector<std::string> ircProxiesPort,
               std::atomic<int>& remainingProxyCounter,
               QObject *parent) : QObject(parent),
                                  m_remainingProxyCounter(remainingProxyCounter)
{
    testSocket = new QTcpSocket ;

    address =  QString::fromStdString(ServerAddresses::getServerAddress());
    port = ServerAddresses::getServerPort();
    room = "#xxxxx";

    pongReceived = false;
    pingCounter = 0;

    timer = new QTimer(this);

    endOfList = false;
    nick = nick_;
    proxyIPvec = ircProxies;
    proxyPortVec = ircProxiesPort;

    connect(&mThread, &QThread::started,
            this, &Tester::runCon); //when this thread is STARTED

    connect(this, &Tester::proxyReady,
            &storage, &GoodProxyStorage::storeReady);

    //connect(this, SIGNAL(proxyReady(QString,QString)),
    //&storage, SLOT(startThreadforConnection2(QString,QString)));
}

void Tester::pingPre()
{}

void Tester::runCon()
{
    testSocket = new QTcpSocket ;
    qDebug() << "runCon()";
    proxyHttp(); //sets global proxies

    connect(testSocket, &QTcpSocket::connected,
            this, &Tester::connected); //if connected run nick
    connect(testSocket, &QTcpSocket::disconnected,
            this, &Tester::disconnected); //if disconnected run disc

    connect(testSocket, &QTcpSocket::readyRead,
            this, &Tester::readyReady);

    //CONNECTION
    qDebug() << "Connecting...";
    if(endOfList==false && m_remainingProxyCounter > 0)
    {
        testSocket->connectToHost(address, port);

        m_remainingProxyCounter--;
        qDebug() << "(((((Remaining proxies: m_remainingProxyCounter: " << m_remainingProxyCounter << ")))))";
    }
    else
    {
        return;
    }

    if(!testSocket->waitForConnected(13000))
    {
        qDebug() << "Error " << QproxyIP << ":" << QproxyPort << " - " << testSocket->errorString();

        proxyList(); //try again
        runCon();
    }
    else //if connected:
    {
        qDebug() << "CONNECTED! to" << QproxyIP << ":" << QproxyPort;

        if(std::find(goodProxiesIP.begin(), goodProxiesIP.end(), QproxyIP) == goodProxiesIP.end())
        {//if IP not found
            qDebug() << "loop.exit();";
        }
        else
        {
            qDebug() << "disconnected(); cause found in vector";
            disconnected();
        }
    }
}

void Tester::connected()
{
    qDebug() << "---------connected()";
    qDebug() << "Connected! to: " << QproxyIP << ":" << QproxyPort;

    std::string _nick = "NICK " + nick + "\r\n\r\n\r\n";
    testSocket->flush();
    testSocket->write(_nick.c_str());

    std::string _user = "USER " + nick + " 0 * :" + nick + "\r\n\r\n\r\n";

    testSocket->flush();
    testSocket->write(_user.c_str());

    testSocket->flush();
//    room = "JOIN " + room + "\r\n\r\n\r\n";
    room = "JOIN #grunge\r\n\r\n\r\n";
    testSocket->write(room.c_str());
    testSocket->flush();
    //testSocket->write("JOIN #metal \r\n\r\n\r\n");

    //emit proxyReady(QproxyIP, QproxyPort); THIS IS FOR FILE @@@@@

    testSocket->write("PRIVMSG EmoGore\r\n\r\n\r\n");
    testSocket->flush();
}

void Tester::disconnected()
{
    testSocket->disconnect(this);
    testSocket->close();
    testSocket->reset();
    qDebug() << "Disconnected!";

    proxyList();
    runCon();
}


void Tester::readyReady()
{
    QString data;
    testSocket->ReadOnly;
    while(testSocket->canReadLine())
    {
        data = testSocket->readLine();
        ping(data); //check for ping while being connected
        qDebug() << data;
    }
    testSocket->reset();
}

void Tester::proxyList()
{
    if(m_remainingProxyCounter> 0)
    {
        qDebug() << "proxyList()";
        proxyIP = proxyIPvec[m_remainingProxyCounter];
        proxyPort = proxyPortVec[m_remainingProxyCounter];
        QproxyIP = QString::fromStdString(proxyIP);
        QproxyPort = QString::fromStdString(proxyPort);
        //GetData data;
    }
    else
    {
        qDebug() << "End of Proxy List";
        endOfList=true;

    }

}

void Tester::proxyHttp()
{
    QNetworkProxy proxy;
    //proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName(QproxyIP);
    proxy.setPort(QproxyPort.toShort());
    QNetworkProxy::setApplicationProxy(proxy);
}








void Tester::ping(QString data)
{
    qDebug() << "ping";

    if(data.indexOf("PING")!=-1)
    {
        int index = data.indexOf("PING");
        data[index+1] = 'O';
        std::string mString = data.toStdString();
        mString+="\r\n\r\n\r\n";
        const char * y = mString.c_str();
        testSocket->write(y);
        qDebug() << "I WROTE: " << y ;
        room = "JOIN " + room  + "\r\n\r\n\r\n";

        testSocket->flush();
        //testSocket->write(room.c_str()); ROOOOOOM
        //testSocket->write("JOIN #metal \r\n\r\n\r\n");

        //testSocket->flush();
        //testSocket->write("JOIN #metal\r\n\r\n\r\n");
        //testSocket->write("JOIN #grunge\r\n\r\n\r\n");

        //data.clear();
        //mString.clear();

        pongReceived = true;
        mutex.lock();
        pingCounter++;
        mutex.unlock();

        if(pongReceived==true && pingCounter>2)
        {
            goodProxiesIP.push_back(QproxyIP);
            mutex.lock();
            emit proxyReady(QproxyIP, QproxyPort);
            QproxyIP.clear();
            QproxyPort.clear();
            mutex.unlock();


            pongReceived = false; //reset
            pingCounter = 0; //reset
            disconnected(); //disconnects after connection
        }

    }
}



