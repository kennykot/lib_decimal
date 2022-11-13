#include <check.h>

#include "s21_decimal.h"

//  ARITHMETIC FUNCTIONS

START_TEST(s21_add_decimal_1) {
  //  negative plus positive
  s21_decimal a = {{1, 1, 0, 2147516416}}, b = {{0, 0, 1, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 4294967295);
  ck_assert_int_eq(result1.bits[1], 4294967294);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 0);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_add_decimal_2) {
  //  positive plus positive
  s21_decimal a = {{0, 0, 1, 0}}, b = {{123, 123, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 123);
  ck_assert_int_eq(result1.bits[1], 123);
  ck_assert_int_eq(result1.bits[2], 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_add_decimal_3) {
  //  1.00005 plus 10.00005
  s21_decimal a = {{100005, 0, 0, 0}}, b = {{1000005, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  set_scale(&a, 5);
  set_scale(&b, 5);
  int res = s21_add(a, b, &result1);
  int scale = get_scale(&result1);

  ck_assert_int_eq(result1.bits[0], 1100010);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(scale, 5);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_add_decimal_4_POS_INF) {
  //  positive infinity
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}}, b = {{1, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 0);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 0);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_add_decimal_5_NEG_INF) {
  //  negative infinity
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 2147516416}},
              b = {{1, 0, 0, 2147516416}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 0);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 0);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(s21_sub_decimal_1) {
  //  greater minus less
  s21_decimal a = {{1, 3, 2, 0}}, b = {{3, 3, 3, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_sub(b, a, &result1);

  ck_assert_int_eq(result1.bits[0], 2);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_sub_decimal_2) {
  //  less minus greater
  s21_decimal a = {{3, 3, 3, 0}}, b = {{1, 2, 3, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_sub(b, a, &result1);

  ck_assert_int_eq(result1.bits[0], 2);
  ck_assert_int_eq(result1.bits[1], 1);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 2147483648);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_sub_decimal_3) {
  //  1.00005 minus 10.00005
  s21_decimal a = {{100005, 0, 0, 0}}, b = {{1000005, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  set_scale(&a, 5);
  set_scale(&b, 5);
  int res = s21_sub(a, b, &result1);
  int scale = get_scale(&result1);

  ck_assert_int_eq(result1.bits[0], 900000);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(scale, 5);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_sub_decimal_4_POS_INF) {
  //  positive infinity
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}},
              b = {{1, 0, 0, 2147516416}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_sub(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 0);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 0);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_sub_decimal_5_NEG_INF) {
  //  negative infinity
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 2147516416}},
              b = {{1, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_sub(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 0);
  ck_assert_int_eq(result1.bits[1], 0);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 0);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(s21_mul_decimal_1) {
  //  10 * 0.25
  s21_decimal a = {{1000000, 0, 0, 2147811328}}, b = {{250, 0, 0, 2147680256}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_mul(a, b, &result1);
  ck_assert_int_eq(result1.bits[0], 250000000);
  ck_assert_int_eq(result1.bits[3], 524288);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_mul_decimal_2) {
  //  0.25 * 1.8446744069415e19
  s21_decimal a = {{250, 0, 0, 2147680256}}, b = {{0, 4294967295, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_mul(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 0);
  ck_assert_int_eq(result1.bits[1], 4294967046);
  ck_assert_int_eq(result1.bits[2], 249);
  ck_assert_int_eq(result1.bits[3], 2147680256);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_mul_decimal_3_POS_INF) {
  //  positive infinity
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}}, b = {{0, 0, 2, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_mul(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 4294967295);
  ck_assert_int_eq(result1.bits[1], 4294967295);
  ck_assert_int_eq(result1.bits[2], 4294967295);
  ck_assert_int_eq(result1.bits[3], 2147483647);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_mul_decimal_4_NEG_INF) {
  //  negative infinity
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 2147516416}},
              b = {{0, 0, 2, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_mul(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 4294967295);
  ck_assert_int_eq(result1.bits[1], 4294967295);
  ck_assert_int_eq(result1.bits[2], 4294967295);
  ck_assert_int_eq(result1.bits[3], 2147483647);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_mod_decimal_BIG) {
  s21_decimal a = {{4294967295, 4294967295, 4, 0}}, b = {{4294967295, 1, 3, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  s21_mod(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 0);
  ck_assert_int_eq(result1.bits[1], 4294967294);
  ck_assert_int_eq(result1.bits[2], 1);
}
END_TEST

START_TEST(s21_mod_decimal) {
  s21_decimal a = {{100000005, 0, 0, 0}}, b = {{1, 0, 0, 0}};
  set_scale(&a, 7);
  s21_decimal result1 = {{0, 0, 0, 0}};
  s21_mod(a, b, &result1);
  int scale = get_scale(&result1);

  ck_assert_int_eq(result1.bits[0], 5);
  ck_assert_int_eq(scale, 7);
}
END_TEST

START_TEST(s21_mod_decimal_ZERO) {
  s21_decimal a = {{9999, 0, 0, 0}}, b = {{0, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_mod(a, b, &result1);
  ck_assert_int_eq(res, 3);
}
END_TEST

START_TEST(s21_mod_decimal_LESS) {
  s21_decimal a = {{2, 2, 4, 0}}, b = {{2, 2, 4, 0}};
  set_scale(&a, 15);
  set_scale(&b, 14);
  s21_decimal result1 = {{0, 0, 0, 0}};
  s21_mod(a, b, &result1);
  int scale = get_scale(&result1);

  ck_assert_int_eq(result1.bits[0], 2);
  ck_assert_int_eq(result1.bits[1], 2);
  ck_assert_int_eq(result1.bits[2], 4);
  ck_assert_int_eq(scale, 15);
}
END_TEST

START_TEST(s21_mod_decimal_INF) {
  s21_decimal a = {{1, 0, 1, 0}}, b = {{100001, 0, 0, 0}};
  set_scale(&a, 19);
  set_scale(&b, 5);
  set_sign(&a, 0);
  set_sign(&b, 0);
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_mod(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 1704837121);
  ck_assert_int_eq(result1.bits[1], 1966637576);
  ck_assert_int_eq(result1.bits[2], 0);
  ck_assert_int_eq(result1.bits[3], 1245184);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_div_decimal_ZERO) {
  s21_decimal a = {{1, 1, 2, 0}}, b = {{0, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_div(a, b, &result1);
  ck_assert_int_eq(res, 3);
}
END_TEST

START_TEST(s21_div_decimal_int) {
  s21_decimal a = {{123456, 0, 0, 0}}, b = {{300, 0, 0, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_div(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 41152);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_div_decimal_float) {
  s21_decimal a = {{123456, 0, 0, 0}}, b = {{300, 0, 0, 0}};
  set_scale(&a, 3);
  set_scale(&b, 2);
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_div(a, b, &result1);
  int scale = get_scale(&result1);

  ck_assert_int_eq(result1.bits[0], 41152);
  ck_assert_int_eq(scale, 3);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_div_decimal_max_float) {
  s21_decimal a = {{4294967295, 1, 0, 0}}, b = {{2, 0, 0, 0}};
  set_scale(&a, 7);
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_div(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 4294967292);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_div_decimal_MAX) {
  s21_decimal a = {{4294967295, 4294967295, 4294967294, 0}},
              b = {{4294967295, 4294967295, 1, 0}};
  s21_decimal result1 = {{0, 0, 0, 0}};
  int res = s21_div(a, b, &result1);

  ck_assert_int_eq(result1.bits[0], 4294967291);
  ck_assert_int_eq(res, 0);
}
END_TEST

//  COMPARISON FUNCTIONS

START_TEST(is_less) {
  s21_decimal a = {{1234, 0, 0, 0}}, b = {{123400, 0, 0, 65536}},
              c = {{1234, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
  ck_assert_int_eq(s21_is_less(a, c), 0);
}
END_TEST

START_TEST(is_equal) {
  s21_decimal a = {{1234, 0, 0, 0}}, b = {{123400, 0, 0, 65536}},
              c = {{1234, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), 0);
  ck_assert_int_eq(s21_is_equal(a, c), 1);
}
END_TEST

START_TEST(is_not_eq) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{1, 1, 1, 0}}, c = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
  ck_assert_int_eq(s21_is_not_equal(a, c), 1);
}
END_TEST

START_TEST(is_greater) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{1, 1, 1, 0}}, c = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 0);
  ck_assert_int_eq(s21_is_greater(a, c), 1);
}
END_TEST

START_TEST(is_greater_or_eq) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{1, 1, 1, 0}}, c = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(a, c), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(c, a), 0);
}
END_TEST

START_TEST(is_less_or_eq) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{1, 1, 1, 0}}, c = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
  ck_assert_int_eq(s21_is_less_or_equal(a, c), 0);
  ck_assert_int_eq(s21_is_less_or_equal(c, a), 1);
}
END_TEST

//  CONVERTORS FUNCTIONS

START_TEST(s21_from_int_to_decimalTest1) {
  // 6412
  int src1 = 1;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest2) {
  // 6428
  int src1 = -1;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest3) {
  // 6444
  int src1 = 0;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest4) {
  // 6460
  int src1 = 0;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest5) {
  // 6476
  int src1 = -987879878;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);

  origin.bits[0] = 0b00111010111000011101100111000110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest6) {
  // 6492
  int src1 = -2147483646;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);

  origin.bits[0] = 0b01111111111111111111111111111110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest7) {
  // 6508
  int src1 = 2147483646;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);

  origin.bits[0] = 0b01111111111111111111111111111110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest8) {
  // 6524
  int src1 = 796132784;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);

  origin.bits[0] = 0b00101111011101000000010110110000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_int_to_decimalTest9) {
  // 6540
  int src1 = -12345677;
  s21_decimal origin, result;
  s21_decimal *res = &result;
  s21_from_int_to_decimal(src1, res);
  origin.bits[0] = 0b00000000101111000110000101001101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(int_to_decimal_1) {
  int test_int = 0;
  s21_decimal *decimal = NULL;
  convert_result status = s21_from_int_to_decimal(test_int, decimal);
  ck_assert_int_eq(status, CONVERT_ERROR);
}
END_TEST

START_TEST(int_to_decimal_2) {
  int test_int = 0;
  s21_decimal *decimal = NULL;
  convert_result status = s21_from_int_to_decimal(test_int, decimal);
  ck_assert_int_eq(status, CONVERT_ERROR);
}
END_TEST

START_TEST(int_to_decimal_3) {
  int test_int = 0;
  s21_decimal decimal = {0};
  convert_result status = s21_from_int_to_decimal(test_int, &decimal);

  ck_assert_int_eq(status, CONVERT_GOOD);
  ck_assert_int_eq(test_int, decimal.bits[0]);
  ck_assert_int_eq(0, decimal.bits[1]);
  ck_assert_int_eq(0, decimal.bits[2]);
}
END_TEST

START_TEST(int_to_decimal_4) {
  int test_int = INT32_MAX;
  s21_decimal decimal = {0};
  convert_result status = s21_from_int_to_decimal(test_int, &decimal);

  ck_assert_int_eq(status, CONVERT_GOOD);
  ck_assert_int_eq(test_int, decimal.bits[0]);
  ck_assert_int_eq(0, decimal.bits[1]);
  ck_assert_int_eq(0, decimal.bits[2]);
}
END_TEST

START_TEST(int_to_decimal_5) {
  int test_int = INT32_MIN + 1;
  s21_decimal decimal = {0};
  convert_result status = s21_from_int_to_decimal(-test_int, &decimal);

  ck_assert_int_eq(status, CONVERT_GOOD);
  ck_assert_int_eq(-test_int, decimal.bits[0]);
  ck_assert_int_eq(0, decimal.bits[1]);
  ck_assert_int_eq(0, decimal.bits[2]);
}
END_TEST

START_TEST(int_to_decimal_6) {
  int test_int = -126123;
  s21_decimal decimal = {0};
  convert_result status = s21_from_int_to_decimal(-test_int, &decimal);

  ck_assert_int_eq(status, CONVERT_GOOD);
  ck_assert_int_eq(-test_int, decimal.bits[0]);
  ck_assert_int_eq(0, decimal.bits[1]);
  ck_assert_int_eq(0, decimal.bits[2]);
}
END_TEST

START_TEST(int_to_decimal_7) {
  int test_int = 3752;
  s21_decimal decimal = {0};
  convert_result status = s21_from_int_to_decimal(test_int, &decimal);

  ck_assert_int_eq(status, CONVERT_GOOD);
  ck_assert_int_eq(test_int, decimal.bits[0]);
  ck_assert_int_eq(0, decimal.bits[1]);
  ck_assert_int_eq(0, decimal.bits[2]);
}
END_TEST

START_TEST(s21_from_decimal_to_int_0) {
  s21_decimal src;
  int result = 0, number = 0;
  src.bits[0] = UINT32_MAX;
  src.bits[1] = UINT32_MAX;
  src.bits[2] = UINT32_MAX;
  src.bits[3] = UINT32_MAX;
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_int_1) {
  s21_decimal src;
  int result = 0, number = 0;
  src.bits[0] = INT_MAX;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_float_eq(number, 2147483647);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_int_2) {
  s21_decimal src;
  int result = 0, number = 0;
  src.bits[0] = 133141;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_int_eq(number, 133141);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_int_4) {
  s21_decimal src;
  int result = 0, number = 0;
  long int c = 2147483648;
  src.bits[0] = 123451234;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = c;
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_int_eq(number, -123451234);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_int_5) {
  s21_decimal src;
  int result = 0, number = 0;
  long int c = 2147483648;
  src.bits[0] = 18;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = c;  // 2147483648
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_int_eq(number, -18);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_int_6) {
  s21_decimal src;
  int result = 0, number = 0;
  long int c = 4294967295;
  src.bits[0] = c;
  src.bits[1] = c;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_int(src, &number);
  ck_assert_int_eq(number, 0);
  ck_assert_int_eq(result, 1);
}
END_TEST

int check, result, code;

START_TEST(s21_test_from_decimal_to_int_0) {
  s21_decimal a = {{100, 0, 0, 0}};
  check = 100;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_1) {
  s21_decimal a = {{INT_MAX, 0, 0, 0}};
  check = INT_MAX;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_3) {
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  check = 0;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_4) {
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, INT_MAX}};
  check = 0;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_6) {
  s21_decimal a = {{4294967295, 4294967295, 0, 0}};
  check = 0;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST
START_TEST(s21_test_from_decimal_to_int_7) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}};
  check = 1;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_8) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 4294967295}};
  check = 0;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_9) {
  s21_decimal a = {{INT_MAX, 0, 0, 2147483648}};
  check = -INT_MAX;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_10) {
  s21_decimal a = {{INT_MAX, 0, 0, 2147483648}};
  check = -INT_MAX;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_11) {
  s21_decimal a = {{123456789, 0, 0, 0}};  // 2147483647
  set_scale(&a, 1);
  check = 12345678;  // (int)(INT_MAX / 10^1)

  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_12) {
  s21_decimal a = {{INT_MAX, 0, 0, 2149318656}};
  check = 0;  // (int)(INT_MAX / 10^28)
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_14) {
  s21_decimal a = {{INT_MAX, 0, 0, 1835008}};
  check = 0;  // (int)(INT_MAX / 10^28)
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_test_from_decimal_to_int_16) {
  s21_decimal a;
  a.bits[0] = 0b00000000000000000000000000000001;
  a.bits[1] = 0b00000000000000000000000000000000;
  a.bits[2] = 0b00000000000000000000000000000000;
  a.bits[3] = 0b00000000000000000000000000000000;
  check = 1;
  code = s21_from_decimal_to_int(a, &result);
  ck_assert_int_eq(result, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest1) {
  // 6556
  s21_decimal src1;
  // src1 = 2;

  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 2);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest2) {
  // 6570
  s21_decimal src1;
  // src1 = 0;

  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest3) {
  // 6584
  s21_decimal src1;
  // src1 = 3.5;

  src1.bits[0] = 0b00000000000000000000000000100011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 3);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest4) {
  // 6598
  s21_decimal src1;
  // src1 = 4.5;

  src1.bits[0] = 0b00000000000000000000000000101101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 4);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest5) {
  // 6612
  s21_decimal src1;
  // src1 = -4.5;

  src1.bits[0] = 0b00000000000000000000000000101101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000010000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, -4);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest6) {
  // 6626
  s21_decimal src1;
  int code = 0;
  // src1 = -5454.3526545;

  src1.bits[0] = 0b10110011000011000010101010010001;
  src1.bits[1] = 0b00000000000000000000000000001100;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000001110000000000000000;
  int result;
  //   print_2(&src1);
  //   int *res = &result;
  code = s21_from_decimal_to_int(src1, &result);
  ck_assert_int_eq(result, -5454);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest7) {
  // 6640
  s21_decimal src1;
  // src1 = -5.492654545456454545645464;

  src1.bits[0] = 0b00000111100110110001111110011000;
  src1.bits[1] = 0b01100110010010001001000110100011;
  src1.bits[2] = 0b00000000000001001000101100011101;
  src1.bits[3] = 0b10000000000110000000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, -5);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest8) {
  // 6654
  s21_decimal src1;
  // src1 = 796132784.879754;

  src1.bits[0] = 0b00011011110101100011100010001010;
  src1.bits[1] = 0b00000000000000101101010000010100;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000001100000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 796132784);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest9) {
  // 6668
  s21_decimal src1;
  // src1 = -12345677.187654345678987654346;

  src1.bits[0] = 0b01111000100010101111010011001010;
  src1.bits[1] = 0b01100010000010101110010010000111;
  src1.bits[2] = 0b00100111111001000001101011010101;
  src1.bits[3] = 0b10000000000101010000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, -12345677);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest10) {
  // 6682
  s21_decimal src1;
  // src1 = 2.5086531268974139743;

  src1.bits[0] = 0b01100101111011101101100101011111;
  src1.bits[1] = 0b01011100001001010100001101000110;
  src1.bits[2] = 0b00000000000000000000000000000001;
  src1.bits[3] = 0b00000000000100110000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 2);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest11) {
  // 6696
  s21_decimal src1;
  // src1 = 1;

  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest12) {
  // 6710
  s21_decimal src1;
  // src1 = 1.25;

  src1.bits[0] = 0b00000000000000000000000001111101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000100000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest13) {
  // 6724
  s21_decimal src1;
  // src1 = -1.25;

  src1.bits[0] = 0b00000000000000000000000001111101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000100000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest14) {
  // 6738
  s21_decimal src1;
  // src1 = -12.25;

  src1.bits[0] = 0b00000000000000000000010011001001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000100000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, -12);
}
END_TEST

