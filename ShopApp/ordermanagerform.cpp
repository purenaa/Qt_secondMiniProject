#include "ordermanagerform.h"
#include "ui_ordermanagerform.h"
#include "clientmanagerform.h"
#include "productmanagerform.h"
#include <QFile>
#include <QMenu>
#include <QDate>
#include <QDateEdit>
#include <QPalette>


OrderManagerForm::OrderManagerForm(ClientManagerForm* clientManger, ProductManagerForm* productManager,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManagerForm)
{
    this->clientManager = clientManger;
    this->productManager = productManager;
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 1900 << 1300;               //1900 : treeWidget size, 1300 : toolBox size
    ui->splitter->setSizes(sizes);

    QAction* removeAction = new QAction(tr("&Remove"));         //항목 제거
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;       //menu : 멤버변수
    QDate now = QDate::currentDate();       //now : 현재날짜
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->purchaseDayDateEdit->setDate(now);                              //purchaseDayDateEdit을 현재날짜로 설정
    ui->purchaseDayDateEdit->setCalendarPopup(true);
    //ui->orderIDLineEdit->setPalette(QPalette(Qt::gray));

    //treeWidget size 변경
    ui->treeWidget->header()->resizeSection(0, 100);            //Purchase Day size
    ui->treeWidget->header()->resizeSection(1, 65);             //Order ID size
    ui->treeWidget->header()->resizeSection(2, 65);             //Client Name size
    ui->treeWidget->header()->resizeSection(3, 240);            //Product Name size
    ui->treeWidget->header()->resizeSection(4, 40);             //Purchase Quantity size
    ui->treeWidget->header()->resizeSection(5, 70);             //Total Price size
    //searchTreeWidget size 변경
    ui->searchTreeWidget->header()->resizeSection(0, 80);           //Purchase Day size
    ui->searchTreeWidget->header()->resizeSection(1, 60);           //Order ID size
    ui->searchTreeWidget->header()->resizeSection(2, 60);           //Client Name size
    ui->searchTreeWidget->header()->resizeSection(3, 60);           //Product Name size
    ui->searchTreeWidget->header()->resizeSection(4, 80);           //Purchase Quantity size
    ui->searchTreeWidget->header()->resizeSection(5, 90);           //Total Price size
    //clientTreeWidget size 변경
    ui->clientTreeWidget->header()->resizeSection(0, 80);           //Client Name size
    ui->clientTreeWidget->header()->resizeSection(1, 100);          //PhoneNumber size
    ui->clientTreeWidget->header()->resizeSection(2, 100);          //Address size
    ui->clientTreeWidget->header()->resizeSection(3, 100);          //Email size
    //productTreeWidget size 변경
    ui->productTreeWidget->header()->resizeSection(0, 120);         //Product Name size
    ui->productTreeWidget->header()->resizeSection(1, 120);         //Price size
    ui->productTreeWidget->header()->resizeSection(2, 120);         //Inventory Amount size


    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    QFile file("orderList.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);      //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공합니다.
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");      //QList 클래스는 동적 배열을 제공하는 템플릿 클래스입니다.
        if(row.size()) {
            int orderID = row[1].toInt();
            Order* order = new Order(row[0], orderID, row[2], row[3], row[4], row[5]);
            ui->treeWidget->addTopLevelItem(order);
            orderList.insert(orderID, order);
        }
    }
    file.close( );
}

OrderManagerForm::~OrderManagerForm()
{
    delete ui;

    QFile file("orderList.txt");       //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공합니다.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : orderList) {
        Order* order = v;

        out << order->getPurchaseDay() << ", " << order->OrderID() << ", ";
        out << order->getClientName() << ", " << order->getProductName()<< ", ";
        out << order->getPurchaseQuantity() << ", " << order->getTotalPrice() << "\n";
    }
    file.close( );
}

int OrderManagerForm::makeId( )
{
    if(orderList.size( ) == 0) {
        return 1000000;
    } else {
        auto orderID = orderList.lastKey();
        return ++orderID;
    }
}

void OrderManagerForm::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        orderList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        ui->treeWidget->update();
    }
}

void OrderManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void OrderManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->purchaseDayDateEdit->setDate(QDate::fromString(item->text(0),"YYYY-MM-DD"));
    ui->orderIDLineEdit->setText(item->text(1));
    ui->clientNameLineEdit->setText(item->text(2));
    ui->productNameComboBox->setCurrentText(item->text(3));
    ui->purchaseQuantityLineEdit->setText(item->text(4));
    ui->totalPriceLineEdit->setText(item->text(5));

    ui->clientTreeWidget->clear();                                              //누를때마다 treeWidget초기화 시켜주기 위함
    Client* clickedClient = clientManager->searchName(item->text(2));           //누른 주문 정보의 회원 이름 정보로 해당 이름의 회원 정보를 가져옴
    if(clickedClient != nullptr)                                        // 해당 이름의 회원 정보가 있을때만 작업하기 위한 예외처리, 없을경우 회원 정보에 없는 회원 이름일경우 프로그램 Down...
    {
        QTreeWidgetItem* clickedClientItem = new QTreeWidgetItem();             // 회원 정보 아이템을 담을 clickedClientItem 객체 생성
        clickedClientItem->setText(0,clickedClient->getClientName());           // clickedClientItem의 0번에 회원 이름 추가
        clickedClientItem->setText(1,clickedClient->getPhoneNumber());          // clickedClientItem의 1번에 전화번호 추가
        clickedClientItem->setText(2,clickedClient->getAddress());              // clickedClientItem의 2번에 주소 추가
        clickedClientItem->setText(3,clickedClient->getEmail());                // clickedClientItem의 3번에 이메일 추가
        ui->clientTreeWidget->addTopLevelItem(clickedClientItem);               // 회원 정보를 담은 clickedClientItem을 treeWidget초기화 추가
    }

    ui->productTreeWidget->clear();
    Product* clickedProduct = productManager->searchName(item->text(3));           //누른 주문 정보의 상품 이름 정보로 해당 이름의 상품 정보를 가져옴
    if(clickedProduct != nullptr)                                        // 해당 이름의 상품 정보가 있을때만 작업하기 위한 예외처리, 없을경우 상품 정보에 없는 상품 이름일경우 프로그램 Down...
    {
        QTreeWidgetItem* clickedProductItem = new QTreeWidgetItem();               // 상품 정보 아이템을 담을 clickedProductItem 객체 생성
        clickedProductItem->setText(0,clickedProduct->getProductName());           // clickedProductItem의 0번에 상품명 추가
        clickedProductItem->setText(1,clickedProduct->getPrice());                 // clickedProductItem의 1번에 가격 추가
        clickedProductItem->setText(2,clickedProduct->getInventoryAmount());       // clickedProductItem의 2번에 재고수량 추가
        ui->productTreeWidget->addTopLevelItem(clickedProductItem);                 // 상품 정보를 담은 clickedProductItem의 treeWidget초기화 추가
    }
}

void OrderManagerForm::on_addPushButton_clicked()
{
    QString purchaseDay, clientName, productName, purchaseQuantity, totalPrice;
    int orderID = makeId( );

    purchaseDay = ui->purchaseDayDateEdit->text();
    clientName = ui->clientNameLineEdit->text();
    productName = ui->productNameComboBox->currentText();
    purchaseQuantity = ui->purchaseQuantityLineEdit->text();
    totalPrice = ui->totalPriceLineEdit->text();

    if(purchaseDay.length()) {
        Order* order = new Order(purchaseDay, orderID, clientName, productName, purchaseQuantity, totalPrice);
        orderList.insert(orderID, order);
        ui->treeWidget->addTopLevelItem(order);
    }
    ui->clientNameLineEdit->setText("");
    ui->productNameComboBox->currentText();
    ui->purchaseQuantityLineEdit->setText("");
    ui->totalPriceLineEdit->setText("");
}


void OrderManagerForm::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int key = item->text(1).toInt();
        Order* orderID = orderList[key];
        QString purchaseDay, clientName, productName, purchaseQuantity, totalPrice;

        purchaseDay = ui->purchaseDayDateEdit->text();
        clientName = ui->clientNameLineEdit->text();
        productName = ui->productNameComboBox->currentText();
        purchaseQuantity = ui->purchaseQuantityLineEdit->text();
        totalPrice = ui->totalPriceLineEdit->text();

        orderID->setPurchaseDay(purchaseDay);
        orderID->setClientName(clientName);
        orderID->setProductName(productName);
        orderID->setPurchaseQuantity(purchaseQuantity);
        orderID->setTotalPrice(totalPrice);
        orderList[key] = orderID;
    }
}

void OrderManagerForm::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            Order* order = static_cast<Order*>(i);
            QString purchaseDay = order->getPurchaseDay();
            int orderID = order->OrderID();
            QString clientName = order->getClientName();
            QString productName = order->getProductName();
            QString purchaseQuantity = order->getPurchaseQuantity();
            QString totalPrice = order->getTotalPrice();
            Order* item = new Order(purchaseDay, orderID, clientName, productName, purchaseQuantity, totalPrice);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}








