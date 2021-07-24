/* Copyright (c) 2017 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/device.h>
#include <linux/power_supply.h>
#include <linux/pmic-voter.h>
#include "smb-lib.h"
#include "smb-reg.h"
#include "storm-watch.h"

static int set_eb_param(const char *val, const struct kernel_param *kp)
{
	int rv = param_set_int(val, kp);

	if (rv)
		return rv;

	if (the_chip) {
		the_chip->mmi.update_eb_params++;
		vote(the_chip->awake_votable, HEARTBEAT_VOTER, true, true);
		cancel_delayed_work(&the_chip->mmi.heartbeat_work);
		schedule_delayed_work(&the_chip->mmi.heartbeat_work,
				      msecs_to_jiffies(HEARTBEAT_EB_MS));
	}

	return 0;
}

static struct kernel_param_ops eb_ops = {
	.set = set_eb_param,
	.get = param_get_int,
};

int eb_rechrg_start_soc = 70;
module_param_cb(eb_rechrg_start_soc, &eb_ops, &eb_rechrg_start_soc, 0644);
int eb_rechrg_stop_soc = 80;
module_param_cb(eb_rechrg_stop_soc, &eb_ops, &eb_rechrg_stop_soc, 0644);
int eb_attach_start_soc = 100;
module_param_cb(eb_attach_start_soc, &eb_ops, &eb_attach_start_soc, 0644);
int eb_attach_stop_soc = 100;
module_param_cb(eb_attach_stop_soc, &eb_ops, &eb_attach_stop_soc, 0644);
int eb_low_start_soc = 16;
module_param_cb(eb_low_start_soc, &eb_ops, &eb_low_start_soc, 0644);
int eb_low_stop_soc = 100;
module_param_cb(eb_low_stop_soc, &eb_ops, &eb_low_stop_soc, 0644);
int eb_on_sw = 1;
module_param_cb(eb_on_sw, &eb_ops, &eb_on_sw, 0644);
