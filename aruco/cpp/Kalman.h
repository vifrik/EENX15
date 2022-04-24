/**
* Implementation of KalmanFilter class.
*
* @author: Hayk Martirosyan
* @date: 2014.11.15
*/

#ifndef ARUCOCPP_KALMAN_H
#define ARUCOCPP_KALMAN_H

#include <Eigen/Dense>

class Kalman {
public:
    Kalman(
            double dt,
            const Eigen::MatrixXd &A,
            const Eigen::MatrixXd &C,
            const Eigen::MatrixXd &Q,
            const Eigen::MatrixXd &R,
            const Eigen::MatrixXd &P
    );

    Kalman();

    void init();

    void init(double t0, const Eigen::VectorXd &x0);

    void update(const Eigen::VectorXd &y);

    void update(const Eigen::VectorXd &y, double dt, const Eigen::MatrixXd A);

    Eigen::VectorXd state() { return x_hat; };

    double time() { return t; };

private:
    Eigen::MatrixXd A, C, Q, R, P, K, P0;
    int m, n;
    double t0, t;
    double dt;
    bool initialized;
    Eigen::MatrixXd I;
    Eigen::VectorXd x_hat, x_hat_new;
};

#endif //ARUCOCPP_KALMAN_H
