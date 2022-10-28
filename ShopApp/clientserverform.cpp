#include "clientserverform.h"
#include "ui_clientserverform.h"
#include "logthread.h"
#include "ui_clientserverform.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QApplication>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QFile>
#include <QFileInfo>
#include <QProgressDialog>

ClientServerForm::ClientServerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientServerForm), totalSize(0), byteReceived(0)
{
    ui->setupUi(this);
    QList<int> sizes;
    sizes << 120 << 500;
    ui->splitter->setSizes(sizes);

    ui->clientTreeWidget->hideColumn(2);

    chatServer = new QTcpServer(this);
    connect(chatServer, SIGNAL(newConnection( )), SLOT(clientConnect( )));
    if (!chatServer->listen(QHostAddress::Any, PORT_NUMBER)) {
        QMessageBox::critical(this, tr("Chatting Server"), \
                              tr("Unable to start the server: %1.") \
                              .arg(chatServer->errorString( )));
        close( );
        return;
    }

    fileServer = new QTcpServer(this);
    connect(fileServer, SIGNAL(newConnection()), SLOT(acceptConnection()));
    if (!fileServer->listen(QHostAddress::Any, PORT_NUMBER+1)) {
        QMessageBox::critical(this, tr("Chatting Server"), \
                              tr("Unable to start the server: %1.") \
                              .arg(fileServer->errorString( )));
        close( );
        return;
    }

    qDebug("Start listening ...");

    QAction* inviteAction = new QAction(tr("&Invite"));
    inviteAction->setObjectName("Invite");
    connect(inviteAction, SIGNAL(triggered()), SLOT(inviteClient()));

    QAction* removeAction = new QAction(tr("&Kick out"));
    connect(removeAction, SIGNAL(triggered()), SLOT(kickOut()));

    menu = new QMenu;
    menu->addAction(inviteAction);
    menu->addAction(removeAction);
    ui->clientTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    progressDialog = new QProgressDialog(0);
    progressDialog->setAutoClose(true);
    progressDialog->reset();

    logThread = new LogThread(this);
    logThread->start();

    connect(ui->savePushButton, SIGNAL(clicked()), logThread, SLOT(saveData()));

    qDebug() << tr("The server is running on port %1.").arg(chatServer->serverPort( ));
}

ClientServerForm::~ClientServerForm()
{
    delete ui;

    logThread->terminate();
    chatServer->close( );
    fileServer->close( );
}

void ClientServerForm::clientConnect( )
{
    QTcpSocket *clientConnection = chatServer->nextPendingConnection( );
    connect(clientConnection, SIGNAL(readyRead( )), SLOT(receiveData( )));
    connect(clientConnection, SIGNAL(disconnected( )), SLOT(removeClient()));
    qDebug("new connection is established...");
}

void ClientServerForm::receiveData( )
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket *>(sender( ));
    QByteArray bytearray = clientConnection->read(BLOCK_SIZE);

    Chat_Status type;       // 채팅의 목적
    char data[1020];        // 전송되는 메시지/데이터
    memset(data, 0, 1020);

    QDataStream in(&bytearray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> type;
    in.readRawData(data, 1020);

    QString ip = clientConnection->peerAddress().toString();
    quint16 port = clientConnection->peerPort();
    QString name = QString::fromStdString(data);

    qDebug() << ip << " : " << type;

    switch(type) {
    case Chat_Login:
    {
        foreach(auto item, ui->clientTreeWidget->findItems(name, Qt::MatchFixedString, 1)) {
            if(item->text(2) != "-") {
                item->setText(2, "-");
                item->setIcon(0, QIcon("p8.png"));
                clientList.append(clientConnection);        // QList<QTcpSocket*> clientList;
                clientSocketHash[name] = clientConnection;
            }
        }
        break;
    }
    case Chat_In:
        foreach(auto item, ui->clientTreeWidget->findItems(name, Qt::MatchFixedString, 1)) {
            if(item->text(2) != "O") {
                item->setText(2, "O");
                item->setIcon(0, QIcon("g3.png"));
            }
            clientNameHash[port] = name;
        }
        break;
    case Chat_Talk: {
        foreach(QTcpSocket *sock, clientList) {
            if(clientNameHash.contains(sock->peerPort()) && sock != clientConnection) {
                QByteArray sendArray;
                sendArray.clear();
                QDataStream out(&sendArray, QIODevice::WriteOnly);
                out << Chat_Talk;
                sendArray.append("<font color=lightsteelblue>");
                sendArray.append(clientNameHash[port].toStdString().data());
                sendArray.append("</font> : ");
                sendArray.append(name.toStdString().data());
                sock->write(sendArray);
                qDebug() << sock->peerPort();
            }
        }

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->messageTreeWidget);
        item->setText(0, ip);
        item->setText(1, QString::number(port));
        item->setText(2, QString::number(clientIDHash[clientNameHash[port]]));
        item->setText(3, clientNameHash[port]);
        item->setText(4, QString(data));
        item->setText(5, QDateTime::currentDateTime().toString());
        item->setToolTip(4, QString(data));
        ui->messageTreeWidget->addTopLevelItem(item);

        for(int i = 0; i < ui->messageTreeWidget->columnCount(); i++)
            ui->messageTreeWidget->resizeColumnToContents(i);

        logThread->appendData(item);
    }
        break;
    case Chat_Out:
        foreach(auto item, ui->clientTreeWidget->findItems(name, Qt::MatchContains, 1)) {
            if(item->text(2) != "-") {
                item->setText(2, "-");
                item->setIcon(0, QIcon("p8.png"));
            }
            clientNameHash.remove(port);
        }
        break;
    case Chat_LogOut:
        foreach(auto item, ui->clientTreeWidget->findItems(name, Qt::MatchContains, 1)) {
            if(item->text(2) != "X") {
                item->setText(2, "X");
                item->setIcon(0, QIcon("x5ling.png"));
                clientList.removeOne(clientConnection);        // QList<QTcpSocket*> clientList;
                clientSocketHash.remove(name);
            }
        }
        break;
    }
}

