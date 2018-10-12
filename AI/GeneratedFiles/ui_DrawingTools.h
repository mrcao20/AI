/********************************************************************************
** Form generated from reading UI file 'DrawingTools.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWINGTOOLS_H
#define UI_DRAWINGTOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mccanvas.h"

QT_BEGIN_NAMESPACE

class Ui_DrawingTools
{
public:
    QGridLayout *gridLayout;
    McCanvas *f_canvas;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pb_undo;
    QPushButton *pb_redo;
    QPushButton *pb_reset;
    QDial *d_lineWidth;
    QLabel *l_lineWidth;
    QDial *d_alpha;
    QLabel *l_alpha;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pb_lineColor;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QPushButton *pb_brushColor;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pb_drawRect;
    QPushButton *pb_drawPolygon;
    QPushButton *pb_drawEllipse;
    QPushButton *pb_drawLine;
    QPushButton *pb_drawWithPen;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *DrawingTools)
    {
        if (DrawingTools->objectName().isEmpty())
            DrawingTools->setObjectName(QStringLiteral("DrawingTools"));
        DrawingTools->resize(731, 484);
        gridLayout = new QGridLayout(DrawingTools);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        f_canvas = new McCanvas(DrawingTools);
        f_canvas->setObjectName(QStringLiteral("f_canvas"));
        f_canvas->setFocusPolicy(Qt::StrongFocus);
        f_canvas->setFrameShape(QFrame::StyledPanel);
        f_canvas->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(f_canvas, 2, 0, 1, 1);

        groupBox_2 = new QGroupBox(DrawingTools);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox#groupBox_2{\n"
"	border:none;\n"
"	background-color:rgb(188, 188, 188);\n"
"}"));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 0, 0);
        pb_undo = new QPushButton(groupBox_2);
        pb_undo->setObjectName(QStringLiteral("pb_undo"));
        pb_undo->setStyleSheet(QLatin1String("QPushButton#pb_undo{\n"
"	background-color:color(0, 0, 0, 0);\n"
"}\n"
"QPushButton#pb_undo:hover{\n"
"	background-color:color(255, 255, 255, 100);\n"
"}"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_undo->setIcon(icon);
        pb_undo->setIconSize(QSize(19, 19));

        horizontalLayout->addWidget(pb_undo);

        pb_redo = new QPushButton(groupBox_2);
        pb_redo->setObjectName(QStringLiteral("pb_redo"));
        pb_redo->setStyleSheet(QLatin1String("QPushButton#pb_redo{\n"
"	background-color:color(0, 0, 0, 0);\n"
"}\n"
"QPushButton#pb_redo:hover{\n"
"	background-color:color(255, 255, 255, 100);\n"
"}"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_redo->setIcon(icon1);
        pb_redo->setIconSize(QSize(19, 19));

        horizontalLayout->addWidget(pb_redo);

        pb_reset = new QPushButton(groupBox_2);
        pb_reset->setObjectName(QStringLiteral("pb_reset"));
        pb_reset->setStyleSheet(QLatin1String("QPushButton#pb_reset{\n"
"	background-color:color(0, 0, 0, 0);\n"
"}\n"
"QPushButton#pb_reset:hover{\n"
"	background-color:color(255, 255, 255, 100);\n"
"}"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_reset->setIcon(icon2);
        pb_reset->setIconSize(QSize(19, 19));

        horizontalLayout->addWidget(pb_reset);

        d_lineWidth = new QDial(groupBox_2);
        d_lineWidth->setObjectName(QStringLiteral("d_lineWidth"));
        d_lineWidth->setMinimumSize(QSize(51, 51));
        d_lineWidth->setMaximumSize(QSize(51, 51));
        d_lineWidth->setMinimum(1);
        d_lineWidth->setMaximum(20);

        horizontalLayout->addWidget(d_lineWidth);

        l_lineWidth = new QLabel(groupBox_2);
        l_lineWidth->setObjectName(QStringLiteral("l_lineWidth"));

        horizontalLayout->addWidget(l_lineWidth);

        d_alpha = new QDial(groupBox_2);
        d_alpha->setObjectName(QStringLiteral("d_alpha"));
        d_alpha->setMinimumSize(QSize(51, 51));
        d_alpha->setMaximumSize(QSize(51, 51));
        d_alpha->setMaximum(255);

        horizontalLayout->addWidget(d_alpha);

        l_alpha = new QLabel(groupBox_2);
        l_alpha->setObjectName(QStringLiteral("l_alpha"));

        horizontalLayout->addWidget(l_alpha);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        pb_lineColor = new QPushButton(groupBox_2);
        pb_lineColor->setObjectName(QStringLiteral("pb_lineColor"));
        pb_lineColor->setMinimumSize(QSize(0, 23));
        pb_lineColor->setStyleSheet(QStringLiteral("border:1px solid white;"));
        pb_lineColor->setFlat(true);

        verticalLayout->addWidget(pb_lineColor);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        pb_brushColor = new QPushButton(groupBox_2);
        pb_brushColor->setObjectName(QStringLiteral("pb_brushColor"));
        pb_brushColor->setMinimumSize(QSize(0, 23));
        pb_brushColor->setStyleSheet(QStringLiteral("border:1px solid white;"));
        pb_brushColor->setFlat(true);

        verticalLayout_2->addWidget(pb_brushColor);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox = new QGroupBox(DrawingTools);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setStyleSheet(QLatin1String("QGroupBox#groupBox{\n"
"	border:none;\n"
"	background-color:rgb(85, 255, 0, 100);\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 0, 0);
        pb_drawRect = new QPushButton(groupBox);
        pb_drawRect->setObjectName(QStringLiteral("pb_drawRect"));
        pb_drawRect->setMinimumSize(QSize(0, 15));
        pb_drawRect->setMaximumSize(QSize(16777215, 15));
        pb_drawRect->setStyleSheet(QLatin1String("QPushButton#pb_drawRect{\n"
"	border:none;\n"
"	background-color:rgb(85, 255, 0, 0);\n"
"}\n"
"QPushButton#pb_drawRect:hover{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}\n"
"QPushButton#pb_drawRect:checked{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/rect.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_drawRect->setIcon(icon3);
        pb_drawRect->setIconSize(QSize(33, 27));
        pb_drawRect->setCheckable(true);
        pb_drawRect->setChecked(true);

        horizontalLayout_2->addWidget(pb_drawRect);

        pb_drawPolygon = new QPushButton(groupBox);
        pb_drawPolygon->setObjectName(QStringLiteral("pb_drawPolygon"));
        pb_drawPolygon->setStyleSheet(QLatin1String("QPushButton#pb_drawPolygon{\n"
"	border:none;\n"
"	background-color:rgb(85, 255, 0, 0);\n"
"}\n"
"QPushButton#pb_drawPolygon:hover{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}\n"
"QPushButton#pb_drawPolygon:checked{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_drawPolygon->setIcon(icon4);
        pb_drawPolygon->setIconSize(QSize(33, 27));
        pb_drawPolygon->setCheckable(true);

        horizontalLayout_2->addWidget(pb_drawPolygon);

        pb_drawEllipse = new QPushButton(groupBox);
        pb_drawEllipse->setObjectName(QStringLiteral("pb_drawEllipse"));
        pb_drawEllipse->setMinimumSize(QSize(0, 15));
        pb_drawEllipse->setMaximumSize(QSize(16777215, 15));
        pb_drawEllipse->setStyleSheet(QLatin1String("QPushButton#pb_drawEllipse{\n"
"	border:none;\n"
"	background-color:rgb(85, 255, 0, 0);\n"
"}\n"
"QPushButton#pb_drawEllipse:hover{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}\n"
"QPushButton#pb_drawEllipse:checked{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/ellipse.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_drawEllipse->setIcon(icon5);
        pb_drawEllipse->setIconSize(QSize(33, 27));
        pb_drawEllipse->setCheckable(true);

        horizontalLayout_2->addWidget(pb_drawEllipse);

        pb_drawLine = new QPushButton(groupBox);
        pb_drawLine->setObjectName(QStringLiteral("pb_drawLine"));
        pb_drawLine->setStyleSheet(QLatin1String("QPushButton#pb_drawLine{\n"
"	border:none;\n"
"	background-color:rgb(85, 255, 0, 0);\n"
"}\n"
"QPushButton#pb_drawLine:hover{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}\n"
"QPushButton#pb_drawLine:checked{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/line.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_drawLine->setIcon(icon6);
        pb_drawLine->setIconSize(QSize(33, 27));
        pb_drawLine->setCheckable(true);

        horizontalLayout_2->addWidget(pb_drawLine);

        pb_drawWithPen = new QPushButton(groupBox);
        pb_drawWithPen->setObjectName(QStringLiteral("pb_drawWithPen"));
        pb_drawWithPen->setMinimumSize(QSize(0, 15));
        pb_drawWithPen->setMaximumSize(QSize(16777215, 15));
        pb_drawWithPen->setStyleSheet(QLatin1String("QPushButton#pb_drawWithPen{\n"
"	border:none;\n"
"	background-color:rgb(85, 255, 0, 0);\n"
"}\n"
"QPushButton#pb_drawWithPen:hover{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}\n"
"QPushButton#pb_drawWithPen:checked{\n"
"	background-color:rgb(85, 255, 0, 130);\n"
"}"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/GraphProc/images/graphProc/drawTools/pen.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_drawWithPen->setIcon(icon7);
        pb_drawWithPen->setIconSize(QSize(33, 27));
        pb_drawWithPen->setCheckable(true);

        horizontalLayout_2->addWidget(pb_drawWithPen);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);


        retranslateUi(DrawingTools);

        QMetaObject::connectSlotsByName(DrawingTools);
    } // setupUi

    void retranslateUi(QWidget *DrawingTools)
    {
        DrawingTools->setWindowTitle(QApplication::translate("DrawingTools", "DrawingTools", nullptr));
        groupBox_2->setTitle(QApplication::translate("DrawingTools", "property", nullptr));
#ifndef QT_NO_TOOLTIP
        pb_undo->setToolTip(QApplication::translate("DrawingTools", "Undo", nullptr));
#endif // QT_NO_TOOLTIP
        pb_undo->setText(QString());
#ifndef QT_NO_SHORTCUT
        pb_undo->setShortcut(QApplication::translate("DrawingTools", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
#ifndef QT_NO_TOOLTIP
        pb_redo->setToolTip(QApplication::translate("DrawingTools", "Redo", nullptr));
#endif // QT_NO_TOOLTIP
        pb_redo->setText(QString());
#ifndef QT_NO_SHORTCUT
        pb_redo->setShortcut(QApplication::translate("DrawingTools", "Ctrl+Y", nullptr));
#endif // QT_NO_SHORTCUT
#ifndef QT_NO_TOOLTIP
        pb_reset->setToolTip(QApplication::translate("DrawingTools", "Reset", nullptr));
#endif // QT_NO_TOOLTIP
        pb_reset->setText(QString());
#ifndef QT_NO_SHORTCUT
        pb_reset->setShortcut(QApplication::translate("DrawingTools", "Ctrl+R", nullptr));
#endif // QT_NO_SHORTCUT
        l_lineWidth->setText(QApplication::translate("DrawingTools", "\347\272\277\345\256\275:1", nullptr));
        l_alpha->setText(QApplication::translate("DrawingTools", "alpha\345\200\274:0", nullptr));
        label->setText(QApplication::translate("DrawingTools", "\347\272\277\346\235\241\351\242\234\350\211\262", nullptr));
        pb_lineColor->setText(QString());
        label_2->setText(QApplication::translate("DrawingTools", "\345\241\253\345\205\205\351\242\234\350\211\262", nullptr));
        pb_brushColor->setText(QString());
        groupBox->setTitle(QApplication::translate("DrawingTools", "shape", nullptr));
#ifndef QT_NO_TOOLTIP
        pb_drawRect->setToolTip(QApplication::translate("DrawingTools", "Draw a Rect", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pb_drawPolygon->setToolTip(QApplication::translate("DrawingTools", "Draw A Polygon", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pb_drawEllipse->setToolTip(QApplication::translate("DrawingTools", "Draw A Ellipse", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pb_drawLine->setToolTip(QApplication::translate("DrawingTools", "Draw A Line", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pb_drawWithPen->setToolTip(QApplication::translate("DrawingTools", "Draw With Pen", nullptr));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class DrawingTools: public Ui_DrawingTools {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWINGTOOLS_H
