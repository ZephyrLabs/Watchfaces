#include <libs/lvgl/lvgl.h>
#include "WatchFaceAnalog.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"

#include <cmath>

//LV_IMG_DECLARE(bg_clock);

using namespace Pinetime::Applications::Screens;

#define HOUR_LENGTH   60
#define MINUTE_LENGTH 70
#define SECOND_LENGTH 90
#define PI            3.14159265358979323846
#define dot_size      16

// ##
static int16_t coordinate_x_relocate(int16_t x) {
  return ((x) + LV_HOR_RES / 2);
}

// ##
static int16_t coordinate_y_relocate(int16_t y) {
  return (((y) -LV_HOR_RES / 2) < 0) ? (0 - ((y) -LV_HOR_RES / 2)) : ((y) -LV_HOR_RES / 2);
}

WatchFaceAnalog::WatchFaceAnalog(Pinetime::Applications::DisplayApp* app,
                                 Controllers::DateTime& dateTimeController,
                                 Controllers::Battery& batteryController,
                                 Controllers::Ble& bleController,
                                 Controllers::NotificationManager& notificatioManager,
                                 Controllers::Settings& settingsController)
  : Screen(app),
    currentDateTime {{}},
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificatioManager {notificatioManager},
    settingsController {settingsController} {
  settingsController.SetClockFace(1);

  sHour = 99;
  sMinute = 99;
  sSecond = 99;
  //create bars at 3,6,9, and 2 bars at 12
  three_bar = lv_line_create(lv_scr_act(), nullptr);
  six_bar = lv_line_create(lv_scr_act(), nullptr);
  nine_bar = lv_line_create(lv_scr_act(), nullptr);
  twelve_bar_1 = lv_line_create(lv_scr_act(), nullptr);
  twelve_bar_2 = lv_line_create(lv_scr_act(), nullptr);
  //create dots for other hours
  one_dot = lv_led_create(lv_scr_act(), nullptr);
  two_dot = lv_led_create(lv_scr_act(), nullptr);
  four_dot = lv_led_create(lv_scr_act(), nullptr);
  five_dot = lv_led_create(lv_scr_act(), nullptr);
  seven_dot = lv_led_create(lv_scr_act(), nullptr);
  eight_dot = lv_led_create(lv_scr_act(), nullptr);
  ten_dot = lv_led_create(lv_scr_act(), nullptr);
  eleven_dot = lv_led_create(lv_scr_act(), nullptr);

  lv_style_init(&dots_style);
  lv_style_set_radius(&dots_style, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);   //corner radius, not size
  lv_style_set_bg_color(&dots_style, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_style_set_bg_opa(&dots_style, LV_STATE_DEFAULT, LV_OPA_100);

  lv_style_init(&bars_style);
  lv_style_set_line_width(&bars_style, LV_STATE_DEFAULT, 8);
  lv_style_set_line_color(&bars_style, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_style_set_line_rounded(&bars_style, LV_STATE_DEFAULT, true);
  lv_style_set_radius(&bars_style, LV_STATE_DEFAULT, 4);

  lv_obj_add_style(one_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(two_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(four_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(five_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(seven_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(eight_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(ten_dot, LV_LED_PART_MAIN, &dots_style);
  lv_obj_add_style(eleven_dot, LV_LED_PART_MAIN, &dots_style);

  lv_obj_add_style(three_bar, LV_LINE_PART_MAIN, &bars_style);
  lv_obj_add_style(six_bar, LV_LINE_PART_MAIN, &bars_style);
  lv_obj_add_style(nine_bar, LV_LINE_PART_MAIN, &bars_style);
  lv_obj_add_style(twelve_bar_1, LV_LINE_PART_MAIN, &bars_style);
  lv_obj_add_style(twelve_bar_2, LV_LINE_PART_MAIN, &bars_style);

  lv_obj_align(one_dot, NULL, LV_ALIGN_CENTER, 110 * sin(1 * 30 * PI / 180), -110 * cos(1 * 30 * PI / 180));
  lv_obj_align(two_dot, NULL, LV_ALIGN_CENTER, 110 * sin(2 * 30 * PI / 180), -110 * cos(2 * 30 * PI / 180));
  lv_obj_align(four_dot, NULL, LV_ALIGN_CENTER, 110 * sin(4 * 30 * PI / 180), -110 * cos(4 * 30 * PI / 180));
  lv_obj_align(five_dot, NULL, LV_ALIGN_CENTER, 110 * sin(5 * 30 * PI / 180), -110 * cos(5 * 30 * PI / 180));
  lv_obj_align(seven_dot, NULL, LV_ALIGN_CENTER, 110 * sin(7 * 30 * PI / 180), -110 * cos(7 * 30 * PI / 180));
  lv_obj_align(eight_dot, NULL, LV_ALIGN_CENTER, 110 * sin(8 * 30 * PI / 180), -110 * cos(8 * 30 * PI / 180));
  lv_obj_align(ten_dot, NULL, LV_ALIGN_CENTER, 110 * sin(10 * 30 * PI / 180), -110 * cos(10 * 30 * PI / 180));
  lv_obj_align(eleven_dot, NULL, LV_ALIGN_CENTER, 110 * sin(11 * 30 * PI / 180), -110 * cos(11 * 30 * PI / 180));

  bar_point_horitz[0].x = 0;
  bar_point_horitz[0].y = 0;
  bar_point_horitz[1].x = 24;
  bar_point_horitz[1].y = 0;
  lv_line_set_points(three_bar, bar_point_horitz, 2);
  lv_line_set_points(nine_bar, bar_point_horitz, 2);
  bar_point_vert[0].x = 0;
  bar_point_vert[0].y = 0;
  bar_point_vert[1].x = 0;
  bar_point_vert[1].y = 24;
  lv_line_set_points(six_bar, bar_point_vert, 2);
  lv_line_set_points(twelve_bar_1, bar_point_vert, 2);
  lv_line_set_points(twelve_bar_2, bar_point_vert, 2);

  lv_obj_align(three_bar, NULL, LV_ALIGN_CENTER, 100, 4);
  lv_obj_align(six_bar, NULL, LV_ALIGN_CENTER, 2, 100);
  lv_obj_align(nine_bar, NULL, LV_ALIGN_CENTER, -100, 4);
  lv_obj_align(twelve_bar_1, NULL, LV_ALIGN_CENTER, -6, -100);
  lv_obj_align(twelve_bar_2, NULL, LV_ALIGN_CENTER, 10, -100);

  lv_obj_set_size(one_dot, dot_size, dot_size);
  lv_obj_set_size(two_dot, dot_size, dot_size);
  lv_obj_set_size(four_dot, dot_size, dot_size);
  lv_obj_set_size(five_dot, dot_size, dot_size);
  lv_obj_set_size(seven_dot, dot_size, dot_size);
  lv_obj_set_size(eight_dot, dot_size, dot_size);
  lv_obj_set_size(ten_dot, dot_size, dot_size);
  lv_obj_set_size(eleven_dot, dot_size, dot_size);

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryIcon, Symbols::batteryHalf);
  lv_obj_align(batteryIcon, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -8, -4);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  lv_obj_align(notificationIcon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 8, -4);

  // Date - Day / Week day

  label_date = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text_fmt(label_date, "%s", dateTimeController.DayOfWeekShortToString());
  lv_label_set_align(label_date, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(label_date, NULL, LV_ALIGN_CENTER, 40, 0);

  label_day = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(label_day, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text_fmt(label_day, "%02i", dateTimeController.Day());
  lv_label_set_align(label_day, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(label_day, NULL, LV_ALIGN_CENTER, -60, 0);

  min_bar_1 = lv_line_create(lv_scr_act(), NULL);
  hour_bar_1 = lv_line_create(lv_scr_act(), NULL);
  second_body = lv_line_create(lv_scr_act(), NULL);

  lv_style_init(&second_line_style);
  lv_style_set_line_width(&second_line_style, LV_STATE_DEFAULT, 3);
  lv_style_set_line_color(&second_line_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_style_set_line_rounded(&second_line_style, LV_STATE_DEFAULT, true);
  lv_obj_add_style(second_body, LV_LINE_PART_MAIN, &second_line_style);

  lv_style_init(&min_hour_style);
  lv_style_set_line_width(&min_hour_style, LV_STATE_DEFAULT, 3);
  lv_style_set_line_color(&min_hour_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_style_set_line_rounded(&min_hour_style, LV_STATE_DEFAULT, true);
  lv_obj_add_style(min_bar_1, LV_LINE_PART_MAIN, &min_hour_style);

  //hands (ordered later to overlap hands)
  seconds_dot = lv_led_create(lv_scr_act(), nullptr);
  min_bar_1 = lv_line_create(lv_scr_act(), nullptr);
  min_bar_2 = lv_line_create(lv_scr_act(), nullptr);
  hour_bar_1 = lv_line_create(lv_scr_act(), nullptr);
  hour_bar_2 = lv_line_create(lv_scr_act(), nullptr);

  min_tri_1 = lv_line_create(lv_scr_act(), nullptr);
  min_tri_2 = lv_line_create(lv_scr_act(), nullptr);
  min_tri_3 = lv_line_create(lv_scr_act(), nullptr);
  hour_tri_1 = lv_line_create(lv_scr_act(), nullptr);
  hour_tri_2 = lv_line_create(lv_scr_act(), nullptr);
  hour_tri_3 = lv_line_create(lv_scr_act(), nullptr);

  lv_style_init(&tri_style);
  lv_style_set_line_width(&tri_style, LV_STATE_DEFAULT, 2);
  lv_style_set_line_color(&tri_style, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_style_set_line_rounded(&tri_style, LV_STATE_DEFAULT, true);

  lv_obj_add_style(seconds_dot, LV_LED_PART_MAIN, &dots_style);

  lv_obj_add_style(min_bar_1, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(min_bar_2, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(hour_bar_1, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(hour_bar_2, LV_LED_PART_MAIN, &tri_style);

  lv_obj_add_style(min_tri_1, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(min_tri_2, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(min_tri_3, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(hour_tri_1, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(hour_tri_2, LV_LED_PART_MAIN, &tri_style);
  lv_obj_add_style(hour_tri_3, LV_LED_PART_MAIN, &tri_style);

  lv_obj_set_size(seconds_dot, (dot_size * .5) , (dot_size * .5));

  UpdateClock();
}

WatchFaceAnalog::~WatchFaceAnalog() {

  lv_style_reset(&min_hour_style);
  lv_style_reset(&second_line_style);

  lv_obj_clean(lv_scr_act());
}

void WatchFaceAnalog::UpdateClock() {

  hour = dateTimeController.Hours();
  minute = dateTimeController.Minutes();
  second = dateTimeController.Seconds();

  if (sMinute != minute) {
    minute_point_1[0].x = coordinate_x_relocate(5 * sin((minute - 1) * 6 * PI / 180));
    minute_point_1[0].y = coordinate_y_relocate(5 * cos((minute - 1) * 6 * PI / 180));
    minute_point_1[1].x = coordinate_x_relocate(MINUTE_LENGTH * sin((minute - 1) * 6 * PI / 180));
    minute_point_1[1].y = coordinate_y_relocate(MINUTE_LENGTH * cos((minute - 1) * 6 * PI / 180));

    minute_point_2[0].x = coordinate_x_relocate(5 * sin((minute + 1) * 6 * PI / 180));
    minute_point_2[0].y = coordinate_y_relocate(5 * cos((minute + 1) * 6 * PI / 180));
    minute_point_2[1].x = coordinate_x_relocate(MINUTE_LENGTH * sin((minute + 1) * 6 * PI / 180));
    minute_point_2[1].y = coordinate_y_relocate(MINUTE_LENGTH * cos((minute + 1) * 6 * PI / 180));

    //base
    minute_tri_set_1[0].x = coordinate_x_relocate(MINUTE_LENGTH * sin((minute - 1) * 6 * PI / 180));
    minute_tri_set_1[0].y = coordinate_y_relocate(MINUTE_LENGTH * cos((minute - 1) * 6 * PI / 180));
    minute_tri_set_1[1].x = coordinate_x_relocate(MINUTE_LENGTH * sin((minute + 1) * 6 * PI / 180));
    minute_tri_set_1[1].y = coordinate_y_relocate(MINUTE_LENGTH * cos((minute + 1) * 6 * PI / 180));
    //left of base
    minute_tri_set_2[0].x = coordinate_x_relocate(MINUTE_LENGTH * sin((minute - 1) * 6 * PI / 180));
    minute_tri_set_2[0].y = coordinate_y_relocate(MINUTE_LENGTH * cos((minute - 1) * 6 * PI / 180));
    minute_tri_set_2[1].x = coordinate_x_relocate((MINUTE_LENGTH * 1.2) * sin(minute * 6 * PI / 180));
    minute_tri_set_2[1].y = coordinate_y_relocate((MINUTE_LENGTH * 1.2) * cos(minute * 6 * PI / 180));
    //right of base
    minute_tri_set_3[0].x = coordinate_x_relocate((MINUTE_LENGTH * 1.2) * sin(minute * 6 * PI / 180));
    minute_tri_set_3[0].y = coordinate_y_relocate((MINUTE_LENGTH * 1.2) * cos(minute * 6 * PI / 180));
    minute_tri_set_3[1].x = coordinate_x_relocate(MINUTE_LENGTH * sin((minute + 1) * 6 * PI / 180));
    minute_tri_set_3[1].y = coordinate_y_relocate(MINUTE_LENGTH * cos((minute + 1) * 6 * PI / 180));

    lv_line_set_points(min_bar_1, minute_point_1, 2);
    lv_line_set_points(min_bar_2, minute_point_2, 2);
    lv_line_set_points(min_tri_1, minute_tri_set_1, 2);
    lv_line_set_points(min_tri_2, minute_tri_set_2, 2);
    lv_line_set_points(min_tri_3, minute_tri_set_3, 2);
  }

  if (sHour != hour || sMinute != minute) {
    sHour = hour;
    sMinute = minute;
    hour_point_1[0].x = coordinate_x_relocate(5 * sin((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_point_1[0].y = coordinate_y_relocate(5 * cos((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_point_1[1].x = coordinate_x_relocate(HOUR_LENGTH * sin((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_point_1[1].y = coordinate_y_relocate(HOUR_LENGTH * cos((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180));

    hour_point_2[0].x = coordinate_x_relocate(5 * sin((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_point_2[0].y = coordinate_y_relocate(5 * cos((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_point_2[1].x = coordinate_x_relocate(HOUR_LENGTH * sin((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_point_2[1].y = coordinate_y_relocate(HOUR_LENGTH * cos((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180));

    hour_tri_set_1[0].x = coordinate_x_relocate(HOUR_LENGTH * sin((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180)); //end 1
    hour_tri_set_1[0].y = coordinate_y_relocate(HOUR_LENGTH * cos((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180)); 
    hour_tri_set_1[1].x = coordinate_x_relocate(HOUR_LENGTH * sin((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180)); //end 2
    hour_tri_set_1[1].y = coordinate_y_relocate(HOUR_LENGTH * cos((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180));

    hour_tri_set_2[0].x = coordinate_x_relocate(HOUR_LENGTH * sin((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180)); //tri 1-1
    hour_tri_set_2[0].y = coordinate_y_relocate(HOUR_LENGTH * cos((((hour > 12 ? hour - 11.75 : hour + .25) * 30) + (minute * 0.5)) * PI / 180));
    hour_tri_set_2[1].x = coordinate_x_relocate((HOUR_LENGTH * 1.2) * sin((((hour > 12 ? hour - 12 : hour) * 30) + (minute * 0.5)) * PI / 180)); //tri 1-2
    hour_tri_set_2[1].y = coordinate_y_relocate((HOUR_LENGTH * 1.2) * cos((((hour > 12 ? hour - 12 : hour) * 30) + (minute * 0.5)) * PI / 180));

    hour_tri_set_3[0].x = coordinate_x_relocate((HOUR_LENGTH * 1.2) * sin((((hour > 12 ? hour - 12 : hour) * 30) + (minute * 0.5)) * PI / 180)); //tri 2-2
    hour_tri_set_3[0].y = coordinate_y_relocate((HOUR_LENGTH * 1.2) * cos((((hour > 12 ? hour - 12 : hour) * 30) + (minute * 0.5)) * PI / 180));
    hour_tri_set_3[1].x = coordinate_x_relocate(HOUR_LENGTH * sin((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180)); //end 2
    hour_tri_set_3[1].y = coordinate_y_relocate(HOUR_LENGTH * cos((((hour > 12 ? hour - 12.25 : hour - .25) * 30) + (minute * 0.5)) * PI / 180));

    lv_line_set_points(hour_bar_1, hour_point_1, 2);
    lv_line_set_points(hour_bar_2, hour_point_2, 2);
    lv_line_set_points(hour_tri_1, hour_tri_set_1, 2);
    lv_line_set_points(hour_tri_2, hour_tri_set_2, 2);
    lv_line_set_points(hour_tri_3, hour_tri_set_3, 2);
  }

  if (sSecond != second) {
    sSecond = second;
    second_point[0].x = coordinate_x_relocate(20 * sin((180 + second * 6) * PI / 180));
    second_point[0].y = coordinate_y_relocate(20 * cos((180 + second * 6) * PI / 180));
    second_point[1].x = coordinate_x_relocate(SECOND_LENGTH * sin(second * 6 * PI / 180));
    second_point[1].y = coordinate_y_relocate(SECOND_LENGTH * cos(second * 6 * PI / 180));
    lv_line_set_points(second_body, second_point, 2);
    lv_obj_align(seconds_dot, NULL, LV_ALIGN_CENTER, -(SECOND_LENGTH - 20) * sin((180 + second * 6) * PI / 180), (SECOND_LENGTH - 20) * cos((180 + second * 6) * PI / 180));
  }
}

bool WatchFaceAnalog::Refresh() {

  batteryPercentRemaining = batteryController.PercentRemaining();
  if (batteryPercentRemaining.IsUpdated()) {
    auto batteryPercent = batteryPercentRemaining.Get();
    lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
  }

  notificationState = notificatioManager.AreNewNotificationsAvailable();

  if (notificationState.IsUpdated()) {
    if (notificationState.Get() == true)
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
    else
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  }

  currentDateTime = dateTimeController.CurrentDateTime();

  if (currentDateTime.IsUpdated()) {

    month = dateTimeController.Month();
    day = dateTimeController.Day();
    dayOfWeek = dateTimeController.DayOfWeek();

    UpdateClock();

    if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date, "%s", dateTimeController.DayOfWeekShortToString());
      lv_label_set_text_fmt(label_day, "%02i", day);

      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }

  return true;
}