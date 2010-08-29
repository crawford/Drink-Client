#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include "FrmClient.h"

class TrayIcon : QObject {
Q_OBJECT
public:
	TrayIcon(QString user, QString pass);
	void show();

private:
	QString username;
	QString password;
	QSystemTrayIcon icon;
	FrmClient *win;
	QMenu* createMenu();

private slots:
	void showWindow();
};

#endif
