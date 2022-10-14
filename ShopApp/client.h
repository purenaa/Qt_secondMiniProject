#ifndef CLIENT_H
#define CLIENT_H

#include <QTreeWidgetItem>

class Client : public QTreeWidgetItem
{
public:
    explicit Client(int clientID = 0, QString = "", QString = "", QString = "", QString = "");

    QString getClientName() const;
    void setClientName(QString&);
    QString getPhoneNumber() const;
    void setPhoneNumber(QString&);
    QString getAddress() const;
    void setAddress(QString&);
    QString getEmail() const;
    void setEmail(QString&);
    int ClientID() const;
    bool operator==(const Client &other) const;
};
#endif // CLIENT_H
