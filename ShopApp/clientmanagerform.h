#ifndef CLIENTMANAGERFORM_H
#define CLIENTMANAGERFORM_H

#include "client.h"
#include <QWidget>

class Client;
class QMenu;
class QTreeWidgetItem;

namespace Ui { class ClientManagerForm; }

class ClientManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManagerForm(QWidget *parent = nullptr);      //생성자
    ~ClientManagerForm();       //소멸자
    void loadData();            //파일 입출력

private slots:
    /* QTreeWidget을 위한 슬롯 */
    void removeItem();      //항목(item)을 제거하는 함수
    void showContextMenu(const QPoint &);       //마우스 커서 위치
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);   //treeWidget에 있는 항목(item)을 선택했을 때
    void on_addPushButton_clicked();        //Add(고객 추가) 버튼을 클릭했을 때
    void on_modifyPushButton_clicked();     //Modify(수정) 버튼을 클릭했을 때
    void on_searchPushButton_clicked();     //검색 기능을 수행하는 함수
    void sendSearchedClient(QString);
    void sendSearchedID(int);               //OrderManagerForm에서 void sig_requesSearchID(int);가 신호(singals)를 발생


signals:
    void clientAdded(int, QString);
    void sig_sendSearchedClient(QList<Client*>);
    void sig_sendSearchedID(Client*);
    void sig_sendClientInfo(QString);
    void sig_sendModifyClient(int, int, QString);

private:
    int makeId();       //고객 ID를 자동 할당해주는 함수
    QMap<int, Client*> clientList;      //고객 정보를 Map에 담아 List화
    Ui::ClientManagerForm *ui;          //ClientManagerForm의 ui
    QMenu* menu;    //QMenu 클래스는 메뉴 모음, 상황에 맞는 메뉴 및 기타 팝업 메뉴에서 사용할 수 있는 메뉴 위젯을 제공
};
#endif // CLIENTMANAGERFORM_H
