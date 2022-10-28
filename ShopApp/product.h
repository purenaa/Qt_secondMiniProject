#ifndef PRODUCT_H
#define PRODUCT_H

#include <QTreeWidgetItem>

class Product : public QTreeWidgetItem
{  
public:
    explicit Product(int productID = 0, QString = "", QString = "", int = 0);


    QString getProductName() const;         //제품 이름 반환
    void setProductName(QString&);          //제품 이름 설정

    QString getPrice() const;               //제품 가격 반환
    void setPrice(QString&);                //제품 가격 설정

    int getInventoryAmount() const;         //제품 재고수량 반환
    void setInventoryAmount(int&);          //제품 재고수량 설정

    int ProductID() const;                  //제품 ID 반환
};
#endif // PRODUCT_H

