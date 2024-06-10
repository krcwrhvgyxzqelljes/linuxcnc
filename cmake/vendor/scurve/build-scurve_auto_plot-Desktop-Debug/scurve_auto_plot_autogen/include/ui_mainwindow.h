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
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_9;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QFrame *frame_2;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *doubleSpinBox_feed_override;
    QLabel *label_28;
    QLabel *label_27;
    QLineEdit *lineEdit_maxvel;
    QLabel *label_3;
    QPushButton *pushButton_setall;
    QLabel *label;
    QLineEdit *lineEdit_maxjer;
    QLabel *label_2;
    QLineEdit *lineEdit_maxacc;
    QLabel *label_29;
    QDoubleSpinBox *doubleSpinBox_adafee;
    QLabel *label_30;
    QGridLayout *gridLayout_5;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_resume;
    QPushButton *pushButton_run;
    QCheckBox *checkBox_revmot;
    QPushButton *pushButton_pause_timer;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_13;
    QLabel *label_segpro;
    QLabel *label_12;
    QLabel *label_10;
    QLabel *label_seg_ve;
    QLabel *label_curvel;
    QLabel *label_seg_vo;
    QLabel *label_20;
    QLabel *label_curacc;
    QLabel *label_segbeg;
    QLabel *label_11;
    QLabel *label_21;
    QLabel *label_6;
    QLabel *label_trjpro;
    QCheckBox *checkBox_revmot_status;
    QLabel *label_5;
    QLabel *label_18;
    QLabel *label_17;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_vectnr;
    QCheckBox *checkBox_pause;
    QLabel *label_9;
    QLabel *label_7;
    QLabel *label_8;
    QCheckBox *checkBox_finished;
    QLabel *label_14;
    QLabel *label_19;
    QLabel *label_15;
    QLabel *label_curpos;
    QLabel *label_16;
    QLabel *label_22;
    QLabel *label_segend;
    QLabel *label_26;
    QLabel *label_23;
    QLabel *label_trjlen;
    QLabel *label_31;
    QLabel *label_32;
    QCheckBox *checkBox_pause_timer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1043, 605);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(35, 35, 35);\n"
