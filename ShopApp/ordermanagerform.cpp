#include "ordermanagerform.h"
#include "ui_ordermanagerform.h"
#include <QFile>
#include <QMenu>
#include <QDate>
#include <QDateEdit>
#include <QPalette>
#include <QTreeWidgetItem>
#include <QMessageBox>


OrderManagerForm::OrderManagerForm(QWidget *parent)                 //생성자
    : QWidget(parent), clientID(-1),
      ui(new Ui::OrderManagerForm)

{
    ui->setupUi(this);                                              //지정한 위젯에 대한 사용자 인터페이스를 설정

    QList<int> sizes;
    sizes << 1900 << 1300;                                          //1900 : treeWidget size, 1300 : toolBox size
    ui->splitter->setSizes(sizes);                                  //splitter 크기 조절

    QAction* removeAction = new QAction(tr("&Remove"));             //삭제 기능 생성
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;                                               //menu : 멤버변수       //메뉴 생성
    QDate now = QDate::currentDate();                               //now : 현재날짜
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);    //setContextMenuPolicy : 위젯이 상황에 맞는 메뉴를 표시하는 방법
                                                                    //CustomContextMenu : 위젯이 신호를 보냅니다.
    ui->purchaseDayDateEdit->setDate(now);                          //purchaseDayDateEdit을 현재날짜로 설정
    ui->purchaseDayDateEdit->setCalendarPopup(true);
    emit sig_requestProductNames();

    //treeWidget size 변경
    ui->treeWidget->header()->resizeSection(0, 100);                //Purchase Day size
    ui->treeWidget->header()->resizeSection(1, 65);                 //Order ID size
    ui->treeWidget->header()->resizeSection(2, 65);                 //Client Name size
    ui->treeWidget->header()->resizeSection(3, 210);                //Product Name size
    ui->treeWidget->header()->resizeSection(4, 55);                 //Purchase Quantity size
    ui->treeWidget->header()->resizeSection(5, 70);                 //Total Price size
    //searchTreeWidget size 변경
    ui->searchTreeWidget->header()->resizeSection(0, 80);           //Purchase Day size
    ui->searchTreeWidget->header()->resizeSection(1, 60);           //Order ID size
    ui->searchTreeWidget->header()->resizeSection(2, 60);           //Client Name size
    ui->searchTreeWidget->header()->resizeSection(3, 60);           //Product Name size
    ui->searchTreeWidget->header()->resizeSection(4, 80);           //Purchase Quantity size
    ui->searchTreeWidget->header()->resizeSection(5, 90);           //Total Price size
    //clientTreeWidget size 변경
    ui->clientTreeWidget->header()->resizeSection(0, 50);           //Client ID size
    ui->clientTreeWidget->header()->resizeSection(1, 60);           //Client Name size
    ui->clientTreeWidget->header()->resizeSection(2, 100);          //Phone Number size
    ui->clientTreeWidget->header()->resizeSection(3, 100);          //Address size
    ui->clientTreeWidget->header()->resizeSection(3, 100);          //Email size
    //productTreeWidget size 변경
    ui->productTreeWidget->header()->resizeSection(0, 120);         //Product Name size
    ui->productTreeWidget->header()->resizeSection(1, 80);          //Price size
    ui->productTreeWidget->header()->resizeSection(2, 120);         //Inventory Amount size

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));
}

void OrderManagerForm::loadData()
{
    QFile file("orderList.txt");                                    //파일 이름 : orderList.txt 으로 저장
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))          //파일이 열어서 읽을 수 없으면 return
        return;

    QTextStream in(&file);                                          //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공합니다.
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");                      //line을 ,로 구분함  //QList 클래스는 동적 배열을 제공하는 템플릿 클래스
        if(row.size()) {
            int orderID = row[1].toInt();
            Order* order = new Order(row[0], orderID, row[6].toInt(), row[2], row[3], row[4].toInt(), row[5]);
                                                                    //0: 구매 날짜, 1: 주문 ID, 2: 고객 이름, 3: 제품 이름, 4: 구매 수량, 5: 총 구매 가격
            ui->treeWidget->addTopLevelItem(order);                 //treeWidget에 order를 최상위 항목으로 추가
            orderList.insert(orderID, order);                       //key : orderID, value : order
        }
    }
    file.close( );                                                  //파일을 닫음
}