void ClientServerForm::removeClient()
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket *>(sender( ));

    QString name = clientNameHash[clientConnection->peerPort()];
    foreach(auto item, ui->clientTreeWidget->findItems(name, Qt::MatchContains, 1)) {
        item->setText(2, "X");
        item->setIcon(0, QIcon("x5ling.png"));
    }

    clientList.removeOne(clientConnection);
    clientConnection->deleteLater();
}

void ClientServerForm::addClient(int id, QString name)
{
    clientIDList << id;
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->clientTreeWidget);
    item->setText(2, "X");
    item->setIcon(0, QIcon("x5ling.png"));
    item->setText(1, name);
    ui->clientTreeWidget->addTopLevelItem(item);
    clientIDHash[name] = id;
    ui->clientTreeWidget->resizeColumnToContents(0);
}

void ClientServerForm::slot_updateModifyClient(int id, int index, QString clinetname)
{
    ui->clientTreeWidget->topLevelItem(index)->setText(1, clinetname);
    clientIDHash[clinetname] = id;
}


void ClientServerForm::on_clientTreeWidget_customContextMenuRequested(const QPoint &pos)
{
    foreach(QAction *action, menu->actions()) {
        if(action->objectName() == "Invite")
            action->setEnabled(ui->clientTreeWidget->currentItem()->text(2) != "O");
        else
            action->setEnabled(ui->clientTreeWidget->currentItem()->text(2) == "O");
    }
    QPoint globalPos = ui->clientTreeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void ClientServerForm::kickOut()
{
    QString name = ui->clientTreeWidget->currentItem()->text(1);
    QTcpSocket* sock = clientSocketHash[name];

    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out << Chat_KickOut;
    out.writeRawData("", 1020);
    sock->write(sendArray);

    ui->clientTreeWidget->currentItem()->setText(2, "-");
    ui->clientTreeWidget->currentItem()->setIcon(0, QIcon("p8.png"));
    //    clientSocketHash.remove(name);
}

void ClientServerForm::inviteClient()
{
    if(ui->clientTreeWidget->topLevelItemCount()) {
        QString name = ui->clientTreeWidget->currentItem()->text(1);

        QByteArray sendArray;
        QDataStream out(&sendArray, QIODevice::WriteOnly);
        out << Chat_Invite;
        out.writeRawData("", 1020);

        QTcpSocket* sock = clientSocketHash[name];
        if(sock == nullptr)
        {
            return;
        }
        sock->write(sendArray);

        foreach(auto item, ui->clientTreeWidget->findItems(name, Qt::MatchFixedString, 1)) {
            if(item->text(1) != "O") {
                item->setText(2, "O");
                item->setIcon(0, QIcon("g3.png"));
            }
        }
        quint64 port = sock->peerPort();
        clientNameHash[port] = name;
    }
}


/* 파일 전송 */
void ClientServerForm::acceptConnection()
{
    qDebug("Connected, preparing to receive files!");

    QTcpSocket* receivedSocket = fileServer->nextPendingConnection();
    connect(receivedSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
}

void ClientServerForm::readClient()
{
    qDebug("Receiving file ...");
    QTcpSocket* receivedSocket = dynamic_cast<QTcpSocket *>(sender( ));
    QString filename;
    QString name;

    if (byteReceived == 0) { // just started to receive data, this data is file information
        progressDialog->reset();
        progressDialog->show();

        QString ip = receivedSocket->peerAddress().toString();
        quint16 port = receivedSocket->peerPort();
        qDebug() << receivedSocket->peerName();

        QDataStream in(receivedSocket);
        in >> totalSize >> byteReceived >> filename >> name;
        progressDialog->setMaximum(totalSize);

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->messageTreeWidget);
        item->setText(0, ip);
        item->setText(1, QString::number(port));
        item->setText(2, QString::number(clientIDHash[name]));
        item->setText(3, name);
        item->setText(4, filename);
        item->setText(5, QDateTime::currentDateTime().toString());
        item->setToolTip(4, filename);

        for(int i = 0; i < ui->messageTreeWidget->columnCount(); i++)
            ui->messageTreeWidget->resizeColumnToContents(i);

        ui->messageTreeWidget->addTopLevelItem(item);
        logThread->appendData(item);

        QFileInfo info(filename);
        QString currentFileName = info.fileName();
        file = new QFile(currentFileName);
        file->open(QFile::WriteOnly);
    } else { // Officially read the file content
        inBlock = receivedSocket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }

    progressDialog->setValue(byteReceived);

    if (byteReceived == totalSize) {
        qDebug() << QString("%1 receive completed").arg(filename);

        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        progressDialog->reset();
        progressDialog->hide();
        file->close();
        delete file;
    }
}

//treeWidget을 반복문으로 돌면서 해당 이름이 있는지, 있다면 이미 로그인 되어 있는지 없는지 확인한다.
void ClientServerForm::slot_checkLogin(QString name, int id)
{
    for(int i=0; i < ui->clientTreeWidget->topLevelItemCount(); i++)
    {
        if(ui->clientTreeWidget->topLevelItem(i)->text(1) == name)    //같은 이름이 있는지  topLevelItem: 뒤에 있는 index 값을 가져오는 것
        {
            if(ui->clientTreeWidget->topLevelItem(i)->text(2) == "X")     //로그인 되어 있는지 없는지
            {
                            //(int login, int id)
                emit sig_checkLogin(1, id);      //이 경우에만 로그인 가능하니까 가능 여부를 다시 chatClient로 시그널을 보내주자.    //정상적인 로그인
                return;
            }
            else    //이미 로그인 되어 있는 경우
            {
                emit sig_checkLogin(2, id);
                return;
            }
        }
    }
    emit sig_checkLogin(3, id);     //만약 위해서 안 걸렸다면 false로 반환하자.    //등록된 회원이 아닌 경우
}

void ClientServerForm::slot_updateClientInfo(QString clientName)
{
    int idx = -1;
    for(int i=0; i < ui->clientTreeWidget->topLevelItemCount(); i++)
    {
        if(ui->clientTreeWidget->topLevelItem(i)->text(1) == clientName)    //  topLevelItem: 뒤에 있는 index 값을 가져오는 것
        {
            idx = i;
            break;
        }
    }
    if(idx != -1)
    {
        ui->clientTreeWidget->takeTopLevelItem(idx);
        ui->clientTreeWidget->update();
    }
}


void ClientServerForm::on_clearPushButton_clicked()
{
    ui->messageTreeWidget->clear();
}


void ClientServerForm::loadData()
{
//    QFile file("chatList.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return;

//    QTextStream in(&file);      //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공합니다.
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        QTextStream out(&file);
//        for (int i = 0 ; i < ui->messageTreeWidget->topLevelItemCount(); i ++)
//        {
//            ui->messageTreeWidget->topLevelItem(i);
//        }
//    }
//    file.close( );
}



void ClientServerForm::on_savePushButton_clicked()
{
//    if(ui->messageTreeWidget == nullptr)
//        return;

//    QFile file("chatList.txt");       //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공합니다.
//    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//        return;

//    QTextStream out(&file);
//    for (int i = 0 ; i < ui->messageTreeWidget->topLevelItemCount(); i ++)
//    {
//        auto item = ui->messageTreeWidget->topLevelItem(i);

//        out << item->text(0) << "," <<  item->text(1);
//        out << item->text(2) << "," <<  item->text(3);
//        out << item->text(4) << "," <<  item->text(5) << "\n";
//    }
//    file.close( );
}

