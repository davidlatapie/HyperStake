#include "charitydialog.h"
#include "ui_charitydialog.h"

#include "clientmodel.h"
#include "notificator.h"
#include "version.h"

#include "wallet.h"
#include "walletdb.h"
#include "main.h"
#include "init.h"

charityDialog::charityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::charityDialog)
{
    ui->setupUi(this);
}

charityDialog::~charityDialog()
{
    delete ui;
}

void charityDialog::setModel(ClientModel *model)
{

}

void charityDialog::on_buttonBox_accepted()
{
    CBitcoinAddress address = ui->charityAddressEdit->text().toStdString();

                   if (!address.IsValid())
                   {
                       //notificator->notify(Notificator::Warning, tr("Invalid HyperStake address"));
                       close();
                   }


                    QString str = ui->charityPercentEdit->text();
                    bool fIntConversion;
                    unsigned int nCharityPercent = str.toInt(&fIntConversion, 10);

                   //if (nCharityPercent < 0)
                 //  {
                   //notificator->notify(Notificator::Warning, tr("Invalid parameter, expected valid percentage"));
                 //  close();
              //     }


                   if (pwalletMain->IsLocked())
                   {
                      // notificator->notify(Notificator::Warning, tr("Error: Please enter the wallet passphrase with walletpassphrase first."));
                       close();
                   }

                   //Turn off if we set to zero.
                   //Future: After we allow multiple addresses, only turn of this address
                   if(nCharityPercent == 0)
                   {
                       pwalletMain->fStakeForCharity = false;
                       pwalletMain->StakeForCharityAddress = "";
                       pwalletMain->nStakeForCharityPercent = 0;
                      // notificator->notify(Notificator::Warning, tr("0 Percent Selected, void"));
                       close();
                   }

                   //For now max percentage is 50.
                   if (nCharityPercent > 50 )
                      nCharityPercent = 50;

                   pwalletMain->StakeForCharityAddress = address;
                   pwalletMain->nStakeForCharityPercent = nCharityPercent;
                   pwalletMain->fStakeForCharity = true;

                  // notificator->notify(Notificator::Warning, tr("Stake For Charity Set"));
                   close();
}

void charityDialog::on_buttonBox_rejected()
{
    close();
}
