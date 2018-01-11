#include "pyawareplugin.h"

PyAwarePlugin::PyAwarePlugin(QString host, int port) :
    piaware_host(host), piaware_port(port)
{
    this->piaware_socket = new QTcpSocket(this);
    this->piaware_alive_timer = new QTimer(this);

    QObject::connect(piaware_socket, SIGNAL(connected()),this, SLOT(connected()));
    QObject::connect(piaware_socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    QObject::connect(piaware_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    QObject::connect(this, SIGNAL(doReconnect()),this, SLOT(doConnect()));
    QObject::connect(piaware_alive_timer, SIGNAL(timeout()), this, SLOT(checkConnection()));

    piaware_alive_timer->start(5000);
}

void PyAwarePlugin::checkConnection()
{
    QProcess pingProcess;
    QStringList params;

    params << "-n" << "1";
    params << this->piaware_host;

    pingProcess.start("ping",params,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    if (pingProcess.exitCode() == 0)
    {
        if(this->piaware_socket->state() != QTcpSocket::ConnectedState)
            emit this->doReconnect();
    }
    else
    {
        if(this->piaware_socket->state() == QTcpSocket::ConnectedState)
        {
            qDebug()<<"Closing socket...";
            this->piaware_socket->close();
        }
    }
}

void PyAwarePlugin::doConnect()
{
    qDebug() << "Connecting to "<<this->piaware_host<<":"<<this->piaware_port;

    // Non blocking call
    piaware_socket->connectToHost(this->piaware_host, this->piaware_port);

    if(!this->piaware_socket->waitForConnected(1000))
    {
        qDebug() << "Error: " << piaware_socket->errorString();
    }
}

void PyAwarePlugin::connected()
{
    qDebug() << "Socket connected!";
}

void PyAwarePlugin::disconnected()
{
    qDebug() << "Socket disconnected!";
}


void PyAwarePlugin::readyRead()
{
    QStringList list;

    while(this->piaware_socket->canReadLine())
    {
        QString data = QString(this->piaware_socket->readLine());
        data.remove(QRegExp("[\\n\\t\\r]"));
        list.append(data);
        qDebug()<<data;
    }
}
