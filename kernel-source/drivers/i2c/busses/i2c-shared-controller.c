/*
 * Intel SOC I2C bus sharing semphore implementation
 * Copyright (c) 2014, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <asm/iosf_mbi.h>

#define PUNIT_SEMAPHORE 0x7
static unsigned long start_time, end_time;

static int get_sem(struct device *dev, u32 *sem)
{
	u32 reg_val;
	int ret;

	ret = iosf_mbi_read(BT_MBI_UNIT_PMC, BT_MBI_BUNIT_READ, PUNIT_SEMAPHORE,
			    &reg_val);
	if (ret) {
		dev_WARN(dev, "iosf failed to read punit semaphore\n");
		return ret;
	}

	*sem = reg_val & 0x1;

	return 0;
}

static void reset_semaphore(struct device *dev)
{
	u32 data;

	if (iosf_mbi_read(BT_MBI_UNIT_PMC, BT_MBI_BUNIT_READ,
				PUNIT_SEMAPHORE, &data)) {
		dev_err(dev, "iosf failed to reset punit semaphore\n");
		return;
	}

	data = data & 0xfffffffe;
	if (iosf_mbi_write(BT_MBI_UNIT_PMC, BT_MBI_BUNIT_WRITE,
				 PUNIT_SEMAPHORE, data))
		dev_err(dev, "iosf failed to reset punit semaphore\n");
}

int i2c_acquire_ownership(struct device *dev)
{
	u32 sem = 0;
	int ret;
	int timeout = 100;

	/* host driver writes 0x2 to side band semaphore register */
	ret = iosf_mbi_write(BT_MBI_UNIT_PMC, BT_MBI_BUNIT_WRITE,
				 PUNIT_SEMAPHORE, 0x2);
	if (ret) {
		dev_WARN(dev, "iosf failed to request punit semaphore\n");
		return ret;
	}

	/* host driver waits for bit 0 to be set in semaphore register */
	while (1) {
		ret = get_sem(dev, &sem);
		if (!ret && sem) {
			start_time = jiffies;
			dev_dbg(dev, "punit semaphore acquired after %d attempts\n",
				101 - timeout);
			return 0;
		}

		usleep_range(1000, 2000);
		timeout--;
		if (timeout <= 0) {
			dev_err(dev, "punit semaphore timed out, resetting\n");
			reset_semaphore(dev);
			iosf_mbi_read(BT_MBI_UNIT_PMC, BT_MBI_BUNIT_READ,
				PUNIT_SEMAPHORE, &sem);
			dev_err(dev, "PUNIT SEM: %d\n", sem);
			WARN_ON(1);
			return -ETIMEDOUT;
		}

	}
}

int i2c_release_ownership(struct device *dev)
{
	reset_semaphore(dev);
	end_time = jiffies;
	dev_dbg(dev, "punit semaphore release call finish, held for %ldms\n",
		(end_time - start_time) * 1000 / HZ);
	return 0;
}
