#ifndef ORDERMANAGERFORM_H
#define ORDERMANAGERFORM_H

#include "order.h"
#include <QWidget>

class QMenu;
class QTreeWidgetItem;
class ProductManagerForm;
class ClientManagerForm;
namespace Ui { class OrderManagerForm; }

class OrderManagerForm : public QWidget
{
    Q_OBJECT

public:
    // 생성자의 인자로 ClientManagerForm과 ProductManagerForm을 받아 고객정보와 상품정보를 가져온다.
    explicit OrderManagerForm(ClientManagerForm*, ProductManagerForm*, QWidget *parent = nullptr);
    ~OrderManagerForm();


private slots:
    void removeItem();
    void showContextMenu(const QPoint &);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();

private:
    int makeId();

    QMap<int, Order*> orderList;
    Ui::OrderManagerForm *ui;
    QMenu* menu;
    ClientManagerForm *clientManager;
    ProductManagerForm *productManager;
};

#endif // ORDERMANAGERFORM_H
