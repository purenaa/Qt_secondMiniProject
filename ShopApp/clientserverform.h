#ifndef CHATSERVERFORM_H
#define CHATSERVERFORM_H

#include <QWidget>
#include <QList>
#include <QHash>

class QLabel;
class QTcpServer;
class QTcpSocket;
class QFile;
class QProgressDialog;
class LogThread;

namespace Ui {
class ClientServerForm;
}

typedef enum {
    Chat_Login,             // 로그인(서버 접속)   --> 초대를 위한 정보 저장
    Chat_In,                // 채팅방 입장
    Chat_Talk,              // 채팅
    Chat_Out,               // 채팅방 퇴장         --> 초대 가능
    Chat_LogOut,            // 로그 아웃(서버 단절) --> 초대 불가능
    Chat_Invite,            // 초대
    Chat_KickOut,           // 강퇴
} Chat_Status;

class ClientServerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientServerForm(QWidget *parent = nullptr);
    ~ClientServerForm();
    void loadData();

private:
    const int BLOCK_SIZE = 1024;
    const int PORT_NUMBER = 8000;

    Ui::ClientServerForm *ui;
    QTcpServer *chatServer;
    QTcpServer *fileServer;
    QList<QTcpSocket*> clientList;
    QList<int> clientIDList;
    QHash<quint16, QString> clientNameHash;
    QHash<QString, QTcpSocket*> clientSocketHash;
    QHash<QString, int> clientIDHash;
    QMenu* menu;
    QFile* file;
    QProgressDialog* progressDialog;
    qint64 totalSize;
    qint64 byteReceived;
    QByteArray inBlock;
    LogThread* logThread;




private slots:
    void acceptConnection();                /* 파일 서버 */
    void readClient();

    void clientConnect( );                  /* 채팅 서버 */
    void receiveData( );
    void removeClient( );
    void addClient(int, QString);
    void inviteClient();
    void kickOut();
    void on_clientTreeWidget_customContextMenuRequested(const QPoint &pos);

    //이름을 넘겨 받아 그 사람이 로그인 가능한지 확인하는 slot을 만든다.
    void slot_checkLogin(QString, int);

    void slot_updateClientInfo(QString);
    void slot_updateModifyClient(int, int, QString);

    void on_clearPushButton_clicked();

    void on_savePushButton_clicked();

signals:
    //로그인 가능한지 여부를 false, true로 인자를 넘긴다.
    void sig_checkLogin(int, int);

};

#endif // CHATSERVERFORM_H