OrderManagerForm::~OrderManagerForm()                               //소멸자
{
    delete ui;

    QFile file("orderList.txt");                                    //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))         //파일을 열어서 쓸 수 없으면 return
        return;

    QTextStream out(&file);                                         //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공
    for (const auto& v : orderList) {
        Order* order = v;
        out << order->getPurchaseDay() << ", " << order->OrderID() << ", ";
        out << order->getClientName() << ", " << order->getProductName()<< ", ";
        out << order->getPurchaseQuantity() << ", " << order->getTotalPrice() << ", ";
        out << order->getClientID() << "\n";
    }
    file.close( );                                                  //파일을 닫음
}

int OrderManagerForm::makeId( )                                     //주문 ID를 자동 할당해주는 함수
{
    if(orderList.size( ) == 0) {                                    //초기 orderList에 정보가 없을 경우 주문 ID 1000000을 반환
        return 1000000;
    } else {
        auto orderID = orderList.lastKey();                         //orderList에가 비어있지 않으면
        return ++orderID;                                           //이전 반환 값 1000000에 ++productID
    }
}

void OrderManagerForm::removeItem()                                 //항목(item)을 제거해주는 함수
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();          //item = 현재 항목
    if(item != nullptr) {                                           //item이 nullptr이 아니면
        orderList.remove(item->text(1).toInt());                    //orderList의 선택된 orderID 제거
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        ui->treeWidget->update();                                   //treeWidget 업데이트
        emit sig_reduceInventoryAmount(item->text(3), item->text(4).toInt()*(-1));      //고객이 구매한 만큼 재고 수량을 차감해주는 시그널 발생
    }
}

void OrderManagerForm::showContextMenu(const QPoint &pos)           //마우스 커서 위치
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void OrderManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)       //treeWidget에 있는 항목(item)을 선택했을 때
{
    Q_UNUSED(column);                                               //Q_UNUSED()는 인자를 만들어놓고 사용하지 않을 때 쓰면 경고를 없앨 수 있다.
    QDate date = QDate::fromString(item->text(0),"yyyy-MM-dd");     //인덱스 0에 저장된 해당 날짜
    ui->purchaseDayDateEdit->setDate(date);                         //treeWidget에서 선택된 항목 중 해당 구매 날짜가 purchaseDayDateEdit에 나타납니다.
    ui->orderIDLineEdit->setText(item->text(1));                    //treeWidget에서 선택된 항목 중 주문 ID가 orderIDLineEdit에 나타납니다.
    ui->clientNameLineEdit->setText(item->text(2));                 //treeWidget에서 선택된 항목 중 고객 이름이 clientNameLineEdit에 나타납니다.
    ui->productNameComboBox->setCurrentText(item->text(3));         //treeWidget에서 선택된 항목 중 제품 이름이 productNameComboBox에 나타납니다.
    ui->purchaseQuantitySpinBox->setValue(item->text(4).toInt());   //treeWidget에서 선택된 항목 중 구매 수량이 purchaseQuantitySpinBox에 나타납니다.
    ui->totalPriceLineEdit->setText(item->text(5));                 //treeWidget에서 선택된 항목 중 총 구매 가격이 totalPriceLineEdit에 나타납니다.
    ui->clientTreeWidget->clear();                                  //clientTreeWidget 초기화

    Order * order = orderList[item->text(1).toInt()];
    int clientID = 1;
    clientID = order->getClientID();
    emit sig_requesSearchID(clientID);
    ui->productTreeWidget->clear();                                 //productTreeWidget 초기화
    emit sig_requesProductNameSearch(item->text(3));
}

