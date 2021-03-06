#include "LinkageSynthesisOptionDialog.h"

LinkageSynthesisOptionDialog::LinkageSynthesisOptionDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	ui.checkBox4RLinkage->setChecked(true);
	ui.checkBoxSliderCrank->setChecked(true);
	ui.lineEditNumSamples->setText("10000");
	ui.lineEditStdDevPosition->setText("0.8");
	ui.lineEditStdDevOrientation->setText("0.04");
	ui.checkBoxAvoidBranchDefect->setChecked(true);
	ui.lineEditMinTransmissionAngle->setText("0.15");
	ui.lineEditPositionErrorWeight->setText("1");
	ui.lineEditLinkageLocationWeight->setText("10");
	ui.lineEditTrajectoryWeight->setText("1");
	ui.lineEditSizeWeight->setText("0.1");
	ui.lineEditNumParticles->setText("100");
	ui.lineEditNumIterations->setText("10");
	ui.checkBoxRecordFile->setChecked(false);

	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

LinkageSynthesisOptionDialog::~LinkageSynthesisOptionDialog() {
}

void LinkageSynthesisOptionDialog::onOK() {
	accept();
}

void LinkageSynthesisOptionDialog::onCancel() {
	reject();
}
