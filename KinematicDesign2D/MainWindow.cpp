#include "MainWindow.h"
#include <QFileDialog>
#include "LinkageSynthesisOptionDialog.h"
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	canvas = new canvas::Canvas(this);
	setCentralWidget(canvas);

	QActionGroup* groupMode = new QActionGroup(this);
	groupMode->addAction(ui.actionSelect);
	groupMode->addAction(ui.actionFixedRectangle);
	groupMode->addAction(ui.actionFixedCircle);
	groupMode->addAction(ui.actionFixedPolygon);
	groupMode->addAction(ui.actionMovingRectangle);
	groupMode->addAction(ui.actionMovingCircle);
	groupMode->addAction(ui.actionMovingPolygon);
	groupMode->addAction(ui.actionLinkageRegion);
	groupMode->addAction(ui.actionLinkageAvoidanceRegion);
	groupMode->addAction(ui.actionKinematics);
	ui.actionSelect->setChecked(true);
	
	groupLayer = new QActionGroup(this);
	initLayerMenu(2);

	ui.actionCollisionCheck->setChecked(canvas->collision_check);

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(onNew()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionUndo, SIGNAL(triggered()), this, SLOT(onUndo()));
	connect(ui.actionRedo, SIGNAL(triggered()), this, SLOT(onRedo()));
	connect(ui.actionCopy, SIGNAL(triggered()), this, SLOT(onCopy()));
	connect(ui.actionPaste, SIGNAL(triggered()), this, SLOT(onPaste()));
	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(onDelete()));
	connect(ui.actionSelectAll, SIGNAL(triggered()), this, SLOT(onSelectAll()));
	connect(ui.actionCircularRepeat, SIGNAL(triggered()), this, SLOT(onCircularRepeat()));
	connect(ui.actionSelect, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionFixedRectangle, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionFixedCircle, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionFixedPolygon, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionMovingRectangle, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionMovingCircle, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionMovingPolygon, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionLinkageRegion, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionLinkageAvoidanceRegion, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionKinematics, SIGNAL(triggered()), this, SLOT(onModeChanged()));
	connect(ui.actionAddLayer, SIGNAL(triggered()), this, SLOT(onAddLayer()));
	connect(ui.actionInsertLayer, SIGNAL(triggered()), this, SLOT(onInsertLayer()));
	connect(ui.actionDeleteLayer, SIGNAL(triggered()), this, SLOT(onDeleteLayer()));
	connect(ui.actionCalculateSolution4RLinkage, SIGNAL(triggered()), this, SLOT(onCalculateSolution4RLinkage()));
	connect(ui.actionCalculateSolutionSliderCrank, SIGNAL(triggered()), this, SLOT(onCalculateSolutionSliderCrank()));
	connect(ui.actionRun, SIGNAL(triggered()), this, SLOT(onRun()));
	connect(ui.actionRunBackward, SIGNAL(triggered()), this, SLOT(onRunBackward()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(onStop()));
	connect(ui.actionStepForward, SIGNAL(triggered()), this, SLOT(onStepForward()));
	connect(ui.actionStepBackward, SIGNAL(triggered()), this, SLOT(onStepBackward()));
	connect(ui.actionCollisionCheck, SIGNAL(triggered()), this, SLOT(onCollisionCheck()));
}

MainWindow::~MainWindow() {
}

void MainWindow::initLayerMenu(int num_layers) {
	for (int i = 0; i < menuLayers.size(); i++) {
		disconnect(menuLayers[i], SIGNAL(triggered()), this, SLOT(onLayerChanged()));
		ui.menuLayer->removeAction(menuLayers[i]);
		groupLayer->removeAction(menuLayers[i]);
		delete menuLayers[i];
	}
	menuLayers.clear();

	for (int i = 0; i < num_layers; i++) {
		menuLayers.push_back(ui.menuLayer->addAction(QString("Layer %1").arg(i + 1)));
		menuLayers[i]->setCheckable(true);
		groupLayer->addAction(menuLayers[i]);
		connect(menuLayers[i], SIGNAL(triggered()), this, SLOT(onLayerChanged()));
	}
	menuLayers[0]->setChecked(true);
}