"color: rgb(255, 255, 255);\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_9 = new QGridLayout(centralWidget);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 150));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);


        gridLayout_9->addWidget(frame, 0, 0, 1, 2);

        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_8 = new QGridLayout(frame_2);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupBox_2 = new QGroupBox(frame_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        doubleSpinBox_feed_override = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_feed_override->setObjectName(QString::fromUtf8("doubleSpinBox_feed_override"));
        doubleSpinBox_feed_override->setMaximum(150.000000000000000);
        doubleSpinBox_feed_override->setValue(100.000000000000000);

        gridLayout_4->addWidget(doubleSpinBox_feed_override, 5, 1, 1, 1);

        label_28 = new QLabel(groupBox_2);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_4->addWidget(label_28, 5, 2, 1, 1);

        label_27 = new QLabel(groupBox_2);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_4->addWidget(label_27, 5, 0, 1, 1);

        lineEdit_maxvel = new QLineEdit(groupBox_2);
        lineEdit_maxvel->setObjectName(QString::fromUtf8("lineEdit_maxvel"));

        gridLayout_4->addWidget(lineEdit_maxvel, 3, 1, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 3, 0, 1, 1);

        pushButton_setall = new QPushButton(groupBox_2);
        pushButton_setall->setObjectName(QString::fromUtf8("pushButton_setall"));

        gridLayout_4->addWidget(pushButton_setall, 4, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 1, 0, 1, 1);

        lineEdit_maxjer = new QLineEdit(groupBox_2);
        lineEdit_maxjer->setObjectName(QString::fromUtf8("lineEdit_maxjer"));

        gridLayout_4->addWidget(lineEdit_maxjer, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 2, 0, 1, 1);

        lineEdit_maxacc = new QLineEdit(groupBox_2);
        lineEdit_maxacc->setObjectName(QString::fromUtf8("lineEdit_maxacc"));

        gridLayout_4->addWidget(lineEdit_maxacc, 2, 1, 1, 1);

        label_29 = new QLabel(groupBox_2);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_4->addWidget(label_29, 6, 0, 1, 1);

        doubleSpinBox_adafee = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_adafee->setObjectName(QString::fromUtf8("doubleSpinBox_adafee"));
        doubleSpinBox_adafee->setMinimum(-100.000000000000000);
        doubleSpinBox_adafee->setMaximum(100.000000000000000);
        doubleSpinBox_adafee->setValue(100.000000000000000);

        gridLayout_4->addWidget(doubleSpinBox_adafee, 6, 1, 1, 1);

        label_30 = new QLabel(groupBox_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_4->addWidget(label_30, 6, 2, 1, 1);


        gridLayout_6->addLayout(gridLayout_4, 0, 1, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        pushButton_pause = new QPushButton(groupBox_2);
        pushButton_pause->setObjectName(QString::fromUtf8("pushButton_pause"));

        gridLayout_5->addWidget(pushButton_pause, 1, 0, 1, 1);

        pushButton_resume = new QPushButton(groupBox_2);
        pushButton_resume->setObjectName(QString::fromUtf8("pushButton_resume"));

        gridLayout_5->addWidget(pushButton_resume, 2, 0, 1, 1);

        pushButton_run = new QPushButton(groupBox_2);
        pushButton_run->setObjectName(QString::fromUtf8("pushButton_run"));

        gridLayout_5->addWidget(pushButton_run, 3, 0, 1, 1);

        checkBox_revmot = new QCheckBox(groupBox_2);
        checkBox_revmot->setObjectName(QString::fromUtf8("checkBox_revmot"));

        gridLayout_5->addWidget(checkBox_revmot, 0, 0, 1, 1);

        pushButton_pause_timer = new QPushButton(groupBox_2);
        pushButton_pause_timer->setObjectName(QString::fromUtf8("pushButton_pause_timer"));

        gridLayout_5->addWidget(pushButton_pause_timer, 4, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer, 2, 1, 1, 1);


        gridLayout_8->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox = new QGroupBox(frame_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        gridLayout_7 = new QGridLayout(groupBox);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 4, 2, 1, 1);

        label_segpro = new QLabel(groupBox);
        label_segpro->setObjectName(QString::fromUtf8("label_segpro"));

        gridLayout_2->addWidget(label_segpro, 7, 1, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 3, 2, 1, 1);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 2, 3, 1, 1);

        label_seg_ve = new QLabel(groupBox);
        label_seg_ve->setObjectName(QString::fromUtf8("label_seg_ve"));

        gridLayout_2->addWidget(label_seg_ve, 8, 4, 1, 1);

        label_curvel = new QLabel(groupBox);
        label_curvel->setObjectName(QString::fromUtf8("label_curvel"));
        label_curvel->setMinimumSize(QSize(100, 0));

        gridLayout_2->addWidget(label_curvel, 0, 1, 1, 1);

        label_seg_vo = new QLabel(groupBox);
        label_seg_vo->setObjectName(QString::fromUtf8("label_seg_vo"));

        gridLayout_2->addWidget(label_seg_vo, 7, 4, 1, 1);

        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_2->addWidget(label_20, 4, 3, 1, 1);

        label_curacc = new QLabel(groupBox);
        label_curacc->setObjectName(QString::fromUtf8("label_curacc"));

        gridLayout_2->addWidget(label_curacc, 1, 1, 1, 1);

        label_segbeg = new QLabel(groupBox);
        label_segbeg->setObjectName(QString::fromUtf8("label_segbeg"));

        gridLayout_2->addWidget(label_segbeg, 1, 4, 1, 1);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 4, 0, 1, 1);

        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_2->addWidget(label_21, 8, 5, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        label_trjpro = new QLabel(groupBox);
        label_trjpro->setObjectName(QString::fromUtf8("label_trjpro"));

        gridLayout_2->addWidget(label_trjpro, 4, 1, 1, 1);

        checkBox_revmot_status = new QCheckBox(groupBox);
        checkBox_revmot_status->setObjectName(QString::fromUtf8("checkBox_revmot_status"));

        gridLayout_2->addWidget(checkBox_revmot_status, 6, 4, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        label_18 = new QLabel(groupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_2->addWidget(label_18, 8, 3, 1, 1);

        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 7, 2, 1, 1);

        label_24 = new QLabel(groupBox);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_2->addWidget(label_24, 2, 2, 1, 1);

        label_25 = new QLabel(groupBox);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_2->addWidget(label_25, 1, 5, 1, 1);

        label_vectnr = new QLabel(groupBox);
        label_vectnr->setObjectName(QString::fromUtf8("label_vectnr"));
        label_vectnr->setMinimumSize(QSize(100, 0));

        gridLayout_2->addWidget(label_vectnr, 0, 4, 1, 1);

        checkBox_pause = new QCheckBox(groupBox);
        checkBox_pause->setObjectName(QString::fromUtf8("checkBox_pause"));

        gridLayout_2->addWidget(checkBox_pause, 4, 4, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 3, 0, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 3, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 1, 3, 1, 1);

        checkBox_finished = new QCheckBox(groupBox);
        checkBox_finished->setObjectName(QString::fromUtf8("checkBox_finished"));

        gridLayout_2->addWidget(checkBox_finished, 3, 4, 1, 1);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 3, 3, 1, 1);

        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_2->addWidget(label_19, 7, 5, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_2->addWidget(label_15, 7, 0, 1, 1);

        label_curpos = new QLabel(groupBox);
        label_curpos->setObjectName(QString::fromUtf8("label_curpos"));

        gridLayout_2->addWidget(label_curpos, 2, 1, 1, 1);

        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 7, 3, 1, 1);

        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_2->addWidget(label_22, 0, 2, 1, 1);

        label_segend = new QLabel(groupBox);
        label_segend->setObjectName(QString::fromUtf8("label_segend"));

        gridLayout_2->addWidget(label_segend, 2, 4, 1, 1);

        label_26 = new QLabel(groupBox);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_2->addWidget(label_26, 2, 5, 1, 1);

        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_2->addWidget(label_23, 1, 2, 1, 1);

        label_trjlen = new QLabel(groupBox);
        label_trjlen->setObjectName(QString::fromUtf8("label_trjlen"));

        gridLayout_2->addWidget(label_trjlen, 3, 1, 1, 1);

        label_31 = new QLabel(groupBox);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_2->addWidget(label_31, 6, 3, 1, 1);

        label_32 = new QLabel(groupBox);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_2->addWidget(label_32, 5, 3, 1, 1);

        checkBox_pause_timer = new QCheckBox(groupBox);
        checkBox_pause_timer->setObjectName(QString::fromUtf8("checkBox_pause_timer"));

        gridLayout_2->addWidget(checkBox_pause_timer, 5, 4, 1, 1);


        gridLayout_7->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout_8->addWidget(groupBox, 0, 1, 1, 1);


        gridLayout_9->addWidget(frame_2, 1, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1043, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "scurve auto plot", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "scurve control", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "feed override", nullptr));
        lineEdit_maxvel->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "maxvel", nullptr));
        pushButton_setall->setText(QCoreApplication::translate("MainWindow", "set all", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "maxjerk", nullptr));
        lineEdit_maxjer->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "maxacc", nullptr));
        lineEdit_maxacc->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "adaptive feed", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        pushButton_pause->setText(QCoreApplication::translate("MainWindow", "pause", nullptr));
        pushButton_resume->setText(QCoreApplication::translate("MainWindow", "resume", nullptr));
        pushButton_run->setText(QCoreApplication::translate("MainWindow", "run", nullptr));
        checkBox_revmot->setText(QCoreApplication::translate("MainWindow", "Reverse motion", nullptr));
        pushButton_pause_timer->setText(QCoreApplication::translate("MainWindow", "pause timer", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "scurve status", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "curvel", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        label_segpro->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "mm", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "lenght end", nullptr));
        label_seg_ve->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_curvel->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_seg_vo->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "pause", nullptr));
        label_curacc->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_segbeg->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "traject progress", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "mm/s", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "curpos", nullptr));
        label_trjpro->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        checkBox_revmot_status->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "curacc", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "segment ve", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "mm", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "mm", nullptr));
        label_vectnr->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        checkBox_pause->setText(QString());
        label_9->setText(QCoreApplication::translate("MainWindow", "traject lenght", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "vector nr", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "lenght beg", nullptr));
        checkBox_finished->setText(QString());
        label_14->setText(QCoreApplication::translate("MainWindow", "traject finished", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "mm/s", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "segment progress", nullptr));
        label_curpos->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "segment vo", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "mm/s", nullptr));
        label_segend->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "mm", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "mm/s^2", nullptr));
        label_trjlen->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "reverse motion", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "pause timer", nullptr));
        checkBox_pause_timer->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
