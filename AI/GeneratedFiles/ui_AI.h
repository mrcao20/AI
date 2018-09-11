/********************************************************************************
** Form generated from reading UI file 'AI.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AI_H
#define UI_AI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AIClass
{
public:

    void setupUi(QWidget *AIClass)
    {
        if (AIClass->objectName().isEmpty())
            AIClass->setObjectName(QStringLiteral("AIClass"));
        AIClass->resize(600, 400);

        retranslateUi(AIClass);

        QMetaObject::connectSlotsByName(AIClass);
    } // setupUi

    void retranslateUi(QWidget *AIClass)
    {
        AIClass->setWindowTitle(QApplication::translate("AIClass", "AI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AIClass: public Ui_AIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AI_H
