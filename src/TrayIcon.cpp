#include "TrayIcon.h"
#include <QMenu>
#include <QAction>
#include <QApplication>

TrayIcon::TrayIcon(QString user, QString pass) : QObject(), icon(this) {
	username = user;
	password = pass;
	#ifdef Q_OS_WIN 
		icon.setIcon(QIcon(":/Icons/images/soda_32.png"));
	#endif
	#ifdef Q_OS_MAC
		icon.setIcon(QIcon(":/Icons/images/soda_16_bw.png"));
	#endif
	icon.setContextMenu(createMenu());
	icon.setToolTip("Drink Client");
	win = new FrmClient(username, password, 0, 
				Qt::Dialog | 
				Qt::MSWindowsFixedSizeDialogHint | 
				Qt::WindowTitleHint | 
				Qt::WindowSystemMenuHint);
}

void TrayIcon::show() {
	icon.show();
	showWindow();
}

QMenu* TrayIcon::createMenu() {
	QMenu *menu = new QMenu();
	QAction *quit = new QAction("Quit", this);
	QAction *show = new QAction("Show Selections...", this);

	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(show, SIGNAL(triggered()), this, SLOT(showWindow()));
	
	menu->addAction(show);
	menu->addAction(quit);
	
	menu->setDefaultAction(show);
	return menu;
}

void TrayIcon::showWindow() {
	win->show();
}
