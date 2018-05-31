/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
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
    QAction *actionGenerateLinkage;
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
    QAction *actionShowGridLines;
    QAction *actionShowInputPoses;
    QAction *actionResetPosition;
    QAction *actionSaveImage;
    QAction *actionShowLinkage;
    QAction *actionRestrictMotionRange;
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
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon);
        actionStop = new QAction(MainWindowClass);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon1);
        actionStepForward = new QAction(MainWindowClass);
        actionStepForward->setObjectName(QStringLiteral("actionStepForward"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/step_forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStepForward->setIcon(icon2);
        actionStepBackward = new QAction(MainWindowClass);
        actionStepBackward->setObjectName(QStringLiteral("actionStepBackward"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Resources/step_backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStepBackward->setIcon(icon3);
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon4;
        icon4.addFile(QStringLiteral("Resources/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon4);
        actionShowCenterPointCurve = new QAction(MainWindowClass);
        actionShowCenterPointCurve->setObjectName(QStringLiteral("actionShowCenterPointCurve"));
        actionShowCenterPointCurve->setCheckable(true);
        actionShowCirclePointCurve = new QAction(MainWindowClass);
        actionShowCirclePointCurve->setObjectName(QStringLiteral("actionShowCirclePointCurve"));
        actionShowCirclePointCurve->setCheckable(true);
        actionSelect = new QAction(MainWindowClass);
        actionSelect->setObjectName(QStringLiteral("actionSelect"));
        actionSelect->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral("Resources/select.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect->setIcon(icon5);
        actionFixedRectangle = new QAction(MainWindowClass);
        actionFixedRectangle->setObjectName(QStringLiteral("actionFixedRectangle"));
        actionFixedRectangle->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral("Resources/fixed_rectangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFixedRectangle->setIcon(icon6);
        actionFixedPolygon = new QAction(MainWindowClass);
        actionFixedPolygon->setObjectName(QStringLiteral("actionFixedPolygon"));
        actionFixedPolygon->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral("Resources/fixed_polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFixedPolygon->setIcon(icon7);
        actionLayer1 = new QAction(MainWindowClass);
        actionLayer1->setObjectName(QStringLiteral("actionLayer1"));
        actionLayer1->setCheckable(true);
        actionLayer2 = new QAction(MainWindowClass);
        actionLayer2->setObjectName(QStringLiteral("actionLayer2"));
        actionLayer2->setCheckable(true);
        actionNew = new QAction(MainWindowClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon8;
        icon8.addFile(QStringLiteral("Resources/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon8);
        actionSave = new QAction(MainWindowClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon9;
        icon9.addFile(QStringLiteral("Resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon9);
        actionKinematics = new QAction(MainWindowClass);
        actionKinematics->setObjectName(QStringLiteral("actionKinematics"));
        actionKinematics->setCheckable(true);
        actionLayer3 = new QAction(MainWindowClass);
        actionLayer3->setObjectName(QStringLiteral("actionLayer3"));
        actionLayer3->setCheckable(true);
        actionGenerateLinkage = new QAction(MainWindowClass);
        actionGenerateLinkage->setObjectName(QStringLiteral("actionGenerateLinkage"));
        QIcon icon10;
        icon10.addFile(QStringLiteral("Resources/fourbar_linkage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerateLinkage->setIcon(icon10);
        actionCollisionCheck = new QAction(MainWindowClass);
        actionCollisionCheck->setObjectName(QStringLiteral("actionCollisionCheck"));
        actionCollisionCheck->setCheckable(true);
        actionUndo = new QAction(MainWindowClass);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        QIcon icon11;
        icon11.addFile(QStringLiteral("Resources/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon11);
        actionRedo = new QAction(MainWindowClass);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        QIcon icon12;
        icon12.addFile(QStringLiteral("Resources/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon12);
        actionCopy = new QAction(MainWindowClass);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        QIcon icon13;
        icon13.addFile(QStringLiteral("Resources/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon13);
        actionPaste = new QAction(MainWindowClass);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        QIcon icon14;
        icon14.addFile(QStringLiteral("Resources/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon14);
        actionDelete = new QAction(MainWindowClass);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        QIcon icon15;
        icon15.addFile(QStringLiteral("Resources/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon15);
        actionSelectAll = new QAction(MainWindowClass);
        actionSelectAll->setObjectName(QStringLiteral("actionSelectAll"));
        actionRunBackward = new QAction(MainWindowClass);
        actionRunBackward->setObjectName(QStringLiteral("actionRunBackward"));
        actionFixedCircle = new QAction(MainWindowClass);
        actionFixedCircle->setObjectName(QStringLiteral("actionFixedCircle"));
        actionFixedCircle->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QStringLiteral("Resources/fixed_circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFixedCircle->setIcon(icon16);
        actionOptions = new QAction(MainWindowClass);
        actionOptions->setObjectName(QStringLiteral("actionOptions"));
        actionLinkageRegion = new QAction(MainWindowClass);
        actionLinkageRegion->setObjectName(QStringLiteral("actionLinkageRegion"));
        actionLinkageRegion->setCheckable(true);
        QIcon icon17;
        icon17.addFile(QStringLiteral("Resources/linkage_region.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLinkageRegion->setIcon(icon17);
        actionTest = new QAction(MainWindowClass);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        actionCircularRepeat = new QAction(MainWindowClass);
        actionCircularRepeat->setObjectName(QStringLiteral("actionCircularRepeat"));
        actionGenerateSliderCrank = new QAction(MainWindowClass);
        actionGenerateSliderCrank->setObjectName(QStringLiteral("actionGenerateSliderCrank"));
        QIcon icon18;
        icon18.addFile(QStringLiteral("Resources/slider_crank.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerateSliderCrank->setIcon(icon18);
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
        QIcon icon19;
        icon19.addFile(QStringLiteral("Resources/moving_rectangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMovingRectangle->setIcon(icon19);
        actionMovingCircle = new QAction(MainWindowClass);
        actionMovingCircle->setObjectName(QStringLiteral("actionMovingCircle"));
        actionMovingCircle->setCheckable(true);
        QIcon icon20;
        icon20.addFile(QStringLiteral("Resources/moving_circle.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMovingCircle->setIcon(icon20);
        actionMovingPolygon = new QAction(MainWindowClass);
        actionMovingPolygon->setObjectName(QStringLiteral("actionMovingPolygon"));
        actionMovingPolygon->setCheckable(true);
        QIcon icon21;
        icon21.addFile(QStringLiteral("Resources/moving_polygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMovingPolygon->setIcon(icon21);
        actionLinkageAvoidance = new QAction(MainWindowClass);
        actionLinkageAvoidance->setObjectName(QStringLiteral("actionLinkageAvoidance"));
        actionLinkageAvoidance->setCheckable(true);
        QIcon icon22;
        icon22.addFile(QStringLiteral("Resources/linkage_avoidance.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLinkageAvoidance->setIcon(icon22);
        actionGenerateWattI = new QAction(MainWindowClass);
        actionGenerateWattI->setObjectName(QStringLiteral("actionGenerateWattI"));
        QIcon icon23;
        icon23.addFile(QStringLiteral("Resources/watt_i_linkage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerateWattI->setIcon(icon23);
        actionMirror = new QAction(MainWindowClass);
        actionMirror->setObjectName(QStringLiteral("actionMirror"));
        actionShowSolutions = new QAction(MainWindowClass);
        actionShowSolutions->setObjectName(QStringLiteral("actionShowSolutions"));
        actionShowSolutions->setCheckable(true);
        actionShowGridLines = new QAction(MainWindowClass);
        actionShowGridLines->setObjectName(QStringLiteral("actionShowGridLines"));
        actionShowGridLines->setCheckable(true);
        actionShowGridLines->setChecked(true);
        actionShowInputPoses = new QAction(MainWindowClass);
        actionShowInputPoses->setObjectName(QStringLiteral("actionShowInputPoses"));
        actionShowInputPoses->setCheckable(true);
        actionShowInputPoses->setChecked(true);
        actionResetPosition = new QAction(MainWindowClass);
        actionResetPosition->setObjectName(QStringLiteral("actionResetPosition"));
        QIcon icon24;
        icon24.addFile(QStringLiteral("Resources/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionResetPosition->setIcon(icon24);
        actionSaveImage = new QAction(MainWindowClass);
        actionSaveImage->setObjectName(QStringLiteral("actionSaveImage"));
        actionShowLinkage = new QAction(MainWindowClass);
        actionShowLinkage->setObjectName(QStringLiteral("actionShowLinkage"));
        actionShowLinkage->setCheckable(true);
        actionShowLinkage->setChecked(true);
        actionRestrictMotionRange = new QAction(MainWindowClass);
        actionRestrictMotionRange->setObjectName(QStringLiteral("actionRestrictMotionRange"));
        actionRestrictMotionRange->setCheckable(true);
        actionRestrictMotionRange->setChecked(true);
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
        menuFile->addAction(actionSaveImage);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTool->addAction(actionGenerateLinkage);
        menuTool->addSeparator();
        menuTool->addAction(actionRun);
        menuTool->addAction(actionRunBackward);
        menuTool->addAction(actionStop);
        menuTool->addAction(actionResetPosition);
        menuTool->addSeparator();
        menuTool->addAction(actionStepForward);
        menuTool->addAction(actionStepBackward);
        menuTool->addSeparator();
        menuTool->addAction(actionCollisionCheck);
        menuTool->addAction(actionRestrictMotionRange);
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
        menuView->addAction(actionShowSolutions);
        menuView->addAction(actionShowGridLines);
        menuView->addAction(actionShowInputPoses);
        menuView->addAction(actionShowLinkage);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Kinematic Design 2D", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", Q_NULLPTR));
        actionRun->setText(QApplication::translate("MainWindowClass", "Run", Q_NULLPTR));
        actionStop->setText(QApplication::translate("MainWindowClass", "Stop", Q_NULLPTR));
        actionStepForward->setText(QApplication::translate("MainWindowClass", "Step Forward", Q_NULLPTR));
        actionStepForward->setShortcut(QApplication::translate("MainWindowClass", "Right", Q_NULLPTR));
        actionStepBackward->setText(QApplication::translate("MainWindowClass", "Step Backward", Q_NULLPTR));
        actionStepBackward->setShortcut(QApplication::translate("MainWindowClass", "Left", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindowClass", "Open", Q_NULLPTR));
        actionOpen->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", Q_NULLPTR));
        actionShowCenterPointCurve->setText(QApplication::translate("MainWindowClass", "Show Center Point Curve", Q_NULLPTR));
        actionShowCirclePointCurve->setText(QApplication::translate("MainWindowClass", "Show Circle Point Curve", Q_NULLPTR));
        actionSelect->setText(QApplication::translate("MainWindowClass", "Select", Q_NULLPTR));
        actionFixedRectangle->setText(QApplication::translate("MainWindowClass", "Rectangle", Q_NULLPTR));
        actionFixedPolygon->setText(QApplication::translate("MainWindowClass", "Polygon", Q_NULLPTR));
        actionLayer1->setText(QApplication::translate("MainWindowClass", "Layer 1", Q_NULLPTR));
        actionLayer2->setText(QApplication::translate("MainWindowClass", "Layer 2", Q_NULLPTR));
        actionNew->setText(QApplication::translate("MainWindowClass", "New", Q_NULLPTR));
        actionNew->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+N", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindowClass", "Save", Q_NULLPTR));
        actionSave->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", Q_NULLPTR));
        actionKinematics->setText(QApplication::translate("MainWindowClass", "Kinematics", Q_NULLPTR));
        actionLayer3->setText(QApplication::translate("MainWindowClass", "Layer 3", Q_NULLPTR));
        actionGenerateLinkage->setText(QApplication::translate("MainWindowClass", "Generate Linkage", Q_NULLPTR));
        actionCollisionCheck->setText(QApplication::translate("MainWindowClass", "Collision Check", Q_NULLPTR));
        actionUndo->setText(QApplication::translate("MainWindowClass", "Undo", Q_NULLPTR));
        actionUndo->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Z", Q_NULLPTR));
        actionRedo->setText(QApplication::translate("MainWindowClass", "Redo", Q_NULLPTR));
        actionRedo->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Y", Q_NULLPTR));
        actionCopy->setText(QApplication::translate("MainWindowClass", "Copy", Q_NULLPTR));
        actionCopy->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+C", Q_NULLPTR));
        actionPaste->setText(QApplication::translate("MainWindowClass", "Paste", Q_NULLPTR));
        actionPaste->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+V", Q_NULLPTR));
        actionDelete->setText(QApplication::translate("MainWindowClass", "Delete", Q_NULLPTR));
        actionDelete->setShortcut(QApplication::translate("MainWindowClass", "Del", Q_NULLPTR));
        actionSelectAll->setText(QApplication::translate("MainWindowClass", "Select All", Q_NULLPTR));
        actionSelectAll->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+A", Q_NULLPTR));
        actionRunBackward->setText(QApplication::translate("MainWindowClass", "Run Backward", Q_NULLPTR));
        actionFixedCircle->setText(QApplication::translate("MainWindowClass", "Circle", Q_NULLPTR));
        actionOptions->setText(QApplication::translate("MainWindowClass", "Options", Q_NULLPTR));
        actionLinkageRegion->setText(QApplication::translate("MainWindowClass", "Linkage Region", Q_NULLPTR));
        actionTest->setText(QApplication::translate("MainWindowClass", "Test", Q_NULLPTR));
        actionCircularRepeat->setText(QApplication::translate("MainWindowClass", "Circular Repeat", Q_NULLPTR));
        actionGenerateSliderCrank->setText(QApplication::translate("MainWindowClass", "Generate Slider Crank", Q_NULLPTR));
        actionAddLayer->setText(QApplication::translate("MainWindowClass", "Add Layer", Q_NULLPTR));
        actionInsertLayer->setText(QApplication::translate("MainWindowClass", "Insert Layer", Q_NULLPTR));
        actionDebug->setText(QApplication::translate("MainWindowClass", "Debug", Q_NULLPTR));
        actionDeleteLayer->setText(QApplication::translate("MainWindowClass", "Delete Layer", Q_NULLPTR));
        actionRectangle_2->setText(QApplication::translate("MainWindowClass", "Rectangle", Q_NULLPTR));
        actionMovingRectangle->setText(QApplication::translate("MainWindowClass", "Rectangle", Q_NULLPTR));
        actionMovingCircle->setText(QApplication::translate("MainWindowClass", "Circle", Q_NULLPTR));
        actionMovingPolygon->setText(QApplication::translate("MainWindowClass", "Polygon", Q_NULLPTR));
        actionLinkageAvoidance->setText(QApplication::translate("MainWindowClass", "Linkage Avoidance Region", Q_NULLPTR));
        actionGenerateWattI->setText(QApplication::translate("MainWindowClass", "Generate Watt I", Q_NULLPTR));
        actionMirror->setText(QApplication::translate("MainWindowClass", "Mirror", Q_NULLPTR));
        actionShowSolutions->setText(QApplication::translate("MainWindowClass", "Show Solutions", Q_NULLPTR));
        actionShowGridLines->setText(QApplication::translate("MainWindowClass", "Show Grid Lines", Q_NULLPTR));
        actionShowInputPoses->setText(QApplication::translate("MainWindowClass", "Show Input Poses", Q_NULLPTR));
        actionResetPosition->setText(QApplication::translate("MainWindowClass", "Reset Position", Q_NULLPTR));
        actionSaveImage->setText(QApplication::translate("MainWindowClass", "Save Image", Q_NULLPTR));
        actionSaveImage->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+P", Q_NULLPTR));
        actionShowLinkage->setText(QApplication::translate("MainWindowClass", "Show Linkage", Q_NULLPTR));
        actionRestrictMotionRange->setText(QApplication::translate("MainWindowClass", "Restrict Motion Range", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", Q_NULLPTR));
        menuTool->setTitle(QApplication::translate("MainWindowClass", "Kinematics", Q_NULLPTR));
        menuMode->setTitle(QApplication::translate("MainWindowClass", "Mode", Q_NULLPTR));
        menuFixed_Body->setTitle(QApplication::translate("MainWindowClass", "Fixed Body", Q_NULLPTR));
        menuMoving_Body->setTitle(QApplication::translate("MainWindowClass", "Moving Body", Q_NULLPTR));
        menuLayer->setTitle(QApplication::translate("MainWindowClass", "Layer", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindowClass", "Edit", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
