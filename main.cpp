#include <QCoreApplication>

#include "pyawareplugin.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Set proxy if necessary
    QNetworkProxy ROAproxy;
    ROAproxy.setHostName("proxy.roa");
    ROAproxy.setPort(8080);
    QNetworkProxy::setApplicationProxy(ROAproxy);

    PyAwarePlugin piaware_plugin("192.168.3.240", 30003);
    piaware_plugin.doConnect();

    return a.exec();
}
