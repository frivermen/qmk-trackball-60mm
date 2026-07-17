#include QMK_KEYBOARD_H
#include "bootloader.h"

enum custom_keycodes {
    DRAG_SCROLL = SAFE_RANGE
};

bool set_scrolling = false;
bool hold_mode = false;              // true = удержание, false = переключение
uint8_t pressed_buttons = 0;
static bool suppress = false;       // флаг подавления клика при выходе из скролла
static bool drag_scroll_held = false; // отслеживаем, зажат ли DRAG_SCROLL

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = -mouse_report.y;  // инвертированная ось Y
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool pressed = record->event.pressed;

    switch (keycode) {
        case MS_BTN1:
        case MS_BTN2:
        case MS_BTN3:
            if (pressed) {
                pressed_buttons++;
                if (pressed_buttons == 4) {
                    bootloader_jump();
                    return false;
                }
                // Комбинация для переключения режима: DRAG_SCROLL зажат + MS_BTN2
                if (drag_scroll_held && keycode == MS_BTN3) {
                    hold_mode = !hold_mode;
                    set_scrolling = false;
                    suppress = false;
                    return false;   // подавляем сам клик MS_BTN3
                }

                // При скролле в режиме переключения – выключаем скролл и подавляем клик
                if (set_scrolling && !hold_mode) {
                    set_scrolling = false;
                    suppress = true;
                    return false;
                }
                return true;   // обычный клик
            } else {
                // Отпускание
                pressed_buttons--;
                // Если это отпускание MS_BTN3 после переключения режима – подавляем
                if (keycode == MS_BTN3 && drag_scroll_held) {
                    return false;
                }
                if (suppress) {
                    suppress = false;
                    return false;
                }
                return true;
            }

        case DRAG_SCROLL:
            if (pressed) {
                pressed_buttons++;
                if (pressed_buttons == 4) {
                    bootloader_jump();
                    return false;
                }
                drag_scroll_held = true;
                if (hold_mode) {
                    set_scrolling = true;        // удержание: включить скролл
                } else {
                    set_scrolling = !set_scrolling; // переключение
                }
                return false;   // DRAG_SCROLL не отправляет код
            } else {
                pressed_buttons--;
                drag_scroll_held = false;
                if (hold_mode) {
                    set_scrolling = false;       // удержание: отпустили – скролл выкл
                }
                return false;
            }

        default:
            return true;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        MS_BTN1,
        MS_BTN2,
        MS_BTN3,
        DRAG_SCROLL
    )
};
