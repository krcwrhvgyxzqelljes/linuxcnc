/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QLabel *label_dro_x;
    QLabel *label_y;
    QLabel *label_dro_dtg_x;
    QLabel *label_dtg_z;
    QLabel *label_dro_uh_x;
    QLabel *label_dro_y;
    QLabel *label_vel;
    QLabel *label_dtg_y;
    QLabel *label_dro_uh_y;
    QLabel *label_x;
    QLabel *label_dro_dtg_z;
    QLabel *label_dro_vel;
    QLabel *label_dtg_x;
    QLabel *label_dro_uh_z;
    QLabel *label_dro_dtg_y;
    QLabel *label_dro_z;
    QLabel *label_z;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(624, 433);
        Form->setWindowOpacity(0.000000000000000);
        gridLayout_2 = new QGridLayout(Form);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_dro_x = new QLabel(Form);
        label_dro_x->setObjectName(QString::fromUtf8("label_dro_x"));
        label_dro_x->setMinimumSize(QSize(75, 0));
        label_dro_x->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_x, 0, 2, 1, 1);

        label_y = new QLabel(Form);
        label_y->setObjectName(QString::fromUtf8("label_y"));
        label_y->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_y, 1, 1, 1, 1);

        label_dro_dtg_x = new QLabel(Form);
        label_dro_dtg_x->setObjectName(QString::fromUtf8("label_dro_dtg_x"));
        label_dro_dtg_x->setMinimumSize(QSize(75, 0));
        label_dro_dtg_x->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_dtg_x, 0, 4, 1, 1);

        label_dtg_z = new QLabel(Form);
        label_dtg_z->setObjectName(QString::fromUtf8("label_dtg_z"));
        label_dtg_z->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dtg_z, 2, 3, 1, 1);

        label_dro_uh_x = new QLabel(Form);
        label_dro_uh_x->setObjectName(QString::fromUtf8("label_dro_uh_x"));
        label_dro_uh_x->setMinimumSize(QSize(30, 0));
        label_dro_uh_x->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_uh_x, 0, 0, 1, 1);

        label_dro_y = new QLabel(Form);
        label_dro_y->setObjectName(QString::fromUtf8("label_dro_y"));
        label_dro_y->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_y, 1, 2, 1, 1);

        label_vel = new QLabel(Form);
        label_vel->setObjectName(QString::fromUtf8("label_vel"));
        label_vel->setMinimumSize(QSize(0, 0));
        label_vel->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_vel, 3, 1, 1, 1);

        label_dtg_y = new QLabel(Form);
        label_dtg_y->setObjectName(QString::fromUtf8("label_dtg_y"));
        label_dtg_y->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dtg_y, 1, 3, 1, 1);

        label_dro_uh_y = new QLabel(Form);
        label_dro_uh_y->setObjectName(QString::fromUtf8("label_dro_uh_y"));
        label_dro_uh_y->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_uh_y, 1, 0, 1, 1);

        label_x = new QLabel(Form);
        label_x->setObjectName(QString::fromUtf8("label_x"));
        label_x->setMinimumSize(QSize(40, 0));
        label_x->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_x, 0, 1, 1, 1);

        label_dro_dtg_z = new QLabel(Form);
        label_dro_dtg_z->setObjectName(QString::fromUtf8("label_dro_dtg_z"));
        label_dro_dtg_z->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_dtg_z, 2, 4, 1, 1);

        label_dro_vel = new QLabel(Form);
        label_dro_vel->setObjectName(QString::fromUtf8("label_dro_vel"));
        label_dro_vel->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_vel, 3, 2, 1, 1);

        label_dtg_x = new QLabel(Form);
        label_dtg_x->setObjectName(QString::fromUtf8("label_dtg_x"));
        label_dtg_x->setMinimumSize(QSize(50, 0));
        label_dtg_x->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dtg_x, 0, 3, 1, 1);

        label_dro_uh_z = new QLabel(Form);
        label_dro_uh_z->setObjectName(QString::fromUtf8("label_dro_uh_z"));
        label_dro_uh_z->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_uh_z, 2, 0, 1, 1);

        label_dro_dtg_y = new QLabel(Form);
        label_dro_dtg_y->setObjectName(QString::fromUtf8("label_dro_dtg_y"));
        label_dro_dtg_y->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_dtg_y, 1, 4, 1, 1);

        label_dro_z = new QLabel(Form);
        label_dro_z->setObjectName(QString::fromUtf8("label_dro_z"));
        label_dro_z->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_dro_z, 2, 2, 1, 1);

        label_z = new QLabel(Form);
        label_z->setObjectName(QString::fromUtf8("label_z"));
        label_z->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0,0);"));

        gridLayout->addWidget(label_z, 2, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        label_dro_x->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_y->setText(QCoreApplication::translate("Form", "Y", nullptr));
        label_dro_dtg_x->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_dtg_z->setText(QCoreApplication::translate("Form", "DTG Z", nullptr));
        label_dro_uh_x->setText(QCoreApplication::translate("Form", "UH", nullptr));
        label_dro_y->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_vel->setText(QCoreApplication::translate("Form", "Vel", nullptr));
        label_dtg_y->setText(QCoreApplication::translate("Form", "DTG Y", nullptr));
        label_dro_uh_y->setText(QCoreApplication::translate("Form", "UH", nullptr));
        label_x->setText(QCoreApplication::translate("Form", "X", nullptr));
        label_dro_dtg_z->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_dro_vel->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_dtg_x->setText(QCoreApplication::translate("Form", "DTG X", nullptr));
        label_dro_uh_z->setText(QCoreApplication::translate("Form", "UH", nullptr));
        label_dro_dtg_y->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_dro_z->setText(QCoreApplication::translate("Form", "0.000", nullptr));
        label_z->setText(QCoreApplication::translate("Form", "Z", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
