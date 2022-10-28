#include "order.h"

#include <QLabel>
#include <QDate>
#include <iostream>

using namespace std;

Order::Order(QString purchaseDay, int orderID, int clientID, QString clientName,
             QString productName, int purchaseQuantity, QString totalPrice)
{
    this->clientID = clientID;
    setText(0, purchaseDay);                        //index 0번 : 구매 날짜
    setText(1, QString::number(orderID));           //index 1번 : 주문ID
    setText(2, clientName);                         //index 2번 : 고객 이름
    setText(3, productName);                        //index 3번 : 상품 이름
    setText(4, QString::number(purchaseQuantity));  //index 4번 : 구매 수량
    setText(5, totalPrice);                         //index 5번 : 총 구매 가격
}

int Order::getClientID() const                      //고객 ID 반환
{
    return clientID;
}

void Order::setClientID(int& clientID)              //고객 ID 설정
{
    this->clientID = clientID;
}

QString Order::getPurchaseDay() const               //구매 날짜 반환
{
    return text(0);
}

void Order::setPurchaseDay(QString& purchasDay)     //구매 날짜 설정
{
    setText(0, purchasDay);
}

QString Order::getClientName() const                //고객 이름 반환
{
    return text(2);
}

void Order::setClientName(QString& clientName)      //고객 이름 설정 (modify에서 사용)
{
    setText(2, clientName);
}

QString Order::getProductName() const               //상품 이름 반환
{
    return text(3);
}

void Order::setProductName(QString& productName)    //상품 이름 설정 (modify에서 사용)
{
    setText(3, productName);
}


int Order::getPurchaseQuantity() const              //구매 수량 반환
{
    return text(4).toInt();
}

void Order::setPurchaseQuantity(int& purchaseQuantity)		//구매 수량 설정
{
    setText(4, QString::number(purchaseQuantity));
}

QString Order::getTotalPrice() const                //총 구매 가격 반환
{
    return text(5);
}

void Order::setTotalPrice(QString& totalPrice)      //총 구매 가격 설정
{
    setText(5, totalPrice);
}

int Order::OrderID() const                          //고객 ID 반환
{
    return text(1).toInt();
}

