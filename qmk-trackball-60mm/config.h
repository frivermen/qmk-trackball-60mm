/* Copyright 2024 Frivermen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/* PAW3204 Sensor */
#define PAW3204_SCLK_PIN GP8
#define PAW3204_SDIO_PIN GP9
#define POINTING_DEVICE_TASK_THROTTLE_MS 4            // 1 second / 4ms = 250Hz

/* Sense */
#define MOUSE_EXTENDED_REPORT

/* High Resolution Scrolling */
#define WHEEL_EXTENDED_REPORT
#define POINTING_DEVICE_HIRES_SCROLL_ENABLE
#define POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER 120   // по умолчанию
#define POINTING_DEVICE_HIRES_SCROLL_EXPONENT 0       // по умолчанию

