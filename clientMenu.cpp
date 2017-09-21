#include "clientMenu.h"
#include "ui_clientMenu.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <iostream>

using namespace  std;

clientMenu::clientMenu(QString path, QWidget *parent) : QWidget(parent), ui(new Ui::clientMenu), imPath(path)
{
    ui->setupUi(this);

    // ********** Set server address --------->>
    address = "163.20.133.59"; // School
    setHostAddressAndPort(address, 37);

    // ********** Show Image ----------------->>
    if(! image.load(imPath))
        ui->imagePreView->setText(tr("沒有圖片!"));
    else
        ui->imagePreView->setPixmap(QPixmap::fromImage(image.scaled(500,500)));

    // ********** Setting Items -------------->>
    ui->sendProgress->setValue(0);

    // ********** Connect Signal & Slot ------>>
    connect(ui->ipSelect , SIGNAL(currentTextChanged(QString))        , this, SLOT(changeIP(QString)));
    connect(ui->sendBtn  , SIGNAL(clicked())                          , this, SLOT(start()));
    connect(ui->cancelBtn, SIGNAL(clicked())                          , this, SLOT(close()));
    connect(&client      , SIGNAL(connected())                        , this, SLOT(startTransfer()));
    connect(&client      , SIGNAL(bytesWritten(qint64))               , this, SLOT(updateProgress(qint64)));
    connect(&client      , SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(&client      , SIGNAL(readyRead())                        , this, SLOT(recieveResult()));

}

void clientMenu::setHostAddressAndPort(QString address, quint16 port)
{
    hostAddress.setAddress(address);
    this->hostPort = port;
}

void clientMenu::changeIP(QString ip)
{
    qDebug() << ip;
    address = ip;
    setHostAddressAndPort(address, 37);
}

void clientMenu::start()
{
    // ********** Check File Path ------------>>
    if(imPath == NULL) {
        ui->stateInfo->setText(tr("檔案路徑不存在!"));
        return;
    }

    file = new QFile(imPath);
    ui->sendBtn->setEnabled(false);
    client.connectToHost(hostAddress, hostPort);
    client.waitForConnected();
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void clientMenu::startTransfer()
{
    // ********** Check File ----------------->>
    if (!file->open(QIODevice::ReadOnly)) {
        ui->stateInfo->setText(tr("無法讀取檔案!"));\
        delete file;
        file = 0;
        return;
    }

    // ********** Write File Information ----->>
    QByteArray fileInfo;
    QDataStream sendOut(&fileInfo, QIODevice::WriteOnly);

    fileSize = file->size() + sizeof(qint64); // totatl send size
    ui->sendProgress->setMaximum(fileSize);   // assign maximum progress bar value

    sendOut << (qint64)fileSize;              // write file size
    fileInfo.append(file->readAll());         // write file data
    // sendOut << file->readAll();            // can't use this (have extra bytes)

    // ********** Send File Information ------>>
    client.write(fileInfo);                   // send file information
}

void clientMenu::updateProgress(qint64 numBytes)
{
    // ********** Update Progress Value ------>>
    int written = ui->sendProgress->value() + (int)numBytes;
    ui->sendProgress->setValue(written);

    // ********** Finish Sending ------------->>
    if(ui->sendProgress->value() == ui->sendProgress->maximum()) {
        ui->stateInfo->setText(tr("圖片傳送完成!"));
    }
}

void clientMenu::closeConnection()
{
    // Client
    client.close();

    // File
    file->close();
    delete file;
    file = 0;

    ui->sendProgress->setValue(0);      // reset progress bar

    QApplication::restoreOverrideCursor();
}

void clientMenu::displayError(QAbstractSocket::SocketError socketError)
{
    QMessageBox::information(this, "Network error", "The following error occurred: " + client.errorString());
    closeConnection();
}

void clientMenu::recieveResult()
{
    if(client.bytesAvailable() > 0){
        prosResult = client.readAll();
        qDebug() << prosResult;
        ui->stateInfo->setText(prosResult);
        closeConnection();
    }
}

clientMenu::~clientMenu()
{
    delete ui;
}
