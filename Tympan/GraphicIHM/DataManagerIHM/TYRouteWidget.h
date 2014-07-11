/*
 * Copyright (C) <2012> <EDF-R&D> <FRANCE>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/**
 * \file TYRouteWidget.h
 * \brief outil IHM pour une route (fichier header)
 * \author Projet_Tympan
 *
 *
 *
 *
 *
 */

#ifndef __TY_ROUTE_WIDGET__
#define __TY_ROUTE_WIDGET__

#include <QDialog>

#include "TYWidget.h"

#include "Tympan/GraphicIHM/DataManagerIHM/ui_TYRouteWidget.h" // Generated by Qt's uic
#include "Tympan/GraphicIHM/DataManagerIHM/ui_TYRouteWidget_AADT.h" // Generated by Qt's uic
#include "Tympan/models/business/infrastructure/TYRoute.h"
#include "Tympan/models/business/acoustique/TYTrafic.h"

class TYAcousticLineWidget;

class TYRouteWidget_AADT_Dialog
    : public QDialog
    , private Ui::AADT_Dialog
{
    Q_OBJECT

public:
    TYRouteWidget_AADT_Dialog(QWidget* _pParent = NULL);
};

/**
 * \class TYRouteWidget
 * \brief objet IHM pour une route
 */
class TYRouteWidget
    : public TYWidget /* TYWidget is a typedef to QWidget*/
    , private Ui::RouteWidget
{
    Q_OBJECT

    TY_DECL_METIER_WIDGET(TYRoute)

    // Methodes
public:
    /**
     * Constructeur.
     */
    TYRouteWidget(TYRoute* pElement, QWidget* _pParent = NULL);
    /**
     * Destructeur.
     */
    virtual ~TYRouteWidget();


public slots:
    virtual void updateContent();
    virtual void apply();


protected:

    TYAcousticLineWidget* _elmW;

    QComboBox*  q_RoadSurfaceType_Combo;
    QComboBox*  q_RoadSurfaceFormulation_Combo;
    QCheckBox*  q_RoadSurfaceDraining_Check;
    QSpinBox*   q_RoadSurfaceAge_Spin;
    QComboBox*  q_RoadFlowType_Combo;
    QSpinBox*   q_RoadSpeed_Spin[TYRoute::NB_TRAFFIC_REGIMES][TYTrafic::NB_VEHICLE_TYPES];
    QSpinBox*   q_RoadFlow_Spin[TYRoute::NB_TRAFFIC_REGIMES][TYTrafic::NB_VEHICLE_TYPES];
    QPushButton* q_AADT_Push;

    QButtonGroup* p_ModeCalcul_ButtonGroup;
    QPushButton*   q_EditSpectre_Button[TYRoute::NB_TRAFFIC_REGIMES];

protected slots:
    void display_AADT_dialog();
    void checkComputationMode(int);
    void setSpeedBoxEnabled(bool enabled = true);
    void setFlowBoxEnabled(bool enabled = true);
    void setSpectresEditable(bool enabled = true);
    void displaySpectrumDay() {displaySpectrum(TYRoute::Day);};
    void displaySpectrumEvening() {displaySpectrum(TYRoute::Evening);};;
    void displaySpectrumNight() {displaySpectrum(TYRoute::Night);};;
    void onRoadSurfaceChange(int);
    void onRoadSurfaceFormulationChange(int);

private:
    void apply_road_surface();
    void update_road_surface();
    void apply_road_traffic();
    void update_road_traffic();

    bool spectrum_read_only;
    void displaySpectrum(TYRoute::TrafficRegimes);
};


#endif // __TY_ROUTE_WIDGET__
