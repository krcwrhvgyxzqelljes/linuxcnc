/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout_35;
    QGridLayout *gridLayout_19;
    QSlider *horizontalSlider_spindle_override;
    QLabel *label_43;
    QLabel *label_feed_override;
    QLabel *label_text_max_velocity;
    QSlider *horizontalSlider_rapid_override;
    QLabel *label_39;
    QLabel *label_text_jog_speed;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QLabel *label_3;
    QCheckBox *checkBox_mist;
    QCheckBox *checkBox_flood;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_4;
    QPushButton *pushButton_home_all;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_jog_min;
    QPushButton *pushButton_jog_plus;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QRadioButton *radioButton_x;
    QRadioButton *radioButton_y;
    QRadioButton *radioButton_z;
    QSpacerItem *horizontalSpacer_7;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton_spindle_ccw;
    QToolButton *toolButton_spindle_stop;
    QToolButton *toolButton_spindle_cw;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_spindle_min;
    QPushButton *pushButton_spindle_plus;
    QSpinBox *spinBox_spindlespeed;
    QLabel *label_spindle_rpm;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_8;
    QPlainTextEdit *plainTextEdit_mdi_history;
    QLineEdit *lineEdit_mdi_command;
    QToolButton *toolButton_mdi_command_exec;
    QLabel *label_text_history;
    QLabel *label_text_mdi_command;
    QLabel *label_42;
    QLabel *label_44;
    QPlainTextEdit *plainTextEdit_active_gcodes;
    QSlider *horizontalSlider_jog_speed;
    QLabel *label_max_velocity;
    QLabel *label_jog_speed;
    QLabel *label_spindle_override;
    QSlider *horizontalSlider_max_velocity;
    QLabel *label_rapid_override;
    QSlider *horizontalSlider_feed_override;
    QGridLayout *gridLayout_16;
    QToolButton *toolButton_view_bottom;
    QToolButton *toolButton_emergency;
    QToolButton *toolButton_machine_on;
    QDoubleSpinBox *doubleSpinBox_p1x;
    QToolButton *toolButton_view_3d;
    QDoubleSpinBox *doubleSpinBox_distribution;
    QToolButton *toolButton_view_top;
    QDoubleSpinBox *doubleSpinBox_offset;
    QToolButton *toolButton_zoom_plus;
    QLabel *label_6;
    QToolButton *toolButton_pause;
    QDoubleSpinBox *doubleSpinBox_filter;
    QToolButton *toolButton_fit_all;
    QDoubleSpinBox *doubleSpinBox_p2x;
    QToolButton *toolButton_view_right;
    QLabel *label_machine_on_status;
    QToolButton *toolButton_run;
    QCheckBox *checkBox;
    QToolButton *toolButton_stop;
    QLabel *label_emergency_status;
    QPushButton *pushButton_calc;
    QLabel *label_5;
    QToolButton *toolButton_file_open;
    QDoubleSpinBox *doubleSpinBox_p1y;
    QToolButton *toolButton_view_left;
    QToolButton *toolButton_view_front;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QToolButton *toolButton_zoom_min;
    QDoubleSpinBox *doubleSpinBox_p2y;
    QPushButton *pushButton_test;
    QToolButton *toolButton_view_back;
    QToolButton *toolButton_reload;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_occ;
    QFrame *frame_gcode;
    QGridLayout *gridLayout_34;
    QGridLayout *gridLayout_gcode;
    QLabel *label_machine_status;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1384, 894);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(51, 57, 59);\n"
