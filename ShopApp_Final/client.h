#ifndef CLIENT_H
#define CLIENT_H

#include <QTreeWidgetItem>

class Client : public QTreeWidgetItem
{
public:
    explicit Client(int clientID = 0, QString = "", QString = "", QString = "", QString = "");


    QString getClientName() const;      //고객 이름 반환
    void setClientName(QString&);       //고객 이름 설정

    QString getPhoneNumber() const;     //고객 전화번호 반환
    void setPhoneNumber(QString&);      //고객 전화번호 설정

    QString getAddress() const;         //고객 주소 반환
    void setAddress(QString&);          //고객 주소 설정

    QString getEmail() const;           //고객 이메일 반환
    void setEmail(QString&);            //고객 이메일 설정

    int ClientID() const;               //고객 ID 반환
};
#endif // CLIENT_H
