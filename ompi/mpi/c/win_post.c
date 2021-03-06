/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/runtime/params.h"
#include "ompi/communicator/communicator.h"
#include "ompi/errhandler/errhandler.h"
#include "ompi/win/win.h"
#include "ompi/mca/osc/osc.h"

#if OMPI_BUILD_MPI_PROFILING
#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_Win_post = PMPI_Win_post
#endif
#define MPI_Win_post PMPI_Win_post
#endif

static const char FUNC_NAME[] = "MPI_Win_post";


int MPI_Win_post(MPI_Group group, int assert, MPI_Win win)
{
    int rc;

    if (MPI_PARAM_CHECK) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);

        if (ompi_win_invalid(win)) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_WIN, FUNC_NAME);
        } else if (0 != (assert & ~(MPI_MODE_NOCHECK | MPI_MODE_NOSTORE |
                                    MPI_MODE_NOPUT))) {
            return OMPI_ERRHANDLER_INVOKE(win, MPI_ERR_ASSERT, FUNC_NAME);
        }
    }

    OPAL_CR_ENTER_LIBRARY();

    rc = win->w_osc_module->osc_post(group, assert, win);
    OMPI_ERRHANDLER_RETURN(rc, win, rc, FUNC_NAME);
}
