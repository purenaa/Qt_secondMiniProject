#ifndef CLIENTMANAGERFORM_H
#define CLIENTMANAGERFORM_H

#include "client.h"
#include <QWidget>

class QMenu;
class QTreeWidgetItem;

namespace Ui { class ClientManagerForm; }

class ClientManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ClientManagerForm(QWidget *parent = nullptr);
    ~ClientManagerForm();
    Client* searchName(const QString &);

private slots:
    /* QTreeWidget을 위한 슬롯 */
    void removeItem();
    void showContextMenu(const QPoint &);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_addPushButton_clicked();
    void on_modifyPushButton_clicked();
    void on_searchPushButton_clicked();

private:
    int makeId();

    QMap<int, Client*> clientList;
    Ui::ClientManagerForm *ui;
    QMenu* menu;    //QMenu 클래스는 메뉴 모음, 상황에 맞는 메뉴 및 기타 팝업 메뉴에서 사용할 수 있는 메뉴 위젯을 제공합니다.
};
#endif // CLIENTMANAGERFORM_H