START_TEST(s21_from_decimal_to_intTest15) {
  // 6752
  s21_decimal src1;
  // src1 = -3.5;

  src1.bits[0] = 0b00000000000000000000000000100011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000010000000000000000;
  int result;
  int *res = &result;
  s21_from_decimal_to_int(src1, res);
  ck_assert_int_eq(result, -3);
}
END_TEST

START_TEST(s21_from_float_to_decimal_1) {
  float src1 = 0.02;
  s21_decimal result = {0};
  s21_decimal etalon = {0};
  s21_from_float_to_decimal(src1, &result);
  etalon.bits[0] = 2;
  etalon.bits[1] = 0;
  etalon.bits[2] = 0;
  etalon.bits[3] = 0;
  set_scale(&etalon, 2);
  ck_assert_int_eq(etalon.bits[3], result.bits[3]);
  ck_assert_int_eq(etalon.bits[2], result.bits[2]);
  ck_assert_int_eq(etalon.bits[1], result.bits[1]);
  ck_assert_int_eq(etalon.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_float_to_decimal_2) {
  float src1 = -0.02;
  s21_decimal result = {0};
  s21_decimal etalon = {0};
  s21_from_float_to_decimal(src1, &result);
  etalon.bits[0] = 2;
  etalon.bits[1] = 0;
  etalon.bits[2] = 0;
  etalon.bits[3] = 2147483648;
  set_scale(&etalon, 2);
  ck_assert_int_eq(etalon.bits[3], result.bits[3]);
  ck_assert_int_eq(etalon.bits[2], result.bits[2]);
  ck_assert_int_eq(etalon.bits[1], result.bits[1]);
  ck_assert_int_eq(etalon.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_float_to_decimal_3) {
  float src1 = 0.000000002;
  s21_decimal result = {0};
  s21_decimal etalon = {0};
  s21_from_float_to_decimal(src1, &result);
  etalon.bits[0] = 2;
  etalon.bits[1] = 0;
  etalon.bits[2] = 0;
  etalon.bits[3] = 0;
  set_scale(&etalon, 9);
  ck_assert_int_eq(etalon.bits[3], result.bits[3]);
  ck_assert_int_eq(etalon.bits[2], result.bits[2]);
  ck_assert_int_eq(etalon.bits[1], result.bits[1]);
  ck_assert_int_eq(etalon.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_float_to_decimal_4) {
  float src1 = -5.492654;
  s21_decimal result = {0};
  s21_decimal etalon = {0};
  s21_from_float_to_decimal(src1, &result);
  etalon.bits[0] = 0b00000000010100111100111110101110;
  etalon.bits[1] = 0b00000000000000000000000000000000;
  etalon.bits[2] = 0b00000000000000000000000000000000;
  etalon.bits[3] = 0b10000000000001100000000000000000;
  ck_assert_int_eq(etalon.bits[3], result.bits[3]);
  ck_assert_int_eq(etalon.bits[2], result.bits[2]);
  ck_assert_int_eq(etalon.bits[1], result.bits[1]);
  ck_assert_int_eq(etalon.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_float_to_decimal_5) {
  float src1 = 7.961328E+08;
  s21_decimal result = {0};
  s21_decimal etalon = {0};
  s21_from_float_to_decimal(src1, &result);
  etalon.bits[0] = 0b00101111011101000000010111000000;
  etalon.bits[1] = 0b00000000000000000000000000000000;
  etalon.bits[2] = 0b00000000000000000000000000000000;
  etalon.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(etalon.bits[3], result.bits[3]);
  ck_assert_int_eq(etalon.bits[2], result.bits[2]);
  ck_assert_int_eq(etalon.bits[1], result.bits[1]);
  ck_assert_int_eq(etalon.bits[0], result.bits[0]);
}
END_TEST

START_TEST(s21_from_decimal_to_float_1) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;

  src.bits[0] = 18122;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  set_scale(&src, 3);

  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 18.122);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_2) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  long int c = 2147483648;
  src.bits[0] = 1812;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = c;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -1812);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_3) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 0XFFFFFF;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 16777215);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_5) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  long int c = 4294967295;
  src.bits[0] = c;
  src.bits[1] = c;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_7) {
  s21_decimal dec;
  int result = 0;
  float number = 0.0;
  long int c = 2147483648;
  dec.bits[0] = 1812;
  dec.bits[1] = 0;
  dec.bits[2] = 0;
  dec.bits[3] = c;
  result = s21_from_decimal_to_float(dec, &number);
  ck_assert_float_eq(number, -1812);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_8) {
  s21_decimal dec;
  int result = 0;
  float number = 0.0;

  dec.bits[0] = 0XFFFFFF;
  dec.bits[1] = 0;
  dec.bits[2] = 0;
  dec.bits[3] = 0;
  result = s21_from_decimal_to_float(dec, &number);
  ck_assert_float_eq(number, 16777215);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_10) {
  s21_decimal dec;
  int result = 0;
  float number = 0.0;
  long int c = 4294967295;
  dec.bits[0] = c;
  dec.bits[1] = c;
  dec.bits[2] = 0;
  dec.bits[3] = 0;
  result = s21_from_decimal_to_float(dec, &number);
  ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest1) {
  s21_decimal src1;

  float src2;
  float *srcp = &src2;

  src1.bits[0] = 0b00000000000000000110001101111001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", 2546.5);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest2) {
  s21_decimal src1;

  float src2;
  float *srcp = &src2;

  src1.bits[0] = 0b00000000000000001011010101110101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000001010000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", -0.46453);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest3) {
  s21_decimal src1;

  float src2;
  float *srcp = &src2;

  src1.bits[0] = 0b00000000000000000000000000100011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", 3.5);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest4) {
  s21_decimal src1;

  float src2;
  float *srcp = &src2;

  src1.bits[0] = 0b00000000000000000000000000101101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", 4.5);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest5) {
  s21_decimal src1;
  float src2;
  float *srcp = &src2;
  src1.bits[0] = 0b00000000000000000000000000101101;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000010000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", -4.5);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest6) {
  s21_decimal src1;
  float src2;
  float *srcp = &src2;
  src1.bits[0] = 0b00000000000010000110000110010001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000001010000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", -5.49265);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_floatTest7) {
  s21_decimal src1;
  float src2;
  float *srcp = &src2;
  src1.bits[0] = 0b01100101111011101101100101011111;
  src1.bits[1] = 0b01011100001001010100001101000110;
  src1.bits[2] = 0b00000000000000000000000000000001;
  src1.bits[3] = 0b00000000000100110000000000000000;
  s21_from_decimal_to_float(src1, srcp);
  char var3str[1000];
  char result[1000];
  snprintf(var3str, sizeof(var3str), "%.6f", 2.50865312689741);
  snprintf(result, sizeof(result), "%f", src2);
  ck_assert_str_eq(result, var3str);
}
END_TEST

