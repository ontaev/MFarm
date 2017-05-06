#include "insemination.h"
#include "ui_insemination.h"
#include "mainwindow.h"

#include <QtSql>
#include <QtGui>

insemination::insemination(QWidget *parent, int id) :
    QDialog(parent),
    ui(new Ui::insemination)
{
    ui->setupUi(this);
    insemination::id = id;
    connect(ui->closeButton, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
}

insemination::~insemination()
{
    delete ui;
}

void insemination::submit()
{
    QSqlQuery query;

    query.prepare("INSERT INTO insemination (cow_id, plan_insemination_month, plan_bull, "
                  " fact_insemination_date_1, fact_bull_1, fact_insemination_date_2, fact_bull_2, fact_insemination_date_3, fact_bull_3, "
                  " spring_rectal_result, spring_plan_calving, autumn_rectal_result, autumn_plan_calving) "
                  "VALUES (:cow_id, :plan_insemination_month, :plan_bull, "
                  " :fact_insemination_date_1, :fact_bull_1, :fact_insemination_date_2, :fact_bull_2, :fact_insemination_date_3, :fact_bull_3, "
                  " :spring_rectal_result, :spring_plan_calving, :autumn_rectal_result, :autumn_plan_calving)");


    query.bindValue(":cow_id", id);
    qDebug() << ui->PlanMonth->date();
    qDebug() << ui->FirstDate->date();
    query.bindValue(":plan_insemination_month", ui->PlanMonth->date());
    query.bindValue(":plan_bull", ui->PlanBull->value());
    query.bindValue(":fact_insemination_date_1", ui->FirstDate->date());
    query.bindValue(":fact_bull_1", ui->FirstBull->value());
    query.bindValue(":fact_insemination_date_2", ui->SecondDate->date());
    query.bindValue(":fact_bull_2", ui->SecondBull->value());
    query.bindValue(":fact_insemination_date_3", ui->ThirdDate->date());
    query.bindValue(":fact_bull_3", ui->ThirdBull->value());
    query.bindValue(":spring_rectal_result", ui->SpringResult->currentText());
    query.bindValue(":spring_plan_calving", ui->SpringCalving->date());
    query.bindValue(":autumn_rectal_result", ui->AutumnResult->currentText());
    query.bindValue(":autumn_plan_calving", ui->AutumnCalving->date());

    if(!query.exec()){
        QMessageBox::warning(this, tr("Insemination Table"),tr("The database reported an error: %1").arg(query.lastError().databaseText()));
        MainWindow::saveLog(QString::fromLocal8Bit("Submitting status: ERROR %1").arg(query.lastError().databaseText()));
    }
    else{
        emit(insem_added());
        MainWindow::saveLog("Insemination submitting status: OK");
    }


}
