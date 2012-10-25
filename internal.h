#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#ifndef _FILE_OFFSET_BITS
# define _FILE_OFFSET_BITS 64
#endif

#ifndef _LARGEFILE_SOURCE
# define _LARGEFILE_SOURCE 1
#endif

#ifndef _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "wpio.h"

#include "file.h"
#include "wpes.h"

#endif // _INTERNAL_H_
