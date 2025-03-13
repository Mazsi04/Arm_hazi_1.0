/*
 * WidgetSelection.h
 *
 *  Created on: Mar 13, 2025
 *      Author: mazsi
 */

#ifndef WIDGETSELECTION_H_
#define WIDGETSELECTION_H_

typedef enum {
    WIDGET_SELECT_DEVICE,
    WIDGET_WIFI,
    WIDGET_BLE,
    WIDGET_ROOM_SELECT,
    WIDGET_SETTINGS,
    WIDGET_ABOUT,
    WIDGET_COUNT // Összes widget számának tárolására
}WidgetSelection;

#endif /* WIDGETSELECTION_H_ */