"color: rgb(255, 255, 255);\n"
""));
        MainWindow->setIconSize(QSize(30, 30));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setAutoFillBackground(false);
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(51, 57, 59);\n"
"color: rgb(255, 255, 255);\n"
""));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(400, 500));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_35 = new QGridLayout(frame);
        gridLayout_35->setObjectName(QString::fromUtf8("gridLayout_35"));
        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        horizontalSlider_spindle_override = new QSlider(frame);
        horizontalSlider_spindle_override->setObjectName(QString::fromUtf8("horizontalSlider_spindle_override"));
        horizontalSlider_spindle_override->setMaximum(100);
        horizontalSlider_spindle_override->setValue(0);
        horizontalSlider_spindle_override->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(horizontalSlider_spindle_override, 3, 2, 1, 1);

        label_43 = new QLabel(frame);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        QFont font;
        font.setFamily(QString::fromUtf8("Sans Serif"));
        font.setPointSize(10);
        label_43->setFont(font);
        label_43->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_43, 2, 0, 1, 1);

        label_feed_override = new QLabel(frame);
        label_feed_override->setObjectName(QString::fromUtf8("label_feed_override"));
        label_feed_override->setFont(font);
        label_feed_override->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_feed_override, 1, 1, 1, 1);

        label_text_max_velocity = new QLabel(frame);
        label_text_max_velocity->setObjectName(QString::fromUtf8("label_text_max_velocity"));
        label_text_max_velocity->setFont(font);
        label_text_max_velocity->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_text_max_velocity, 5, 0, 1, 1);

        horizontalSlider_rapid_override = new QSlider(frame);
        horizontalSlider_rapid_override->setObjectName(QString::fromUtf8("horizontalSlider_rapid_override"));
        horizontalSlider_rapid_override->setMaximum(100);
        horizontalSlider_rapid_override->setValue(0);
        horizontalSlider_rapid_override->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(horizontalSlider_rapid_override, 2, 2, 1, 1);

        label_39 = new QLabel(frame);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setFont(font);
        label_39->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_39, 6, 0, 1, 2);

        label_text_jog_speed = new QLabel(frame);
        label_text_jog_speed->setObjectName(QString::fromUtf8("label_text_jog_speed"));
        label_text_jog_speed->setFont(font);
        label_text_jog_speed->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_text_jog_speed, 4, 0, 1, 1);

        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_7 = new QGridLayout(tab);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

        checkBox_mist = new QCheckBox(tab);
        checkBox_mist->setObjectName(QString::fromUtf8("checkBox_mist"));

        gridLayout_5->addWidget(checkBox_mist, 0, 1, 1, 1);

        checkBox_flood = new QCheckBox(tab);
        checkBox_flood->setObjectName(QString::fromUtf8("checkBox_flood"));

        gridLayout_5->addWidget(checkBox_flood, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 0, 2, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 2, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pushButton_home_all = new QPushButton(tab);
        pushButton_home_all->setObjectName(QString::fromUtf8("pushButton_home_all"));

        gridLayout_4->addWidget(pushButton_home_all, 2, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_jog_min = new QPushButton(tab);
        pushButton_jog_min->setObjectName(QString::fromUtf8("pushButton_jog_min"));
        pushButton_jog_min->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(pushButton_jog_min);

        pushButton_jog_plus = new QPushButton(tab);
        pushButton_jog_plus->setObjectName(QString::fromUtf8("pushButton_jog_plus"));
        pushButton_jog_plus->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(pushButton_jog_plus);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);


        gridLayout_4->addLayout(horizontalLayout, 1, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        radioButton_x = new QRadioButton(tab);
        radioButton_x->setObjectName(QString::fromUtf8("radioButton_x"));
        radioButton_x->setChecked(true);

        horizontalLayout_2->addWidget(radioButton_x);

        radioButton_y = new QRadioButton(tab);
        radioButton_y->setObjectName(QString::fromUtf8("radioButton_y"));

        horizontalLayout_2->addWidget(radioButton_y);

        radioButton_z = new QRadioButton(tab);
        radioButton_z->setObjectName(QString::fromUtf8("radioButton_z"));

        horizontalLayout_2->addWidget(radioButton_z);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);


        gridLayout_4->addLayout(horizontalLayout_2, 0, 0, 1, 2);


        gridLayout_6->addLayout(gridLayout_4, 0, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        toolButton_spindle_ccw = new QToolButton(tab);
        toolButton_spindle_ccw->setObjectName(QString::fromUtf8("toolButton_spindle_ccw"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/share/axis/images/spindle_ccw.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_spindle_ccw->setIcon(icon);
        toolButton_spindle_ccw->setCheckable(true);

        horizontalLayout_3->addWidget(toolButton_spindle_ccw);

        toolButton_spindle_stop = new QToolButton(tab);
        toolButton_spindle_stop->setObjectName(QString::fromUtf8("toolButton_spindle_stop"));
        toolButton_spindle_stop->setCheckable(true);

        horizontalLayout_3->addWidget(toolButton_spindle_stop);

        toolButton_spindle_cw = new QToolButton(tab);
        toolButton_spindle_cw->setObjectName(QString::fromUtf8("toolButton_spindle_cw"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/share/axis/images/spindle_cw.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_spindle_cw->setIcon(icon1);
        toolButton_spindle_cw->setCheckable(true);

        horizontalLayout_3->addWidget(toolButton_spindle_cw);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton_spindle_min = new QPushButton(tab);
        pushButton_spindle_min->setObjectName(QString::fromUtf8("pushButton_spindle_min"));
        pushButton_spindle_min->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_4->addWidget(pushButton_spindle_min);

        pushButton_spindle_plus = new QPushButton(tab);
        pushButton_spindle_plus->setObjectName(QString::fromUtf8("pushButton_spindle_plus"));
        pushButton_spindle_plus->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_4->addWidget(pushButton_spindle_plus);

        spinBox_spindlespeed = new QSpinBox(tab);
        spinBox_spindlespeed->setObjectName(QString::fromUtf8("spinBox_spindlespeed"));
        spinBox_spindlespeed->setMaximum(50000);
        spinBox_spindlespeed->setValue(10);

        horizontalLayout_4->addWidget(spinBox_spindlespeed);

        label_spindle_rpm = new QLabel(tab);
        label_spindle_rpm->setObjectName(QString::fromUtf8("label_spindle_rpm"));

        horizontalLayout_4->addWidget(label_spindle_rpm);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        gridLayout_3->addLayout(horizontalLayout_4, 1, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_3, 1, 0, 1, 1);


        gridLayout_7->addLayout(gridLayout_6, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 1, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_9 = new QGridLayout(tab_2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        plainTextEdit_mdi_history = new QPlainTextEdit(tab_2);
        plainTextEdit_mdi_history->setObjectName(QString::fromUtf8("plainTextEdit_mdi_history"));

        gridLayout_8->addWidget(plainTextEdit_mdi_history, 1, 0, 1, 2);

        lineEdit_mdi_command = new QLineEdit(tab_2);
        lineEdit_mdi_command->setObjectName(QString::fromUtf8("lineEdit_mdi_command"));

        gridLayout_8->addWidget(lineEdit_mdi_command, 3, 0, 1, 1);

        toolButton_mdi_command_exec = new QToolButton(tab_2);
        toolButton_mdi_command_exec->setObjectName(QString::fromUtf8("toolButton_mdi_command_exec"));

        gridLayout_8->addWidget(toolButton_mdi_command_exec, 3, 1, 1, 1);

        label_text_history = new QLabel(tab_2);
        label_text_history->setObjectName(QString::fromUtf8("label_text_history"));

        gridLayout_8->addWidget(label_text_history, 0, 0, 1, 2);

        label_text_mdi_command = new QLabel(tab_2);
        label_text_mdi_command->setObjectName(QString::fromUtf8("label_text_mdi_command"));

        gridLayout_8->addWidget(label_text_mdi_command, 2, 0, 1, 2);


        gridLayout_9->addLayout(gridLayout_8, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout_19->addWidget(tabWidget, 0, 0, 1, 3);

        label_42 = new QLabel(frame);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setFont(font);
        label_42->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_42, 3, 0, 1, 1);

        label_44 = new QLabel(frame);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setFont(font);
        label_44->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_44, 1, 0, 1, 1);

        plainTextEdit_active_gcodes = new QPlainTextEdit(frame);
        plainTextEdit_active_gcodes->setObjectName(QString::fromUtf8("plainTextEdit_active_gcodes"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(plainTextEdit_active_gcodes->sizePolicy().hasHeightForWidth());
        plainTextEdit_active_gcodes->setSizePolicy(sizePolicy1);
        plainTextEdit_active_gcodes->setMaximumSize(QSize(16777215, 50));
        plainTextEdit_active_gcodes->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(plainTextEdit_active_gcodes, 7, 0, 1, 3);

        horizontalSlider_jog_speed = new QSlider(frame);
        horizontalSlider_jog_speed->setObjectName(QString::fromUtf8("horizontalSlider_jog_speed"));
        horizontalSlider_jog_speed->setMaximum(2000);
        horizontalSlider_jog_speed->setValue(1000);
        horizontalSlider_jog_speed->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(horizontalSlider_jog_speed, 4, 2, 1, 1);

        label_max_velocity = new QLabel(frame);
        label_max_velocity->setObjectName(QString::fromUtf8("label_max_velocity"));
        label_max_velocity->setFont(font);
        label_max_velocity->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_max_velocity, 5, 1, 1, 1);

        label_jog_speed = new QLabel(frame);
        label_jog_speed->setObjectName(QString::fromUtf8("label_jog_speed"));
        label_jog_speed->setFont(font);
        label_jog_speed->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_jog_speed, 4, 1, 1, 1);

        label_spindle_override = new QLabel(frame);
        label_spindle_override->setObjectName(QString::fromUtf8("label_spindle_override"));
        label_spindle_override->setFont(font);
        label_spindle_override->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_spindle_override, 3, 1, 1, 1);

        horizontalSlider_max_velocity = new QSlider(frame);
        horizontalSlider_max_velocity->setObjectName(QString::fromUtf8("horizontalSlider_max_velocity"));
        horizontalSlider_max_velocity->setMaximum(2000);
        horizontalSlider_max_velocity->setValue(0);
        horizontalSlider_max_velocity->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(horizontalSlider_max_velocity, 5, 2, 1, 1);

        label_rapid_override = new QLabel(frame);
        label_rapid_override->setObjectName(QString::fromUtf8("label_rapid_override"));
        label_rapid_override->setFont(font);
        label_rapid_override->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));

        gridLayout_19->addWidget(label_rapid_override, 2, 1, 1, 1);

        horizontalSlider_feed_override = new QSlider(frame);
        horizontalSlider_feed_override->setObjectName(QString::fromUtf8("horizontalSlider_feed_override"));
        horizontalSlider_feed_override->setMaximum(120);
        horizontalSlider_feed_override->setValue(0);
        horizontalSlider_feed_override->setSliderPosition(0);
        horizontalSlider_feed_override->setOrientation(Qt::Horizontal);

        gridLayout_19->addWidget(horizontalSlider_feed_override, 1, 2, 1, 1);


        gridLayout_35->addLayout(gridLayout_19, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame, 1, 0, 1, 1);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        toolButton_view_bottom = new QToolButton(centralwidget);
        toolButton_view_bottom->setObjectName(QString::fromUtf8("toolButton_view_bottom"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/vendor/icons/view-bottom.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_bottom->setIcon(icon2);
        toolButton_view_bottom->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_bottom, 0, 15, 1, 1);

        toolButton_emergency = new QToolButton(centralwidget);
        toolButton_emergency->setObjectName(QString::fromUtf8("toolButton_emergency"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolButton_emergency->sizePolicy().hasHeightForWidth());
        toolButton_emergency->setSizePolicy(sizePolicy2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/share/axis/images/tool_estop.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_emergency->setIcon(icon3);
        toolButton_emergency->setIconSize(QSize(28, 28));
        toolButton_emergency->setCheckable(true);

        gridLayout_16->addWidget(toolButton_emergency, 0, 0, 1, 1);

        toolButton_machine_on = new QToolButton(centralwidget);
        toolButton_machine_on->setObjectName(QString::fromUtf8("toolButton_machine_on"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/share/axis/images/tool_power.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_machine_on->setIcon(icon4);
        toolButton_machine_on->setIconSize(QSize(28, 28));
        toolButton_machine_on->setCheckable(true);

        gridLayout_16->addWidget(toolButton_machine_on, 0, 2, 1, 1);

        doubleSpinBox_p1x = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_p1x->setObjectName(QString::fromUtf8("doubleSpinBox_p1x"));
        doubleSpinBox_p1x->setMinimum(-100.000000000000000);
        doubleSpinBox_p1x->setMaximum(100.000000000000000);
        doubleSpinBox_p1x->setSingleStep(1.000000000000000);
        doubleSpinBox_p1x->setValue(55.000000000000000);

        gridLayout_16->addWidget(doubleSpinBox_p1x, 0, 20, 1, 1);

        toolButton_view_3d = new QToolButton(centralwidget);
        toolButton_view_3d->setObjectName(QString::fromUtf8("toolButton_view_3d"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/vendor/icons/view-iso.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_3d->setIcon(icon5);
        toolButton_view_3d->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_3d, 0, 18, 1, 1);

        doubleSpinBox_distribution = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_distribution->setObjectName(QString::fromUtf8("doubleSpinBox_distribution"));
        doubleSpinBox_distribution->setMaximum(1000.000000000000000);
        doubleSpinBox_distribution->setSingleStep(0.100000000000000);
        doubleSpinBox_distribution->setValue(1.000000000000000);

        gridLayout_16->addWidget(doubleSpinBox_distribution, 0, 24, 1, 1);

        toolButton_view_top = new QToolButton(centralwidget);
        toolButton_view_top->setObjectName(QString::fromUtf8("toolButton_view_top"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/vendor/icons/view-top.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_top->setIcon(icon6);
        toolButton_view_top->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_top, 0, 14, 1, 1);

        doubleSpinBox_offset = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_offset->setObjectName(QString::fromUtf8("doubleSpinBox_offset"));
        doubleSpinBox_offset->setMinimum(0.000000000000000);
        doubleSpinBox_offset->setMaximum(10000.000000000000000);
        doubleSpinBox_offset->setSingleStep(0.100000000000000);
        doubleSpinBox_offset->setValue(0.500000000000000);

        gridLayout_16->addWidget(doubleSpinBox_offset, 1, 24, 1, 1);

        toolButton_zoom_plus = new QToolButton(centralwidget);
        toolButton_zoom_plus->setObjectName(QString::fromUtf8("toolButton_zoom_plus"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/vendor/icons/zoom_plus.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_zoom_plus->setIcon(icon7);
        toolButton_zoom_plus->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_zoom_plus, 0, 9, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_16->addWidget(label_6, 2, 23, 1, 1);

        toolButton_pause = new QToolButton(centralwidget);
        toolButton_pause->setObjectName(QString::fromUtf8("toolButton_pause"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/share/axis/images/tool_pause.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_pause->setIcon(icon8);
        toolButton_pause->setIconSize(QSize(28, 28));
        toolButton_pause->setCheckable(true);

        gridLayout_16->addWidget(toolButton_pause, 0, 7, 1, 1);

        doubleSpinBox_filter = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_filter->setObjectName(QString::fromUtf8("doubleSpinBox_filter"));
        doubleSpinBox_filter->setMaximum(100.000000000000000);
        doubleSpinBox_filter->setSingleStep(0.100000000000000);
        doubleSpinBox_filter->setValue(0.100000000000000);

        gridLayout_16->addWidget(doubleSpinBox_filter, 2, 24, 1, 1);

        toolButton_fit_all = new QToolButton(centralwidget);
        toolButton_fit_all->setObjectName(QString::fromUtf8("toolButton_fit_all"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/vendor/icons/zoom_extend.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_fit_all->setIcon(icon9);
        toolButton_fit_all->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_fit_all, 0, 11, 1, 1);

        doubleSpinBox_p2x = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_p2x->setObjectName(QString::fromUtf8("doubleSpinBox_p2x"));
        doubleSpinBox_p2x->setMinimum(-100.000000000000000);
        doubleSpinBox_p2x->setMaximum(100.000000000000000);
        doubleSpinBox_p2x->setSingleStep(1.000000000000000);
        doubleSpinBox_p2x->setValue(70.000000000000000);

        gridLayout_16->addWidget(doubleSpinBox_p2x, 1, 20, 1, 1);

        toolButton_view_right = new QToolButton(centralwidget);
        toolButton_view_right->setObjectName(QString::fromUtf8("toolButton_view_right"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/vendor/icons/view-right.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_right->setIcon(icon10);
        toolButton_view_right->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_right, 0, 17, 1, 1);

        label_machine_on_status = new QLabel(centralwidget);
        label_machine_on_status->setObjectName(QString::fromUtf8("label_machine_on_status"));
        label_machine_on_status->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);"));

        gridLayout_16->addWidget(label_machine_on_status, 0, 3, 1, 1);

        toolButton_run = new QToolButton(centralwidget);
        toolButton_run->setObjectName(QString::fromUtf8("toolButton_run"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/share/axis/images/tool_run.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_run->setIcon(icon11);
        toolButton_run->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_run, 0, 6, 1, 1);

        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_16->addWidget(checkBox, 1, 22, 1, 1);

        toolButton_stop = new QToolButton(centralwidget);
        toolButton_stop->setObjectName(QString::fromUtf8("toolButton_stop"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/share/axis/images/tool_stop.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_stop->setIcon(icon12);
        toolButton_stop->setIconSize(QSize(28, 28));
        toolButton_stop->setCheckable(false);

        gridLayout_16->addWidget(toolButton_stop, 0, 8, 1, 1);

        label_emergency_status = new QLabel(centralwidget);
        label_emergency_status->setObjectName(QString::fromUtf8("label_emergency_status"));
        label_emergency_status->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);"));

        gridLayout_16->addWidget(label_emergency_status, 0, 1, 1, 1);

        pushButton_calc = new QPushButton(centralwidget);
        pushButton_calc->setObjectName(QString::fromUtf8("pushButton_calc"));

        gridLayout_16->addWidget(pushButton_calc, 2, 25, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_16->addWidget(label_5, 1, 23, 1, 1);

        toolButton_file_open = new QToolButton(centralwidget);
        toolButton_file_open->setObjectName(QString::fromUtf8("toolButton_file_open"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/share/axis/images/tool_open.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_file_open->setIcon(icon13);
        toolButton_file_open->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_file_open, 0, 4, 1, 1);

        doubleSpinBox_p1y = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_p1y->setObjectName(QString::fromUtf8("doubleSpinBox_p1y"));
        doubleSpinBox_p1y->setMinimum(-100.000000000000000);
        doubleSpinBox_p1y->setMaximum(100.000000000000000);
        doubleSpinBox_p1y->setValue(60.000000000000000);

        gridLayout_16->addWidget(doubleSpinBox_p1y, 0, 21, 1, 1);

        toolButton_view_left = new QToolButton(centralwidget);
        toolButton_view_left->setObjectName(QString::fromUtf8("toolButton_view_left"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/vendor/icons/view-left.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_left->setIcon(icon14);
        toolButton_view_left->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_left, 0, 16, 1, 1);

        toolButton_view_front = new QToolButton(centralwidget);
        toolButton_view_front->setObjectName(QString::fromUtf8("toolButton_view_front"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/vendor/icons/view-front.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_front->setIcon(icon15);
        toolButton_view_front->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_front, 0, 12, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_16->addItem(horizontalSpacer, 0, 25, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_16->addWidget(label_4, 0, 23, 1, 1);

        toolButton_zoom_min = new QToolButton(centralwidget);
        toolButton_zoom_min->setObjectName(QString::fromUtf8("toolButton_zoom_min"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/vendor/icons/zoom_min.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_zoom_min->setIcon(icon16);
        toolButton_zoom_min->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_zoom_min, 0, 10, 1, 1);

        doubleSpinBox_p2y = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_p2y->setObjectName(QString::fromUtf8("doubleSpinBox_p2y"));
        doubleSpinBox_p2y->setMinimum(-100.000000000000000);
        doubleSpinBox_p2y->setMaximum(100.000000000000000);
        doubleSpinBox_p2y->setValue(60.000000000000000);

        gridLayout_16->addWidget(doubleSpinBox_p2y, 1, 21, 1, 1);

        pushButton_test = new QPushButton(centralwidget);
        pushButton_test->setObjectName(QString::fromUtf8("pushButton_test"));

        gridLayout_16->addWidget(pushButton_test, 0, 19, 1, 1);

        toolButton_view_back = new QToolButton(centralwidget);
        toolButton_view_back->setObjectName(QString::fromUtf8("toolButton_view_back"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/vendor/icons/view-back.svg"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_view_back->setIcon(icon17);
        toolButton_view_back->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_view_back, 0, 13, 1, 1);

        toolButton_reload = new QToolButton(centralwidget);
        toolButton_reload->setObjectName(QString::fromUtf8("toolButton_reload"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/share/axis/images/tool_reload.gif"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_reload->setIcon(icon18);
        toolButton_reload->setIconSize(QSize(28, 28));

        gridLayout_16->addWidget(toolButton_reload, 0, 5, 1, 1);


        gridLayout_2->addLayout(gridLayout_16, 0, 0, 1, 2);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_occ = new QGridLayout();
        gridLayout_occ->setObjectName(QString::fromUtf8("gridLayout_occ"));

        gridLayout->addLayout(gridLayout_occ, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame_2, 1, 1, 1, 1);

        frame_gcode = new QFrame(centralwidget);
        frame_gcode->setObjectName(QString::fromUtf8("frame_gcode"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_gcode->sizePolicy().hasHeightForWidth());
        frame_gcode->setSizePolicy(sizePolicy3);
        frame_gcode->setMinimumSize(QSize(0, 150));
        frame_gcode->setMaximumSize(QSize(16777215, 150));
        frame_gcode->setFrameShape(QFrame::StyledPanel);
        frame_gcode->setFrameShadow(QFrame::Raised);
        gridLayout_34 = new QGridLayout(frame_gcode);
        gridLayout_34->setObjectName(QString::fromUtf8("gridLayout_34"));
        gridLayout_gcode = new QGridLayout();
        gridLayout_gcode->setObjectName(QString::fromUtf8("gridLayout_gcode"));

        gridLayout_34->addLayout(gridLayout_gcode, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame_gcode, 2, 0, 1, 2);

        label_machine_status = new QLabel(centralwidget);
        label_machine_status->setObjectName(QString::fromUtf8("label_machine_status"));

        gridLayout_2->addWidget(label_machine_status, 3, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1384, 19));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "axis c++", nullptr));
        label_43->setText(QCoreApplication::translate("MainWindow", "Rapid Override:", nullptr));
        label_feed_override->setText(QCoreApplication::translate("MainWindow", "0%", nullptr));
        label_text_max_velocity->setText(QCoreApplication::translate("MainWindow", "Max Velocity:", nullptr));
        label_39->setText(QCoreApplication::translate("MainWindow", "Active G-Codes:", nullptr));
        label_text_jog_speed->setText(QCoreApplication::translate("MainWindow", "Jog Speed:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Coolant:", nullptr));
        checkBox_mist->setText(QCoreApplication::translate("MainWindow", "mist", nullptr));
        checkBox_flood->setText(QCoreApplication::translate("MainWindow", "flood", nullptr));
        pushButton_home_all->setText(QCoreApplication::translate("MainWindow", "Home All", nullptr));
        pushButton_jog_min->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        pushButton_jog_plus->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Axis:", nullptr));
        radioButton_x->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        radioButton_y->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        radioButton_z->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Spindle:", nullptr));
        toolButton_spindle_ccw->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_spindle_stop->setText(QCoreApplication::translate("MainWindow", "stop", nullptr));
        toolButton_spindle_cw->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        pushButton_spindle_min->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        pushButton_spindle_plus->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        label_spindle_rpm->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Manual Control", nullptr));
        toolButton_mdi_command_exec->setText(QCoreApplication::translate("MainWindow", "go", nullptr));
        label_text_history->setText(QCoreApplication::translate("MainWindow", "History:", nullptr));
        label_text_mdi_command->setText(QCoreApplication::translate("MainWindow", "MDI Command:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Mdi", nullptr));
        label_42->setText(QCoreApplication::translate("MainWindow", "Spindle Override:", nullptr));
        label_44->setText(QCoreApplication::translate("MainWindow", "Feed Override:", nullptr));
        plainTextEdit_active_gcodes->setPlainText(QCoreApplication::translate("MainWindow", "G1", nullptr));
        label_max_velocity->setText(QCoreApplication::translate("MainWindow", "0 mm/min", nullptr));
        label_jog_speed->setText(QCoreApplication::translate("MainWindow", "0 mm/min", nullptr));
        label_spindle_override->setText(QCoreApplication::translate("MainWindow", "0%", nullptr));
        label_rapid_override->setText(QCoreApplication::translate("MainWindow", "0%", nullptr));
        toolButton_view_bottom->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_emergency->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_machine_on->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_view_3d->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_view_top->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_zoom_plus->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "filter out <", nullptr));
        toolButton_pause->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_fit_all->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_view_right->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        label_machine_on_status->setText(QCoreApplication::translate("MainWindow", "[]", nullptr));
        toolButton_run->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "clear drawing", nullptr));
        toolButton_stop->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        label_emergency_status->setText(QCoreApplication::translate("MainWindow", "[]", nullptr));
        pushButton_calc->setText(QCoreApplication::translate("MainWindow", "calc", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "offset", nullptr));
        toolButton_file_open->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_view_left->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_view_front->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "distribution", nullptr));
        toolButton_zoom_min->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        pushButton_test->setText(QCoreApplication::translate("MainWindow", "test", nullptr));
        toolButton_view_back->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_reload->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        label_machine_status->setText(QCoreApplication::translate("MainWindow", "machine status", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
