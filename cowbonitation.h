#ifndef COWBONITATION_H
#define COWBONITATION_H

#include <QDialog>

namespace Ui {
class cowBonitation;
}

class cowBonitation : public QDialog
{
    Q_OBJECT
    
public:
    explicit cowBonitation(QWidget *parent = 0, int id = 0);
    ~cowBonitation();

signals:
    void bon_added();

private slots:
    void submit();
    
private:
    Ui::cowBonitation *ui;
    int id;
};

#endif // COWBONITATION_H
