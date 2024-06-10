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
    bool is_fit_all();
    void reset_btn_press();

private slots:
    void on_toolButton_fit_all_pressed();

private:
    Ui::Form *ui;
    bool fit_all=0;
};

#endif // FORM_H
