#include "cowbonitation.h"
#include "ui_cowbonitation.h"
#include "mainwindow.h"

#include <QtSql>
#include <QtGui>

cowBonitation::cowBonitation(QWidget *parent, int id) :
    QDialog(parent),
    ui(new Ui::cowBonitation)
{
    ui->setupUi(this);
    cowBonitation::id = id;

    connect(ui->closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
}

cowBonitation::~cowBonitation()
{
    delete ui;
}

void cowBonitation::submit()
{
    QSqlQuery query;

    query.prepare("INSERT INTO cowbonitation (id_animal, date, oteli, vozrast_otela, virazh_porodi,"
                  " muscle, golova_sheya, grud, holka_spina, krestec, okorok, vimya, "
                  " konechnosti, obsh_ball, moloch, vosproizv_spos, massa, virazh_tipa_ekst, "
                  " porodnost, klass_materi, klass_otca, otca_sobstv_prod, itogo, klass, naznach)"
                  " VALUES (:id_animal, :date, :oteli, :vozrast_otela, :virazh_porodi,"
                  " :muscle, :golova_sheya, :grud, :holka_spina, :krestec, :okorok, :vimya, "
                  " :konechnosti, :obsh_ball, :moloch, :vosproizv_spos, :massa, :virazh_tipa_ekst, "
                  " :porodnost, :klass_materi, :klass_otca, :otca_sobstv_prod, :itogo, :klass, :naznach)");


    query.bindValue(":id_animal", id);
    query.bindValue(":date", ui->DateTxt->date());
    query.bindValue(":oteli", ui->NCalvingTxt->text());
    query.bindValue(":vozrast_otela", ui->AgeTxt->text());
    query.bindValue(":virazh_porodi", ui->BEvidenceTxt->value());
    query.bindValue(":muscle", ui->MusclesTxt->value());
    query.bindValue(":golova_sheya", ui->HeadNeckTxt->value());
    query.bindValue(":grud", ui->BreastTxt->value());
    query.bindValue(":holka_spina", ui->WithersBackTxt->value());
    query.bindValue(":krestec", ui->SacrumTxt->value());
    query.bindValue(":okorok", ui->HamTxt->value());
    query.bindValue(":vimya", ui->SacrumTxt->value());
    query.bindValue(":konechnosti", ui->LimbsTxt->value());
    query.bindValue(":obsh_ball", ui->TotalTxt->value());
    query.bindValue(":moloch", ui->MilkinessTxt->value());
    query.bindValue(":vosproizv_spos", ui->ReprodAbilityTxt->value());
    query.bindValue(":massa", ui->WeightTxt->value());
    query.bindValue(":virazh_tipa_ekst", ui->TEvidenceTxt->value());
    query.bindValue(":porodnost", ui->BreedTxt->value());
    query.bindValue(":klass_materi", ui->MClassTxt->value());
    query.bindValue(":klass_otca", ui->MClassTxt->value());
    query.bindValue(":otca_sobstv_prod", ui->FatherProdTxt->value());
    query.bindValue(":itogo", ui->SumPointsTxt->value());
    query.bindValue(":klass", ui->ClassTxt->text());
    query.bindValue(":naznach", ui->PurposeTxt->text());


    if(!query.exec()){
        QMessageBox::warning(this, tr("Cow bonitation Table"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(query.lastError().databaseText()));
    }
    else{
        emit(bon_added());
        MainWindow::saveLog("Cow bonitation submitting status: OK");
    }


}
