#ifndef LDB_HNSW_H
#define LDB_HNSW_H

#include <postgres.h>

#include <fmgr.h>
#include <utils/relcache.h>

#if PG_VERSION_NUM < 110000
#error "Requires PostgreSQL 11+"
#endif

/* Build phases */
/* PROGRESS_CREATEIDX_SUBPHASE_INITIALIZE is 1 */
#define LDB_PROGRESS_HNSW_PHASE_IN_MEMORY_INSERT 2
#define LDB_PROGRESS_HNSW_PHASE_LOAD             3

typedef enum
{
    REAL_ARRAY,
    INT_ARRAY,
    VECTOR,
    UNKNOWN
} HnswColumnType;

/* Exported functions */
PGDLLEXPORT void _PG_init(void);
PGDLLEXPORT void _PG_fini(void);

PGDLLEXPORT Datum l2sq_dist(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum vector_l2sq_dist(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hamming_dist(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum hamming_dist_with_guard(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum cos_dist(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum cos_dist_with_guard(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum vector_cos_dist(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum lantern_reindex_external_index(PG_FUNCTION_ARGS);

HnswColumnType GetColumnTypeFromOid(Oid oid);
HnswColumnType GetIndexColumnType(Relation index);
void*          DatumGetSizedArray(Datum datum, HnswColumnType type, int dimensions);

#define LDB_UNUSED(x) (void)(x)

#endif  // LDB_HNSW_H
