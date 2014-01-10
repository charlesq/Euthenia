#include <big_number.hpp>
#include <iostream>



int main(int argc, char *argv[])
{
    big_number bn1, bn2, bn3;
    bn1 = 100;
    bn2 = 300;
    bn1 +=  300 + bn2;
    bn3 = bn1 + bn2  + bn3;
    std::cout <<" bn1 = ";
    print_bn(bn1, 10);
    std::cout << "  bn2 = ";
    print_bn(bn2, 10);
    std::cout << "  bn3 = ";
    print_bn(bn3, 10);
    std::cout << std::endl;

    if (bn1 > 1000)
        std::cout << "bn1 > 1000" << std::endl;
    if (bn1 >= bn2)
        std::cout << "bn1 >= bn2" << std::endl; 
    if (bn2 != bn2)
        std::cout << " bn2 != bn2" << std::endl;
    if (bn2 < bn3) {}
    else
    {
        std::cout << "bn2 >= bn3" << std::endl;
    }
    if (bn1 == bn3)
        std::cout << "bn1 == bn3" << std::endl;
    bn1 -= 100;
    bn2 = bn1 - bn2;
    std::cout << " bn1 -= 100; bn2 = bn1 - bn2 " << std::endl;
    std::cout <<" bn1 = ";
    print_bn(bn1, 10);
    std::cout << "  bn2 = ";
    print_bn(bn2, 10);
    std::cout << std::endl;
    bn3 = bn1 * bn2;
    std::cout << " bn3 = bn1 * bn2 " ;
    print_bn(bn3, 10);
    std::cout << std::endl;
    bn3 %= bn1; 
    std::cout << " bn3 %= bn1 ";
    print_bn(bn3, 10);
    std::cout << std::endl;
    return 0;
 
}
