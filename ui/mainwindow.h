#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class View;

namespace Ui {
    class MainWindow;
}

/**
 * @class MainWindow
 *
 * The main graphical user interface class (GUI class) for our application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_tropism_constant_valueChanged(int value);

    void on_show_leaves_clicked();

    void on_show_flowers_clicked();

    void on_top_branch_radius_valueChanged(int value);

    void on_bottom_branch_raidus_valueChanged(int value);

    void on_monopodial_clicked();

    void on_sympodial_clicked();

    void on_ternary_clicked();

    void on_export_to_obj_clicked();

    void on_leaf_z_textChanged(const QString &arg1);

    void on_tropism_x_textChanged(const QString &arg1);

    void on_tropism_y_textChanged(const QString &arg1);

    void on_tropism_z_textChanged(const QString &arg1);

    void on_leaf_x_textChanged(const QString &arg1);

    void on_param1_textChanged(const QString &arg1);

    void on_param2_textChanged(const QString &arg1);

    void on_param3_textChanged(const QString &arg1);

    void on_param4_textChanged(const QString &arg1);

    void on_param5_textChanged(const QString &arg1);


    void on_long_leaf_clicked();

    void on_maple_leaf_clicked();


    void on_thickness_textChanged(const QString &arg1);

    void on_terminal_nodes_clicked();

    void on_random_leaf_placement_clicked();

    void on_iterations_textChanged(const QString &arg1);

private:
    // Auto-generated by Qt. DO NOT RENAME!
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
