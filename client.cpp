#include "client.h"
#include "ui_client.h"

#include <QTcpSocket>
#include <QtDebug>
#include <QMessageBox>
#include <QNetworkProxy>


Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    setFixedSize(400,190);

    total_bytes = 0;
    bytes_received = 0;
    filename_size = 0;

    client_tcpsocket = new QTcpSocket(this);
    client_tcpsocket->setProxy(QNetworkProxy::NoProxy);

    tcp_port = 5555;

    connect(client_tcpsocket,SIGNAL(readyRead()),this,SLOT(read_data()));
    connect(client_tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(show_error(QAbstractSocket::SocketError)));

}

void Client::set_fileName(QString name){
    locate_file = new QFile(name);
}

void Client::set_hostAddress(QHostAddress address){
    hostAddress = address;
    new_connection();
}

void Client::show_error(QAbstractSocket::SocketError socketError){
    switch(socketError){
    case QAbstractSocket::RemoteHostClosedError:break;
    default: qDebug() << client_tcpsocket->errorString();
    }
}

//与服务器连接

void Client::new_connection(){
    block_size = 0;
    client_tcpsocket->abort();
    client_tcpsocket->connectToHost(hostAddress,tcp_port);
    time.start();
}

//接收服务器传来的文件数据
void Client::read_data(){

    QDataStream in(client_tcpsocket);
    in.setVersion(QDataStream::Qt_5_12);

    float used_time = time.elapsed();
    qint64 placeholder = (qint64)sizeof(qint64) * 2;

    if (bytes_received <= placeholder) {
        if ((client_tcpsocket->bytesAvailable() >= placeholder) && (filename_size == 0))
        {
            in >> total_bytes >> filename_size;
            bytes_received += placeholder;
        }

        if((client_tcpsocket->bytesAvailable() >= filename_size) && (filename_size != 0)){
            in >> filename;
            bytes_received += filename_size;

            if(!locate_file->open(QFile::WriteOnly)){
                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1 :\n %2.").arg(filename).arg(locate_file->errorString()));
                return;
            }
        }
        else {
            return;
        }
    }

    if (bytes_received < total_bytes) {

        bytes_received += client_tcpsocket->bytesAvailable();
        in_block = client_tcpsocket->readAll();
        locate_file->write(in_block);
        in_block.resize(0);
    }

    ui->client_progressBar->setMaximum(total_bytes);
    ui->client_progressBar->setValue(bytes_received);

    double speed = bytes_received / used_time;
    ui->client_status_label->setText(tr("已接收 %1MB (%2MB/s) \n共 %3 MB 已用时：%4 s \n 估计剩余时间：%5 s")
                                      .arg(bytes_received / (1024*1024))
                                      .arg(speed*1000/(1024*1024),0,'f',0)
                                      .arg(total_bytes / (1024 * 1024))
                                      .arg(used_time/1000,0,'f',0)
                                      .arg(total_bytes/speed/1000 - used_time/1000,0,'f',0));

    if(bytes_received == total_bytes)
    {
        locate_file->close();
        client_tcpsocket->close();
        ui->client_status_label->setText(tr("接收文件 %1 完毕").arg(filename));
    }
}

void Client::on_client_cancel_btn_clicked(){
    client_tcpsocket->abort();
    if(locate_file->isOpen()){
        locate_file->close();
    }
}

void Client::on_client_close_btn_clicked(){
    client_tcpsocket->abort();
    if(locate_file->isOpen()){
        locate_file->close();
    }
    close();
}

void Client::closeEvent(QCloseEvent *){
    on_client_close_btn_clicked();
}

Client::~Client()
{
    delete ui;
}
