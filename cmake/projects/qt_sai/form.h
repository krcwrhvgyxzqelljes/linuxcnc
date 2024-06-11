#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

public slots:
    void update_dro(double x, double y, double z, double dtgx, double dtgy, double dtgz, double vel, bool homedx, bool homedy, bool homedz);

private slots:


private:
    Ui::Form *ui;
};

#endif // FORM_H