void MainWindow::onNew() {
	canvas->clear();
	setWindowTitle("Kinematic Design 2D");
}

void MainWindow::onOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open design file..."), "", tr("Design files (*.xml)"));
	if (filename.isEmpty()) return;

	canvas->open(filename);
	setWindowTitle("Kinematic Design 2D - " + QFileInfo(filename).fileName());
}

void MainWindow::onSave() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save design file..."), "", tr("Design files (*.xml)"));
	if (filename.isEmpty()) return;

	canvas->save(filename);
	setWindowTitle("Kinematic Design 2D - " + QFileInfo(filename).fileName());
}

void MainWindow::onUndo() {
	canvas->undo();
}

void MainWindow::onRedo() {
	canvas->redo();
}

void MainWindow::onCopy() {
	canvas->copySelectedShapes();
}

void MainWindow::onPaste() {
	canvas->pasteCopiedShapes();
}

void MainWindow::onDelete() {
	canvas->deleteSelectedShapes();
}

void MainWindow::onSelectAll() {
	canvas->selectAll();
}

void MainWindow::onCircularRepeat() {
	canvas->circularRepeat(8);
}

void MainWindow::onModeChanged() {
	if (ui.actionSelect->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_SELECT);
	}
	else if (ui.actionFixedRectangle->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_FIXED_RECTANGLE);
	}
	else if (ui.actionFixedCircle->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_FIXED_CIRCLE);
	}
	else if (ui.actionFixedPolygon->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_FIXED_POLYGON);
	}
	else if (ui.actionMovingRectangle->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_MOVING_RECTANGLE);
	}
	else if (ui.actionMovingCircle->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_MOVING_CIRCLE);
	}
	else if (ui.actionMovingPolygon->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_MOVING_POLYGON);
	}
	else if (ui.actionLinkageRegion->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_LINKAGE_REGION);
	}
	else if (ui.actionLinkageAvoidanceRegion->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_LINKAGE_AVOIDANCE);
	}
	else if (ui.actionKinematics->isChecked()) {
		canvas->setMode(canvas::Canvas::MODE_KINEMATICS);
	}
	update();
}

void MainWindow::onAddLayer() {
	menuLayers.push_back(ui.menuLayer->addAction(QString("Layer %1").arg(menuLayers.size() + 1)));
	menuLayers.back()->setCheckable(true);
	groupLayer->addAction(menuLayers.back());
	menuLayers.back()->setChecked(true);
	connect(menuLayers.back(), SIGNAL(triggered()), this, SLOT(onLayerChanged()));

	canvas->addLayer();
}

void MainWindow::onInsertLayer() {
	menuLayers.push_back(ui.menuLayer->addAction(QString("Layer %1").arg(menuLayers.size() + 1)));
	menuLayers.back()->setCheckable(true);
	groupLayer->addAction(menuLayers.back());
	connect(menuLayers.back(), SIGNAL(triggered()), this, SLOT(onLayerChanged()));

	canvas->insertLayer();
}

void MainWindow::onDeleteLayer() {
	disconnect(menuLayers.back(), SIGNAL(triggered()), this, SLOT(onLayerChanged()));
	ui.menuLayer->removeAction(menuLayers.back());
	groupLayer->removeAction(menuLayers.back());
	delete menuLayers.back();
	menuLayers.resize(menuLayers.size() - 1);

	canvas->deleteLayer();
}

void MainWindow::onLayerChanged() {
	for (int i = 0; i < menuLayers.size(); i++) {
		if (menuLayers[i]->isChecked()) {
			canvas->setLayer(i);
			break;
		}
	}
}

