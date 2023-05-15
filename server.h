#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QTime>


namespace Ui {
class Server;
}

class QFile;
class QTcpServer;
class QTcpSocket;

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

    void init_server(); //初始化服务器
    void refused();  //关闭服务器

protected:
    void closeEvent(QCloseEvent*);

private:
    Ui::Server *ui;
    qint16 tcp_server_port;
    QTcpServer * tcp_server;

    QString filename;
    QString the_filename;
    QFile * locate_file; //待发送的文件


    qint64 total_bytes; //总共需要发送的字节数
    qint64 bytes_written; //已经发送的字节数
    qint64 bytes_to_be_written; //待发送的字节数
    qint64 playload_size;

    QByteArray out_block; //缓存一次发送的数据

    QTcpSocket * client_tcp_socket; //客户端连接的套接字

    QTime time;

private slots:
    void send_data(); //发送数据
    void update_file_transmission_progressbar(qint64 num_bytes); //更新进度条

    void on_server_open_btn_clicked();
    void on_server_send_btn_clicked();
    void on_server_close_btn_clicked();

signals:
    void send_file_name(QString file_name);
};

#endif // SERVER_H
