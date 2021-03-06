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
 * \file TYReseauTransportEditor.cpp
 * \brief Construit un ReseauTransport a partir des points saisis
 */


#include <qinputdialog.h>

#include "Tympan/models/business/OLocalizator.h"
#include "Tympan/models/business/infrastructure/TYReseauTransport.h"
#include "Tympan/gui/app/TYRenderWindowInteractor.h"
#include "Tympan/gui/app/TYModelerFrame.h"
#include "Tympan/gui/app/TYSiteModelerFrame.h"
#include "Tympan/gui/app/TYActions.h"
#include "Tympan/gui/app/TYApplication.h"
#include "Tympan/gui/app/TYMainWindow.h"
#include "TYReseauTransportEditor.h"


#define TR(id) OLocalizator::getString("TYReseauTransportEditor", (id))


TYReseauTransportEditor::TYReseauTransportEditor(TYModelerFrame* pModeler) :
    TYPolyLineEditor(pModeler)
{
    QObject::connect(this, SIGNAL(endedSavingPoints()), this, SLOT(endReseauTransport()));
}

TYReseauTransportEditor::~TYReseauTransportEditor()
{
}

void TYReseauTransportEditor::endReseauTransport()
{
    if ( !(getSavedPoints().size() > 1) || (!_pModeler->askForResetResultat()) )
    {
        return;
    }

    LPTYReseauTransport pReseauTransport = new TYReseauTransport();
    pReseauTransport->setTabPoint(getSavedPoints());

    if (pReseauTransport->edit(_pModeler) == QDialog::Accepted)
    {
        TYSiteNode* pSite = ((TYSiteModelerFrame*)_pModeler)->getSite();

        if (pSite->getInfrastructure()->addResTrans(pReseauTransport))
        {
            // On ajoute ce reseau a la selection du calcul courant
            if (getTYApp()->getCurProjet() && (getTYApp()->getCurProjet()->getSite() == pSite))
            {
                LPTYCalcul pCalcul = getTYApp()->getCurProjet()->getCurrentCalcul();

                if (pCalcul)
                {
                    pCalcul->addToSelection(pReseauTransport);
                }
            }

            TYAction* pAction = new TYAddElementToInfraAction((LPTYElement&) pReseauTransport, pSite->getInfrastructure(), _pModeler, TR("id_action_addrestrans"));
            _pModeler->getActionManager()->addAction(pAction);

            TYProjet* pProjet = getTYApp()->getCurProjet();
            pProjet->getSite()->getInfrastructure()->updateGraphicTree();
            pProjet->getSite()->updateGraphic();
            refreshSiteFrame();
            _pModeler->getView()->getRenderer()->updateDisplayList();
            _pModeler->updateView();
        }

        // repasse en mode camera selection
        getTYMainWnd()->setDefaultCameraMode();
    }
}
