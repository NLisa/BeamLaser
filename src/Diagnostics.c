#include <Diagnostics.h>
#include <stdlib.h>
#include <stdio.h>

void blDiagnosticsProcess(struct BLDiagnostics *diagnostics, int i,
    struct SimulationState *simulationState) {
  while (diagnostics) {
    diagnostics->process(i, simulationState, diagnostics->ctx);
    diagnostics = diagnostics->next;
  }
}

void blDiagnosticsProcessDestroy(struct BLDiagnostics *diagnostics) {
  if (diagnostics) {
    struct BLDiagnostics *next = diagnostics->next;
    diagnostics->destroy(diagnostics->ctx);
    free(diagnostics);
    blDiagnosticsProcessDestroy(next);
  }
}


/*
 * Field diagnostics
 */
struct BLDiagnosticsFieldStateCtx {
  int dumpPeriodicity;
};

static void blDiagnosticsFieldStateProcess(int i,
    struct SimulationState* simulationState, void *c) {
  struct BLDiagnosticsFieldStateCtx *ctx = c;
  int rank = 0;
#ifdef BL_WITH_MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
  if (rank == 0 && ((i % ctx->dumpPeriodicity) == 0)) {
    printf("%9d  %9d  %le  %le\n", i,
        simulationState->ensemble.numPtcls,
        simulationState->fieldState.q,
        simulationState->fieldState.p);
  }
}

static void blDiagnosticsFieldStateDestroy(void *ctx) {
  free(ctx);
}

struct BLDiagnostics* blDiagnosticFieldStateCreate(int dumpPeriodicity,
    struct BLDiagnostics* next) {
  struct BLDiagnostics *this = malloc(sizeof(*this));
  this->process = blDiagnosticsFieldStateProcess;
  this->destroy = blDiagnosticsFieldStateDestroy;
  struct BLDiagnosticsFieldStateCtx *ctx = malloc(sizeof(*ctx));
  ctx->dumpPeriodicity = dumpPeriodicity;
  this->ctx = ctx;
  this->next = next;
  return this;
}

/*
 * Atom diagnostics
 */
struct BLDiagnosticsPtclsCtx {
  int dumpPeriodicity;
  const char *fileName;
};

static void blDiagnosticsPtclsProcess(int i,
    struct SimulationState* simulationState, void *c) {
  struct BLDiagnosticsPtclsCtx *ctx = c;
  int rank = 0;
#ifdef BL_WITH_MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
  char fileName[1000];
  sprintf(fileName, "%s_%d_%d.txt", ctx->fileName, rank, i);
  FILE *f = fopen(fileName, "w");
  if (!f) return;
  struct BLEnsemble *ensemble = &simulationState->ensemble;
  int j;
  for (j = 0; j < ensemble->numPtcls; ++j) {
    fprintf(f, "%e ", ensemble->x[j]);
  }
  fprintf(f, "\n");
  for (j = 0; j < ensemble->numPtcls; ++j) {
    fprintf(f, "%e ", ensemble->y[j]);
  }
  fprintf(f, "\n");
  for (j = 0; j < ensemble->numPtcls; ++j) {
    fprintf(f, "%e ", ensemble->z[j]);
  }
  fprintf(f, "\n");
  for (j = 0; j < ensemble->numPtcls; ++j) {
    fprintf(f, "%e ", ensemble->vx[j]);
  }
  fprintf(f, "\n");
  for (j = 0; j < ensemble->numPtcls; ++j) {
    fprintf(f, "%e ", ensemble->vy[j]);
  }
  fprintf(f, "\n");
  for (j = 0; j < ensemble->numPtcls; ++j) {
    fprintf(f, "%e ", ensemble->vz[j]);
  }
  fprintf(f, "\n");
  fclose(f);
}

static void blDiagnosticsPtclsDestroy(void *ctx) {
  free(ctx);
}

struct BLDiagnostics* blDiagnosticPtclsCreate(int dumpPeriodicity,
    const char *fileName, struct BLDiagnostics* next) {
  struct BLDiagnostics *this = malloc(sizeof(*this));
  this->process = blDiagnosticsPtclsProcess;
  this->destroy = blDiagnosticsPtclsDestroy;
  struct BLDiagnosticsPtclsCtx *ctx = malloc(sizeof(*ctx));
  ctx->dumpPeriodicity = dumpPeriodicity;
  ctx->fileName = fileName;
  this->ctx = ctx;
  this->next = next;
  return this;
}

