#include "productmanagerform.h"
#include "ui_productmanagerform.h"
#include "product.h"

#include <QMenu>
#include <QFile>
#include <QList>
#include <QPixmap>

ProductManagerForm::ProductManagerForm(QWidget *parent)         //생성자
    : QWidget(parent),
      ui(new Ui::ProductManagerForm)
{
    ui->setupUi(this);                                          //지정한 위젯에 대한 사용자 인터페이스를 설정

    QList<int> sizes;
    sizes << 1900 << 1300;                                      //1800 : treeWidget size, 1300 : toolBox size
    ui->splitter->setSizes(sizes);                              //splitter 크기 조절


    QAction* removeAction = new QAction(tr("&Remove"));         //삭제 기능 생성
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;                                           //menu : 멤버변수       //메뉴 생성
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);        //setContextMenuPolicy : 위젯이 상황에 맞는 메뉴를 표시하는 방법
                                                                        //CustomContextMenu : 위젯이 신호를 보냅니다.
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

    //treeWidget size 변경
    ui->treeWidget->header()->resizeSection(0, 70);             //Product ID size
    ui->treeWidget->header()->resizeSection(1, 380);            //Product Name size
    ui->treeWidget->header()->resizeSection(2, 80);             //Price size
    ui->treeWidget->header()->resizeSection(3, 70);             //Inventory Amount size
    //searchTreeWidget size 변경
    ui->searchTreeWidget->header()->resizeSection(0, 80);       //Product ID size
    ui->searchTreeWidget->header()->resizeSection(1, 130);      //Product Name size
    ui->searchTreeWidget->header()->resizeSection(2, 60);       //Price size
    ui->searchTreeWidget->header()->resizeSection(3, 120);      //Inventory Amount size
}

/* 파일 입출력 */
void ProductManagerForm::loadData()
{
    QFile file("productList.txt");                              //파일 이름 : productList.txt 으로 저장
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))      //파일이 열어서 읽을 수 없으면 return
        return;

    QTextStream in(&file);                                      //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공합니다.
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> row = line.split(", ");                  //line을 ,로 구분함  //QList 클래스는 동적 배열을 제공하는 템플릿 클래스
        if(row.size()) {
            int productID = row[0].toInt();
            Product* product = new Product(productID, row[1], row[2], row[3].toInt());
                                                                //0: 제품 ID, 1: 제품 이름, 2: 제품 가격
            ui->treeWidget->addTopLevelItem(product);           //treeWidget에 product를 최상위 항목으로 추가
            productList.insert(productID, product);             //key : productID, value : product
        }
    }
    file.close( );                                              //파일을 닫음
}

ProductManagerForm::~ProductManagerForm()                       //소멸자
{
    delete ui;

    QFile file("productList.txt");                              //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))     //파일을 열어서 쓸 수 없으면 return
        return;

    QTextStream out(&file);                                     //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공
    for (const auto& v : productList) {
        Product* product = v;
        out << product->ProductID() << ", " << product->getProductName() << ", ";
        out << product->getPrice() << ", " << product->getInventoryAmount() << "\n";
    }
    file.close( );                                              //파일을 닫음
}

int ProductManagerForm::makeId( )                               //제품 ID를 자동 할당해주는 함수
{
    if(productList.size( ) == 0) {                              //초기 productList에 정보가 없을 경우 제품 ID 10000을 반환
        return 10000;
    } else {
        auto productID = productList.lastKey();                 //productList가 비어있지 않으면
        return ++productID;                                     //이전 반환 값 10000에 ++productID
    }
}

void ProductManagerForm::removeItem()                           //항목(item)을 제거해주는 함수
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();      //item = 현재 항목
    if(item != nullptr) {                                       //item이 nullptr이 아니면
        productList.remove(item->text(0).toInt());              //productList의 선택된 productID 제거
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        ui->treeWidget->update();                               //treeWidget 업데이트
    }
}

void ProductManagerForm::showContextMenu(const QPoint &pos)      //마우스 커서 위치
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void ProductManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)       //treeWidget에 있는 항목(item)을 선택했을 경우
{
    Q_UNUSED(column);                                               //Q_UNUSED()는 인자를 만들어놓고 사용하지 않을 때 쓰면 경고를 없앨 수 있다.
    ui->productIDLineEdit->setText(item->text(0));                  //treeWidget에서 선택된 항목 중 제품 ID가 productIDLineEdit에 나타납니다.
    ui->productNameLineEdit->setText(item->text(1));                //treeWidget에서 선택된 항목 중 제품 ID가 productNameLineEdit에 나타납니다.
    ui->priceLineEdit->setText(item->text(2));                      //treeWidget에서 선택된 항목 중 제품 ID가 priceLineEdit에 나타납니다.
    ui->inventoryAmountSpinBox->setValue(item->text(3).toInt());    //treeWidget에서 선택된 항목 중 제품 ID가 inventoryAmountSpinBox에 나타납니다.

    QString imagePath(tr("img/%1.png").arg(item->text(1)));         //이미지 경로
    QPixmap* selectImage = new QPixmap(imagePath);
    ui->imageLabel->setPixmap(*selectImage);                        //imageLabel 위치에 제품 이름과 일치하는 제품 사진을 보여줍니다.
}

