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

#include "Solver.h"
#include "Engine/AcousticRaytracerConfiguration.h"
#include "Acoustic/ValidRay.h"
#include "Acoustic/PostTreatment.h"
#include "Tools/FaceSelector.h"
#include "Tools/LengthSelector.h"
#include "Tools/DiffractionSelector.h"
#include "Tools/ReflectionSelector.h"
#include "Tools/CloseEventSelector.h"
#include "Tools/FermatSelector.h"
#include "Tools/DiffractionPathSelector.h"
#include "Tools/DiffractionAngleSelector.h"
#include "Tools/CleanerSelector.h"
#include "Tools/SelectorManager.h"
#include "Tools/Logger.h"


bool Solver::postTreatmentScene(Scene* scene, std::vector<Source>& sources, std::vector<Recepteur>& recepteurs)
{
    return true;
}

double Solver::leafTreatment(vector<Intersection> &primitives)
{
    return -1.0;
}


bool Solver::valideIntersection(Ray* r, Intersection* inter)
{
    return false;
}

bool Solver::valideRayon(Ray* r)
{
    valid_rays.push_back(r);
    return true;
}

void Solver::clean()
{
	// Clear valid_rays completely and delete the corresponding rays
    while (!valid_rays.empty())
    {
        Ray* r = valid_rays.back();
        valid_rays.pop_back();
        delete r;
    }
}

void Solver::finish()
{
    return;
}

bool Solver::invalidRayon(Ray* r)
{
	// Each invalidated ray is deleted
    delete r;
    return true;
}

bool Solver::loadParameters()
{
    return true;
}


bool BasicSolver::postTreatmentScene(Scene* scene, std::vector<Source>& sources, std::vector<Recepteur>& recepteurs)
{
    selectorManagerValidation.addSelector( new CleanerSelector<Ray>() );
    selectorManagerValidation.addSelector( new LengthSelector<Ray>(AcousticRaytracerConfiguration::get()->MaxLength) );

    if (AcousticRaytracerConfiguration::get()->UsePostFilters)
    {
        if (AcousticRaytracerConfiguration::get()->DebugUseCloseEventSelector) { selectorManagerValidation.addSelector( new CloseEventSelector<Ray>() ); }
        if (AcousticRaytracerConfiguration::get()->DebugUseDiffractionAngleSelector) { selectorManagerValidation.addSelector(new DiffractionAngleSelector<Ray>()); }
        if (AcousticRaytracerConfiguration::get()->DebugUseDiffractionPathSelector) { selectorManagerValidation.addSelector(new DiffractionPathSelector<Ray>(AcousticRaytracerConfiguration::get()->MaxPathDifference)); }
        if (AcousticRaytracerConfiguration::get()->DebugUseFermatSelector) { selectorManagerValidation.addSelector( new FermatSelector<Ray>() ); }
        if (AcousticRaytracerConfiguration::get()->DebugUseFaceSelector) { selectorManagerValidation.addSelector( new FaceSelector<Ray>(HISTORY_PRIMITIVE) ); }
    }

    selectorManagerIntersection.addSelector( new DiffractionSelector<Ray>(AcousticRaytracerConfiguration::get()->MaxDiffraction) );
    selectorManagerIntersection.addSelector( new ReflectionSelector<Ray>(AcousticRaytracerConfiguration::get()->MaxReflexion, AcousticRaytracerConfiguration::get()->UseSol) );

    // Ajoute des cylindres sur les arretes diffractantes
    PostTreatment::constructEdge(scene);

    return true;
}

bool BasicSolver::valideIntersection(Ray* r, Intersection* inter)
{
    if (r->events.size() > static_cast<unsigned int>(AcousticRaytracerConfiguration::get()->MaxProfondeur)) { return false; }

    bool isValid = false;

    // cas d'un triangle (sol)
    if ( ( inter->forme == TRIANGLE ) &&
         ( r->nbReflexion < static_cast<unsigned int>(AcousticRaytracerConfiguration::get()->MaxReflexion) ) &&
         !( !AcousticRaytracerConfiguration::get()->UseSol && inter->p->isSol() ) )
    {
        isValid = ValidRay::validTriangleWithSpecularReflexion(r, inter);
    }

    // cas du cylindre (arrete de diffraction)
    else if (inter->forme == CYLINDRE && r->nbDiffraction < static_cast<unsigned int>(AcousticRaytracerConfiguration::get()->MaxDiffraction))
    {
        isValid = ValidRay::validCylindreWithDiffraction(r, inter);
    }

#ifdef _ALLOW_TARGETING_
    if (isValid && AcousticRaytracerConfiguration::get()->EnableFullTargets) { ValidRay::appendDirectionToEvent(r->events.back(), targetManager); }
#endif //_ALLOW_TARGETING_

    return (isValid); //(isValid && selectorManagerIntersection.appendData(r));
}

bool BasicSolver::valideRayon(Ray* r)
{
    selectorManagerValidation.appendData(r);
#ifdef _DEBUG
    if (selectorManagerValidation.getSelectedData().size() % 1000 == 0 )
    {
        std::cout << "Nombre de rayon valides = " << selectorManagerValidation.getSelectedData().size()<< std::endl;
    }

#endif
    return true;
}

bool BasicSolver::invalidRayon(Ray* r)
{
    if (!AcousticRaytracerConfiguration::get()->KeepDebugRay)
    {
        delete r;
        r = NULL;
    }
    else
    {
        debug_rays.push_back(r);
    }

    return true;
}

void BasicSolver::finish()
{
    std::map<unsigned long long int, Ray*> selectedData = selectorManagerValidation.getSelectedData();

    for (std::map<unsigned long long, Ray*>::iterator it = selectedData.begin(); it != selectedData.end(); it++)
    {
        valid_rays.push_back(it->second);
    }

    selectorManagerIntersection.reset();
    selectorManagerValidation.reset();

    return;
}
