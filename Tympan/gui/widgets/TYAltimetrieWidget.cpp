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
 * \file TYAltimetrieWidget.cpp
 * \brief outil IHM pour une altimetrie
 *
 *
 */




#include "Tympan/models/business/OLocalizator.h"
#include "Tympan/models/business/topography/TYAltimetrie.h"
//Added by qt3to4:
#include <QGridLayout>


#include "TYAltimetrieWidget.h"

#define TR(id) OLocalizator::getString("TYAltimetrieWidget", (id))


TYAltimetrieWidget::TYAltimetrieWidget(TYAltimetrie* pElement, QWidget* _pParent /*=NULL*/):
    TYWidget(pElement, _pParent)
{

    _elmW = new TYElementWidget(pElement, this);

    resize(300, 220);
    setWindowTitle(TR("id_caption"));
    _altimetrieLayout = new QGridLayout();
    setLayout(_altimetrieLayout);

    _altimetrieLayout->addWidget(_elmW, 0, 0);
    updateContent();
}

TYAltimetrieWidget::~TYAltimetrieWidget()
{
}

void TYAltimetrieWidget::updateContent()
{
    _elmW->updateContent();
}

void TYAltimetrieWidget::apply()
{
    _elmW->apply();

    emit modified();
}

