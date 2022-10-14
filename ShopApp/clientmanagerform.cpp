#include "clientmanagerform.h"
#include "ui_clientmanagerform.h"
#include "client.h"

#include <QFile>
#include <QMenu>

ClientManagerForm::ClientManagerForm(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ClientManagerForm)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 1900 << 1300;               //1900 : treeWidget size, 1300 : toolBox size
    ui->splitter->setSizes(sizes);

    QAction* removeAction = new QAction(tr("&Remove"));         //항목 제거
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;       //menu : 멤버변수
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);        //setContextMenuPolicy : 위젯이 상황에 맞는 메뉴를 표시하는 방법   //CustomContextMenu : 위젯이 신호를 보냅니다.

    //treeWidget size 변경
    ui->treeWidget->header()->resizeSection(0, 70);       //Client ID size
    ui->treeWidget->header()->resizeSection(1, 90);       //Client Name size
    ui->treeWidget->header()->resizeSection(2, 130);      //Phone Number size
    ui->treeWidget->header()->resizeSection(3, 150);      //Address size
    ui->treeWidget->header()->resizeSection(4, 120);      //Email size
    //searchTreeWidget size 변경
    ui->searchTreeWidget->header()->resizeSection(0, 60);       //Client ID size
    ui->searchTreeWidget->header()->resizeSection(1, 80);       //Client Name size
    ui->searchTreeWidget->header()->resizeSection(2, 100);      //Phone Number size
    ui->searchTreeWidget->header()->resizeSection(3, 100);      //Address size
    ui->searchTreeWidget->header()->resizeSection(4, 100);      //Email size

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    QFile file("clientList.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);      //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공합니다.
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");      //QList 클래스는 동적 배열을 제공하는 템플릿 클래스입니다.
        if(row.size()) {
            int clientID = row[0].toInt();
            Client* client = new Client(clientID, row[1], row[2], row[3], row[4]);
            ui->treeWidget->addTopLevelItem(client);
            clientList.insert(clientID, client);
        }
    }
    file.close( );
}

ClientManagerForm::~ClientManagerForm()
{
    delete ui;

    QFile file("clientlist.txt");       //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공합니다.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : clientList) {
        Client* client = v;
        out << client->ClientID() << ", " << client->getClientName() << ", ";
        out << client->getPhoneNumber() << ", ";
        out << client->getAddress() << ", " << client->getEmail() << "\n";
    }
    file.close( );
}

int ClientManagerForm::makeId( )
{
    if(clientList.size( ) == 0) {
        return 1;
    } else {
        auto clientID = clientList.lastKey();
        return ++clientID;
    }
}

void ClientManagerForm::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        clientList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        ui->treeWidget->update();
    }
}

void ClientManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void ClientManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->clientIDLineEdit->setText(item->text(0));
    ui->clientNameLineEdit->setText(item->text(1));
    ui->phoneNumberLineEdit->setText(item->text(2));
    ui->addressLineEdit->setText(item->text(3));
    ui->emailLineEdit->setText(item->text(4));
}

void ClientManagerForm::on_addPushButton_clicked()
{
    QString clientname, phonenumber, address, email;
    int clientID = makeId( );
    clientname = ui->clientNameLineEdit->text();
    phonenumber = ui->phoneNumberLineEdit->text();
    address = ui->addressLineEdit->text();
    email = ui->emailLineEdit->text();
    if(clientname.length()) {
        Client* client = new Client(clientID, clientname, phonenumber, address, email);
        clientList.insert(clientID, client);
        ui->treeWidget->addTopLevelItem(client);
    }
    ui->clientIDLineEdit->setText("");
    ui->clientNameLineEdit->setText("");
    ui->phoneNumberLineEdit->setText("");
    ui->addressLineEdit->setText("");
    ui->emailLineEdit->setText("");
}

void ClientManagerForm::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int key = item->text(0).toInt();
        Client* clientID = clientList[key];
        QString clinetname, phonenumber, address, email;
        clinetname = ui->clientNameLineEdit->text();
        phonenumber = ui->phoneNumberLineEdit->text();
        address = ui->addressLineEdit->text();
        email = ui->emailLineEdit->text();
        clientID->setClientName(clinetname);
        clientID->setPhoneNumber(phonenumber);
        clientID->setAddress(address);
        clientID->setEmail(email);
        clientList[key] = clientID;
    }
}

void ClientManagerForm::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            Client* client = static_cast<Client*>(i);
            int clientID = client->ClientID();
            QString clientName = client->getClientName();
            QString phonenumber = client->getPhoneNumber();
            QString address = client->getAddress();
            QString email = client->getEmail();
            Client* item = new Client(clientID, clientName, phonenumber, address, email);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}

Client* ClientManagerForm::searchName(const QString &inputName)      //inputName : Order에서 입력받은 회원 이름
{
    foreach(Client* c, clientList)         //foreach(variable, container)
    {
        if(inputName == c->getClientName())         //Order에서 입력받은 이름과 Client에 있는 회원 이름 정보가 일치할 경우
            return c;                               //Client 정보를 반환
    }
    return nullptr;         //Order에서 입력받은 회원 이름 정보가 clientList 없을 경우 nullptr을 반환
}
