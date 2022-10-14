#ifndef ORDER_H
#define ORDER_H

#include <QTreeWidgetItem>

class Order : public QTreeWidgetItem
{
public:
    explicit Order(QString = "", int orderID = 0, /*int clinetID = 0, int productID = 0,*/ QString = "", QString = "", QString = "", QString = "");

//    int getClientID() const;
//    void setClientID(int&);
//    int getProductID() const;
//    void setProductID(int&);
    QString getPurchaseDay() const;
    void setPurchaseDay(QString&);
    QString getClientName() const;
    void setClientName(QString&);
    QString getProductName() const;
    void setProductName(QString&);
    QString getPurchaseQuantity() const;
    void setPurchaseQuantity(QString&);
    QString getTotalPrice() const;
    void setTotalPrice(QString&);
    int OrderID() const;
    bool operator==(const Order &other) const;
};

#endif // ORDER_H
