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
    ui->top_radius_top->setText(QString("0.50"));
    ui->botto_raidus_val->setText(QString("0.50"));

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
    ui->view->settingsChanged();
}

void MainWindow::on_show_leaves_clicked()
{
    settings.use_leaves = ui->show_leaves->isChecked();
    ui->view->settingsChanged();
}

void MainWindow::on_show_flowers_clicked()
{
    settings.use_flowers = ui->show_flowers->isChecked();
    ui->view->settingsChanged();
}

void MainWindow::on_top_branch_radius_valueChanged(int value)
{
    settings.branch_top_radius = value / 100.0;
    std::string str = std::to_string(settings.branch_top_radius);
    ui->top_radius_top->setText(QString(str.substr(0,std::min<int>(4, str.size())).c_str()));
    ui->view->settingsChanged();
}

void MainWindow::on_bottom_branch_raidus_valueChanged(int value)
{
    //done
    settings.branch_base_radius = value / 100.0;
    std::string str = std::to_string(settings.branch_base_radius);
    ui->botto_raidus_val->setText(QString(str.substr(0,std::min<int>(4, str.size())).c_str()));
    ui->view->settingsChanged();
}

void MainWindow::on_monopodial_clicked()
{
    //done
    settings.treetype = TT_MONOPODIAL;
    ui->param1_label->setText("Trunk Contraction Ratio");
    ui->param2_label->setText("Branch Contraction Ratio");
    ui->param3_label->setText("Branch Angle From Trunk");
    ui->param4_label->setText("Lateral Branching Angle");
    ui->param5_label->setText("Divergence Angle");
    ui->view->settingsChanged();
}

void MainWindow::on_sympodial_clicked()
{
    //done
    settings.treetype = TT_SYMPODIAL;
    ui->param1_label->setText("Contraction Ratio 1");
    ui->param2_label->setText("Contraction Ratio 2");
    ui->param3_label->setText("Branching Angle 1");
    ui->param4_label->setText("Branching Angle 2");
    ui->param5_label->setText("Width Decrease Rate");
    ui->view->settingsChanged();
}

void MainWindow::on_ternary_clicked()
{
    //done
    settings.treetype = TT_TERNARY;
    ui->param1_label->setText("Divergence Angle 1");
    ui->param2_label->setText("Divergence Angke 2");
    ui->param3_label->setText("Branching Angle");
    ui->param4_label->setText("Elongation Rate");
    ui->param5_label->setText("Width Increase Rate");
    ui->view->settingsChanged();
}

void MainWindow::on_export_to_obj_clicked()
{

}

void MainWindow::on_tropism_x_textChanged(const QString &arg1)
{
    settings.tropism_vector.x = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_tropism_y_textChanged(const QString &arg1)
{
    settings.tropism_vector.y = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_tropism_z_textChanged(const QString &arg1)
{
    settings.tropism_vector.z = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_leaf_x_textChanged(const QString &arg1)
{
    settings.leaf_size.x = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_leaf_y_textChanged(const QString &arg1)
{
    settings.leaf_size.y = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_leaf_z_textChanged(const QString &arg1)
{
    settings.leaf_size.z = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}


void MainWindow::on_param1_textChanged(const QString &arg1)
{
    settings.param_a = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_param2_textChanged(const QString &arg1)
{
    settings.param_b = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_param3_textChanged(const QString &arg1)
{
    settings.param_c = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}


void MainWindow::on_param4_textChanged(const QString &arg1)
{
    settings.param_d = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}

void MainWindow::on_param5_textChanged(const QString &arg1)
{
    settings.param_e = std::strtof(arg1.toStdString().c_str(), nullptr);
    ui->view->settingsChanged();
}


void MainWindow::on_long_leaf_clicked()
{
    settings.leaftype = LeafType::LT_LONG;
    ui->view->settingsChanged();
}

void MainWindow::on_maple_leaf_clicked()
{
    settings.leaftype = LeafType::LT_MAPLE;
    ui->view->settingsChanged();
}
