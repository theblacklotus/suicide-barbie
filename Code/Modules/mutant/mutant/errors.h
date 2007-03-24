#ifndef MUTANT_ERRORS_H_
#define MUTANT_ERRORS_H_

#include "cfg.h"


#define THROW_IoError(msg) (ASSERT(!"IoError"));
//throw( EIoError( IO_ERROR, msg ) );

#define THROW_MutantError(msg) (ASSERT(!"MutantError"));
// throw_mutant( msg );

#endif
