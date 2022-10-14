#include "product.h"

#include <QLabel>
#include <iostream>

using namespace std;

Product::Product(int productID, QString productName, QString price, QString inventoryAmount)
{
    setText(0, QString::number(productID));
    setText(1, productName);
    setText(2, price);
    setText(3, inventoryAmount);
}

QString Product::getProductName() const		//상품 이름 반환
{
    return text(1);
}

void Product::setProductName(QString& productName)		//상품 이름 설정
{
    setText(1, productName);
}

QString Product::getPrice() const		//상품 가격 반환
{
    return text(2);
}

void Product::setPrice(QString& price)		//상품 가격 설정
{
    setText(2, price);
}

QString Product::getInventoryAmount() const		//상품 재고수량 반환
{
    return text(3);
}

void Product::setInventoryAmount(QString& inventoryAmount)		//상품 재고수량 설정
{
    setText(3, inventoryAmount);
}

int Product::ProductID() const      //회원 ID 반환
{
    return text(0).toInt();
}

// Define copy assignment operator.
bool Product::operator==(const Product &other) const {
    return (this->text(1) == other.text(1));
}
