#include "widget.h"
#include "ui_widget.h"

#include <QUdpSocket>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QNetworkInterface>
#include <QProcess>
#include <QFileDialog>
#include <QColorDialog>

#include "server.h"
#include "client.h"


Widget::Widget(QWidget *parent,QString _username) :
    QWidget(parent),ui(new Ui::Widget),m_username(_username)

{
    ui->setupUi(this);
    udp_socket = new QUdpSocket(this);
    port = 23232;
    udp_socket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);

    connect(udp_socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    send_msg(UserEnter);

    server = new Server(this); //服务器
    connect(server,SIGNAL(send_file_name(QString)),this,SLOT(get_filename(QString)));

    connect(ui->msgTextEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this,SLOT(current_format_changed(const QTextCharFormat)));
}

void Widget::send_msg(MsgType type, QString server_address){
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString ip_address = get_ip();
    out<<type<<get_user();

    switch(type){
    case Msg:
        if(ui->msgTextEdit->toPlainText() == ""){
            QMessageBox::warning(0,tr("警告"),tr("发送信息不能为空"),QMessageBox::Ok);
            return;
        }
        out << ip_address <<get_msg();
        ui->msgBrowser->verticalScrollBar()->setValue(
                    ui->msgBrowser->verticalScrollBar()->maximum());
        break;

    case UserEnter:
        out << ip_address;
        break;
    case UserLeft:
        break;

    case FileName:{
        int row = ui->userTableWidget->currentRow();
        QString client_address = ui->userTableWidget->item(row,1)->text();
        out << ip_address << client_address << m_filename;
        break;
    }

    case Refuse:{
        out << server_address;
        break;
    }
    }

    udp_socket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);

}

//接收 UDP 广播发送来的消息
void Widget::processPendingDatagrams(){
    while(udp_socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udp_socket->pendingDatagramSize());
        udp_socket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in >> msgType;
        QString username,ip_address,msg;

        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh::mm:ss");

        switch(msgType){
        case Msg:
            in >> username >> ip_address >> msg;
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("[" + username + "]" + time);
            ui->msgBrowser->append(msg);
            break;

        case UserEnter:
            in >> username >> ip_address;
            user_enter(username,ip_address);
            break;

        case UserLeft:
            in >> username;
            user_left(username,time);
            break;

        case FileName:{
            in >> username >> ip_address;
            QString client_address , filename;
            in >> client_address >> filename;

            hasPendingFile(username,ip_address,client_address,filename);
            break;
        }

        case Refuse:{
            in >> username;
            QString server_address;
            in >> server_address;

            QString ip_address = get_ip();
            if(ip_address == server_address){
                server->refused();
            }
            break;
        }
        }

    }
}

//处理新用户加入
void Widget::user_enter(QString username, QString ip_address){
    bool is_empty = ui->userTableWidget->findItems(username,Qt::MatchExactly).isEmpty();

    if(is_empty){
        QTableWidgetItem * user = new QTableWidgetItem(username);
        QTableWidgetItem * ip = new QTableWidgetItem(ip_address);

        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,user);
        ui->userTableWidget->setItem(0,1,ip);
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->msgBrowser->append(tr("%1 在线!").arg(username));
        ui->userCountLabel->setText(tr("在线人数: %1").arg(ui->userTableWidget->rowCount()));

        send_msg(UserEnter);
    }
}

//用户离开
void Widget::user_left(QString username, QString time){

    int idx = ui->userTableWidget->findItems(username,Qt::MatchExactly).first()->row();
    ui->userTableWidget->removeRow(idx);

    ui->msgBrowser->setTextColor(Qt::gray);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));

    ui->msgBrowser->append(tr("%1 于 %2 离开!").arg(username).arg(time));
    ui->userCountLabel->setText(tr("在线人数 : %1").arg(ui->userTableWidget->rowCount()));
}

//获取 IP 地址
QString Widget::get_ip(){
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            return address.toString();
        }
    }
    return "";
}

//获取用户名
QString Widget::get_user(){
    return m_username;
}

//获取用户输入的聊天信息
QString Widget::get_msg(){
    QString msg = ui->msgTextEdit->toHtml();

    ui->msgTextEdit->clear();
    ui->msgTextEdit->setFocus();
    return msg;
}


