#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include <QGLFormat>
#include <QSettings>
#include <Settings.h>
#include <QKeyEvent>
#include <iostream>
#include "gl/GLDebug.h"
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up settings
    settings.cameraFar = 100.0f;
    settings.cameraNear = .1f;
    settings.cameraFov = .5;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tropism_constant_valueChanged(int value)
{
    settings.tropism_constant = value / 100.0;
    std::string str = std::to_string(settings.tropism_constant);
    ui->tropism_constant_val->setText(QString(str.substr(0,std::min<int>(4, str.size())).c_str()));
}

void MainWindow::on_show_leaves_clicked()
{
    settings.use_leaves = ui->show_leaves->isChecked();
}

void MainWindow::on_show_flowers_clicked()
{
    settings.use_flowers = ui->show_flowers->isChecked();
}

void MainWindow::on_monopodial_clicked()
{
    settings.changed_treetype = true;
    settings.treetype = TT_MONOPODIAL;
    ui->param1_label->setText("Trunk Contraction Ratio");
    ui->param2_label->setText("Branch Contraction Ratio");
    ui->param3_label->setText("Branch Angle From Trunk");
    ui->param4_label->setText("Lateral Branching Angle");
    ui->param5_label->setText("Divergence Angle");
    ui->param1->setText(".9");
    ui->param2->setText(".6");
    ui->param3->setText("45");
    ui->param4->setText("45");
    ui->param5->setText("137.5");
    ui->iterations->setText("10");
    settings.param_a = .9;
    settings.param_b = .6;
    settings.param_c = 45;
    settings.param_d = 45;
    settings.param_e = 137.5;
    settings.iterations = 10;
    settings.changed_treetype = false;
}

void MainWindow::on_sympodial_clicked()
{
    settings.changed_treetype = true;
    settings.treetype = TT_SYMPODIAL;
    ui->param1_label->setText("Contraction Ratio 1");
    ui->param2_label->setText("Contraction Ratio 2");
    ui->param3_label->setText("Branching Angle 1");
    ui->param4_label->setText("Branching Angle 2");
    ui->param5_label->setText("Width Decrease Rate");
    ui->param1->setText(".9");
    ui->param2->setText(".8");
    ui->param3->setText("35");
    ui->param4->setText("35");
    ui->param5->setText(".707");
    ui->iterations->setText("10");
    settings.param_a = .9;
    settings.param_b = .8;
    settings.param_c = 35;
    settings.param_d = 35;
    settings.param_e = .707;
    settings.iterations = 10;
    settings.changed_treetype = false;
}

void MainWindow::on_ternary_clicked()
{
    settings.changed_treetype = true;
    settings.treetype = TT_TERNARY;
    ui->param1_label->setText("Divergence Angle 1");
    ui->param2_label->setText("Divergence Angle 2");
    ui->param3_label->setText("Branching Angle");
    ui->param4_label->setText("Elongation Rate");
    ui->param5_label->setText("Width Increase Rate");
    ui->param1->setText("94.74");
    ui->param2->setText("132.63");
    ui->param3->setText("18.95");
    ui->param4->setText("1.109");
    ui->param5->setText(".8");
    ui->iterations->setText("7");
    settings.param_a = 94.74;
    settings.param_b = 132.63;
    settings.param_c = 18.95;
    settings.param_d = 1.109;
    settings.param_e = .8;
    settings.iterations = 7;
    settings.changed_treetype = false;
}

void MainWindow::on_export_to_obj_clicked()
{
    ui->view->saveMeshToFile();
}

void MainWindow::on_tropism_x_textChanged(const QString &arg1)
{
    settings.tropism_vector.x = std::strtof(arg1.toStdString().c_str(), nullptr);
}

void MainWindow::on_tropism_y_textChanged(const QString &arg1)
{
    settings.tropism_vector.y = std::strtof(arg1.toStdString().c_str(), nullptr);
}

void MainWindow::on_tropism_z_textChanged(const QString &arg1)
{
    settings.tropism_vector.z = std::strtof(arg1.toStdString().c_str(), nullptr);
}


void MainWindow::on_param1_textChanged(const QString &arg1)
{
    settings.param_a = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
    if (!settings.changed_treetype) {
    }
}

void MainWindow::on_param2_textChanged(const QString &arg1)
{
    settings.param_b = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
    if (!settings.changed_treetype) {
    }
}

void MainWindow::on_param3_textChanged(const QString &arg1)
{
    settings.param_c = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
    if (!settings.changed_treetype) {
    }
}


void MainWindow::on_param4_textChanged(const QString &arg1)
{
    settings.param_d = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
    if (!settings.changed_treetype) {
    }
}

void MainWindow::on_param5_textChanged(const QString &arg1)
{
    settings.param_e = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
    if (!settings.changed_treetype) {
    }
}

void MainWindow::on_iterations_textChanged(const QString &arg1)
{
    int it = settings.iterations;
    try {
        it = std::stoi(arg1.toStdString().c_str(), nullptr);
    } catch (const std::invalid_argument& e) {

    }
    settings.iterations = glm::min(it,10);
    if (!settings.changed_treetype) {
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->view->settingsChanged();
}


void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    settings.prob_of_flower_vs_leaf = value / 100.0;
    std::string str = std::to_string(settings.prob_of_flower_vs_leaf);
    ui->prob->setText(QString(str.substr(0,std::min<int>(4, str.size())).c_str()));
}

void MainWindow::on_leaf_size_textChanged(const QString &arg1)
{
    settings.leaf_size = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
}

void MainWindow::on_flower_size_textChanged(const QString &arg1)
{
    settings.flower_size = std::max<float>(0,std::strtof(arg1.toStdString().c_str(), nullptr));
}
