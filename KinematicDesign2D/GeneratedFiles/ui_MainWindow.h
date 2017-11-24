/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionRun;
    QAction *actionStop;
    QAction *actionStepForward;
    QAction *actionStepBackward;
    QAction *actionOpen;
    QAction *actionShowCenterPointCurve;
    QAction *actionShowCirclePointCurve;
    QAction *actionSelect;
    QAction *actionFixedRectangle;
    QAction *actionFixedPolygon;
    QAction *actionLayer1;
    QAction *actionLayer2;
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionKinematics;
    QAction *actionLayer3;
    QAction *actionGenerate4RLinkage;
    QAction *actionCollisionCheck;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionDelete;
    QAction *actionSelectAll;
    QAction *actionRunBackward;
    QAction *actionFixedCircle;
    QAction *actionOptions;
    QAction *actionLinkageRegion;
    QAction *actionTest;
    QAction *actionCircularRepeat;
    QAction *actionGenerateSliderCrank;
    QAction *actionAddLayer;
    QAction *actionInsertLayer;
    QAction *actionDebug;
    QAction *actionDeleteLayer;
    QAction *actionRectangle_2;
    QAction *actionMovingRectangle;
    QAction *actionMovingCircle;
    QAction *actionMovingPolygon;
    QAction *actionLinkageAvoidance;
    QAction *actionGenerateWattI;
    QAction *actionMirror;
    QAction *actionShowSolutions;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTool;
    QMenu *menuMode;
    QMenu *menuFixed_Body;
    QMenu *menuMoving_Body;
    QMenu *menuLayer;
    QMenu *menuEdit;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(702, 737);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionRun = new QAction(MainWindowClass);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        actionStop = new QAction(MainWindowClass);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionStepForward = new QAction(MainWindowClass);
        actionStepForward->setObjectName(QStringLiteral("actionStepForward"));
        actionStepBackward = new QAction(MainWindowClass);
        actionStepBackward->setObjectName(QStringLiteral("actionStepBackward"));
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionShowCenterPointCurve = new QAction(MainWindowClass);
        actionShowCenterPointCurve->setObjectName(QStringLiteral("actionShowCenterPointCurve"));
        actionShowCenterPointCurve->setCheckable(true);
        actionShowCirclePointCurve = new QAction(MainWindowClass);
        actionShowCirclePointCurve->setObjectName(QStringLiteral("actionShowCirclePointCurve"));
        actionShowCirclePointCurve->setCheckable(true);
        actionSelect = new QAction(MainWindowClass);
        actionSelect->setObjectName(QStringLiteral("actionSelect"));
        actionSelect->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/select.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect->setIcon(icon1);
        actionFixedRectangle = new QAction(MainWindowClass);
        actionFixedRectangle->setObjectName(QStringLiteral("actionFixedRectangle"));
        actionFixedRectangle->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/fixed_rectangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFixedRectangle->setIcon(icon2);
        actionFixedPolygon = new QAction(MainWindowClass);
        actionFixedPolygon->setObjectName(QStringLiteral("actionFixedPolygon"));
        actionFixedPolygon->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral("Resources/fixed_polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFixedPolygon->setIcon(icon3);
        actionLayer1 = new QAction(MainWindowClass);
        actionLayer1->setObjectName(QStringLiteral("actionLayer1"));
        actionLayer1->setCheckable(true);
        actionLayer2 = new QAction(MainWindowClass);
        actionLayer2->setObjectName(QStringLiteral("actionLayer2"));
        actionLayer2->setCheckable(true);
        actionNew = new QAction(MainWindowClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon4;
        icon4.addFile(QStringLiteral("Resources/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon4);
        actionSave = new QAction(MainWindowClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon5;
        icon5.addFile(QStringLiteral("Resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon5);
        actionKinematics = new QAction(MainWindowClass);
        actionKinematics->setObjectName(QStringLiteral("actionKinematics"));
        actionKinematics->setCheckable(true);
        actionLayer3 = new QAction(MainWindowClass);
        actionLayer3->setObjectName(QStringLiteral("actionLayer3"));
        actionLayer3->setCheckable(true);
        actionGenerate4RLinkage = new QAction(MainWindowClass);
        actionGenerate4RLinkage->setObjectName(QStringLiteral("actionGenerate4RLinkage"));
        QIcon icon6;
        icon6.addFile(QStringLiteral("Resources/fourbar_linkage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerate4RLinkage->setIcon(icon6);
        actionCollisionCheck = new QAction(MainWindowClass);
        actionCollisionCheck->setObjectName(QStringLiteral("actionCollisionCheck"));
        actionCollisionCheck->setCheckable(true);
        actionUndo = new QAction(MainWindowClass);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        QIcon icon7;
        icon7.addFile(QStringLiteral("Resources/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon7);
        actionRedo = new QAction(MainWindowClass);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        QIcon icon8;
        icon8.addFile(QStringLiteral("Resources/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon8);
        actionCopy = new QAction(MainWindowClass);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        QIcon icon9;
        icon9.addFile(QStringLiteral("Resources/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon9);
        actionPaste = new QAction(MainWindowClass);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        QIcon icon10;
        icon10.addFile(QStringLiteral("Resources/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon10);
        actionDelete = new QAction(MainWindowClass);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        QIcon icon11;
        icon11.addFile(QStringLiteral("Resources/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon11);
        actionSelectAll = new QAction(MainWindowClass);
        actionSelectAll->setObjectName(QStringLiteral("actionSelectAll"));
        actionRunBackward = new QAction(MainWindowClass);
        actionRunBackward->setObjectName(QStringLiteral("actionRunBackward"));
        actionFixedCircle = new QAction(MainWindowClass);
        actionFixedCircle->setObjectName(QStringLiteral("actionFixedCircle"));
        actionFixedCircle->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral("Resources/fixed_circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFixedCircle->setIcon(icon12);
        actionOptions = new QAction(MainWindowClass);
        actionOptions->setObjectName(QStringLiteral("actionOptions"));
        actionLinkageRegion = new QAction(MainWindowClass);
        actionLinkageRegion->setObjectName(QStringLiteral("actionLinkageRegion"));
        actionLinkageRegion->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral("Resources/linkage_region.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLinkageRegion->setIcon(icon13);
        actionTest = new QAction(MainWindowClass);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        actionCircularRepeat = new QAction(MainWindowClass);
        actionCircularRepeat->setObjectName(QStringLiteral("actionCircularRepeat"));
        actionGenerateSliderCrank = new QAction(MainWindowClass);
        actionGenerateSliderCrank->setObjectName(QStringLiteral("actionGenerateSliderCrank"));
        QIcon icon14;
        icon14.addFile(QStringLiteral("Resources/slider_crank.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerateSliderCrank->setIcon(icon14);
        actionAddLayer = new QAction(MainWindowClass);
        actionAddLayer->setObjectName(QStringLiteral("actionAddLayer"));
        actionInsertLayer = new QAction(MainWindowClass);
        actionInsertLayer->setObjectName(QStringLiteral("actionInsertLayer"));
        actionDebug = new QAction(MainWindowClass);
        actionDebug->setObjectName(QStringLiteral("actionDebug"));
        actionDeleteLayer = new QAction(MainWindowClass);
        actionDeleteLayer->setObjectName(QStringLiteral("actionDeleteLayer"));
        actionRectangle_2 = new QAction(MainWindowClass);
        actionRectangle_2->setObjectName(QStringLiteral("actionRectangle_2"));
        actionMovingRectangle = new QAction(MainWindowClass);
        actionMovingRectangle->setObjectName(QStringLiteral("actionMovingRectangle"));
        actionMovingRectangle->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QStringLiteral("Resources/moving_rectangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMovingRectangle->setIcon(icon15);
        actionMovingCircle = new QAction(MainWindowClass);
        actionMovingCircle->setObjectName(QStringLiteral("actionMovingCircle"));
        actionMovingCircle->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QStringLiteral("Resources/moving_circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMovingCircle->setIcon(icon16);
        actionMovingPolygon = new QAction(MainWindowClass);
        actionMovingPolygon->setObjectName(QStringLiteral("actionMovingPolygon"));
        actionMovingPolygon->setCheckable(true);
        QIcon icon17;
        icon17.addFile(QStringLiteral("Resources/moving_polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMovingPolygon->setIcon(icon17);
        actionLinkageAvoidance = new QAction(MainWindowClass);
        actionLinkageAvoidance->setObjectName(QStringLiteral("actionLinkageAvoidance"));
        actionLinkageAvoidance->setCheckable(true);
        QIcon icon18;
        icon18.addFile(QStringLiteral("Resources/linkage_avoidance.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLinkageAvoidance->setIcon(icon18);
        actionGenerateWattI = new QAction(MainWindowClass);
        actionGenerateWattI->setObjectName(QStringLiteral("actionGenerateWattI"));
        QIcon icon19;
        icon19.addFile(QStringLiteral("Resources/watt_i_linkage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerateWattI->setIcon(icon19);
        actionMirror = new QAction(MainWindowClass);
        actionMirror->setObjectName(QStringLiteral("actionMirror"));
        actionShowSolutions = new QAction(MainWindowClass);
        actionShowSolutions->setObjectName(QStringLiteral("actionShowSolutions"));
        actionShowSolutions->setCheckable(true);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 702, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        menuMode = new QMenu(menuBar);
        menuMode->setObjectName(QStringLiteral("menuMode"));
        menuFixed_Body = new QMenu(menuMode);
        menuFixed_Body->setObjectName(QStringLiteral("menuFixed_Body"));
        menuMoving_Body = new QMenu(menuMode);
        menuMoving_Body->setObjectName(QStringLiteral("menuMoving_Body"));
        menuLayer = new QMenu(menuBar);
        menuLayer->setObjectName(QStringLiteral("menuLayer"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuMode->menuAction());
        menuBar->addAction(menuLayer->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTool->addAction(actionGenerate4RLinkage);
        menuTool->addAction(actionGenerateSliderCrank);
        menuTool->addAction(actionGenerateWattI);
        menuTool->addSeparator();
        menuTool->addAction(actionRun);
        menuTool->addAction(actionRunBackward);
        menuTool->addAction(actionStop);
        menuTool->addSeparator();
        menuTool->addAction(actionStepForward);
        menuTool->addAction(actionStepBackward);
        menuTool->addSeparator();
        menuTool->addAction(actionCollisionCheck);
        menuMode->addAction(actionSelect);
        menuMode->addAction(menuFixed_Body->menuAction());
        menuMode->addAction(menuMoving_Body->menuAction());
        menuMode->addSeparator();
        menuMode->addAction(actionLinkageRegion);
        menuMode->addAction(actionLinkageAvoidance);
        menuMode->addSeparator();
        menuMode->addAction(actionKinematics);
        menuFixed_Body->addAction(actionFixedRectangle);
        menuFixed_Body->addAction(actionFixedCircle);
        menuFixed_Body->addAction(actionFixedPolygon);
        menuMoving_Body->addAction(actionMovingRectangle);
        menuMoving_Body->addAction(actionMovingCircle);
        menuMoving_Body->addAction(actionMovingPolygon);
        menuLayer->addAction(actionAddLayer);
        menuLayer->addAction(actionInsertLayer);
        menuLayer->addAction(actionDeleteLayer);
        menuLayer->addSeparator();
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionSelectAll);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCircularRepeat);
        menuView->addAction(actionShowSolutions);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Kinematic Design 2D", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
        actionRun->setText(QApplication::translate("MainWindowClass", "Run", 0));
        actionStop->setText(QApplication::translate("MainWindowClass", "Stop", 0));
        actionStepForward->setText(QApplication::translate("MainWindowClass", "Step Forward", 0));
        actionStepForward->setShortcut(QApplication::translate("MainWindowClass", "Right", 0));
        actionStepBackward->setText(QApplication::translate("MainWindowClass", "Step Backward", 0));
        actionStepBackward->setShortcut(QApplication::translate("MainWindowClass", "Left", 0));
        actionOpen->setText(QApplication::translate("MainWindowClass", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0));
        actionShowCenterPointCurve->setText(QApplication::translate("MainWindowClass", "Show Center Point Curve", 0));
        actionShowCirclePointCurve->setText(QApplication::translate("MainWindowClass", "Show Circle Point Curve", 0));
        actionSelect->setText(QApplication::translate("MainWindowClass", "Select", 0));
        actionFixedRectangle->setText(QApplication::translate("MainWindowClass", "Rectangle", 0));
        actionFixedPolygon->setText(QApplication::translate("MainWindowClass", "Polygon", 0));
        actionLayer1->setText(QApplication::translate("MainWindowClass", "Layer 1", 0));
        actionLayer2->setText(QApplication::translate("MainWindowClass", "Layer 2", 0));
        actionNew->setText(QApplication::translate("MainWindowClass", "New", 0));
        actionNew->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+N", 0));
        actionSave->setText(QApplication::translate("MainWindowClass", "Save", 0));
        actionSave->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0));
        actionKinematics->setText(QApplication::translate("MainWindowClass", "Kinematics", 0));
        actionLayer3->setText(QApplication::translate("MainWindowClass", "Layer 3", 0));
        actionGenerate4RLinkage->setText(QApplication::translate("MainWindowClass", "Generate 4R Linkage", 0));
        actionCollisionCheck->setText(QApplication::translate("MainWindowClass", "Collision Check", 0));
        actionUndo->setText(QApplication::translate("MainWindowClass", "Undo", 0));
        actionUndo->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Z", 0));
        actionRedo->setText(QApplication::translate("MainWindowClass", "Redo", 0));
        actionRedo->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Y", 0));
        actionCopy->setText(QApplication::translate("MainWindowClass", "Copy", 0));
        actionCopy->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+C", 0));
        actionPaste->setText(QApplication::translate("MainWindowClass", "Paste", 0));
        actionPaste->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+V", 0));
        actionDelete->setText(QApplication::translate("MainWindowClass", "Delete", 0));
        actionDelete->setShortcut(QApplication::translate("MainWindowClass", "Del", 0));
        actionSelectAll->setText(QApplication::translate("MainWindowClass", "Select All", 0));
        actionSelectAll->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+A", 0));
        actionRunBackward->setText(QApplication::translate("MainWindowClass", "Run Backward", 0));
        actionFixedCircle->setText(QApplication::translate("MainWindowClass", "Circle", 0));
        actionOptions->setText(QApplication::translate("MainWindowClass", "Options", 0));
        actionLinkageRegion->setText(QApplication::translate("MainWindowClass", "Linkage Region", 0));
        actionTest->setText(QApplication::translate("MainWindowClass", "Test", 0));
        actionCircularRepeat->setText(QApplication::translate("MainWindowClass", "Circular Repeat", 0));
        actionGenerateSliderCrank->setText(QApplication::translate("MainWindowClass", "Generate Slider Crank", 0));
        actionAddLayer->setText(QApplication::translate("MainWindowClass", "Add Layer", 0));
        actionInsertLayer->setText(QApplication::translate("MainWindowClass", "Insert Layer", 0));
        actionDebug->setText(QApplication::translate("MainWindowClass", "Debug", 0));
        actionDeleteLayer->setText(QApplication::translate("MainWindowClass", "Delete Layer", 0));
        actionRectangle_2->setText(QApplication::translate("MainWindowClass", "Rectangle", 0));
        actionMovingRectangle->setText(QApplication::translate("MainWindowClass", "Rectangle", 0));
        actionMovingCircle->setText(QApplication::translate("MainWindowClass", "Circle", 0));
        actionMovingPolygon->setText(QApplication::translate("MainWindowClass", "Polygon", 0));
        actionLinkageAvoidance->setText(QApplication::translate("MainWindowClass", "Linkage Avoidance Region", 0));
        actionGenerateWattI->setText(QApplication::translate("MainWindowClass", "Generate Watt I", 0));
        actionMirror->setText(QApplication::translate("MainWindowClass", "Mirror", 0));
        actionShowSolutions->setText(QApplication::translate("MainWindowClass", "Show Solutions", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuTool->setTitle(QApplication::translate("MainWindowClass", "Kinematics", 0));
        menuMode->setTitle(QApplication::translate("MainWindowClass", "Mode", 0));
        menuFixed_Body->setTitle(QApplication::translate("MainWindowClass", "Fixed Body", 0));
        menuMoving_Body->setTitle(QApplication::translate("MainWindowClass", "Moving Body", 0));
        menuLayer->setTitle(QApplication::translate("MainWindowClass", "Layer", 0));
        menuEdit->setTitle(QApplication::translate("MainWindowClass", "Edit", 0));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
