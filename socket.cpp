#include "socket.h"
#include <iostream>
#include "serveraddresses.h"
static QMutex mutex;

Socket::Socket(QString nk, QString prx,
               QString prt, QObject *parent) : QObject(parent)
{
    qDebug()<< "SOCKET CLASS";
    address =  QString::fromStdString(ServerAddresses::getServerAddress());
    port = ServerAddresses::getServerPort();
    room = "#grunge";


    nickFinal = nk.toStdString();
    Ip_final = prx;
    Port_final = prt;
    numberAttempts =0;
    qDebug()<< "Ip_final " <<Ip_final;
    qDebug()<< "Port_final " <<Port_final;
    std::cout << "nick:::::::: " << nickFinal <<std::endl;
    setSocksProxy();

    runCon();

}


void Socket::runCon()
{
    mutex.lock();
    numberAttempts++;
    mutex.unlock();

    mSocket = new QTcpSocket;

    connect(mSocket, SIGNAL(connected()), // jednorazowe
            this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(readyRead()),
            this, SLOT(readyReady()));


    qDebug() << "Connecting...";
    mSocket->connectToHost(address, port);

    if(!mSocket->waitForConnected(15000))
    {
        qDebug()<< "Error " << mSocket->errorString();
    }

    QEventLoop loop;
    loop.exec();
    //connected(); wielorazowe

    connect(mSocket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(mSocket, SIGNAL(readyRead()),
            this, SLOT(readyReady()));
    connect(mSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));

    if(numberAttempts<5)
    {
        connect(mSocket, SIGNAL(disconnected()),
                this, SLOT(runCon()));
    }


}


void Socket::connected()
{
    qDebug() << "Connected!";

    std::string _nick = "NICK "+ nickFinal  + "\r\n\r\n\r\n";
    mSocket->flush();
    mSocket->write(_nick.c_str());

    std::string _user = "USER " + nickFinal + " 0 * :" + nickFinal + "\r\n\r\n\r\n";

    mSocket->flush();
    mSocket->write(_user.c_str());

    mSocket->flush();
    room_ = "JOIN " + room  + "\r\n\r\n\r\n";
    mSocket->write(room_.c_str());

    mSocket->flush();
    mSocket->write("JOIN #xxxxxx\r\n\r\n\r\n");
    mSocket->write("PRIVMSG #xxxxxx aaa\r\n\r\n\r\n");
}

void Socket::disconnected()
{
    qDebug() << "Disconnected!";
}

void Socket::readyReady()
{
    QString data;
    mSocket->ReadOnly;
    while(mSocket->canReadLine())
    {
        data = mSocket->readLine();
        ping(data); //check for ping while being connected
        qDebug() << data;
    }
    mSocket->reset();
}

void Socket::setSocksProxy()
{
    qDebug() << "setSocksProxy()";

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName(Ip_final);
    proxy.setPort(Port_final.toUShort());

    QNetworkProxy::setApplicationProxy(proxy);
}

void Socket::proxySocks()
{

}

void Socket::proxyVPN()
{

}



void Socket::ping(QString data)
{
    if(data.indexOf("PING")!=-1)
    {
        int index = data.indexOf("PING");
        data[index+1] = 'O';
        std::string mString = data.toStdString();
        mString+="\r\n\r\n\r\n";
        const char * y = mString.c_str();
        mSocket->write(y);
        qDebug() << "I WROTE: " << y ;
        data.clear();
        mString.clear();

        mSocket->flush();

        mSocket->write(room_.c_str());
    }
}
