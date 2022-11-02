#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMdiSubWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ClientManagerForm;
class ProductManagerForm;
class OrderManagerForm;
class ClientServerForm;
class ChatClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionClient_triggered();
    void on_actionProduct_triggered();
    void on_actionOrder_triggered();
    void on_mdiArea_subWindowActivated(QMdiSubWindow *orderupdate);
    void on_actionChatServer_triggered();
    void on_actionChat_Client_triggered();

private:
    Ui::MainWindow *ui;
    ClientManagerForm *clientForm;
    ProductManagerForm *productForm;
    OrderManagerForm *orderForm;
    ClientServerForm *serverForm;
    ChatClient *chatClient;
    int chatClientID;
};
#endif // MAINWINDOW_H
