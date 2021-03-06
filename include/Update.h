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
#ifndef UPDATE_H
#define UPDATE_H

#include <SimulationState.h>

/*
 * Implementation notes:
 *
 * - Still need to add a compound update
 *
 * */
struct BLUpdate {
  void (*takeStep)(double t, double dt, struct BLSimulationState *state, void *ctx);
  void (*destroy)(void *ctx);
  void *ctx;
};

void blUpdateTakeStep(struct BLUpdate *update, double t, double dt,
    struct BLSimulationState *state);
void blUpdateDestroy(struct BLUpdate *update);

struct BLUpdate *blUpdateIdentityCreate();

#endif

