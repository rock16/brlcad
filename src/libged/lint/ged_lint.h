/*                    G E D _ L I N T . H
 * BRL-CAD
 *
 * Copyright (c) 2024 United States Government as represented by
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
/** @file ged_lint.h
 *
 * Private header for libged lint cmd.
 *
 */

#ifndef LIBGED_LINT_GED_PRIVATE_H
#define LIBGED_LINT_GED_PRIVATE_H

#include "common.h"

#include <set>
#include <string>
#include <map>
#include <time.h>
#include "json.hpp"

extern "C" {
#include "bu/ptbl.h"
#include "bu/vls.h"
#include "raytrace.h"
#include "rt/primitives/bot.h"
}
#include "../ged_private.h"

class lint_data {
    public:
	nlohmann::json j;
	struct ged *gedp;

	std::string filter;

	std::string summary(int verbosity);

	//std::vector<std::string> problem_types(std::string &filter);
	//std::vector<std::string> problem_objects(std::string &filter);
};

extern void bot_checks(lint_data *cdata, struct directory *dp, struct rt_bot_internal *bot, int verbose);
extern int _ged_cyclic_check(lint_data *cdata, int argc, struct directory **dpa);
extern int _ged_invalid_shape_check(lint_data *ldata, int argc, struct directory **dpa, int verbosity);
extern int _ged_missing_check(lint_data *mdata, int argc, struct directory **dpa);

#endif /* LIBGED_LINT_GED_PRIVATE_H */

/** @} */
/*
 * Local Variables:
 * mode: C
 * tab-width: 8
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
