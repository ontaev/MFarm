#ifndef BULLBONITATION_H
#define BULLBONITATION_H

#include <QDialog>

namespace Ui {
class bullBonitation;
}

class bullBonitation : public QDialog
{
    Q_OBJECT
    
public:
    explicit bullBonitation(QWidget *parent = 0, int id = 0);
    ~bullBonitation();

signals:
    void bon_added();

private slots:
    void submit();
    
private:
    Ui::bullBonitation *ui;
    int id;
};

#endif // BULLBONITATION_H
