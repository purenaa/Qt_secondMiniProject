#include "mainwindow.h"
#include "chatclient.h"
#include "qobjectdefs.h"
#include "ui_mainwindow.h"
#include "clientmanagerform.h"
#include "productmanagerform.h"
#include "ordermanagerform.h"
#include "clientserverform.h"
#include "chatclient.h"
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chatClientID(0)
{
    ui->setupUi(this);

    orderForm = new OrderManagerForm(this);
    orderForm->setWindowTitle(tr("Order Info"));
    connect(orderForm, SIGNAL(destroyed()), orderForm, SLOT(deleteLater()));

    serverForm = new ClientServerForm(this);
    serverForm->setWindowTitle(tr("Chatting Server"));

    clientForm = new ClientManagerForm(this);
    connect(clientForm, SIGNAL(destroyed()), clientForm, SLOT(deleteLater()));
    clientForm->setWindowTitle(tr("Client Info"));

    productForm = new ProductManagerForm(this);
    productForm->setWindowTitle(tr("Product Info"));
    connect(productForm, SIGNAL(destroyed()), productForm, SLOT(deleteLater()));

    connect(orderForm, SIGNAL(sig_requestProductNames()), productForm, SLOT(slot_getProductNames()));
    connect(productForm, SIGNAL(sig_getProductNames(QList<QString>)), orderForm, SLOT(update(QList<QString>)));
    connect(clientForm, SIGNAL(clientAdded(int, QString)), serverForm, SLOT(addClient(int, QString)));

    connect(orderForm, SIGNAL(sig_requesSearchClient(QString)), clientForm, SLOT(sendSearchedClient(QString)));
    connect(clientForm, SIGNAL(sig_sendSearchedClient(QList<Client*>)), orderForm, SLOT(updateClientTreeWdiget(QList<Client*>)));

    connect(orderForm, SIGNAL(sig_requesSearchID(int)), clientForm, SLOT(sendSearchedID(int)));
    connect(clientForm, SIGNAL(sig_sendSearchedID(Client*)), orderForm, SLOT(updateClient(Client*)));

    connect(orderForm, SIGNAL(sig_requesProductNameSearch(QString, int)), productForm, SLOT(slot_sendProductNameSearch(QString, int)));
    connect(productForm, SIGNAL(sig_sendProductNameSearch(Product*,int)), orderForm, SLOT(updateProduct(Product*,int)));

    connect(orderForm, SIGNAL(sig_reduceInventoryAmount(QString,int)), productForm, SLOT(updateInventoryAmount(QString,int)));

    connect(clientForm, SIGNAL(sig_sendClientInfo(QString)), serverForm, SLOT(slot_updateClientInfo(QString)));
    connect(clientForm, SIGNAL(sig_sendModifyClient(int, int, QString)), serverForm, SLOT(slot_updateModifyClient(int, int, QString)));

    clientForm->loadData();
    productForm->loadData();
    orderForm->loadData();

    QIcon iconClient = QIcon("clientImg.png");          //ClientInfo ?????????
    ui->actionClient->setIcon(iconClient);

    QIcon iconProduct = QIcon("productImg.jpg");        //ProductInfo ?????????
    ui->actionProduct->setIcon(iconProduct);

    QIcon iconOrder = QIcon("orderImg.png");            //OrderInfo ?????????
    ui->actionOrder->setIcon(iconOrder);

    QIcon iconChat_Client = QIcon("chatImg.png");       //ChatServer ?????????
    ui->actionChat_Client->setIcon(iconChat_Client);

    QIcon iconChat_Server = QIcon("serverImg.png");     //ChatClient ?????????
    ui->actionChatServer->setIcon(iconChat_Server);

    QIcon iconQuit = QIcon("quitImg5.png");             //Quit ?????????
    ui->actionQuit->setIcon(iconQuit);

    QMdiSubWindow *cw = ui->mdiArea->addSubWindow(clientForm);
    ui->mdiArea->addSubWindow(productForm);
    ui->mdiArea->addSubWindow(orderForm);
    ui->mdiArea->addSubWindow(serverForm);
    ui->mdiArea->setActiveSubWindow(cw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClient_triggered()
{
    if(clientForm != nullptr) {
        clientForm->setFocus();
    }
}

void MainWindow::on_actionProduct_triggered()
{
    if(productForm != nullptr) {
        productForm->setFocus();
    }
}

void MainWindow::on_actionOrder_triggered()
{
    if(orderForm != nullptr) {
        orderForm->setFocus();
    }
}

void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *orderupdate)
{
    Q_UNUSED(orderupdate);
     emit orderForm->sig_requestProductNames();
}

void MainWindow::on_actionChatServer_triggered()
{
    if(serverForm != nullptr) {
        serverForm->setFocus();
    }
}

void MainWindow::on_actionChat_Client_triggered()
{
    chatClient = new ChatClient(nullptr, chatClientID++);
    chatClient->move(1100, 400);
    chatClient ->show();

    connect(chatClient, SIGNAL(destroyed()), chatClient, SLOT(deleteLater()));

    //????????? ????????? ??? ??? ????????? chatClient??? ????????? ????????? ????????? ?????? ????????? login?????? ????????? ????????? ??? ???????????? ???????????? 4??? ????????????.
    //????????? chatClient?????? ?????? ID??? ???????????? signal??? ?????? ??? id??? ?????? ?????? ?????????.
    connect(chatClient, SIGNAL(sig_checkLogin(QString, int)), serverForm, SLOT(slot_checkLogin(QString, int)));
    connect(serverForm, SIGNAL(sig_checkLogin(int, int)), chatClient, SLOT(slot_login(int, int)));
}


