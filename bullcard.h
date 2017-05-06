#ifndef BULLCARD_H
#define BULLCARD_H

#include <QDialog>
#include <QtSql>
#include <QDataWidgetMapper>

namespace Ui {
class bullCard;
}

class bullCard : public QDialog
{
    Q_OBJECT

private slots:
    void submit();
    void submit_lineage();
    void revertAll();
    void on_MSelectButton_clicked();
    void m_line_update(int);

    void on_FSelectButton_clicked();
    void f_line_update(int);

    void on_photoButton_clicked();

    void on_MCardButton_clicked();

    void on_FCardButton_clicked();

    void on_MMCardButton_clicked();

    void on_FMCardButton_clicked();

    void on_MFCardButton_clicked();

    void on_FFCardButton_clicked();

    void on_newBonButton_clicked();

    void on_newMeasButton_clicked();

    void submit_dev();
    void submit_meas();
    void submit_bon();
    void bon_added();
    void meas_added();

    void on_delBonButton_clicked();

    void on_delMeasButton_clicked();

public:
    explicit bullCard(QWidget *parent = 0, int id = 0);

    ~bullCard();
    
private:
    Ui::bullCard *ui;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *development;
    QSqlRelationalTableModel *measurement;
    QSqlRelationalTableModel *bonitation;
    int mother_old;
    int father_old;
    int mother_new;
    int father_new;
    int mother_current;
    int father_current;
    int id;
};

#endif // BULLCARD_H
