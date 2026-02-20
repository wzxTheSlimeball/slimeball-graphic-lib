//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

namespace Assets{
    struct Matrix{
        double a1;double a2;
        double b1;double b2;
        Matrix();
        Matrix(long double theta);
        Matrix(long long degrees);
        Matrix(double scaleX,double scaleY);
        Matrix(double shearX,double shearY,bool shearFlag);//You can set shearFlag at any value, then the compiler will know that you wanna use shear construction instead of scale construction
        Matrix(double a1,double a2,double b1,double b2);
    };
}


#endif // TRANSLATION_HPP