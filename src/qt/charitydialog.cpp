#include "charitydialog.h"
#include "ui_charitydialog.h"

#include "walletmodel.h"
#include "base58.h"
#include "addressbookpage.h"
#include "init.h"

#include <QMessageBox>
#include <QLineEdit>

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

StakeForCharityDialog::StakeForCharityDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StakeForCharityDialog),
	 model(0)
{
    ui->setupUi(this);
}

StakeForCharityDialog::~StakeForCharityDialog()
{
    delete ui;
}

void StakeForCharityDialog::setModel(WalletModel *model)
{

}

void StakeForCharityDialog::setAddress(const QString &address)
{
	setAddress(address, ui->charityAddressEdit);
}

void StakeForCharityDialog::setAddress(const QString &address, QLineEdit *addrEdit)
{
	addrEdit->setText(address);
	addrEdit->setFocus();
}

void StakeForCharityDialog::on_addressBookButton_clicked()
{
    if (model && model->getAddressTableModel())
    {
        AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::SendingTab, this);
        dlg.setModel(model->getAddressTableModel());
        if (dlg.exec())
			setAddress(dlg.getReturnValue(), ui->charityAddressEdit);
    }
}

void StakeForCharityDialog::on_enableButton_clicked()
{
    bool fValidConversion = false;

    std::string strAddress = ui->charityAddressEdit->text().toStdString();
    if (!CBitcoinAddress(strAddress).IsValid())
    {
        ui->message->setProperty("status", "error");
        ui->message->style()->polish(ui->message);
        ui->message->setText(tr("The entered address:\n") + ui->charityAddressEdit->text() + tr(" is invalid.\nPlease check the address and try again."));
        ui->charityAddressEdit->setFocus();
        return;
    }

    int nCharityPercent = ui->charityPercentEdit->text().toInt(&fValidConversion, 10);
	if (!fValidConversion || nCharityPercent > 100 || nCharityPercent <= 0)
    {
        ui->message->setProperty("status", "error");
        ui->message->style()->polish(ui->message);
        ui->message->setText(tr("Please Enter 1 - 100 for percent."));
        ui->charityPercentEdit->setFocus();
        return;
    }

	pwalletMain->fMultiSend = true;
	std::pair<std::string, int> pMultiSend;
	pMultiSend.first = strAddress;
	pMultiSend.second = nCharityPercent;
	pwalletMain->vMultiSend.push_back(pMultiSend);
	
    ui->message->setProperty("status", "ok");
    ui->message->style()->polish(ui->message);
	
	std::string strMultiSendPrint = "";
	for(int i = 0; i < (int)pwalletMain->vMultiSend.size(); i++)
	{
		pMultiSend = pwalletMain->vMultiSend[i];
		strMultiSendPrint += pMultiSend.first.c_str();
		strMultiSendPrint += " - ";
		strMultiSendPrint += boost::lexical_cast<string>(pMultiSend.second); 
		strMultiSendPrint += "% \n";
	}
    ui->message->setText(tr("You are now sending to\n") + QString(strMultiSendPrint.c_str()));
    return;
}

void StakeForCharityDialog::on_disableButton_clicked()
{
	pwalletMain->fMultiSend = false;
    ui->message->setProperty("status", "");
    ui->message->style()->polish(ui->message);
    ui->message->setText(tr("MultiSend is disabled"));
    return;
}

void StakeForCharityDialog::on_changeAddressBookButton_clicked()
{
	
}
