#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>
#include "ui_FrmLogin.h"

class FrmLogin : public QDialog, Ui::frmLogin {
Q_OBJECT
public:
	FrmLogin(QWidget *parent = 0, Qt::WFlags f = 0);
	QString getUsername();
	QString getPassword();
};

#endif

