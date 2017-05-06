#include "adddb.h"
#include "ui_adddb.h"

#include <QtSql>
#include <QtGui>

addDb::addDb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDb)
{
    ui->setupUi(this);
    checked = false;
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

addDb::~addDb()
{
    delete ui;
}

void addDb::on_checkButton_clicked()
{
    if(checked){
        QSettings settings("Bdm Soft", "MFarm");
        settings.beginGroup("Databases");
            settings.beginGroup(ui->NameTxt->text());
                settings.setValue("Server", ui->ServerTxt->text());
                settings.setValue("Image folder", ui->ImageTxt->text());
            settings.endGroup();
        settings.endGroup();
        this->close();
    }
    else{
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "dbcheck");
        if((ui->ServerTxt->text()=="") or (ui->NameTxt->text()=="")){
            QMessageBox::warning(this, tr("Check database"), tr("Database and server names can not be empty!"));
            return;
        }
        db.setHostName(ui->ServerTxt->text());
        db.setDatabaseName(ui->NameTxt->text());
        db.setUserName("root");
        db.setPassword("Elveg2305");

        if(db.open()){

            checked = true;
            ui->checkButton->setText(tr("Add"));
            QMessageBox::information(this, tr("Check database"), tr("Connection to database %1 succesfully established").arg(ui->NameTxt->text()));
        }
        else{
            QMessageBox::warning(this, tr("Check database"), tr("Can not establish connection to: %1").arg(ui->NameTxt->text()));
        }
    }
    QSqlDatabase::removeDatabase("dbcheck");

}

void addDb::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QCoreApplication::applicationDirPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->ImageTxt->setText(dir);
}

void addDb::on_NameTxt_textChanged(const QString &arg1)
{
    ui->checkButton->setText(tr("Check"));
    checked = false;
}

void addDb::on_ServerTxt_textChanged(const QString &arg1)
{
    ui->checkButton->setText(tr("Check"));
    checked = false;
}
