#include "FrmLogin.h"

FrmLogin::FrmLogin(QWidget *parent, Qt::WFlags f) : QDialog(parent, f) {
	setupUi(this);
}

QString FrmLogin::getUsername() {
	return txtUsername->text();
}

QString FrmLogin::getPassword() {
	return txtPassword->text();
}

