#include "./hm/hm.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

void testAddition() {
    LongNumber numA = LongNumber(12.345, 1000);
    LongNumber numB = LongNumber(7.655, 1000);
    LongNumber result = numA + numB;
    LongNumber expected = LongNumber(20.000, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Addition: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testSubtraction() {
    LongNumber numA = LongNumber(25.678, 1000);
    LongNumber numB = LongNumber(15.678, 1000);
    LongNumber result = numA - numB;
    LongNumber expected = LongNumber(10.000, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Subtraction: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testMultiplication() {
    LongNumber numA = LongNumber(3.14, 1000);
    LongNumber numB = LongNumber(2.0, 1000);
    LongNumber result = numA * numB;
    LongNumber expected = LongNumber(6.28, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Multiplication: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testDivision() {
    LongNumber numA = LongNumber(22.0, 1000);
    LongNumber numB = LongNumber(7.0, 1000);
    LongNumber result = numA / numB;
    LongNumber expected = LongNumber(3.142857142, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Division: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testAdditionWithNegative() {
    LongNumber numA = LongNumber(-12.345, 1000);
    LongNumber numB = LongNumber(7.655, 1000);
    LongNumber result = numA + numB;
    LongNumber expected = LongNumber(-4.690, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Addition with Negative: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testSubtractionWithNegative() {
    LongNumber numA = LongNumber(-25.678, 1000);
    LongNumber numB = LongNumber(-15.678, 1000);
    LongNumber result = numA - numB;
    LongNumber expected = LongNumber(-10.000, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Subtraction with Negative: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testMultiplicationWithNegative() {
    LongNumber numA = LongNumber(-3.14, 1000);
    LongNumber numB = LongNumber(2.0, 1000);
    LongNumber result = numA * numB;
    LongNumber expected = LongNumber(-6.28, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Multiplication with Negative: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testDivisionWithNegative() {
    LongNumber numA = LongNumber(-22.0, 1000);
    LongNumber numB = LongNumber(7.0, 1000);
    LongNumber result = numA / numB;
    LongNumber expected = LongNumber(-3.142857142, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Division with Negative: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testLargeNumbers() {
    LongNumber numA = LongNumber(123456789.123456789, 1000);
    LongNumber numB = LongNumber(987654321.987654321, 1000);
    LongNumber result = numA + numB;
    LongNumber expected = LongNumber(1111111111.111111110, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Large Numbers Addition: " << (diff.abs() < LongNumber(1e-7, 100) ? "OK" : "FAIL") << "\n";
}

void testSmallNumbers() {
    LongNumber numA = LongNumber(0.000000001, 1000);
    LongNumber numB = LongNumber(0.000000002, 1000);
    LongNumber result = numA + numB;
    LongNumber expected = LongNumber(0.000000003, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Small Numbers Addition: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}

void testMultiplicationLargeAndSmall() {
    LongNumber numA = LongNumber(123456789.123456789, 1000);
    LongNumber numB = LongNumber(0.000000001, 1000);
    LongNumber result = numA * numB;
    LongNumber expected = LongNumber(0.123456789, 1000);
    LongNumber diff = result - expected;
    std::cout << "Test Multiplication Large and Small: " << (diff.abs() < LongNumber(1e-9, 1000) ? "OK" : "FAIL") << "\n";
}


int main() {
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testAdditionWithNegative();
    testSubtractionWithNegative();
    testMultiplicationWithNegative();
    testDivisionWithNegative();
    testLargeNumbers();
    testSmallNumbers();
    testMultiplicationLargeAndSmall();
    std::cout << "All tests completed!\n";
    return 0;
}

