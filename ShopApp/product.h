#ifndef PRODUCT_H
#define PRODUCT_H

#include <QTreeWidgetItem>

class Product : public QTreeWidgetItem
{  
public:
    explicit Product(int productID = 0, QString = "", QString = "", QString = "");

    QString getProductName() const;
    void setProductName(QString&);
    QString getPrice() const;
    void setPrice(QString&);
    QString getInventoryAmount() const;
    void setInventoryAmount(QString&);
    int ProductID() const;
    bool operator==(const Product &other) const;

};
#endif // PRODUCT_H

