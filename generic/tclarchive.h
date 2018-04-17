/*
 * Copyright (c) <2018>, <Danilo Chang>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For C++ compilers, use extern "C"
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <archive.h>
#include <archive_entry.h>

/*
 * OK, I know it is ugly. It needs better solution.
 */
#if defined(_WIN32) && !defined(PATH_MAX)
# define PATH_MAX 1024
#else
#include <limits.h>
#endif

int ARCHIVE_CREATE(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int ARCHIVE_EXTRACT(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int ARCHIVE_VERSION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * Only the _Init function is exported.
 */

extern DLLEXPORT int	Archive_Init(Tcl_Interp * interp);

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

