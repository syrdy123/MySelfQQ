#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QFile>
#include <QTime>

class QTcpSocket;


namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

    void set_hostAddress(QHostAddress address);
    void set_fileName(QString name);

protected:
   void closeEvent(QCloseEvent* );

private:
    Ui::Client *ui;

    QTcpSocket * client_tcpsocket; //客户端套接字
    quint16 block_size;
    QHostAddress hostAddress;
    qint16 tcp_port;

    qint64 total_bytes;  //总共需要接收的字节数
    qint64 bytes_received; //已经接收的字节数
    qint64 filename_size;
    QString filename;
    QFile * locate_file; //待接收的文件
    QByteArray in_block; //缓存一次接收的数据

    QTime time;

private slots:

   void new_connection(); //连接到服务器
   void read_data(); //读取文件数据
   void show_error(QAbstractSocket::SocketError); //显示错误信息

   void on_client_cancel_btn_clicked();
   void on_client_close_btn_clicked();
};

#endif // CLIENT_H
