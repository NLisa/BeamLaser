#ifndef PARTICLE_SOURCE_H
#define PARTICLE_SOURCE_H

#include <Utilities.h>


/* Interface for ParticleSources */
struct ParticleSource {
  /* Number of particles produced by this source */
  int (*getNumParticles)(void *ctx);
  /* Produce particles */
  void (*createParticles)(double *x, double *y, double *z,
      double *vx, double *vy, double *vz, double *internalState, void *ctx);
  /* Destroy this sources context */
  void (*destroy)(void *ctx);
  /* Data needed by this particle source */
  void *ctx;
  /* The next particle source */
  struct ParticleSource *next;
};

/*
 * Public interface functions
*/

/* Returns the total number of particles produced by the particleSource */
int blParticleSourceGetNumParticles(struct ParticleSource *particleSource);
/* Create particles for all sources in this list*/
void blParticleSourceCreateParticles(struct ParticleSource *particleSource,
    double *x, double *y, double *z, double *vx, double *vy, double *vz,
    double *internalState);
/* Destroy the sources in the particleSource list */
void blParticleSourceDestroy(struct ParticleSource *particleSource);

/** Constructor for a spatially uniform particle source
 *
 * @param volume The cubic volume in which the particles are created
 * @param numPtcls   The number of particles produced by each call to
 *               blParticleSourceCreateParticles
 * @param vbar   Three vector with average velocity of particles
 * @param deltaV Three vector with standard deviations of particle
 *               velocities
 * @param next   The next particle source.  Use 0 if there are no other
 *               particle sources.
 */
struct ParticleSource *blParticleSourceUniformCreate(
    struct BBox volume, int numPtcls,
    double *vbar, double *deltaV,
    int internalStateSize, double *internalState,
    struct ParticleSource *next);

#endif

