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
 * \file TYCalculGraphic.cpp
 * \brief Representation graphique d'un calcul
 */


#include "Tympan/models/business/TYProjet.h"
#include "Tympan/models/business/TYCalcul.h"
#include "Tympan/gui/gl/TYRayGraphic.h"
#include "TYCalculGraphic.h"

TYCalculGraphic::TYCalculGraphic(TYCalcul* pElement) :
    TYElementGraphic(pElement)
{
}

void TYCalculGraphic::update(bool force /*=false*/)
{
    TYElementGraphic::update(force);
}

void TYCalculGraphic::getChilds(TYListPtrTYElementGraphic& childs, bool recursif /*=true*/)
{
}

void TYCalculGraphic::display(GLenum mode /*= GL_RENDER*/)
{
    if (TYRayGraphic::_gVisible)
    {
        TYTabRay& tabRays = getElement()->getTabRays();
        for (size_t i=0; i<tabRays.size(); i++)
        {
            tabRays.at(i)->getGraphicObject()->display(mode);
        }
    }
}

