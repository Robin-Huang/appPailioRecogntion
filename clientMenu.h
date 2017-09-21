#ifndef CLIENTMENU_H
#define CLIENTMENU_H

#include <QWidget>
#include <QHostAddress>
#include <QTcpSocket>
#include <QImage>
#include <QLabel>
#include <QFile>

namespace Ui {
class clientMenu;
}

class clientMenu : public QWidget
{
    Q_OBJECT

public:
    explicit clientMenu(QString path, QWidget *parent = 0);
    ~clientMenu();

    void setHostAddressAndPort(QString hostAddress, quint16 port);
    void closeConnection();

public slots:
    void changeIP(QString ip);
    void start();
    void startTransfer();
    void updateProgress(qint64 numBytes);
    void displayError(QAbstractSocket::SocketError socketError);
    void recieveResult();

private:
    Ui::clientMenu *ui;

    // Tcp Socket
    QTcpSocket   client;
    QString      address;
    QHostAddress hostAddress;
    quint16      hostPort;

    // Image File
    QFile       *file;
    QImage       image;
    QString      imPath;
    qint64       fileSize;

    QString      prosResult;
};

#endif // CLIENTMENU_H
