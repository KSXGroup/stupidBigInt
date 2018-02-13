#ifndef __BMATH_HPP__
#define __BMATH_HPP__
#include <iostream>
#include "BigInt.hpp"
BigInt pow(BigInt base, BigInt exponent){
    BigInt res = 1;
    while(exponent != 0){
        if(exponent % 2 == 1) res = res * base;
        base = base * base;
        exponent = exponent / 2;
        std::cout << exponent << std::endl;
    }
    return res;
}
#endif // BMATH_HPP
