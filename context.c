#include "context.h"

/*
 * Context switch routine implemented in assembly.
 */
extern void switch_context(context_t **old, context_t *new);