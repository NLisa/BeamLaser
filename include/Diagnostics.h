#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <SimulationState.h>
#include <Ensemble.h>


struct BLDiagnostics {
  void (*process)(int i, struct BLSimulationState *simulationState, void *ctx);
  void (*destroy)(void *ctx);
  void *ctx;
  struct BLDiagnostics *next;
};

void blDiagnosticsProcess(struct BLDiagnostics *diagnostics, int i,
    struct BLSimulationState *simulationState);
void blDiagnosticsDestroy(struct BLDiagnostics *diagnostics);

struct BLDiagnostics* blDiagnosticsFieldStateCreate(int dumpPeriodicity,
    struct BLDiagnostics* next);
struct BLDiagnostics* blDiagnosticsPtclsCreate(int dumpPeriodicity,
    const char *filename, struct BLDiagnostics* next);
struct BLDiagnostics* blDiagnosticsInternalStateCreate(int dumpPeriodicity,
    const char *filename, struct BLDiagnostics* next);

#endif