START_TEST(s21_from_decimal_to_float_001) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;

  src.bits[0] = 18122;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147680256;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -18.122);
  ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_002) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;

  src.bits[0] = 1812;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147483648;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -1812);
  ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_003) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;

  src.bits[0] = 0XFFFFFF;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 16777215);
  ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_004) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;

  src.bits[0] = 23450987;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147745792;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -2345.0987);
  ck_assert_int_eq(result, 0);
}
END_TEST
START_TEST(s21_from_decimal_to_float_005) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;

  src.bits[0] = 4294967295;
  src.bits[1] = 4294967295;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_11) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 18122;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147680256;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -18.122);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_12) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 1812;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147483648;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -1812);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_13) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 0XFFFFFF;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 16777215);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_14) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 23450987;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147745792;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -2345.0987);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_15) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 4294967295;
  src.bits[1] = 4294967295;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
  ck_assert_int_eq(result, 0);
}
END_TEST

//  ANOTHER FUNCTIONS

START_TEST(s21_negate_decimal_pos) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{1, 1, 1, 0}}, c = {{0, 0, 0, 0}};
  set_sign(&b, 1);
  int res = s21_negate(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_negate_decimal_neg) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{1, 1, 1, 0}}, c = {{0, 0, 0, 0}};
  set_sign(&a, 1);
  int res = s21_negate(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_truncate_decimal_1) {
  s21_decimal a = {{4294967295, 4294967295, 0, 2147549184}},
              b = {{2576980376, 429496729, 0, 2147483648}}, c = {{0, 0, 0, 0}};
  set_scale(&a, 1);
  int res = s21_truncate(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_truncate_decimal_15) {
  s21_decimal a = {{1, 1, 1, 0}}, b = {{18446, 0, 0, 2147483648}},
              c = {{0, 0, 0, 0}};
  set_scale(&a, 15);
  set_sign(&a, 1);
  int res = s21_truncate(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_truncate_decimal_MAX) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}}, b = {{7, 0, 0, 0}},
              c = {{0, 0, 0, 0}};
  set_scale(&a, 28);
  int res = s21_truncate(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_round_decimal) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}}, b = {{8, 0, 0, 0}},
              c = {{0, 0, 0, 0}};
  set_scale(&a, 28);
  int res = s21_round(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_round_decimal_6) {
  s21_decimal a = {{7614122, 0, 0, 0}}, b = {{8, 0, 0, 0}}, c = {{0, 0, 0, 0}};
  set_scale(&a, 6);
  int res = s21_round(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_round_decimal_half) {
  s21_decimal a = {{3500, 0, 0, 0}}, b = {{4, 0, 0, 0}}, c = {{0, 0, 0, 0}};
  set_scale(&a, 3);
  int res = s21_round(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_floor_decimal) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295, 0}}, b = {{7, 0, 0, 0}},
              c = {{0, 0, 0, 0}};
  set_scale(&a, 28);
  set_sign(&a, 0);
  int res = s21_floor(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_floor_decimal_neg) {
  s21_decimal a = {{19, 0, 0, 0}}, b = {{2, 0, 0, 2147483648}},
              c = {{0, 0, 0, 0}};
  set_sign(&a, 1);
  set_scale(&a, 1);
  int res = s21_floor(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_floor_decimal_ZERO) {
  s21_decimal a = {{0, 0, 0, 0}}, b = {{0, 0, 0, 0}}, c = {{0, 0, 0, 0}};
  int res = s21_floor(a, &c);
  ck_assert_int_eq(c.bits[0], b.bits[0]);
  ck_assert_int_eq(c.bits[1], b.bits[1]);
  ck_assert_int_eq(c.bits[2], b.bits[2]);
  ck_assert_int_eq(c.bits[3], b.bits[3]);
  ck_assert_int_eq(res, 0);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("Lib_decimal");
  TCase *tc1 = tcase_create("Arithmetic Operators");
  TCase *tc2 = tcase_create("Comparison Operators");
  TCase *tc3 = tcase_create("Convertors and parsers");
  TCase *tc4 = tcase_create("Another functions");
  SRunner *runner = srunner_create(suite);
  int nf;

  puts("\e[4; 46; 33m");

  suite_add_tcase(suite, tc1);
  tcase_add_test(tc1, s21_add_decimal_1);
  tcase_add_test(tc1, s21_add_decimal_2);
  tcase_add_test(tc1, s21_add_decimal_3);
  tcase_add_test(tc1, s21_add_decimal_4_POS_INF);
  tcase_add_test(tc1, s21_add_decimal_5_NEG_INF);
  tcase_add_test(tc1, s21_sub_decimal_1);
  tcase_add_test(tc1, s21_sub_decimal_2);
  tcase_add_test(tc1, s21_sub_decimal_3);
  tcase_add_test(tc1, s21_sub_decimal_4_POS_INF);
  tcase_add_test(tc1, s21_sub_decimal_5_NEG_INF);
  tcase_add_test(tc1, s21_mul_decimal_1);
  tcase_add_test(tc1, s21_mul_decimal_2);
  tcase_add_test(tc1, s21_mul_decimal_3_POS_INF);
  tcase_add_test(tc1, s21_mul_decimal_4_NEG_INF);
  tcase_add_test(tc1, s21_mod_decimal);
  tcase_add_test(tc1, s21_mod_decimal_BIG);
  tcase_add_test(tc1, s21_mod_decimal_ZERO);
  tcase_add_test(tc1, s21_mod_decimal_LESS);
  tcase_add_test(tc1, s21_mod_decimal_INF);
  tcase_add_test(tc1, s21_div_decimal_ZERO);
  tcase_add_test(tc1, s21_div_decimal_int);
  tcase_add_test(tc1, s21_div_decimal_float);
  tcase_add_test(tc1, s21_div_decimal_max_float);
  tcase_add_test(tc1, s21_div_decimal_MAX);

  suite_add_tcase(suite, tc2);
  tcase_add_test(tc2, is_less);
  tcase_add_test(tc2, is_equal);
  tcase_add_test(tc2, is_not_eq);
  tcase_add_test(tc2, is_greater);
  tcase_add_test(tc2, is_greater_or_eq);
  tcase_add_test(tc2, is_less_or_eq);

  suite_add_tcase(suite, tc3);
  tcase_add_test(tc3, int_to_decimal_1);
  tcase_add_test(tc3, int_to_decimal_2);
  tcase_add_test(tc3, int_to_decimal_3);
  tcase_add_test(tc3, int_to_decimal_4);
  tcase_add_test(tc3, int_to_decimal_5);
  tcase_add_test(tc3, int_to_decimal_6);
  tcase_add_test(tc3, int_to_decimal_7);
  tcase_add_test(tc3, s21_from_int_to_decimalTest1);
  tcase_add_test(tc3, s21_from_int_to_decimalTest2);
  tcase_add_test(tc3, s21_from_int_to_decimalTest3);
  tcase_add_test(tc3, s21_from_int_to_decimalTest4);
  tcase_add_test(tc3, s21_from_int_to_decimalTest5);
  tcase_add_test(tc3, s21_from_int_to_decimalTest6);
  tcase_add_test(tc3, s21_from_int_to_decimalTest7);
  tcase_add_test(tc3, s21_from_int_to_decimalTest8);
  tcase_add_test(tc3, s21_from_int_to_decimalTest9);
  tcase_add_test(tc3, s21_from_float_to_decimal_1);
  tcase_add_test(tc3, s21_from_float_to_decimal_2);
  tcase_add_test(tc3, s21_from_float_to_decimal_3);
  tcase_add_test(tc3, s21_from_float_to_decimal_4);
  tcase_add_test(tc3, s21_from_float_to_decimal_5);
  tcase_add_test(tc3, s21_from_decimal_to_int_0);
  tcase_add_test(tc3, s21_from_decimal_to_int_1);
  tcase_add_test(tc3, s21_from_decimal_to_int_2);
  tcase_add_test(tc3, s21_from_decimal_to_int_4);
  tcase_add_test(tc3, s21_from_decimal_to_int_5);
  tcase_add_test(tc3, s21_from_decimal_to_int_6);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_0);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_1);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_3);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_4);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_6);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_7);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_8);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_9);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_10);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_11);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_12);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_14);
  tcase_add_test(tc3, s21_test_from_decimal_to_int_16);
  tcase_add_test(tc3, s21_from_decimal_to_intTest1);
  tcase_add_test(tc3, s21_from_decimal_to_intTest2);
  tcase_add_test(tc3, s21_from_decimal_to_intTest3);
  tcase_add_test(tc3, s21_from_decimal_to_intTest4);
  tcase_add_test(tc3, s21_from_decimal_to_intTest5);
  tcase_add_test(tc3, s21_from_decimal_to_intTest6);
  tcase_add_test(tc3, s21_from_decimal_to_intTest7);
  tcase_add_test(tc3, s21_from_decimal_to_intTest8);
  tcase_add_test(tc3, s21_from_decimal_to_intTest9);
  tcase_add_test(tc3, s21_from_decimal_to_intTest10);
  tcase_add_test(tc3, s21_from_decimal_to_intTest11);
  tcase_add_test(tc3, s21_from_decimal_to_intTest12);
  tcase_add_test(tc3, s21_from_decimal_to_intTest13);
  tcase_add_test(tc3, s21_from_decimal_to_intTest14);
  tcase_add_test(tc3, s21_from_decimal_to_intTest15);
  tcase_add_test(tc3, s21_from_decimal_to_float_1);
  tcase_add_test(tc3, s21_from_decimal_to_float_2);
  tcase_add_test(tc3, s21_from_decimal_to_float_3);
  tcase_add_test(tc3, s21_from_decimal_to_float_5);
  tcase_add_test(tc3, s21_from_decimal_to_float_7);
  tcase_add_test(tc3, s21_from_decimal_to_float_8);
  tcase_add_test(tc3, s21_from_decimal_to_float_10);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest1);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest2);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest3);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest4);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest5);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest6);
  tcase_add_test(tc3, s21_from_decimal_to_floatTest7);
  tcase_add_test(tc3, s21_from_decimal_to_float_001);
  tcase_add_test(tc3, s21_from_decimal_to_float_002);
  tcase_add_test(tc3, s21_from_decimal_to_float_003);
  tcase_add_test(tc3, s21_from_decimal_to_float_004);
  tcase_add_test(tc3, s21_from_decimal_to_float_005);
  tcase_add_test(tc3, s21_from_decimal_to_float_11);
  tcase_add_test(tc3, s21_from_decimal_to_float_12);
  tcase_add_test(tc3, s21_from_decimal_to_float_13);
  tcase_add_test(tc3, s21_from_decimal_to_float_14);
  tcase_add_test(tc3, s21_from_decimal_to_float_15);

  suite_add_tcase(suite, tc4);
  tcase_add_test(tc4, s21_negate_decimal_pos);
  tcase_add_test(tc4, s21_negate_decimal_neg);
  tcase_add_test(tc4, s21_truncate_decimal_1);
  tcase_add_test(tc4, s21_truncate_decimal_15);
  tcase_add_test(tc4, s21_truncate_decimal_MAX);
  tcase_add_test(tc4, s21_round_decimal);
  tcase_add_test(tc4, s21_round_decimal_6);
  tcase_add_test(tc4, s21_round_decimal_half);
  tcase_add_test(tc4, s21_floor_decimal);
  tcase_add_test(tc4, s21_floor_decimal_neg);
  tcase_add_test(tc4, s21_floor_decimal_ZERO);
  srunner_run_all(runner, CK_ENV);
  nf = srunner_ntests_failed(runner);
  srunner_free(runner);

  return nf == 0 ? 0 : 1;
}
