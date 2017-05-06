#include "measurement.h"
#include "ui_measurement.h"
#include "mainwindow.h"

#include <QtSql>
#include <QtGui>

Measurement::Measurement(QWidget *parent, int id, int gender) :
    QDialog(parent),
    ui(new Ui::Measurement)
{
    ui->setupUi(this);
    Measurement::id = id;
    Measurement::gender = gender;

    if(gender){
        ui->Label->setText(tr("Hip joint"));
    }
    else{
        ui->Label->setText(tr("Ischial tuberosity"));
    }

    connect(ui->closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    ui->bullButton->setChecked(gender);
    ui->cowButton->setChecked(!gender);
    ui->bullButton->setCheckable(gender);
    ui->cowButton->setCheckable(!gender);
}

Measurement::~Measurement()
{
    delete ui;
}

void Measurement::submit()
{
    QSqlQuery query;

    query.prepare("INSERT INTO measurement (id_animal, gender, vozrast,"
                  " v_holka, v_krestec, gl_grud,"
                  " sh_grud, sh_maklo, sh_tazobedr,"
                  " dl_tulov, dl_zada, obh_grud, obh_pyast, poluobh_zada)"
                  " VALUES (:id_animal, :gender, :vozrast,"
                  " :v_holka, :v_krestec, :gl_grud,"
                  " :sh_grud, :sh_maklo, :sh_tazobedr,"
                  " :dl_tulov, :dl_zada, :obh_grud, :obh_pyast, :poluobh_zada)");


    query.bindValue(":id_animal", id);
    query.bindValue(":gender", gender);
    query.bindValue(":vozrast", ui->AgeTxt->text());
    query.bindValue(":v_holka", ui->HWithersTxt->text());
    query.bindValue(":v_krestec", ui->HSacrumTxt->text());
    query.bindValue(":gl_grud", ui->DBreastTxt->text());
    query.bindValue(":sh_grud", ui->WBreastTxt->text());
    query.bindValue(":sh_maklo", ui->WHookTxt->text());
    query.bindValue(":sh_tazobedr", ui->WTuberosityTxt->text());
    query.bindValue(":dl_tulov", ui->CrTrunkTxt->text());
    query.bindValue(":dl_zada", ui->CrSeatTxt->text());
    query.bindValue(":obh_grud", ui->GBreastTxt->text());
    query.bindValue(":obh_pyast", ui->GMetacarpusTxt->text());
    query.bindValue(":poluobh_zada", ui->HalfSeatTxt->text());

    if(!query.exec()){
        QMessageBox::warning(this, tr("Measurement Table"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(query.lastError().databaseText()));
    }
    else{
        emit(meas_added());
        MainWindow::saveLog("Measurement submitting status: OK");
    }


}
