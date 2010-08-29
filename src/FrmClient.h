#ifndef FRMCLIENT_H
#define FRMCLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QCloseEvent>
#include "ItemButton.h"
#include "ui_FrmClient.h"

class FrmClient : public QDialog, Ui::frmClient {
Q_OBJECT
public:
	FrmClient(QString user, QString pass, QWidget *parent = 0, Qt::WFlags f = 0);
	~FrmClient();

protected:
	void closeEvent(QCloseEvent *event);
	void reject();
	void showEvent(QShowEvent *event);

private:
	QTcpSocket *drinkSocket;
	QTcpSocket *littleDrinkSocket;
	QTcpSocket *snackSocket;
	void connectToHosts();
	void connectToHost(QTcpSocket *socket, QString host);
	void refreshStats();
	void refreshCredits();
	void parseStats(QWidget *widget, QList<ItemButton*> &list, QTcpSocket *socket);
	void checkDrop(QString item, int slot, QString delay, QTcpSocket *socket);
	QByteArray waitForResponse(QTcpSocket *socket);
	QList<ItemButton*> ldButtons;
	QList<ItemButton*> dButtons;
	QList<ItemButton*> sButtons;
	QString password;
	QString username;
	int numCredits;

private slots:
	void processClick();
};

#endif
