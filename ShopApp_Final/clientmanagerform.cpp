#include "clientmanagerform.h"
#include "ui_clientmanagerform.h"
#include "client.h"

#include <QFile>
#include <QMenu>

ClientManagerForm::ClientManagerForm(QWidget *parent)       //생성자
    : QWidget(parent),
      ui(new Ui::ClientManagerForm)
{
    ui->setupUi(this);                                      //지정한 위젯에 대한 사용자 인터페이스를 설정

    QList<int> sizes;
    sizes << 1900 << 1300;                                  //1900 : treeWidget size, 1300 : toolBox size
    ui->splitter->setSizes(sizes);                          //splitter 크기 조절

    QAction* removeAction = new QAction(tr("&Remove"));     //삭제 기능 생성
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;                                       //menu : 멤버변수       //메뉴 생성
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);        //setContextMenuPolicy : 위젯이 상황에 맞는 메뉴를 표시하는 방법
                                                                        //CustomContextMenu : 위젯이 신호를 보냅니다.
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    //treeWidget size 변경
    ui->treeWidget->header()->resizeSection(0, 70);         //Client ID size
    ui->treeWidget->header()->resizeSection(1, 90);         //Client Name size
    ui->treeWidget->header()->resizeSection(2, 130);        //Phone Number size
    ui->treeWidget->header()->resizeSection(3, 150);        //Address size
    ui->treeWidget->header()->resizeSection(4, 120);        //Email size
    //searchTreeWidget size 변경
    ui->searchTreeWidget->header()->resizeSection(0, 60);       //Client ID size
    ui->searchTreeWidget->header()->resizeSection(1, 80);       //Client Name size
    ui->searchTreeWidget->header()->resizeSection(2, 100);      //Phone Number size
    ui->searchTreeWidget->header()->resizeSection(3, 100);      //Address size
    ui->searchTreeWidget->header()->resizeSection(4, 100);      //Email size
}

/* 파일 입출력 */
void ClientManagerForm::loadData()
{
    QFile file("clientList.txt");                               //파일 이름 : clientList.txt 으로 저장
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))      //파일이 열어서 읽을 수 없으면 return
        return;

    QTextStream in(&file);                                  //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");              //line을 ,로 구분함  //QList 클래스는 동적 배열을 제공하는 템플릿 클래스
        if(row.size()) {
            int clientID = row[0].toInt();
            Client* client = new Client(clientID, row[1], row[2], row[3], row[4]);
                                                            //0: 고객 ID, 1: 고객 이름, 2: 고객 전화번호, 3: 고객 주소, 4: 고객 Email
            ui->treeWidget->addTopLevelItem(client);        //treeWidget에 client를 최상위 항목으로 추가
            clientList.insert(clientID, client);            //key : clientID, value : client
            emit clientAdded(clientID, row[1]);             //clientAdded 시그널 발생
        }
    }
    file.close( );                                          //파일을 닫음
}

ClientManagerForm::~ClientManagerForm()                     //소멸자
{
    delete ui;

    QFile file("clientlist.txt");                               //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))     //파일을 열어서 쓸 수 없으면 return
        return;

    QTextStream out(&file);                                 //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공
    for (const auto& v : clientList) {
        Client* client = v;
        out << client->ClientID() << ", " << client->getClientName() << ", ";
        out << client->getPhoneNumber() << ", ";
        out << client->getAddress() << ", " << client->getEmail() << "\n";
    }
    file.close( );                                          //파일을 닫음
}

int ClientManagerForm::makeId( )                            //고객 ID를 자동 할당해주는 함수
{
    if(clientList.size( ) == 0) {                           //초기 clientList에 정보가 없을 경우 고객 ID 1을 반환
        return 1;
    } else {
        auto clientID = clientList.lastKey();               //clientList가 비어있지 않으면
        return ++clientID;                                  //이전 반환 값 1에 ++clientID
    }
}

void ClientManagerForm::removeItem()                        //항목(item)을 제거해주는 함수
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();      //item = 현재 항목
    if(item != nullptr) {                                       //item이 nullptr이 아니면
        clientList.remove(item->text(0).toInt());               //clientList의 선택된 clientID 제거
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        ui->treeWidget->update();                               //treeWidget 업데이트
        emit sig_sendClientInfo(item->text(1));                 //sig_sendClientInfo 시그널 발생
    }
}

void ClientManagerForm::showContextMenu(const QPoint &pos)      //마우스 커서 위치
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void ClientManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)    //treeWidget에 있는 항목(item)을 선택했을 경우
{
    Q_UNUSED(column);                                       //Q_UNUSED()는 인자를 만들어놓고 사용하지 않을 때 쓰면 경고를 없앨 수 있다.
    ui->clientIDLineEdit->setText(item->text(0));           //treeWidget에서 선택된 항목 중 고객 ID가 clientIDLineEdit에 나타납니다.
    ui->clientNameLineEdit->setText(item->text(1));         //treeWidget에서 선택된 항목 중 고객 이름이 clientNameLineEdit애 나타납니다.
    ui->phoneNumberLineEdit->setText(item->text(2));        //treeWidget에서 선택된 항목 중 고객 전화번호가 phoneNumberLineEdit에 나타납니다.
    ui->addressLineEdit->setText(item->text(3));            //treeWidget에서 선택된 항목 중 고객 주소가 addressLineEdit에 나타납니다.
    ui->emailLineEdit->setText(item->text(4));              //treeWidget에서 선택된 항목 중 고객 이메일이 emailLineEdit에 나타납니다.
}

