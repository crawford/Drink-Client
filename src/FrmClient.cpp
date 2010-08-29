#include "FrmClient.h"
#include <QMessageBox>

FrmClient::FrmClient(QString user, QString pass, QWidget *parent, Qt::WFlags f) : QDialog(parent, f) {
	setupUi(this);

	username = user;
	password = pass;
	numCredits = 0;
	drinkSocket = new QTcpSocket(this);
	littleDrinkSocket = new QTcpSocket(this);
	snackSocket = new QTcpSocket(this);
	connectToHosts();
	refreshCredits();
	refreshStats();
}

FrmClient::~FrmClient() {
	drinkSocket->disconnectFromHost();
	littleDrinkSocket->disconnectFromHost();
	snackSocket->disconnectFromHost();
}

void FrmClient::closeEvent(QCloseEvent *event) {
	event->ignore();
	drinkSocket->disconnectFromHost();
	littleDrinkSocket->disconnectFromHost();
	snackSocket->disconnectFromHost();
	hide();
}

void FrmClient::reject() {
	drinkSocket->disconnectFromHost();
	littleDrinkSocket->disconnectFromHost();
	snackSocket->disconnectFromHost();
	hide();
}

void FrmClient::showEvent(QShowEvent *event) {
	connectToHosts();
	refreshCredits();
	refreshStats();
}

void FrmClient::connectToHosts() {
	connectToHost(drinkSocket, "d.csh.rit.edu");
	connectToHost(littleDrinkSocket, "ld.csh.rit.edu");
	connectToHost(snackSocket, "s.csh.rit.edu");
}

void FrmClient::connectToHost(QTcpSocket *socket, QString host) {
	if(socket->state() != QAbstractSocket::ConnectedState) {
		socket->connectToHost(host, 4242);
		if(socket->waitForConnected(2000)) {
			waitForResponse(socket);
		} else {
			QMessageBox::critical(this, "Communication Error", "Could not establish a connection to " + host + ".");
		}
	}
}

void FrmClient::refreshStats() {	
	drinkSocket->write("STAT\n");
	if(drinkSocket->waitForReadyRead(1000)) {
		while(!dButtons.empty()) {
			delete dButtons.front();
			dButtons.removeFirst();
		}
		parseStats(wgtDrink, dButtons, drinkSocket);
	}

	littleDrinkSocket->write("STAT\n");
	if(littleDrinkSocket->waitForReadyRead(1000)) {
		while(!ldButtons.empty()) {
			delete ldButtons.front();
			ldButtons.removeFirst();
		}
		parseStats(wgtLittleDrink, ldButtons, littleDrinkSocket);
	}

	snackSocket->write("STAT\n");
	if(snackSocket->waitForReadyRead(1000)) {
		while(!sButtons.empty()) {
			delete sButtons.front();
			sButtons.removeFirst();
		}
		parseStats(wgtSnack, sButtons, snackSocket);
	}
}

void FrmClient::parseStats(QWidget *widget, QList<ItemButton*> &list, QTcpSocket *socket) {
	QString line;
	QString item;
	QString price;
	QString count;
	QPixmap icon;
	int slot = 0;

	while(!socket->atEnd()) {
		line = socket->readLine();
		if(line.mid(0, 2) == "OK") {
			break;
		}
		line.remove(0, line.indexOf('"') + 1);
		item = line.mid(0, line.indexOf('"'));
		line.remove(0, line.indexOf('"') + 2);
		price = line.mid(0, line.indexOf(' '));
		line.remove(0, line.indexOf(' ') + 1);
		count = line.mid(0, line.indexOf(' '));
		
		icon = QPixmap(":/Products/images/" + item.toLower().replace(".", "").replace(" ", "") + ".png");
		if(icon.isNull()) {
			icon = QPixmap(":/Products/images/default.png");
		}

		ItemButton *button = new ItemButton(item, count + " Remaining", price + " credits", QIcon(icon), this);
		button->setIconSize(QSize(48, 30));
		if(count == "0" || price.toInt() > numCredits)
			button->setEnabled(false);
		connect(button, SIGNAL(clicked()), button, SLOT(mark()));
		connect(button, SIGNAL(clicked()), this, SLOT(processClick()));
		
		list.append(button);
		widget->layout()->addWidget(button);
		slot++;
	}
}

void FrmClient::refreshCredits() {	
	drinkSocket->write("USER " + username.toAscii() + "\n");
	waitForResponse(drinkSocket);

	drinkSocket->write("PASS " + password.toAscii() + "\n");
	if(waitForResponse(drinkSocket).mid(0, 2) != "OK") {
		QMessageBox::critical(this, "Authentication Error", "Invalid username or password.");
		return;
	}

	drinkSocket->write("GETBALANCE\n");
	QByteArray res = waitForResponse(drinkSocket);
	if(res.mid(0, 2) == "OK") {
		lblCredits->setText("You have <b>" + res.mid(res.indexOf(": ") + 2).trimmed() + "</b> credits.");
	}
	numCredits = res.mid(res.indexOf(": ") + 2).trimmed().toInt();
}

void FrmClient::processClick() {
	//Drink
	for(int i = 0; i < dButtons.size(); i++) {
		if(dButtons[i]->isMarked()) {
			dButtons[i]->unmark();
			checkDrop(dButtons[i]->text(), i + 1, QString::number(spnDrinkDelay->value()), drinkSocket);
		}
	}
	//Little Drink
	for(int i = 0; i < ldButtons.size(); i++) {
		if(ldButtons[i]->isMarked()) {
			ldButtons[i]->unmark();
			checkDrop(ldButtons[i]->text(), i + 1, QString::number(spnLittleDrinkDelay->value()), littleDrinkSocket);
		}
	}
	//Snack
	for(int i = 0; i < sButtons.size(); i++) {
		if(sButtons[i]->isMarked()) {
			sButtons[i]->unmark();
			checkDrop(sButtons[i]->text(), i + 1, QString::number(spnSnackDelay->value()), snackSocket);
		}
	}
}

void FrmClient::checkDrop(QString item, int slot, QString delay, QTcpSocket *socket) {
	if(QMessageBox::question(this, "Drop Confirmation", "Are you sure you want to drop a " + 
		item + "?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
	
		socket->write("USER " + username.toAscii() + "\n");
		waitForResponse(socket);

		socket->write("PASS " + password.toAscii() + "\n");
		if(waitForResponse(socket).mid(0, 2) != "OK") {
			QMessageBox::critical(this, "Authentication Error", "Invalid username or password.");
			return;
		}

		socket->write("DROP " + QString::number(slot).toAscii() + " " + delay.toAscii() + "\n");
		QMessageBox::information(this, "Drop Response", QString(waitForResponse(socket)));
		
		connectToHosts();
		refreshCredits();
		refreshStats();
	}
}

QByteArray FrmClient::waitForResponse(QTcpSocket *socket) {
	int bytes = -1;
	do {	
		socket->waitForReadyRead(5000);
		if(socket->bytesAvailable() == bytes) {
			QMessageBox::critical(this, "Communication Error", "No response from host.");
			break;
		} else
			bytes = socket->bytesAvailable();
	} while(socket->peek(100).right(1) != "\n");
	return socket->readAll();
}