void OrderManagerForm::on_addPushButton_clicked()                   //Add(고객 추가) 버튼을 클릭했을 때
{
    QString purchaseDay, clientName, productName, totalPrice;
    int purchaseQuantity;
    int orderID = makeId( );                                        //주문 ID 자동할당
    purchaseDay = ui->purchaseDayDateEdit->text();                  //purchaseDayDateEdit에서 입력한 text는 구매 날짜
    clientName = ui->clientNameLineEdit->text();                    //clientNameLineEdit에 입력한 text는 고객 이름
    productName = ui->productNameComboBox->currentText();           //productNameComboBox의 currentText는 제품 이름
    purchaseQuantity = ui->purchaseQuantitySpinBox->value();        //purchaseQuantitySpinBox에 입력한 value는 구매 수량
    totalPrice = ui->totalPriceLineEdit->text();                    //totalPriceLineEdit에 입력한 text는 총 구매 가격

    /*order에서 입력한 고객 이름이 client의 고객 정보에 없다. 즉, 등록된 고객이 없을 경우 경고창이 나타난다.*/
    if(this->clientID == -2) {
        QMessageBox::critical(this, "Client Search", "등록되지 않은 고객입니다.");
        return;
    }

    /* 동명이인일 경우 해당 이름을 가진 고객들이 clientTreeWidget에 나타나게 되고 그 중 해당 고객 1명을 선택하지 않으면 경고창이 나타난다. */
    if(this->clientID == -1) {
        QMessageBox::information(this, "Client Select", "고객이 선택되지 않았습니다. 선택 후 다시 시도해주세요.");
        return;
    }

    if(purchaseDay.length()) {
        Order* order = new Order(purchaseDay, orderID, this->clientID, clientName, productName, purchaseQuantity, totalPrice);
        orderList.insert(orderID, order);                           //key : orderID, value : order
        ui->treeWidget->addTopLevelItem(order);                     //treeWidget에 order를 최상위 항목으로 추가
        emit sig_reduceInventoryAmount(productName, purchaseQuantity);
    }
    ui->orderIDLineEdit->setText("");                               //Add 버튼을 누르고 나서 orderIDLineEdit을 공백으로 비워줌
    ui->clientNameLineEdit->setText("");                            //lientNameLineEdit을 공백으로 비워줌
    ui->purchaseQuantitySpinBox->setValue(0);                       //purchaseQuantitySpinBox의 값을 0으로 바꿔줌
    ui->totalPriceLineEdit->setText("");                            //totalPriceLineEdit을 공백으로 비워줌

    this->clientID = -1;

    ui->clientTreeWidget->clear();
    ui->productTreeWidget->clear();
}


void OrderManagerForm::on_modifyPushButton_clicked()                //Modify(수정) 버튼을 클릭했을 때
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();          //item = 현재 항목
    if(item != nullptr) {                                           //항목이 비어있지 않으면
        int key = item->text(1).toInt();
        Order* orderID = orderList[key];
        QString purchaseDay, clientName, productName, totalPrice;
        int purchaseQuantity;
        purchaseDay = ui->purchaseDayDateEdit->text();
        clientName = ui->clientNameLineEdit->text();
        productName = ui->productNameComboBox->currentText();
        purchaseQuantity = ui->purchaseQuantitySpinBox->value();
        totalPrice = ui->totalPriceLineEdit->text();

        emit sig_reduceInventoryAmount(productName, purchaseQuantity-item->text(4).toInt());

        orderID->setPurchaseDay(purchaseDay);                       //구매 날짜 수정
        orderID->setClientName(clientName);                         //주문 고객 이름 수정
        orderID->setProductName(productName);                       //제품 이름 선택 변경
        orderID->setPurchaseQuantity(purchaseQuantity);             //구매 수량 수정
        orderID->setTotalPrice(totalPrice);                         //총 구매 가격 수정
        orderList[key] = orderID;
    }
}

