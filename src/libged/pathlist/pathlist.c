/*                         P A T H L I S T . C
 * BRL-CAD
 *
 * Copyright (c) 2008-2025 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file libged/pathlist.c
 *
 * The pathlist command.
 *
 */

#include "common.h"

#include <string.h>

#include "bu/cmd.h"

#include "../ged_private.h"


static int pathListNoLeaf = 0;


static union tree *
pathlist_leaf_func(struct db_tree_state *UNUSED(tsp), const struct db_full_path *pathp, struct rt_db_internal *ip, void *client_data)
{
    struct ged *gedp = (struct ged *)client_data;
    char *str;

    RT_CK_FULL_PATH(pathp);
    RT_CK_DB_INTERNAL(ip);

    if (pathListNoLeaf) {
	struct db_full_path pp;
	db_full_path_init(&pp);
	db_dup_full_path(&pp, pathp);
	--pp.fp_len;
	str = db_path_to_string(&pp);
	bu_vls_printf(gedp->ged_result_str, " %s", str);
	db_free_full_path(&pp);
    } else {
	str = db_path_to_string(pathp);
	bu_vls_printf(gedp->ged_result_str, " %s", str);
    }

    bu_free((void *)str, "path string");
    return TREE_NULL;
}


int
ged_pathlist_core(struct ged *gedp, int argc, const char *argv[])
{
    static const char *usage = "name";

    GED_CHECK_DATABASE_OPEN(gedp, BRLCAD_ERROR);
    GED_CHECK_ARGC_GT_0(gedp, argc, BRLCAD_ERROR);

    /* initialize result */
    bu_vls_trunc(gedp->ged_result_str, 0);

    /* must be wanting help */
    if (argc == 1) {
	bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	return GED_HELP;
    }

    if (3 < argc) {
	bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	return BRLCAD_ERROR;
    }

    pathListNoLeaf = 0;

    if (argc == 3) {
	if (BU_STR_EQUAL(argv[1], "-noleaf"))
	    pathListNoLeaf = 1;

	++argv;
	--argc;
    }

    struct rt_wdb *wdbp = wdb_dbopen(gedp->dbip, RT_WDB_TYPE_DB_DEFAULT);
    if (db_walk_tree(gedp->dbip, argc-1, (const char **)argv+1, 1,
		     &wdbp->wdb_initial_tree_state,
		     0, 0, pathlist_leaf_func, (void *)gedp) < 0) {
	bu_vls_printf(gedp->ged_result_str, "ged_pathlist_core: db_walk_tree() error");
	return BRLCAD_ERROR;
    }

    return BRLCAD_OK;
}


#ifdef GED_PLUGIN
#include "../include/plugin.h"
struct ged_cmd_impl pathlist_cmd_impl = {
    "pathlist",
    ged_pathlist_core,
    GED_CMD_DEFAULT
};

const struct ged_cmd pathlist_cmd = { &pathlist_cmd_impl };
const struct ged_cmd *pathlist_cmds[] = { &pathlist_cmd, NULL };

static const struct ged_plugin pinfo = { GED_API,  pathlist_cmds, 1 };

COMPILER_DLLEXPORT const struct ged_plugin *ged_plugin_info(void)
{
    return &pinfo;
}
#endif /* GED_PLUGIN */

/*
 * Local Variables:
 * mode: C
 * tab-width: 8
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