//点击发送按钮，发送消息
void Widget::on_sendBtn_clicked(){
    send_msg(Msg);
}

//获取文件名
void Widget::get_filename(QString name){
    m_filename = name;
    send_msg(FileName);
}

//点击传输文件按钮
void Widget::on_sendTBtn_clicked(){
    if(ui->userTableWidget->selectedItems().isEmpty()){
        QMessageBox::warning(0,tr("选择用户"),tr("请先选择用户!"),QMessageBox::Ok);
        return;
    }

    server->show();
    server->init_server();

}

void Widget::hasPendingFile(QString username, QString server_address, QString client_address, QString filename){
    QString ip_address = get_ip();

    if(ip_address == client_address){
        int btn = QMessageBox::information(this,tr("接收文件"),tr("来自 %1(%2) 的文件 : %3 ,是否接收?").arg(username).arg(server_address).arg(filename),
                                           QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes){
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),filename);

            if(!name.isEmpty()){
                Client * client = new Client(this);
                client->set_fileName(name);
                client->set_hostAddress(QHostAddress(server_address));
                client->show();
            }
            else{
                send_msg(Refuse,server_address);
            }
        }

    }
}

//更改字体
void Widget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->msgTextEdit->setCurrentFont(f);
    ui->msgTextEdit->setFocus();
}

//更改字体大小
void Widget::on_sizeComboBox_currentIndexChanged(const QString &arg1)
{
   ui->msgTextEdit->setFontPointSize(arg1.toDouble());
   ui->msgTextEdit->setFocus();
}

void Widget::on_boldTBtn_clicked(bool checked)
{
    //加粗
    if(checked){
        ui->msgTextEdit->setFontWeight(QFont::Bold);
    }
    //取消
    else{
        ui->msgTextEdit->setFontWeight(QFont::Normal);
    }
    ui->msgTextEdit->setFocus();
}

//倾斜
void Widget::on_italicTBtn_clicked(bool checked)
{
    ui->msgTextEdit->setFontItalic(checked);
    ui->msgTextEdit->setFocus();
}

//下划线
void Widget::on_underlineTBtn_clicked(bool checked)
{
   ui->msgTextEdit->setFontUnderline(checked);
   ui->msgTextEdit->setFocus();
}

void Widget::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid()){
        ui->msgTextEdit->setTextColor(color);
        ui->msgTextEdit->setFocus();
    }
}

//一段文本设置不同的格式
void Widget::current_format_changed(const QTextCharFormat &format){
    ui->fontComboBox->setCurrentFont(format.font());

    //如果字体大小出错（因为最小的字体为 8）, 使用12
    if(format.fontPointSize() < 8){
        ui->sizeComboBox->setCurrentIndex(4);
    }
    else{
        ui->sizeComboBox->setCurrentIndex(ui->sizeComboBox->findText(QString::number(format.fontPointSize())));
    }

    ui->boldTBtn->setChecked(format.font().bold());
    ui->italicTBtn->setChecked(format.font().italic());
    ui->underlineTBtn->setChecked(format.font().underline());

    color = format.foreground().color();
}

void Widget::on_saveTBtn_clicked()
{
    if(ui->msgTextEdit->document()->isEmpty()){
        QMessageBox::warning(0,tr("警告"),tr("聊天记录为空，无法保存!"),QMessageBox::Ok);
    }
    else{
        QString name = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),
                                                    tr("聊天记录"),tr("文本(*.txt);;所有文件(*.*)"));
        if(!name.isEmpty()){
            save_file(name);
        }
    }
}

bool Widget::save_file(const QString &filename){
    QFile file(filename);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,tr("保存文件"),tr("无法保存文件 %1 : \n %2").arg(filename).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << ui->msgBrowser->toPlainText();
    return true;
}

void Widget::on_clearTBtn_clicked()
{
    ui->msgBrowser->clear();
}

//关闭窗口
void Widget::on_quitBtn_clicked(){
    close();
}

void Widget::closeEvent(QCloseEvent* e){
    send_msg(UserLeft);
    QWidget::closeEvent(e);
}


Widget::~Widget()
{
    delete ui;
}

