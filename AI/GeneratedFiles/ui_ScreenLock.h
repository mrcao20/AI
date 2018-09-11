/********************************************************************************
** Form generated from reading UI file 'ScreenLock.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENLOCK_H
#define UI_SCREENLOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenLock
{
public:
    QLabel *ScreenLockPictureLabel;
    QLabel *TimeLabel;
    QLabel *DateLabel;
    QLineEdit *passwordEdit;
    QCommandLinkButton *loginButton;
    QLabel *errorLabel;
    QPushButton *verifyButton;

    void setupUi(QWidget *ScreenLock)
    {
        if (ScreenLock->objectName().isEmpty())
            ScreenLock->setObjectName(QStringLiteral("ScreenLock"));
        ScreenLock->resize(1600, 900);
        ScreenLockPictureLabel = new QLabel(ScreenLock);
        ScreenLockPictureLabel->setObjectName(QStringLiteral("ScreenLockPictureLabel"));
        ScreenLockPictureLabel->setGeometry(QRect(0, 0, 1600, 900));
        ScreenLockPictureLabel->setStyleSheet(QStringLiteral("background-color:rgb(0, 0, 0);"));
        ScreenLockPictureLabel->setAlignment(Qt::AlignCenter);
        TimeLabel = new QLabel(ScreenLock);
        TimeLabel->setObjectName(QStringLiteral("TimeLabel"));
        TimeLabel->setGeometry(QRect(40, 611, 241, 101));
        TimeLabel->setStyleSheet(QString::fromUtf8("color:rgb(255, 255, 255);\n"
"font: 25 72pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";"));
        TimeLabel->setAlignment(Qt::AlignCenter);
        DateLabel = new QLabel(ScreenLock);
        DateLabel->setObjectName(QStringLiteral("DateLabel"));
        DateLabel->setGeometry(QRect(32, 720, 381, 51));
        DateLabel->setStyleSheet(QString::fromUtf8("color:rgb(255, 255, 255);\n"
"font: 25 36pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";"));
        DateLabel->setAlignment(Qt::AlignCenter);
        passwordEdit = new QLineEdit(ScreenLock);
        passwordEdit->setObjectName(QStringLiteral("passwordEdit"));
        passwordEdit->setGeometry(QRect(660, 507, 271, 31));
        passwordEdit->setStyleSheet(QStringLiteral("background:rgb(234, 234, 234);"));
        passwordEdit->setMaxLength(20);
        passwordEdit->setEchoMode(QLineEdit::Password);
        loginButton = new QCommandLinkButton(ScreenLock);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(940, 500, 31, 41));
        loginButton->setStyleSheet(QStringLiteral(""));
        errorLabel = new QLabel(ScreenLock);
        errorLabel->setObjectName(QStringLiteral("errorLabel"));
        errorLabel->setGeometry(QRect(730, 480, 91, 31));
        errorLabel->setStyleSheet(QString::fromUtf8("color:rgb(255, 255, 255);\n"
"font: 25 15pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";"));
        errorLabel->setAlignment(Qt::AlignCenter);
        verifyButton = new QPushButton(ScreenLock);
        verifyButton->setObjectName(QStringLiteral("verifyButton"));
        verifyButton->setGeometry(QRect(736, 530, 81, 31));
        verifyButton->setStyleSheet(QString::fromUtf8("border-style:none;\n"
"background-color:rgb(188, 188, 188);\n"
"color:rgb(255, 255, 255);\n"
"font: 25 15pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";"));
        verifyButton->setAutoDefault(true);

        retranslateUi(ScreenLock);

        QMetaObject::connectSlotsByName(ScreenLock);
    } // setupUi

    void retranslateUi(QWidget *ScreenLock)
    {
        ScreenLock->setWindowTitle(QApplication::translate("ScreenLock", "Form", nullptr));
        TimeLabel->setText(QApplication::translate("ScreenLock", "14:16", nullptr));
        DateLabel->setText(QApplication::translate("ScreenLock", "4\346\234\21022\346\227\245, \346\230\237\346\234\237\346\227\245", nullptr));
        passwordEdit->setText(QString());
        loginButton->setText(QString());
        errorLabel->setText(QApplication::translate("ScreenLock", "\345\257\206\347\240\201\351\224\231\350\257\257", nullptr));
        verifyButton->setText(QApplication::translate("ScreenLock", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenLock: public Ui_ScreenLock {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENLOCK_H
