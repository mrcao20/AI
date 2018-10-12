/********************************************************************************
** Form generated from reading UI file 'GraphicalProcesses.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICALPROCESSES_H
#define UI_GRAPHICALPROCESSES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "bgtransparent.h"
#include "mcdrawingtools.h"

QT_BEGIN_NAMESPACE

class Ui_GraphicalProcessesClass
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    BgTransparent *bgTransparent;
    QWidget *snippingTool;
    McDrawingTools *drawingTool;
    QSpacerItem *horizontalSpacer;
    QPushButton *pb_reset;
    QPushButton *pb_file;

    void setupUi(QWidget *GraphicalProcessesClass)
    {
        if (GraphicalProcessesClass->objectName().isEmpty())
            GraphicalProcessesClass->setObjectName(QStringLiteral("GraphicalProcessesClass"));
        GraphicalProcessesClass->resize(768, 533);
        gridLayout = new QGridLayout(GraphicalProcessesClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(GraphicalProcessesClass);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setStyleSheet(QLatin1String("QTabBar::tab{\n"
"	width:80;\n"
"	height:25;\n"
"};"));
        bgTransparent = new BgTransparent();
        bgTransparent->setObjectName(QStringLiteral("bgTransparent"));
        bgTransparent->setStyleSheet(QStringLiteral("QWidget#bgTransparent{background-color:rgb(206, 206, 206)}"));
        tabWidget->addTab(bgTransparent, QString());
        snippingTool = new QWidget();
        snippingTool->setObjectName(QStringLiteral("snippingTool"));
        tabWidget->addTab(snippingTool, QString());
        drawingTool = new McDrawingTools();
        drawingTool->setObjectName(QStringLiteral("drawingTool"));
        tabWidget->addTab(drawingTool, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        pb_reset = new QPushButton(GraphicalProcessesClass);
        pb_reset->setObjectName(QStringLiteral("pb_reset"));

        gridLayout->addWidget(pb_reset, 0, 2, 1, 1);

        pb_file = new QPushButton(GraphicalProcessesClass);
        pb_file->setObjectName(QStringLiteral("pb_file"));
        pb_file->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(pb_file, 0, 0, 1, 1);


        retranslateUi(GraphicalProcessesClass);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(GraphicalProcessesClass);
    } // setupUi

    void retranslateUi(QWidget *GraphicalProcessesClass)
    {
        GraphicalProcessesClass->setWindowTitle(QApplication::translate("GraphicalProcessesClass", "GraphicalProcesses", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(bgTransparent), QApplication::translate("GraphicalProcessesClass", "\350\203\214\346\231\257\351\200\217\346\230\216", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(snippingTool), QApplication::translate("GraphicalProcessesClass", "\346\210\252\345\233\276\345\267\245\345\205\267", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(drawingTool), QApplication::translate("GraphicalProcessesClass", "\347\224\273\345\233\276\345\267\245\345\205\267", nullptr));
        pb_reset->setText(QApplication::translate("GraphicalProcessesClass", "\351\207\215\347\275\256", nullptr));
        pb_file->setText(QApplication::translate("GraphicalProcessesClass", "\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphicalProcessesClass: public Ui_GraphicalProcessesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICALPROCESSES_H
