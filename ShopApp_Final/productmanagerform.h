#ifndef PRODUCTMANAGERFORM_H
#define PRODUCTMANAGERFORM_H

#include "product.h"
#include <QWidget>

class Product;
class QMenu;
class QTreeWidgetItem;

namespace Ui { class ProductManagerForm; }

class ProductManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductManagerForm(QWidget *parent = nullptr);     //생성자
    ~ProductManagerForm();          //소멸자
    void loadData();                //파일 입출력


private slots:
    /* QTreeWidget을 위한 슬롯 */
    void removeItem();      //항목(item)을 제거하는 함수
    void showContextMenu(const QPoint &);       //마우스 커서 위치
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);   //treeWidget에 있는 항목(item)을 선택했을 때
    void on_addPushButton_clicked();        //Add(고객 추가) 버튼을 클릭했을 때
    void on_modifyPushButton_clicked();     //Modify(수정) 버튼을 클릭했을 때
    void on_searchPushButton_clicked();     //검색 기능을 수행하는 함수
    void slot_getProductNames();
    void slot_sendProductNameSearch(QString, int);
    void updateInventoryAmount(QString, int);

signals:
    void sig_getProductNames(QList<QString>);
    void sig_sendProductNameSearch(Product*, int = -1);

private:
    int makeId();       //제품 ID를 자동 할당해주는 함수
    QMap<int, Product*> productList;      //제품 정보를 Map에 담아 List화
    Ui::ProductManagerForm *ui;
    QMenu* menu;    //QMenu 클래스는 메뉴 모음, 상황에 맞는 메뉴 및 기타 팝업 메뉴에서 사용할 수 있는 메뉴 위젯을 제공

};

#endif // PRODUCTMANAGERFORM_H
