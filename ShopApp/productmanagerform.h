#ifndef PRODUCTMANAGERFORM_H
#define PRODUCTMANAGERFORM_H

#include "product.h"
#include <QWidget>

class QMenu;
class QTreeWidgetItem;

namespace Ui { class ProductManagerForm; }

class ProductManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductManagerForm(QWidget *parent = nullptr);
    ~ProductManagerForm();
    Product* searchName(const QString &);


private slots:
    void removeItem();
    void showContextMenu(const QPoint &);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();

private:
    int makeId();

    QMap<int, Product*> productList;
    Ui::ProductManagerForm *ui;
    QMenu* menu;    //QMenu 클래스는 메뉴 모음, 상황에 맞는 메뉴 및 기타 팝업 메뉴에서 사용할 수 있는 메뉴 위젯을 제공합니다.
};

#endif // PRODUCTMANAGERFORM_H
