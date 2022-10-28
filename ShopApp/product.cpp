#include "product.h"

#include <QLabel>
#include <iostream>

using namespace std;

Product::Product(int productID, QString productName, QString price, int inventoryAmount)
{
    setText(0, QString::number(productID));
    setText(1, productName);
    setText(2, price);
    setText(3, QString::number(inventoryAmount));
}

QString Product::getProductName() const                     //제품 이름 반환
{
    return text(1);
}

void Product::setProductName(QString& productName)          //제품 이름 설정
{
    setText(1, productName);
}

QString Product::getPrice() const                           //제품 가격 반환
{
    return text(2);
}

void Product::setPrice(QString& price)                      //제품 가격 설정
{
    setText(2, price);
}

int Product::getInventoryAmount() const                     //제품 재고수량 반환
{
    return text(3).toInt();
}

void Product::setInventoryAmount(int& inventoryAmount)		//제품 재고수량 설정
{
    setText(3, QString::number(inventoryAmount));
}

int Product::ProductID() const                              //제품 ID 반환
{
    return text(0).toInt();
}
