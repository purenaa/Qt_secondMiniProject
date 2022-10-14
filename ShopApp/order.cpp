#include "order.h"

#include <QLabel>
#include <QDate>
#include <iostream>

using namespace std;

Order::Order(QString purchaseDay, int orderID, /*int clientID, int productID,*/ QString clientName, QString productName, QString purchaseQuantity, QString totalPrice)
{

//    setText(1, QString::number(clientID));
//    setText(2, QString::number(productID));
    setText(0, purchaseDay);
    setText(1, QString::number(orderID));
    setText(2, clientName);
    setText(3, productName);
    setText(4, purchaseQuantity);
    setText(5, totalPrice);
}

//int Order::getClientID() const		//회원 ID 반환
//{
//    return text(1).toInt();
//}

//void Order::setClientID(int& clientID)
//{
//    setText(1, QString::number(clientID));
//}

//int Order::getProductID() const		//상품 ID 반환
//{
//    return text(2).toInt();
//}

//void Order::setProductID(int& prodcutID)
//{
//    setText(2, QString::number(prodcutID));
//}

QString Order::getPurchaseDay() const		//구매날짜 반환
{
//    return  QDate::fromString(text(0),"YYYY-MM-DD");    //QString을 QDate로 형변환
    return text(0);
}

void Order::setPurchaseDay(QString& purchasDay)		//구매날짜 설정
{
//    QString str = purchasDay.toString("YYYY-MM-DD");    //QDate을 QString으로 형변환
    setText(0, purchasDay);
}

QString Order::getClientName() const		//회원 이름 반환
{
    return text(2);
}

void Order::setClientName(QString& clientName)      //회원 이름 설정(modify에서 사용)
{
    setText(2, clientName);
}

QString Order::getProductName() const		//상품 이름 반환
{
    return text(3);
}

void Order::setProductName(QString& productName)        //상품 이름 설정(modify에서 사용)
{
    setText(3, productName);
}


QString Order::getPurchaseQuantity() const		//구매수량 반환
{
    return text(4);
}

void Order::setPurchaseQuantity(QString& purchaseQuantity)		//구매수량 설정
{
    setText(4, purchaseQuantity);
}

QString Order::getTotalPrice() const		//총 가격 반환
{
    return text(5);
}

void Order::setTotalPrice(QString& totalPrice)		//총 가격 설정
{
    setText(5, totalPrice);
}

int Order::OrderID() const      //회원 ID 반환
{
    return text(1).toInt();
}

