#include <QApplication>
#include <QSystemTrayIcon>
#include "TrayIcon.h"
#include "FrmLogin.h"

int main(int argc, char ** argv) {
	QApplication app( argc, argv );
	QString username;
	QString password;

	FrmLogin login(0, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint);
	if(login.exec() == QDialog::Accepted) {
		username = login.getUsername();
		password = login.getPassword();
	} else {
		return 1;
	}
	
	TrayIcon tray(username, password);
	tray.show();

	return app.exec();
}

