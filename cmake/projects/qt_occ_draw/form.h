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

signals:
    void toolButton_zoom_plus_pressed();

    void toolButton_zoom_min_pressed();

    void toolButton_fit_all_pressed();

    void toolButton_view_front_pressed();

    void toolButton_view_back_pressed();

    void toolButton_view_top_pressed();

    void toolButton_view_bottom_pressed();

    void toolButton_view_left_pressed();

    void toolButton_view_right_pressed();

    void toolButton_view_3d_pressed();

    void toolButton_draw_3d_2p_line_pressed();

    void toolButton_color_pressed();

public slots:

     void set_message(QString text);
     void set_positions(QString text);
     void set_keyboard_input_text(QString text);

private slots:

    void on_toolButton_zoom_plus_pressed();

    void on_toolButton_zoom_min_pressed();

    void on_toolButton_fit_all_pressed();

    void on_toolButton_view_front_pressed();

    void on_toolButton_view_back_pressed();

    void on_toolButton_view_top_pressed();

    void on_toolButton_view_bottom_pressed();

    void on_toolButton_view_left_pressed();

    void on_toolButton_view_right_pressed();

    void on_toolButton_view_3d_pressed();

    void on_toolButton_draw_3d_2p_line_pressed();

    void on_toolButton_color_pressed();

private:
    Ui::Form *ui;
};

#endif // FORM_H
