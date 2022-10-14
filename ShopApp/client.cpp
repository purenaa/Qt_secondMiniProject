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

QString Client::getClientName() const		//회원 이름 반환
{
    return text(1);
}

void Client::setClientName(QString& clientName)		//회원 이름 설정
{
    setText(1, clientName);
}

QString Client::getPhoneNumber() const		//회원 전화번호 반환
{
    return text(2);
}

void Client::setPhoneNumber(QString& phoneNumber)		//회원 전화번호 설정
{
    setText(2, phoneNumber);
}

QString Client::getAddress() const		//회원 주소 반환
{
    return text(3);
}

void Client::setAddress(QString& address)		//회원 주소 설정
{
    setText(3, address);
}

QString Client::getEmail() const			//회원 이메일 반환
{
    return text(4);
}

void Client::setEmail(QString& email)		//회원 이메일 설정
{
    setText(4, email);
}

int Client::ClientID() const      //회원 ID 반환
{
    return text(0).toInt();
}

// Define copy assignment operator.
bool Client::operator==(const Client &other) const {
    return (this->text(1) == other.text(1));
}


//c_str() : string을 const char로 바꿔줌