void OrderManagerForm::on_searchPushButton_clicked()                //검색 기능을 수행하는 함수
{
    ui->searchTreeWidget->clear();                                  //searchTreeWidget 초기화
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            Order* order = static_cast<Order*>(i);
            QString purchaseDay = order->getPurchaseDay();          //order에 있는 구매 날짜를 찾아줍니다.
            int orderID = order->OrderID();                         //order에 있는 주문 ID를 찾아줍니다.
            int clientID = order->getClientID();                    //order에 있는 고객 ID를 찾아줍니다.
            QString clientName = order->getClientName();            //order에 있는 고객 이름을 찾아줍니다.
            QString productName = order->getProductName();          //order에 있는 제품 이름을 찾아줍니다.
            int purchaseQuantity = order->getPurchaseQuantity();    //order에 있는 구매 수량를 찾아줍니다.
            QString totalPrice = order->getTotalPrice();            //order에 있는 총 구매 가격을 찾아줍니다.
            Order* item = new Order(purchaseDay, orderID, clientID, clientName, productName,
                                    purchaseQuantity, totalPrice);  //찾은 주문 정보를 item에 담아서
            ui->searchTreeWidget->addTopLevelItem(item);            //searchTreeWidget에 보여줍니다.
        }
    }
}

void OrderManagerForm::update(QList<QString> productNameList)
{
    ui->productNameComboBox->clear();                               //productNameComboBox 초기화
    ui->productNameComboBox->addItems(productNameList);
}

void OrderManagerForm::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    clientID = item->text(0).toInt();
}

void OrderManagerForm::on_clientNameLineEdit_textEdited(const QString &clientName)
{
    emit sig_requesSearchClient(clientName);                        //인자: 입력한 이름을 시그널로 넘겨줌, emit : 시그널 발생
}

int OrderManagerForm::calTotalPrice()
{
    return 0;
}

void OrderManagerForm::on_productNameComboBox_textActivated(const QString &item)
{
    QString imagePath(tr("smallImg/%1.png").arg(item));
    QPixmap* selectImage = new QPixmap(imagePath);
    ui->imageLabel->setPixmap(*selectImage);
}

void OrderManagerForm::updateClientTreeWdiget(QList<Client*> searchedClientList)
{
    ui->clientTreeWidget->clear();                                  //clientTreeWidget 초기화
    if(searchedClientList.size() != 0)
    {
        foreach(Client *c, searchedClientList)
        {
            QTreeWidgetItem* clickedClientItem = new QTreeWidgetItem();     // 회원 정보 아이템을 담을 clickedClientItem 객체 생성
            clickedClientItem->setText(0, QString::number(c->ClientID()));  // clickedClientItem의 0번에 회원 ID 추가
            clickedClientItem->setText(1, c->getClientName());              // clickedClientItem의 1번에 회원 이름 추가
            clickedClientItem->setText(2, c->getPhoneNumber());             // clickedClientItem의 2번에 전화번호 추가
            clickedClientItem->setText(3, c->getAddress());                 // clickedClientItem의 3번에 주소 추가
            clickedClientItem->setText(4, c->getEmail());                   // clickedClientItem의 4번에 이메일 추가
            ui->clientTreeWidget->addTopLevelItem(clickedClientItem);       // 회원 정보를 담은 clickedClientItem을 treeWidget초기화 추가
        }
    }
    if(searchedClientList.size() == 1)                                      //동명이인이 없으면 선택할 필요가 없는 경우
    {
        this->clientID = searchedClientList[0]->ClientID();
    }
    else if(searchedClientList.size() == 0)                                 //등록된 회원이 아닌 경우
    {
        this->clientID = -2;
    }
    else
    {
        this->clientID = -1;
    }
}

