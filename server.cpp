#include "server.h"
#include "ui_server.h"

#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QNetworkProxy>



Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    setFixedSize(400,207);

    tcp_server_port = 5555;
    tcp_server = new QTcpServer(this);
    connect(tcp_server,SIGNAL(newConnection()),this,SLOT(send_data()));

    //初始化服务器
    init_server();
}

//初始化服务器
void Server::init_server(){

    playload_size = 64 * 1024;
    total_bytes = 0;
    bytes_written = 0;
    bytes_to_be_written = 0;

    ui->server_choose_label->setText(tr("选择文件/文件夹"));
    ui->server_status_label->setText(tr("请选择要传送的文件!"));
    ui->server_progressBar->reset();

    ui->server_open_btn->setEnabled(true);
    ui->server_send_btn->setEnabled(false);

    tcp_server->close();  //初始化之后就关闭服务器
}

//发送数据
void Server::send_data(){

    //发送之后，发送按钮就暂时处于被禁用的状态
    ui->server_send_btn->setEnabled(false);

    client_tcp_socket = tcp_server->nextPendingConnection();
    //client_tcp_socket->setProxy(QNetworkProxy::NoProxy);

    connect(client_tcp_socket,SIGNAL(bytesWritten(qint64)),this,SLOT(update_file_transmission_progressbar(qint64)));

    ui->server_status_label->setText(tr("开始发送文件 %1 !").arg(the_filename));

    locate_file = new QFile(filename);

    //不能读取文件
    if(!locate_file->open(QFile::ReadOnly)){

        QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1 : \n %2").arg(filename).arg(locate_file->errorString()));
        return;
    }

    total_bytes = locate_file->size();
    QDataStream send_out(&out_block,QIODevice::WriteOnly);
    send_out.setVersion(QDataStream::Qt_5_12);

    time.start();  //开始计时

    send_out << qint64(0) << qint64(0) << the_filename;

    total_bytes += out_block.size();
    send_out.device()->seek(0);

    send_out << total_bytes << qint64(out_block.size() - (sizeof(qint64)) * 2);
    bytes_to_be_written = total_bytes - client_tcp_socket->write(out_block);

    out_block.resize(0);

}

//更新进度条
void Server::update_file_transmission_progressbar(qint64 num_bytes){

    qApp->processEvents();

    bytes_written += num_bytes;

    if(bytes_to_be_written > 0){
        out_block = locate_file->read(qMin(bytes_to_be_written,playload_size));
        bytes_to_be_written -= client_tcp_socket->write(out_block);
        out_block.resize(0);
    }
    else{
        locate_file->close();
    }

    ui->server_progressBar->setMaximum(total_bytes);
    ui->server_progressBar->setValue(bytes_written);

    float used_time = time.elapsed();
    double speed = bytes_written / used_time;

    //更新文件传输的状态
    ui->server_status_label->setText(tr("已经发送 : %1 MB (%2 MB/s) , 共 %3 MB \n 已用时 : %4 s\n 估计剩余时间: %5s")
                                   .arg(bytes_written/(1024 * 1024))
                                   .arg(speed * 1000 / (1024 * 1024),0,'f',0)
                                   .arg(total_bytes / (1024 * 1024))
                                   .arg(used_time/1000,0,'f',0)
                                   .arg(total_bytes/speed/1000 - used_time/1000,0,'f',0));

    //文件数据传输完毕
    if(bytes_written == total_bytes){
        locate_file->close();
        tcp_server->close();
        ui->server_status_label->setText(tr("传送文件 %1 成功!").arg(the_filename));
    }
}

//点击打开按钮，打开选择文件
void Server::on_server_open_btn_clicked(){
    filename = QFileDialog::getOpenFileName(this);

    if(!filename.isEmpty()){
        the_filename = filename.right(filename.size() - filename.lastIndexOf('/') - 1);
        ui->server_status_label->setText(tr("要传送的文件为 : %1 ").arg(the_filename));
        ui->server_open_btn->setEnabled(false);
        ui->server_send_btn->setEnabled(true);
    }
}

//点击发送按钮，发送文件
void Server::on_server_send_btn_clicked(){

    if(!tcp_server->listen(QHostAddress::Any,tcp_server_port)){
        qDebug() << tcp_server->errorString();
        close();
        return;
    }

    ui->server_status_label->setText(tr("等待对方接收文件...."));
    emit send_file_name(the_filename);
}

//点击关闭按钮，关闭服务器
void Server::on_server_close_btn_clicked(){
    if(tcp_server->isListening()){
        tcp_server->close();
        if(locate_file->isOpen()){
            locate_file->close();
        }
        client_tcp_socket->abort();
    }
    close();
}

//系统关闭事件处理函数
void Server::closeEvent(QCloseEvent *)
{
    on_server_close_btn_clicked();
}

void Server::refused(){
    tcp_server->close();
    ui->server_status_label->setText(tr("对方拒绝接收"));
}

Server::~Server()
{
    delete ui;
}
