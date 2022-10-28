#include "client.h"

#include <QLabel>
#include <iostream>

using namespace std;

Client::Client(int clientID, QString clientName, QString phoneNumber, QString address, QString email)
{
    setText(0, QString::number(clientID));
    setText(1, clientName);
    setText(2, phoneNumber);
    setText(3, address);
    setText(4, email);
}

QString Client::getClientName() const                   //고객 이름 반환
{
    return text(1);
}

void Client::setClientName(QString& clientName)         //고객 이름 설정
{
    setText(1, clientName);
}

QString Client::getPhoneNumber() const                  //고객 전화번호 반환
{
    return text(2);
}

void Client::setPhoneNumber(QString& phoneNumber)		//고객 전화번호 설정
{
    setText(2, phoneNumber);
}

QString Client::getAddress() const                      //고객 주소 반환
{
    return text(3);
}

void Client::setAddress(QString& address)               //고객 주소 설정
{
    setText(3, address);
}

QString Client::getEmail() const                        //고객 이메일 반환
{
    return text(4);
}

void Client::setEmail(QString& email)                   //고객 이메일 설정
{
    setText(4, email);
}

int Client::ClientID() const                            //고객 ID 반환
{
    return text(0).toInt();
}
