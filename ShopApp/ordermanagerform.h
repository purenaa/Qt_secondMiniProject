#ifndef ORDERMANAGERFORM_H
#define ORDERMANAGERFORM_H

#include "client.h"
#include "order.h"
#include "product.h"
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
    explicit OrderManagerForm(QWidget *parent = nullptr);   //생성자
    ~OrderManagerForm();                                    //소멸자
    void loadData();                                        //파일 입출력
    int calTotalPrice();

private:
    int makeId();                                           //주문 ID를 자동 할당해주는 함수
    int clientID;
    QMap<int, Order*> orderList;                            //주문 정보를 Map에 담아 List화
    Ui::OrderManagerForm *ui;                               //OrderManagerForm의 ui
    QMenu* menu;    //QMenu 클래스는 메뉴 모음, 상황에 맞는 메뉴 및 기타 팝업 메뉴에서 사용할 수 있는 메뉴 위젯을 제공


private slots:
    void removeItem();                                      //항목(item)을 제거하는 함수
    void showContextMenu(const QPoint &);                   //마우스 커서 위치
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);   //treeWidget에 있는 항목(item)을 선택했을 때
    void on_addPushButton_clicked();                        //Add(주문 추가) 버튼을 클릭했을 때
    void on_modifyPushButton_clicked();                     //Modify(수정) 버튼을 클릭했을 때
    void on_searchPushButton_clicked();                     //검색 기능을 수행하는 함수
    void on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_clientNameLineEdit_textEdited(const QString &clientName);
    void on_productNameComboBox_textActivated(const QString &item);
    void update(QList<QString>);
    void updateClientTreeWdiget(QList<Client*>);
    void updateClient(Client*);
    void updateProduct(Product*, int);
    void on_purchaseQuantitySpinBox_valueChanged(int);



signals:
    void sig_requestProductNames();
    void sig_requesSearchClient(QString);
    void sig_requesSearchID(int);
    void sig_requesProductNameSearch(QString, int = -1);
    void sig_reduceInventoryAmount(QString, int);
};

#endif // ORDERMANAGERFORM_H
