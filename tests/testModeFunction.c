/*
Copyright 2014 Dominic Meiser

This file is part of BeamLaser.

BeamLaser is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BeamLaser is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with BeamLaser.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cgreen/cgreen.h>
#include <ModeFunction.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define H_BAR 1.0545718e-34
#define EPSILON_0 8.85e-12
#define SPEED_OF_LIGHT 299792458.0

#define INTERNAL_STATE_SIZE 2
#define ENSEMBLE_CAPACITY 10
static double x[ENSEMBLE_CAPACITY];
static double y[ENSEMBLE_CAPACITY];
static double z[ENSEMBLE_CAPACITY];
static double complex fx[ENSEMBLE_CAPACITY];
static double complex fy[ENSEMBLE_CAPACITY];
static double complex fz[ENSEMBLE_CAPACITY];


Describe(ModeFunction)
BeforeEach(ModeFunction) {}
AfterEach(ModeFunction) {}

Ensure(ModeFunction, canBeCreated) {
  struct BLModeFunction *modeFunction = blModeFunctionSimplifiedGaussianCreate(
      1.0e-4, 1.0e-6, 1.0e-2);
  assert_that(modeFunction, is_not_null);
  blModeFunctionDestroy(modeFunction);
}

Ensure(ModeFunction, hasNodeAtOrigin) {
  struct BLModeFunction *modeFunction = blModeFunctionSimplifiedGaussianCreate(
      1.0e-4, 1.0e-6, 1.0e-2);
  x[0] = 0.0;
  y[0] = 0.0;
  z[0] = 0.0;
  blModeFunctionEvaluate(modeFunction, 1, x, y, z, fx, fy, fz);
  assert_that_double(fx[0], is_equal_to_double(0.0));
  assert_that_double(fy[0], is_equal_to_double(0.0));
  assert_that_double(fz[0], is_equal_to_double(0.0));
  blModeFunctionDestroy(modeFunction);
}

Ensure(ModeFunction, hasAntiNodeAtQuarterLambda) {
  struct BLModeFunction *modeFunction = blModeFunctionSimplifiedGaussianCreate(
      1.0e-4, 1.0e-6, 1.0e-2);
  x[0] = 1.0e-6 / 4.0;
  y[0] = 0.0;
  z[0] = 0.0;
  blModeFunctionEvaluate(modeFunction, 1, x, y, z, fx, fy, fz);
  assert_that_double(fx[0], is_equal_to_double(0.0));
  double omega = 2.0 * M_PI * SPEED_OF_LIGHT / 1.0e-6;
  double veff = 1.0e-4 * 1.0e-4 * 1.0e-2;
  assert_that_double(fy[0],
      is_equal_to_double(
        sqrt(omega / (2 * H_BAR * EPSILON_0 * veff))));
  assert_that_double(fz[0], is_equal_to_double(0.0));
  blModeFunctionDestroy(modeFunction);
}

Ensure(ModeFunction, hasHalfAmplitudeAtLambdaOverEight) {
  struct BLModeFunction *modeFunction = blModeFunctionSimplifiedGaussianCreate(
      1.0e-4, 1.0e-6, 1.0e-2);
  x[0] = 1.0e-6 / 8.0;
  y[0] = 0.0;
  z[0] = 0.0;
  blModeFunctionEvaluate(modeFunction, 1, x, y, z, fx, fy, fz);
  assert_that_double(fx[0], is_equal_to_double(0.0));
  double omega = 2.0 * M_PI * SPEED_OF_LIGHT / 1.0e-6;
  double veff = 1.0e-4 * 1.0e-4 * 1.0e-2;
  assert_that_double(fy[0],
      is_equal_to_double(
        sqrt(0.5 * omega / (2 * H_BAR * EPSILON_0 * veff))));
  assert_that_double(fz[0], is_equal_to_double(0.0));
  blModeFunctionDestroy(modeFunction);
}

Ensure(ModeFunction, fallsOffToOneOverEAtWaist) {
  struct BLModeFunction *modeFunction = blModeFunctionSimplifiedGaussianCreate(
      1.0e-4, 1.0e-6, 1.0e-2);
  x[0] = 1.0e-6 / 4.0;
  y[0] = 1.0e-4;
  z[0] = 0.0;
  blModeFunctionEvaluate(modeFunction, 1, x, y, z, fx, fy, fz);
  assert_that_double(fx[0], is_equal_to_double(0.0));
  double omega = 2.0 * M_PI * SPEED_OF_LIGHT / 1.0e-6;
  double veff = 1.0e-4 * 1.0e-4 * 1.0e-2;
  const double euler = exp(1.0);
  assert_that_double(fy[0],
      is_equal_to_double(
        sqrt((1.0 / euler) * omega / (2 * H_BAR * EPSILON_0 * veff))));
  assert_that_double(fz[0], is_equal_to_double(0.0));
  blModeFunctionDestroy(modeFunction);
}

Ensure(ModeFunction, uniformCanBeCreated) {
  struct BLModeFunction *modeFunction =
    blModeFunctionUniformCreate(1.3, 0.0, 2.9);
  assert_that(modeFunction, is_not_null);
  blModeFunctionDestroy(modeFunction);
}

Ensure(ModeFunction, uniformHasRightAmplitude) {
  struct BLModeFunction *modeFunction =
    blModeFunctionUniformCreate(1.3, -0.4, 2.3);
  x[0] = 1.3;
  y[0] = 2.7;
  z[0] = -1.5;
  blModeFunctionEvaluate(modeFunction, 1, x, y, z, fx, fy, fz);
  assert_that_double(fx[0], is_equal_to_double(1.3));
  assert_that_double(fy[0], is_equal_to_double(-0.4));
  assert_that_double(fz[0], is_equal_to_double(2.3));
  blModeFunctionDestroy(modeFunction);
}

int main()
{
  TestSuite *suite = create_test_suite();
  add_test_with_context(suite, ModeFunction, canBeCreated);
  add_test_with_context(suite, ModeFunction, hasNodeAtOrigin);
  add_test_with_context(suite, ModeFunction, hasAntiNodeAtQuarterLambda);
  add_test_with_context(suite, ModeFunction, hasHalfAmplitudeAtLambdaOverEight);
  add_test_with_context(suite, ModeFunction, fallsOffToOneOverEAtWaist);
  add_test_with_context(suite, ModeFunction, uniformCanBeCreated);
  add_test_with_context(suite, ModeFunction, uniformHasRightAmplitude);
  int result = run_test_suite(suite, create_text_reporter());
  destroy_test_suite(suite);
  return result;
}