void OrderManagerForm::updateClient(Client* clickedClient)
{
    if(clickedClient != nullptr)                                            //해당 이름의 회원 정보가 있을때만 작업하기 위한 예외처리, 없을경우 회원 정보에 없는 회원 이름일경우 프로그램 Down...
    {
        QTreeWidgetItem* clickedClientItem = new QTreeWidgetItem();         //회원 정보 아이템을 담을 clickedClientItem 객체 생성
        clickedClientItem->setText(0,QString::number(clickedClient->ClientID()));   //clickedClientItem의 0번에 회원 ID 추가
        clickedClientItem->setText(1, clickedClient->getClientName());       //clickedClientItem의 1번에 회원 이름 추가
        clickedClientItem->setText(2, clickedClient->getPhoneNumber());      //clickedClientItem의 2번에 전화번호 추가
        clickedClientItem->setText(3, clickedClient->getAddress());          //clickedClientItem의 3번에 주소 추가
        clickedClientItem->setText(4, clickedClient->getEmail());            //clickedClientItem의 4번에 이메일 추가
        ui->clientTreeWidget->addTopLevelItem(clickedClientItem);            //회원 정보를 담은 clickedClientItem을 treeWidget초기화 추가
    }
}

void OrderManagerForm::updateProduct(Product* product, int purchaseQuantity)
{
    ui->purchaseQuantitySpinBox->setRange(0, product->getInventoryAmount());
    if(purchaseQuantity == -1)
    {
        if(product != nullptr)                                              //해당 이름의 상품 정보가 있을때만 작업하기 위한 예외처리, 없을경우 상품 정보에 없는 상품 이름일경우 프로그램 Down...
        {
            QTreeWidgetItem* clickedProductItem = new QTreeWidgetItem();    // 상품 정보 아이템을 담을 clickedProductItem 객체 생성
            clickedProductItem->setText(0,product->getProductName());       // clickedProductItem의 0번에 상품명 추가
            clickedProductItem->setText(1,product->getPrice());             // clickedProductItem의 1번에 가격 추가
            clickedProductItem->setText(2,QString::number(product->getInventoryAmount()));    // clickedProductItem의 2번에 재고수량 추가
            ui->productTreeWidget->addTopLevelItem(clickedProductItem);     // 상품 정보를 담은 clickedProductItem의 treeWidget초기화 추가

            QString imagePath(tr("smallImg/%1.png").arg(product->getProductName()));    //이미지 경로
            QPixmap* selectImage = new QPixmap(imagePath);
            ui->imageLabel->setPixmap(*selectImage);                        //imageLabel 위치에 제품 이름과 일치하는 제품 사진을 보여줍니다.
        }
    }
    else
    {
        QString price = product->getPrice();                                //price : 제품의 가격
        int totalPrice = purchaseQuantity * price.toInt();                  //총 구매 가격 = 재고수량 x 제품의 가격
        ui->totalPriceLineEdit->setText(QString::number(totalPrice));
    }
}


void OrderManagerForm::on_purchaseQuantitySpinBox_valueChanged(int purchaseQuantity)
{
    QString productName = ui->productNameComboBox->currentText();           //productName = 현재 productNameComboBox에서 선택된 제품의 이름
    emit sig_requesProductNameSearch(productName, purchaseQuantity);
}


void OrderManagerForm::on_clearPushButton_clicked()                         //Clear 버튼을 눌렀을 때
{
    ui->clientTreeWidget->clear();
    ui->productTreeWidget->clear();

    ui->orderIDLineEdit->setText("");                                       //Add 버튼을 누르고 나서 orderIDLineEdit을 공백으로 비워줌
    ui->clientNameLineEdit->setText("");                                    //lientNameLineEdit을 공백으로 비워줌
    ui->purchaseQuantitySpinBox->setValue(0);                               //purchaseQuantitySpinBox의 값을 0으로 바꿔줌
    ui->totalPriceLineEdit->setText("");
}

