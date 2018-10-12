/********************************************************************************
** Form generated from reading UI file 'BgTransparent.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BGTRANSPARENT_H
#define UI_BGTRANSPARENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BgTransparent
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *l_imageState;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *imageLabel;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QFrame *f_bgColor;
    QSpinBox *sb_variations;
    QPushButton *pb_bgColor;
    QLabel *label_4;
    QLabel *label;
    QSpinBox *sb_bgGray;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pb_previousImage;
    QPushButton *pb_autoProcess;
    QPushButton *pb_transparent;
    QSpacerItem *verticalSpacer;
    QPushButton *pb_nextImage;
    QComboBox *transformCbB;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *sb_edgeThreshold;
    QSpinBox *sb_blurThreshold;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *BgTransparent)
    {
        if (BgTransparent->objectName().isEmpty())
            BgTransparent->setObjectName(QStringLiteral("BgTransparent"));
        BgTransparent->resize(731, 450);
        gridLayout = new QGridLayout(BgTransparent);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        l_imageState = new QLabel(BgTransparent);
        l_imageState->setObjectName(QStringLiteral("l_imageState"));

        gridLayout->addWidget(l_imageState, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        frame = new QFrame(BgTransparent);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        imageLabel = new QLabel(frame);
        imageLabel->setObjectName(QStringLiteral("imageLabel"));
        imageLabel->setStyleSheet(QStringLiteral("background-color:rgb(0, 0, 0)"));
        imageLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(imageLabel);


        gridLayout->addWidget(frame, 1, 0, 1, 3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);

        gridLayout_2->addItem(verticalSpacer_2, 4, 0, 1, 2);

        groupBox = new QGroupBox(BgTransparent);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(8);
        groupBox->setFont(font);
        groupBox->setStyleSheet(QStringLiteral("QGroupBox#groupBox{background-color:rgb(157, 157, 157)}"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        f_bgColor = new QFrame(groupBox);
        f_bgColor->setObjectName(QStringLiteral("f_bgColor"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        QBrush brush2(QColor(127, 127, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        QBrush brush3(QColor(170, 170, 170, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush4(QColor(255, 255, 220, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        f_bgColor->setPalette(palette);
        f_bgColor->setAutoFillBackground(true);
        f_bgColor->setFrameShape(QFrame::StyledPanel);
        f_bgColor->setFrameShadow(QFrame::Raised);

        gridLayout_4->addWidget(f_bgColor, 1, 1, 1, 1);

        sb_variations = new QSpinBox(groupBox);
        sb_variations->setObjectName(QStringLiteral("sb_variations"));
        sb_variations->setMaximum(255);

        gridLayout_4->addWidget(sb_variations, 2, 1, 1, 1);

        pb_bgColor = new QPushButton(groupBox);
        pb_bgColor->setObjectName(QStringLiteral("pb_bgColor"));
        pb_bgColor->setMaximumSize(QSize(60, 16777215));

        gridLayout_4->addWidget(pb_bgColor, 1, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_4->addWidget(label_4, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_4->addWidget(label, 3, 0, 1, 1);

        sb_bgGray = new QSpinBox(groupBox);
        sb_bgGray->setObjectName(QStringLiteral("sb_bgGray"));
        sb_bgGray->setMaximum(255);
        sb_bgGray->setValue(255);

        gridLayout_4->addWidget(sb_bgGray, 3, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Preferred);

        gridLayout_4->addItem(verticalSpacer_3, 0, 0, 1, 2);


        gridLayout_2->addWidget(groupBox, 3, 0, 1, 2);

        pb_previousImage = new QPushButton(BgTransparent);
        pb_previousImage->setObjectName(QStringLiteral("pb_previousImage"));
        pb_previousImage->setMaximumSize(QSize(60, 16777215));

        gridLayout_2->addWidget(pb_previousImage, 10, 0, 1, 1);

        pb_autoProcess = new QPushButton(BgTransparent);
        pb_autoProcess->setObjectName(QStringLiteral("pb_autoProcess"));

        gridLayout_2->addWidget(pb_autoProcess, 9, 0, 1, 2);

        pb_transparent = new QPushButton(BgTransparent);
        pb_transparent->setObjectName(QStringLiteral("pb_transparent"));

        gridLayout_2->addWidget(pb_transparent, 11, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 8, 0, 1, 2);

        pb_nextImage = new QPushButton(BgTransparent);
        pb_nextImage->setObjectName(QStringLiteral("pb_nextImage"));
        pb_nextImage->setMaximumSize(QSize(60, 16777215));

        gridLayout_2->addWidget(pb_nextImage, 10, 1, 1, 1);

        transformCbB = new QComboBox(BgTransparent);
        transformCbB->addItem(QString());
        transformCbB->addItem(QString());
        transformCbB->setObjectName(QStringLiteral("transformCbB"));
        transformCbB->setMinimumSize(QSize(75, 0));

        gridLayout_2->addWidget(transformCbB, 1, 0, 1, 2);

        groupBox_2 = new QGroupBox(BgTransparent);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setFont(font);
        groupBox_2->setStyleSheet(QStringLiteral("QGroupBox#groupBox_2{background-color:rgb(157, 157, 157)}"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 2, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 3, 0, 1, 1);

        sb_edgeThreshold = new QSpinBox(groupBox_2);
        sb_edgeThreshold->setObjectName(QStringLiteral("sb_edgeThreshold"));
        sb_edgeThreshold->setMaximum(255);
        sb_edgeThreshold->setValue(200);

        gridLayout_3->addWidget(sb_edgeThreshold, 2, 1, 1, 1);

        sb_blurThreshold = new QSpinBox(groupBox_2);
        sb_blurThreshold->setObjectName(QStringLiteral("sb_blurThreshold"));
        sb_blurThreshold->setMaximum(255);
        sb_blurThreshold->setValue(150);

        gridLayout_3->addWidget(sb_blurThreshold, 3, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Preferred);

        gridLayout_3->addItem(verticalSpacer_4, 0, 0, 1, 2);


        gridLayout_2->addWidget(groupBox_2, 5, 0, 1, 2);


        gridLayout->addLayout(gridLayout_2, 0, 3, 3, 1);


        retranslateUi(BgTransparent);

        QMetaObject::connectSlotsByName(BgTransparent);
    } // setupUi

    void retranslateUi(QWidget *BgTransparent)
    {
        BgTransparent->setWindowTitle(QApplication::translate("BgTransparent", "BgTransparent", nullptr));
        l_imageState->setText(QApplication::translate("BgTransparent", "\345\233\276\347\211\207\344\270\272\347\251\272", nullptr));
        imageLabel->setText(QString());
        groupBox->setTitle(QApplication::translate("BgTransparent", "\345\210\233\345\273\272alpha\351\200\232\351\201\223", nullptr));
        pb_bgColor->setText(QApplication::translate("BgTransparent", "\350\203\214\346\231\257\351\242\234\350\211\262", nullptr));
        label_4->setText(QApplication::translate("BgTransparent", "\345\217\230\345\212\250\345\271\205\345\272\246", nullptr));
        label->setText(QApplication::translate("BgTransparent", "\350\203\214\346\231\257\347\201\260\345\272\246\345\200\274", nullptr));
        pb_previousImage->setText(QApplication::translate("BgTransparent", "\344\270\212\344\270\200\345\274\240", nullptr));
        pb_autoProcess->setText(QApplication::translate("BgTransparent", "\350\207\252\345\212\250\345\244\204\347\220\206", nullptr));
        pb_transparent->setText(QApplication::translate("BgTransparent", "\345\274\200\345\247\213", nullptr));
        pb_nextImage->setText(QApplication::translate("BgTransparent", "\344\270\213\344\270\200\345\274\240", nullptr));
        transformCbB->setItemText(0, QApplication::translate("BgTransparent", "\345\277\253\351\200\237\345\217\230\346\215\242", nullptr));
        transformCbB->setItemText(1, QApplication::translate("BgTransparent", "\345\271\263\346\273\221\345\217\230\346\215\242", nullptr));

        groupBox_2->setTitle(QApplication::translate("BgTransparent", "\350\276\271\347\274\230\345\271\263\346\273\221", nullptr));
        label_2->setText(QApplication::translate("BgTransparent", "\350\276\271\347\274\230\351\230\210\345\200\274", nullptr));
        label_3->setText(QApplication::translate("BgTransparent", "\346\250\241\347\263\212\351\230\210\345\200\274", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BgTransparent: public Ui_BgTransparent {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BGTRANSPARENT_H
