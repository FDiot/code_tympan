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


#include "TYWidget.h"

#include "Tympan/GraphicIHM/DataManagerIHM/ui_TYRouteWidget.h" // Generated by Qt's uic

class TYRoute;
class TYAcousticLineWidget;

/**
 * \class TYRouteWidget
 * \brief objet IHM pour une route
 */
class TYRouteWidget
    : public TYWidget /* TYWidget is a typedef to QWidget*/
    , private Ui::Form
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
};


#endif // __TY_ROUTE_WIDGET__
