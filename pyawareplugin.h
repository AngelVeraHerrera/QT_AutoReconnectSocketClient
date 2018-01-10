#ifndef PYAWAREPLUGIN_H
#define PYAWAREPLUGIN_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QNetworkProxy>
#include <QTimer>
#include <QProcess>
#include <QTDbus/QtDBus>

class PyAwarePlugin : public QObject
{
    Q_OBJECT
public:
    explicit PyAwarePlugin(QString host, int port);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void checkConnection();
    void doConnect();

signals:
    void doReconnect();

private:
    QTcpSocket *piaware_socket;
    QTimer *piaware_alive_timer;
    QString piaware_host;
    int piaware_port;
};

#endif // PYAWAREPLUGIN_H
