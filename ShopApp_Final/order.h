#ifndef ORDER_H
#define ORDER_H

#include <QTreeWidgetItem>

class Order : public QTreeWidgetItem
{
public:
    explicit Order(QString = "", int orderID = 0, int clinetID = 0,
                   QString = "", QString = "", int = 0, QString = "");


    QString getPurchaseDay() const;         //제품 구매 날짜 반환
    void setPurchaseDay(QString&);          //제품 구매 날짜 설정

    int getClientID() const;                //고객 ID 반환
    void setClientID(int&);                 //고객 ID 설정

    QString getClientName() const;          //고객 이름 반환
    void setClientName(QString&);           //고객 이름 설정

    QString getProductName() const;         //제품 이름 반환
    void setProductName(QString&);          //제품 이름 설정

    int getPurchaseQuantity() const;        //제품 구매 수량 반환
    void setPurchaseQuantity(int&);         //제품 구매 수량 설정

    QString getTotalPrice() const;          //총 구매 가격 반환
    void setTotalPrice(QString&);           //총 구매 가격 설정

    int OrderID() const;                    //주문 ID 반환

private:
    int clientID;                           //고객 ID를 저장하는 객체
};

#endif // ORDER_H
