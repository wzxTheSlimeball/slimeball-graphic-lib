//MIT License

//Copyright (c) 2026 Z-Multiplier
#include "Translation.hpp"
#include <cmath>

Assets::Matrix::Matrix(){
    a1=1.0;
    a2=0.0;
    b1=0.0;
    b2=1.0;
}
Assets::Matrix::Matrix(double a1,double a2,double b1,double b2){
    this->a1=a1;
    this->a2=a2;
    this->b1=b1;
    this->b2=b2;
}
Assets::Matrix::Matrix(long double theta):Assets::Matrix::Matrix(std::cos(static_cast<double>(theta)),-std::sin(static_cast<double>(theta)),std::sin(static_cast<double>(theta)),std::cos(static_cast<double>(theta))){}
Assets::Matrix::Matrix(double scaleX,double scaleY):Assets::Matrix::Matrix(scaleX,0.0,0.0,scaleY){}
Assets::Matrix::Matrix(long long degrees):Assets::Matrix::Matrix(std::cos(static_cast<double>(degrees)*std::acos(-1.0)/180.0),-std::sin(static_cast<double>(degrees)*std::acos(-1.0)/180.0),std::sin(static_cast<double>(degrees)*std::acos(-1.0)/180.0),std::cos(static_cast<double>(degrees)*std::acos(-1.0)/180.0)){}
Assets::Matrix::Matrix(double shearX,double shearY,bool shearFlag):Assets::Matrix::Matrix(1.0,shearX,shearY,1.0){(void)shearFlag;}