void ProductManagerForm::on_addPushButton_clicked()                 //Add(고객 추가) 버튼을 클릭했을 때
{
    QString productName, price;
    int inventoryAmount;
    int productID = makeId( );                                      //고객 ID 자동할당
    productName = ui->productNameLineEdit->text();                  //productNameLineEdit에 입력한 text는 제품 이름
    price = ui->priceLineEdit->text();                              //priceLineEdit에 입력한 text는 제품 가격
    inventoryAmount = ui->inventoryAmountSpinBox->value();          //inventoryAmountSpinBox에서 선택한 value는 재고 수량
    if(productName.length()) {
        Product* product = new Product(productID, productName, price, inventoryAmount);
        productList.insert(productID, product);                     //key : productID, value : product
        ui->treeWidget->addTopLevelItem(product);                   //treeWidget에 product를 최상위 항목으로 추가
    }
    ui->productIDLineEdit->setText("");                             //Add 버튼을 누르고 나서 productIDLineEdit을 공백으로 비워줌
    ui->productNameLineEdit->setText("");                           //productNameLineEdit을 공백으로 비워줌
    ui->priceLineEdit->setText("");                                 //priceLineEdit을 공백으로 비워줌
    ui->inventoryAmountSpinBox->setValue(0);                        //inventoryAmountSpinBox의 값을 0으로 바꿔줌
}

void ProductManagerForm::on_modifyPushButton_clicked()              //Modify(수정) 버튼을 클릭했을 때
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();          //item = 현재 항목
    if(item != nullptr) {                                           //항목이 비어있지 않으면
        int key = item->text(0).toInt();
        Product* productID = productList[key];
        QString productName, price;
        int inventoryAmount;
        productName = ui->productNameLineEdit->text();
        price = ui->priceLineEdit->text();
        inventoryAmount = ui->inventoryAmountSpinBox->value();
        productID->setProductName(productName);                     //제품 이름 수정
        productID->setPrice(price);                                 //제품 가격 수정
        productID->setInventoryAmount(inventoryAmount);             //제품 재고수량 수정
        productList[key] = productID;
    }
}

void ProductManagerForm::on_searchPushButton_clicked()              //검색 기능을 수행하는 함수
{
    ui->searchTreeWidget->clear();                                  //searchTreeWidget 초기화
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            Product* product = static_cast<Product*>(i);
            int productID = product->ProductID();                   //product에 있는 제품 ID를 찾아줍니다.
            QString productName = product->getProductName();        //product에 있는 제품 이름을 찾아줍니다.
            QString price = product->getPrice();                    //product에 있는 제품 가격을 찾아줍니다.
            int inventoryAmount = product->getInventoryAmount();    //product에 있는 제품 재고수량을 찾아줍니다.
            Product* item = new Product(productID, productName, price, inventoryAmount);    //찾은 고객 정보를 item에 담아서
            ui->searchTreeWidget->addTopLevelItem(item);                                    //searchTreeWidget에 보여줍니다.
        }
    }
}

void ProductManagerForm::slot_getProductNames()                     //slot_getProductNames 슬롯 함수 구현
{
    QList<QString> productNameList;
    foreach(Product *p, productList){                               //foreach(variable, container)
        productNameList.append(p->getProductName());                //apppend : productNameList에 값을 넣는 함수
    }
    emit sig_getProductNames(productNameList);
}

void ProductManagerForm::slot_sendProductNameSearch(QString inputName, int purchaseQuantity)
{
    foreach(Product *p, productList){
        if(inputName == p->getProductName())
        {
            emit sig_sendProductNameSearch(p, purchaseQuantity);
            return;
        }
    }
    emit sig_sendProductNameSearch(nullptr);
}

void ProductManagerForm::updateInventoryAmount(QString productName, int purchaseQuantity)
{
    foreach(Product* p, productList){
        if(productName == p->getProductName())
        {
            int result = p->getInventoryAmount()-purchaseQuantity;
            p->setInventoryAmount(result);
        }
    }
}



