#include <iostream>
#include <assert.h>

using i64 = long long;

class EuclideanExtended {
    i64 A;
    i64 B;
    i64 X1, Y1, X2, Y2;
    i64 GCD;
public:
    EuclideanExtended(i64 a, i64 b) : A(a), B(b), X1(1), Y1(0), X2(0), Y2(1) {
        // run extended euclid algorithm: gcd (A, B) => A*X1 + B*Y1 = gcd(A, B)
        GCD = EuclidStep(A, B);
    }

    i64 GetGCD() const { return GCD; }

    i64 GetInverseAModB() const {
        assert(GCD == 1);  // A and B prime to each other
        return X1;
    }

    i64 GetInverseBModA() const {
        assert(GCD == 1);  // A and B prime to each other
        return Y1;
    }

private:
    i64 EuclidStep(i64 a, i64 b) {
        if (b == 0) return a;
        // a=b, b=a%b in coefs: new_b = a%b = a - k*b = (X1*A + Y1*B) - k(X2*A + Y2*B), where k = (a/b)
        // => (X1, Y1) = (X2, Y2), (X2, Y2) = (X1-kX2, Y1-kY2)
        i64 k = a / b;
        i64 x = X1, y = Y1;
        X1 = X2;
        Y1 = Y2;
        X2 = x - k * X2;
        Y2 = y - k * Y2;
        return EuclidStep(b, a % b);
    }
};

int main() {
    i64 a, b;
    std::cin >> a >> b;
    EuclideanExtended euclid(a, b);
    std::cout << "GCD(" << a << ", " << b << ") = " << euclid.GetGCD() << std::endl;
    if (euclid.GetGCD() == 1) {
        std::cout << "A^-1 mod B == " << a << "^-1 mod " << b << " = " << euclid.GetInverseAModB() << std::endl;
        std::cout << "B^-1 mod A == " << b << "^-1 mod " << a << " = "  << euclid.GetInverseBModA() << std::endl;
    }
    return 0;
}
