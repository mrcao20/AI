/********************************************************************************
** Form generated from reading UI file 'AppList.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLIST_H
#define UI_APPLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AppListWidget
{
public:
    QTableWidget *AppTableWidget;

    void setupUi(QWidget *AppListWidget)
    {
        if (AppListWidget->objectName().isEmpty())
            AppListWidget->setObjectName(QStringLiteral("AppListWidget"));
        AppListWidget->resize(572, 311);
        AppListWidget->setMinimumSize(QSize(572, 311));
        AppTableWidget = new QTableWidget(AppListWidget);
        if (AppTableWidget->columnCount() < 5)
            AppTableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        AppTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        AppTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        AppTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (AppTableWidget->rowCount() < 3)
            AppTableWidget->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        AppTableWidget->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        AppTableWidget->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        AppTableWidget->setItem(0, 0, __qtablewidgetitem5);
        AppTableWidget->setObjectName(QStringLiteral("AppTableWidget"));
        AppTableWidget->setGeometry(QRect(0, 0, 571, 311));
        AppTableWidget->setMinimumSize(QSize(571, 311));
        AppTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        AppTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        AppTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        AppTableWidget->setIconSize(QSize(60, 60));
        AppTableWidget->setTextElideMode(Qt::ElideNone);
        AppTableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        AppTableWidget->setShowGrid(false);
        AppTableWidget->setRowCount(3);
        AppTableWidget->setColumnCount(5);
        AppTableWidget->horizontalHeader()->setVisible(false);
        AppTableWidget->horizontalHeader()->setDefaultSectionSize(110);
        AppTableWidget->verticalHeader()->setVisible(false);
        AppTableWidget->verticalHeader()->setDefaultSectionSize(100);

        retranslateUi(AppListWidget);

        QMetaObject::connectSlotsByName(AppListWidget);
    } // setupUi

    void retranslateUi(QWidget *AppListWidget)
    {
        AppListWidget->setWindowTitle(QApplication::translate("AppListWidget", "App", nullptr));

        const bool __sortingEnabled = AppTableWidget->isSortingEnabled();
        AppTableWidget->setSortingEnabled(false);
        AppTableWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class AppListWidget: public Ui_AppListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLIST_H
