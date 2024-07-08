// Store the W matrix coefficients for multiple use
#pragma once

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Eigenvalues>

#ifndef W_MATRIX_H_
#define W_MATRIX_H_

class W_matrix{
    private:
    public:
        //Parameteres
        double T,V,F,R,FV_RT;

        // Initial transition rates
        double k_1, k_2fv0, k_2rv0, ko_dN1v0, ko_bN1v0, \
            ko_dNv0, ko_bNv0, ko_dKv0, ko_bKv0, k_31,   \
            k_32, k_4f, k_4r, ki_dN1v0, ki_bN1v0,       \
            ki_dN, ki_bN, ki_dK, ki_bk;

        // Transition rates taking electrical potential into account
        double k_2f,k_2r,ko_dN1,ko_bN1,ko_dN,ko_bN,ko_dK,ko_bK,ki_dN1,ki_bN1;

        // Ion concentrations (mMol)
        double c_Na_out, c_Na_in, c_K_out, c_K_in;

        //W transition matrix
        Eigen::MatrixXd W = Eigen::MatrixXd::Zero(19,19);

        W_matrix(double T, double V, double F, double R, double FV_RT, double  k_1, \
                 double  k_2fv0, double  k_2rv0, double  ko_dN1v0, double  ko_bN1v0, \
                 double ko_dNv0, double  ko_bNv0, double  ko_dKv0, double  ko_bKv0, \
                 double  k_31, double k_32, double  k_4f, double  k_4r, double  ki_dN1v0, \
                 double  ki_bN1v0, double ki_dN, double  ki_bN, double  ki_dK, double  ki_bk,\
                 double c_Na_out, double c_Na_in, double c_K_out, double c_K_in)
        {
            T=T; V=V; F=F; R=R; FV_RT=FV_RT;
            k_1=k_1; k_2fv0=k_2fv0; k_2rv0=k_2rv0; ko_dN1v0=ko_dN1v0; ko_bN1v0=ko_bN1v0;
            ko_dNv0=ko_dNv0; ko_bNv0=ko_bNv0; ko_dKv0=ko_dKv0; ko_bKv0=ko_bKv0;
            k_31=k_31; k_32=k_32; k_4f=k_4f; k_4r=k_4r; ki_dN1v0=ki_dN1v0;
            ki_bN1v0=ki_bN1v0; ki_dN=ki_dN; ki_bN=ki_bN; ki_dK=ki_dK; ki_bk=ki_bk;

            k_2f = k_2fv0*exp(0.1*FV_RT);
            k_2r = k_2rv0*exp(-0.1*FV_RT);
            ko_dN1 = ko_dN1v0*exp(0.65*FV_RT);
            ko_bN1 = ko_bN1v0*exp(-0.65*FV_RT);
            ko_dN = ko_dNv0*exp(0.185*FV_RT);
            ko_bN = ko_bNv0*exp(-0.185*FV_RT);
            ko_dK = ko_dKv0*exp(0.185*FV_RT);
            ko_bK = ko_bKv0*exp(-0.185*FV_RT);
            ki_dN1 = ki_dN1v0*exp(-0.25*FV_RT);
            ki_bN1 = ki_bN1v0*exp(0.25*FV_RT);

            c_Na_out=c_Na_out; c_Na_in=c_Na_in; c_K_out=c_K_out; c_K_in=c_K_in;

            W(0,0) = -(k_1+ki_dN1); W(0,13) = ki_bN1*c_Na_in;
            W(1,0) = k_1; W(1,1) = -k_2f; W(1,2) = k_2r;
            W(2,1) = k_2f; W(2,2) = -(k_2r+ko_dN1); W(2,3)=ko_bN1*c_Na_out;
            W(3,2) = ko_dN1; W(3,3) = -(ko_bN1*c_Na_out+k_31+2*ko_dN); W(3,4) = ko_bN*c_Na_out;
            W(4,3) = 2*ko_dN; W(4,4) = -(ko_bN*c_Na_out+ko_bK*c_K_out+ko_dN); W(4,5) = 2*ko_bN*c_Na_out; W(4,15) = ko_dK;
            W(5,4) = ko_dN; W(5,5) = -(2*ko_bN*c_Na_out+2*ko_bK*c_K_out); W(5,6) = ko_dK;
            W(6,5) = 2*ko_bK*c_K_out; W(6,6) = -(ko_dK+ko_bN*c_Na_out+ko_bK*c_K_out); W(6,7) = 2*ko_dK; W(6,17) = ko_dN;
            W(7,6) = ko_bK*c_K_out; W(7,7) = -(2*ko_dK+k_32);
            W(8,7) = k_32; W(8,8) = -k_4f; W(8,9) = k_4r;
            W(9,8) = k_4f; W(9,9) = -(k_4r+2*ki_dK); W(9,10) = ki_bk*c_K_in;
            W(10,9) = 2*ki_dK; W(10,10) = -(ki_bk*c_K_in+ki_bN*c_Na_in+ki_dK); W(10,11) = 2*ki_bk*c_K_in; W(10,18) = ki_dN;
            W(11,10) = ki_dK; W(11,11) = -(2*ki_bk*c_K_in+2*ki_bN*c_Na_in); W(11,12) = ki_dN;
            W(12,11) = 2*ki_bN*c_Na_in; W(12,12) = -(ki_dN+ki_bk*c_K_in+ki_bN*c_Na_in); W(12,13) = 2*ki_dN; W(12,16) = ki_dK;
            W(13,0) = ki_dN1; W(13,12) = ki_bN*c_Na_in; W(13,13) = -(2*ki_dN+ki_bN1*c_Na_in); W(13,14) = k_4f;
            W(14,3) = k_31; W(14,14) = -k_4f;
            W(15,4) = ko_bK*c_K_out; W(15,15) = -ko_dK;
            W(16,12) = ki_bk*c_K_in; W(16,16) = -ki_dK;
            W(17,6) = ko_bN*c_Na_out; W(17,17) = -ko_dN;
            W(18,10) = ki_bN*c_Na_in; W(18,18) = -ki_dN;
        }
};

class solver{
    private:
    public:
        Eigen::EigenSolver<Eigen::MatrixXd> solver;
        void initialize(Eigen::MatrixXd &W){solver = Eigen::EigenSolver<Eigen::MatrixXd>(W);}
        Eigen::VectorXd get_eigenvalues(Eigen::MatrixXd &W){return solver.eigenvalues().real();}
        Eigen::MatrixXd get_eigenvectors(Eigen::MatrixXd &W){return solver.eigenvectors().real();}
        int steady_state_index(Eigen::VectorXd &eigenvalues, double threshold){
            try{
                for(int i=0;i < eigenvalues.size(); i++){
                    if(fabs(eigenvalues[i]) < threshold){return i;}
                }
                throw (9999);
            }
            catch(int error){
                std::cout << "No eigenvalue found with absolute value under " << threshold << std::endl;
                exit(1);
            }
        }
};

#endif // W_MATRIX_H_
