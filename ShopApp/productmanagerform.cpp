#include "productmanagerform.h"
#include "ui_productmanagerform.h"
#include "product.h"

#include <QMenu>
#include <QFile>

ProductManagerForm::ProductManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductManagerForm)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 1900 << 1300;               //1800 : treeWidget size, 1500 : toolBox size
    ui->splitter->setSizes(sizes);


    QAction* removeAction = new QAction(tr("&Remove"));         //항목 제거
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));

    menu = new QMenu;       //menu : 멤버변수
    menu->addAction(removeAction);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //treeWidget size 변경
    ui->treeWidget->header()->resizeSection(0, 70);       //Product ID size
    ui->treeWidget->header()->resizeSection(1, 380);       //Product Name size
    ui->treeWidget->header()->resizeSection(2, 80);       //Price size
    ui->treeWidget->header()->resizeSection(3, 70);       //Inventory Amount size
    //searchTreeWidget size 변경
    ui->searchTreeWidget->header()->resizeSection(0, 80);       //Product ID size
    ui->searchTreeWidget->header()->resizeSection(1, 130);       //Product Name size
    ui->searchTreeWidget->header()->resizeSection(2, 60);      //Price size
    ui->searchTreeWidget->header()->resizeSection(3, 120);      //Inventory Amount size

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->searchLineEdit, SIGNAL(returnPressed()),
            this, SLOT(on_searchPushButton_clicked()));

     QFile file("productList.txt");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in(&file);      //QTextStream 클래스는 텍스트를 읽고 쓰기에 편리한 인터페이스를 제공합니다.
     while (!in.atEnd()) {
         QString line = in.readLine();
         QList<QString> row = line.split(", ");      //QList 클래스는 동적 배열을 제공하는 템플릿 클래스입니다.
         if(row.size()) {
             int productID = row[0].toInt();
             Product* product = new Product(productID, row[1], row[2], row[3]);
             ui->treeWidget->addTopLevelItem(product);
             productList.insert(productID, product);
         }
     }
     file.close( );
}

ProductManagerForm::~ProductManagerForm()
{
    delete ui;

    QFile file("productList.txt");       //QFile 클래스는 파일을 읽고 쓸 수 있는 인터페이스를 제공합니다.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& v : productList) {
        Product* product = v;
        out << product->ProductID() << ", " << product->getProductName() << ", ";
        out << product->getPrice() << ", " << product->getInventoryAmount() << "\n";
    }
    file.close( );
}

int ProductManagerForm::makeId( )
{
    if(productList.size( ) == 0) {
        return 10000;
    } else {
        auto productID = productList.lastKey();
        return ++productID;
    }
}

void ProductManagerForm::removeItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        productList.remove(item->text(0).toInt());
        ui->treeWidget->takeTopLevelItem(ui->treeWidget->indexOfTopLevelItem(item));
        ui->treeWidget->update();
    }
}

void ProductManagerForm::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void ProductManagerForm::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->productIDLineEdit->setText(item->text(0));
    ui->productNameLineEdit->setText(item->text(1));
    ui->priceLineEdit->setText(item->text(2));
    ui->inventoryAmountLineEdit->setText(item->text(3));
}


void ProductManagerForm::on_addPushButton_clicked()
{
    QString productName, price, inventoryAmount;
    int productID = makeId( );
    productName = ui->productNameLineEdit->text();
    price = ui->priceLineEdit->text();
    inventoryAmount = ui->inventoryAmountLineEdit->text();
    if(productName.length()) {
        Product* product = new Product(productID, productName, price, inventoryAmount);
        productList.insert(productID, product);
        ui->treeWidget->addTopLevelItem(product);
    }
    ui->productIDLineEdit->setText("");
    ui->productNameLineEdit->setText("");
    ui->priceLineEdit->setText("");
    ui->inventoryAmountLineEdit->setText("");
}

void ProductManagerForm::on_modifyPushButton_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if(item != nullptr) {
        int key = item->text(0).toInt();
        Product* productID = productList[key];
        QString productName, price, inventoryAmount;
        productName = ui->productNameLineEdit->text();
        price = ui->priceLineEdit->text();
        inventoryAmount = ui->inventoryAmountLineEdit->text();
        productID->setProductName(productName);
        productID->setPrice(price);
        productID->setInventoryAmount(inventoryAmount);
        productList[key] = productID;
    }
}

void ProductManagerForm::on_searchPushButton_clicked()
{
    ui->searchTreeWidget->clear();
    int i = ui->searchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->treeWidget->findItems(ui->searchLineEdit->text(), flag, i);

        foreach(auto i, items) {
            Product* product = static_cast<Product*>(i);
            int productID = product->ProductID();
            QString productName = product->getProductName();
            QString price = product->getPrice();
            QString inventoryAmount = product->getInventoryAmount();
            Product* item = new Product(productID, productName, price, inventoryAmount);
            ui->searchTreeWidget->addTopLevelItem(item);
        }
    }
}

Product* ProductManagerForm::searchName(const QString &inputName)
{
    foreach(Product *p, productList){
        if(inputName == p->getProductName())
            return p;
    }
    return nullptr;
}