void MainWindow::onCalculateSolution4RLinkage() {
	LinkageSynthesisOptionDialog dlg;
	if (dlg.exec()) {
		std::vector<std::pair<double, double>> sigmas = {
			std::make_pair(dlg.ui.lineEditStdDevPositionFirst->text().toDouble(), dlg.ui.lineEditStdDevOrientationFirst->text().toDouble()),
			std::make_pair(dlg.ui.lineEditStdDevPositionMiddle->text().toDouble(), dlg.ui.lineEditStdDevOrientationMiddle->text().toDouble()),
			std::make_pair(dlg.ui.lineEditStdDevPositionLast->text().toDouble(), dlg.ui.lineEditStdDevOrientationLast->text().toDouble())
		};

		std::vector<double> weights = {
			dlg.ui.lineEditPositionErrorWeight->text().toDouble(),
			dlg.ui.lineEditOrientationErrorWeight->text().toDouble(),
			dlg.ui.lineEditLinkageLocationWeight->text().toDouble(),
			dlg.ui.lineEditTrajectoryWeight->text().toDouble(),
			dlg.ui.lineEditSizeWeight->text().toDouble()
		};

		canvas->calculateSolutions(canvas::Canvas::LINKAGE_4R, 
			dlg.ui.lineEditNumSamples->text().toInt(),
			sigmas,
			dlg.ui.checkBoxAvoidBranchDefect->isChecked(),
			dlg.ui.checkBoxRotatableCrank->isChecked(),
			weights,
			dlg.ui.lineEditNumParticles->text().toInt(),
			dlg.ui.lineEditNumIterations->text().toInt(),
			dlg.ui.checkBoxRecordFile->isChecked());
	}
}

void MainWindow::onCalculateSolutionSliderCrank() {
	LinkageSynthesisOptionDialog dlg;
	if (dlg.exec()) {
		std::vector<std::pair<double, double>> sigmas = {
			std::make_pair(dlg.ui.lineEditStdDevPositionFirst->text().toDouble(), dlg.ui.lineEditStdDevOrientationFirst->text().toDouble()),
			std::make_pair(dlg.ui.lineEditStdDevPositionMiddle->text().toDouble(), dlg.ui.lineEditStdDevOrientationMiddle->text().toDouble()),
			std::make_pair(dlg.ui.lineEditStdDevPositionLast->text().toDouble(), dlg.ui.lineEditStdDevOrientationLast->text().toDouble())
		};

		std::vector<double> weights = {
			dlg.ui.lineEditPositionErrorWeight->text().toDouble(),
			dlg.ui.lineEditOrientationErrorWeight->text().toDouble(),
			dlg.ui.lineEditLinkageLocationWeight->text().toDouble(),
			dlg.ui.lineEditTrajectoryWeight->text().toDouble(),
			dlg.ui.lineEditSizeWeight->text().toDouble()
		};

		canvas->calculateSolutions(canvas::Canvas::LINKAGE_RRRP,
			dlg.ui.lineEditNumSamples->text().toInt(), 
			sigmas,
			dlg.ui.checkBoxAvoidBranchDefect->isChecked(),
			dlg.ui.checkBoxRotatableCrank->isChecked(),
			weights,
			dlg.ui.lineEditNumParticles->text().toInt(),
			dlg.ui.lineEditNumIterations->text().toInt(),
			dlg.ui.checkBoxRecordFile->isChecked());
	}
}

void MainWindow::onRun() {
	canvas->run();
}

void MainWindow::onRunBackward() {
	canvas->invertSpeed();
	canvas->run();
}

void MainWindow::onStop() {
	canvas->stop();
}

void MainWindow::onStepForward() {
	canvas->stepForward();
}

void MainWindow::onStepBackward() {
	canvas->stepBackward();
}

void MainWindow::onCollisionCheck() {
	canvas->collision_check = ui.actionCollisionCheck->isChecked();
}

void MainWindow::keyPressEvent(QKeyEvent* e) {
	canvas->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	canvas->keyReleaseEvent(e);
}

