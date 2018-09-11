/********************************************************************************
** Form generated from reading UI file 'ScreenSaver.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENSAVER_H
#define UI_SCREENSAVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenSaver
{
public:
    QLabel *PictureLabel;

    void setupUi(QWidget *ScreenSaver)
    {
        if (ScreenSaver->objectName().isEmpty())
            ScreenSaver->setObjectName(QStringLiteral("ScreenSaver"));
        ScreenSaver->resize(1600, 900);
        PictureLabel = new QLabel(ScreenSaver);
        PictureLabel->setObjectName(QStringLiteral("PictureLabel"));
        PictureLabel->setGeometry(QRect(0, 0, 1600, 900));
        PictureLabel->setStyleSheet(QStringLiteral("background-color:rgb(0, 0, 0);"));
        PictureLabel->setAlignment(Qt::AlignCenter);

        retranslateUi(ScreenSaver);

        QMetaObject::connectSlotsByName(ScreenSaver);
    } // setupUi

    void retranslateUi(QWidget *ScreenSaver)
    {
        ScreenSaver->setWindowTitle(QApplication::translate("ScreenSaver", "ScreenSaver", nullptr));
        PictureLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ScreenSaver: public Ui_ScreenSaver {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENSAVER_H
