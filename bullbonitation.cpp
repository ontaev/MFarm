#include "bullbonitation.h"
#include "ui_bullbonitation.h"
#include "mainwindow.h"

#include <QtSql>
#include <QtGui>

bullBonitation::bullBonitation(QWidget *parent, int id) :
    QDialog(parent),
    ui(new Ui::bullBonitation)
{
    ui->setupUi(this);
    bullBonitation::id = id;

    connect(ui->closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
}

bullBonitation::~bullBonitation()
{
    delete ui;
}

void bullBonitation::submit()
{
    QSqlQuery query;

    query.prepare("INSERT INTO bullbonitation (id_animal, date, vozrast, virazh_porodi,"
                  " muscle, golova_sheya, grud, holka_spina, krestec, moshonka, okorok,"
                  " konechnosti, obsh_ball, massa, virazh_tipa_ekst, sobstv_product, kach_potomstva,"
                  " porodnost, klass_materi, klass_otca, otca_sobstv_prod, summa, klass, naznach)"
                  " VALUES (:id_animal, :date, :vozrast, :virazh_porodi,"
                  " :muscle, :golova_sheya, :grud, :holka_spina, :krestec, :moshonka, :okorok,"
                  " :konechnosti, :obsh_ball, :massa, :virazh_tipa_ekst, :sobstv_product, :kach_potomstva,"
                  " :porodnost, :klass_materi, :klass_otca, :otca_sobstv_prod, :summa, :klass, :naznach)");


    query.bindValue(":id_animal", id);
    query.bindValue(":date", ui->DateTxt->date());
    query.bindValue(":vozrast", ui->AgeTxt->text());
    query.bindValue(":virazh_porodi", ui->BEvidenceTxt->value());
    query.bindValue(":muscle", ui->MusclesTxt->value());
    query.bindValue(":golova_sheya", ui->HeadNeckTxt->value());
    query.bindValue(":grud", ui->BreastTxt->value());
    query.bindValue(":holka_spina", ui->WithersBackTxt->value());
    query.bindValue(":krestec", ui->SacrumTxt->value());
    query.bindValue(":moshonka", ui->ScrotumTxt->value());
    query.bindValue(":okorok", ui->HamTxt->value());
    query.bindValue(":konechnosti", ui->LimbsTxt->value());
    query.bindValue(":obsh_ball", ui->TotalTxt->value());
    query.bindValue(":massa", ui->WeightTxt->value());
    query.bindValue(":virazh_tipa_ekst", ui->TEvidenceTxt->value());
    query.bindValue(":sobstv_product", ui->OwnProdTxt->value());
    query.bindValue(":kach_potomstva", ui->PQualityTxt->value());
    query.bindValue(":porodnost", ui->BreedTxt->value());
    query.bindValue(":klass_materi", ui->MClassTxt->value());
    query.bindValue(":klass_otca", ui->MClassTxt->value());
    query.bindValue(":otca_sobstv_prod", ui->FatherProdTxt->value());
    query.bindValue(":summa", ui->SumPointsTxt->value());
    query.bindValue(":klass", ui->ClassTxt->text());
    query.bindValue(":naznach", ui->PurposeTxt->text());


    if(!query.exec()){
        QMessageBox::warning(this, tr("Bull bonitation Table"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(query.lastError().databaseText()));
    }
    else{
        emit(bon_added());
        MainWindow::saveLog("Bull bonitation submitting status: OK");
    }


}