void ClientManagerForm::on_addPushButton_clicked()          //Add(고객 추가) 버튼을 클릭했을 때
{
    QString clientname, phonenumber, address, email;
    int clientID = makeId();                                //고객 ID 자동할당
    clientname = ui->clientNameLineEdit->text();            //clientNameLineEdit에 입력한 text는 고객 이름
    phonenumber = ui->phoneNumberLineEdit->text();          //phoneNumberLineEdit에 입력한 text는 고객 전화번호
    address = ui->addressLineEdit->text();                  //addressLineEdit 입력한 text는 고객 주소
    email = ui->emailLineEdit->text();                      //emailLineEdit 입력한 text는 고객 이메일
    if(clientname.length()) {
        Client* client = new Client(clientID, clientname, phonenumber, address, email);
        clientList.insert(clientID, client);                //key : clientID, value : client
        ui->treeWidget->addTopLevelItem(client);            //treeWidget에 client를 최상위 항목으로 추가
        emit clientAdded(clientID, clientname);             //clientAdded 시그널 발생
    }
    ui->clientIDLineEdit->setText("");                      //Add 버튼을 누르고 나서 clientIDLineEdit을 공백으로 비워줌
    ui->clientNameLineEdit->setText("");                    //clientNameLineEdit을 공백으로 비워줌
    ui->phoneNumberLineEdit->setText("");                   //phoneNumberLineEdit을 을 공백으로 비워줌
    ui->addressLineEdit->setText("");                       //addressLineEdit을 공백으로 비워줌
    ui->emailLineEdit->setText("");                         //emailLineEdit을 공백으로 비워줌
}

void ClientManagerForm::on_modifyPushButton_clicked()       //Modify(수정) 버튼을 클릭했을 때
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();      //item = 현재 항목
    if(item != nullptr) {                                       //항목이 비어있지 않으면
        int key = item->text(0).toInt();
        int index = ui->treeWidget->currentIndex().row();       //index = treeWidget에 현재 행의 인덱스, sig_sendModifyClient 시그널에서 인자로 사용하기 위해 선언
        Client* clientID = clientList[key];
        QString clinetname, phonenumber, address, email;
        clinetname = ui->clientNameLineEdit->text();
        phonenumber = ui->phoneNumberLineEdit->text();
        address = ui->addressLineEdit->text();
        email = ui->emailLineEdit->text();
        clientID->setClientName(clinetname);                //고객 이름 수정
        clientID->setPhoneNumber(phonenumber);              //고객 전화번호 수정
        clientID->setAddress(address);                      //고객 주소 수정
        clientID->setEmail(email);                          //고객 이메일 수정
        clientList[key] = clientID;
        emit sig_sendModifyClient(key, index, clinetname);      //ClientFrom에서 고객 이름 변경 시 ServerForm의 고객 이름이 함께 변경되는 시그널
    }
}

void ClientManagerForm::on_searchPushButton_clicked()       //검색 기능을 수행하는 함수
{
    ui->searchTreeWidget->clear();
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            Client* client = static_cast<Client*>(i);
            int clientID = client->ClientID();                  //client에 있는 고객 ID를 찾아줍니다.
            QString clientName = client->getClientName();       //client에 있는 고객 이름을 찾아줍니다.
            QString phonenumber = client->getPhoneNumber();     //client에 있는 고객 전화번호를 찾아줍니다.
            QString address = client->getAddress();             //client에 있는 고객 주소를 찾아줍니다.
            QString email = client->getEmail();                 //client에 있는 고객 이메일을 찾아줍니다.
            Client* item = new Client(clientID, clientName, phonenumber, address, email);       //찾은 고객 정보를 item에 담아서
            ui->searchTreeWidget->addTopLevelItem(item);                                        //searchTreeWidget에 보여줍니다.
        }
    }
}

void ClientManagerForm::sendSearchedClient(QString clientName)      //sendSearchedClient 슬롯 함수 구현
{
    QList<Client*> searchedClientList;
    foreach(Client* c, clientList)                          //foreach(variable, container)
    {
        if(clientName == c->getClientName())                //Order에서 입력받은 이름과 Client에 있는 회원 이름 정보가 일치할 경우
        searchedClientList.append(c);
    }
    emit sig_sendSearchedClient(searchedClientList);
}

void ClientManagerForm::sendSearchedID(int infoClient)      //sendSearchedID 슬롯 함수 구현
{
    Client* client = clientList[infoClient];
    emit sig_sendSearchedID(client);
}

