#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextCharFormat>

class QUdpSocket;
class Server;

namespace Ui {
class Widget;
}

enum MsgType{Msg,UserEnter,UserLeft,FileName,Refuse}; //消息类型

class Widget : public QWidget
{
    Q_OBJECT

public:
     Widget(QWidget *parent,QString _username);
    ~Widget();

protected:
     void user_enter(QString username,QString ip_address);
     void user_left(QString username,QString time);
     void send_msg(MsgType type,QString server_address = "");

     QString get_ip();
     QString get_user();
     QString get_msg();

     void hasPendingFile(QString username,QString server_address,
                         QString client_address,QString filename);

     //保存文件
     bool save_file(const QString &filename);

     void closeEvent(QCloseEvent *);


private:
    Ui::Widget *ui;
    QUdpSocket * udp_socket;
    qint16 port;
    QString m_username;

    QString m_filename;
    Server * server;

    QColor color;

private slots:
    void processPendingDatagrams();
    void on_sendBtn_clicked();

    void get_filename(QString);

    void on_sendTBtn_clicked();
    void on_quitBtn_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_sizeComboBox_currentIndexChanged(const QString &arg1);
    void on_boldTBtn_clicked(bool checked);
    void on_italicTBtn_clicked(bool checked);
    void on_underlineTBtn_clicked(bool checked);
    void on_colorTBtn_clicked();

    void current_format_changed(const QTextCharFormat &format);
    void on_saveTBtn_clicked();
    void on_clearTBtn_clicked();
};

#endif // WIDGET_H
