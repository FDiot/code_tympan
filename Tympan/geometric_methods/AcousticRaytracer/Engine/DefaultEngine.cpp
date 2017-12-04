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

#include <stack>
#include <list>
#include <algorithm>
//Suppress reference to Qt (a Qtime object was created but results from Qtime::elapsed() was not printed) 
//#include <QTime>
#include <vector>

#include "Geometry/mathlib.h"
#include "Acoustic/Event.h"
#include "Ray/Ray.h"
#include "DefaultEngine.h"

/*struct CompareVec
{
    bool operator()(std::vector<unsigned int> list1, std::vector<unsigned int> list2) const
    {
        //On compare jusqu'a ce qu'on atteigne le bout du plus petit vecteur
        int minSize = min(list1.size(), list2.size());
        for (int i = 0; i < minSize; i++)
        {
            if (list1.at(i) < list2.at(i))
            {
                return true;
            }
            else if (list1.at(i) == list2.at(i))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        //Si les vecteurs ont la meme taille, c'est qu'ils sont egaux
        if (list1.size() == list2.size())
        {
            return false;
        }
        //La list avec la plus grande taille est superieure a l'autre
        if (list1.size() < list2.size())
        {
            return true;
        }
        return false;
    }
} myCompare ;*/

bool DefaultEngine::process()
{
    std::size_t max_size(0);

    //QTime time;
    //time.start();
    nbRayonsTraites = 0;

    //We begin to throw a rays directly from each source to each receptor
	initialReceptorTargeting();

	//Traitement des rayons diffractes ainsi que les rayons tires aleatoirement
    while (1)
    {
        //Pile de traitement vide, on prend le suivant dans la liste des sources
        if (pile_traitement.empty())
        {
            Ray* newRay = genRay();
            if (newRay)
            {
                //ray_to_process--;
                pile_traitement.push(newRay);
            }

            //Aucun rayon genere a partir des sources, fin du traitement
            if (pile_traitement.empty())
            {
                solver->finish();
 
                return true;
            }
        }
        else
        {
        	// The current ray of the stack is treated:
            Ray* current_ray = pile_traitement.top();
            pile_traitement.pop();
            std::list<validRay> result;
            traitementRay(current_ray, result);
            for (std::list<validRay>::iterator it = result.begin(); it != result.end(); it++)
            {
                bool currentValid = it->valid;
                Ray* currentValidRay = it->r;
                if (!currentValid)
                {
                    solver->invalidRayon(currentValidRay);
                }
                else
                {
                	// New valid ray to treat:
                    pile_traitement.push(currentValidRay);
                    if (pile_traitement.size() > max_size)
                    {
                        max_size = pile_traitement.size();
                    }
                }
            }
        }
    }
}

Ray* DefaultEngine::genRay()
{
    for (unsigned int i = 0; i < sources->size(); i++)
    {
        if (sources->at(i).getNbRayLeft() > 0)
        {
        	// Generate a ray from the current source(i)
            Ray* new_ray = new Ray();
            new_ray->setConstructId ( rayCounter );
            rayCounter++;
            new_ray->setSource ( (&(sources->at(i))) );
            new_ray->setPosition ( sources->at(i).getPosition());
			vec3 direction;
            sources->at(i).getDirection(direction);
			new_ray->setDirection(direction);
            new_ray->setMint ( 0.);
            new_ray->setMaxt ( 10000.);
            return new_ray;
        }
    }

    return (Ray*)NULL;
}

bool DefaultEngine::traitementRay(Ray* r, std::list<validRay> &result)
{
    nbRayonsTraites++; //Nombre de rayons traites au cours de la simulation

    //Si le dernier evenement du rayon peut generer plusieurs rayons secondaires, on genere
    //un rayon secondaire, puis on copie le rayon restant et on le met dans la pile de traitement.
    //Si le dernier evenement n'a plus de rayon a generer, le rayon n'est pas traite
    if ( !(r->getEvents()->empty()) && (r->getEvents()->back()->isReponseLeft()) )
    {
		copyRayAndAddToStack(r);
    }

    decimal tmin = -1.0f;

	//Recuperation des structures acceleratrices pour le Solver
    Accelerator* accelerator = scene->getAccelerator();
    std::list<Intersection> foundPrims;

    //Appel du Solver pour le choix de la methode de traversee de la structure
    tmin =  accelerator->traverse(r, foundPrims);

    // Recherche pour des recepteurs;
    searchForReceptor(tmin, r);

    //Validation des rayons en generant un evenement. Les premiers rayons valides sont des copies de l'original, le dernier est valide sans copie.
    //De cette maniere on peut valider separement des diffractions et une reflexion a partir d'un seul rayon initial.
    //La copie est necessaire pour ne pas valider 2 fois le meme evenement. Si le rayon ne peut pas etre valide, il sera delete dans la fonction traitement()
  
	if ( foundPrims.size() == 0 ){
	//if no intersection found => return the current ray 'r' as an invalid validRay;
		validRay resultRay;
		resultRay.r = r;
		resultRay.valid = false;
		result.push_back(resultRay);
    
	}else{
	//if some intersections have been found, browse through them
		for (std::list<Intersection,std::allocator<Intersection>>::iterator it=foundPrims.begin();it != foundPrims.end();it++){

			bool valide=false;
			Intersection *inter = NULL;
			Ray* current_ray = NULL;

			if(next(it)!=foundPrims.end()){
			//if the current intersection is not the last one, copy the current ray
				current_ray = new Ray(r);
				current_ray->setConstructId ( rayCounter );
				rayCounter++;

			}else{
			//else validate the current ray 'r' directly without copying it.
				current_ray=r;
			}

			//retrieve the intersection and validate it
			inter = &(*(it));
			valide = solver->valideIntersection(current_ray, inter);

			//create a validRay to return the result of the intersection validation
			validRay resultRay;
			resultRay.r = current_ray;
			resultRay.valid = valide;
			result.push_back(resultRay);

		}
	}


    return true;
}

void DefaultEngine::runStructureBenchmark()
{

    unsigned int nbVec = 1000000;

    //Generation du buffer de vecteur
    BBox sceneBox = scene->getGlobalBox();
    /*std::vector<vec3> buffer = std::vector<vec3>(2*nbVec);
    for(unsigned int i = 0; i < nbVec; i++){
        vec3 newDir = vec3((double)rand() / (double)RAND_MAX, (double)rand() / (double)RAND_MAX, (double)rand() / (double)RAND_MAX);
        newDir.normalize();
        buffer.push_back(newDir);
        vec3 newPos = vec3(((double)rand() / (double)RAND_MAX) * (sceneBox.pMax.x - sceneBox.pMin.x) + sceneBox.pMin.x,
                            ((double)rand() / (double)RAND_MAX) * (sceneBox.pMax.y - sceneBox.pMin.y) + sceneBox.pMin.y,
                            ((double)rand() / (double)RAND_MAX) * (sceneBox.pMax.z - sceneBox.pMin.z) + sceneBox.pMin.z);
        buffer.push_back(newPos);
    }*/

    //QTime time;
    //time.start();

    for (unsigned int i = 0; i < nbVec; i++)
    {
        Ray r;
        r.setMint ( 0.00001f);
        r.setMaxt ( 10000.);
        r.setDirection ( vec3((double)rand() / (double)RAND_MAX, (double)rand() / (double)RAND_MAX, (double)rand() / (double)RAND_MAX) );
        r.getDirection().normalize();
        r.setPosition ( vec3(((double)rand() / (double)RAND_MAX) * (sceneBox.pMax.x - sceneBox.pMin.x) + sceneBox.pMin.x,
                          ((double)rand() / (double)RAND_MAX) * (sceneBox.pMax.y - sceneBox.pMin.y) + sceneBox.pMin.y,
                          ((double)rand() / (double)RAND_MAX) * (sceneBox.pMax.z - sceneBox.pMin.z) + sceneBox.pMin.z) );
        Accelerator* accel = scene->getAccelerator();
        std::list<Intersection> foundPrims;
        accel->traverse(&r, foundPrims);
    }

    //int totalTime = time.elapsed();
    //std::cout << "Fin du benchmark." << std::endl;
    //std::cout << "Nombre de rayons traites : " << nbVec << std::endl;
    //std::cout << "Temps ecoule : " << totalTime << " ms." << std::endl;
    //std::cout << "Vitesse de traitement : " << nbVec / totalTime << " rays/ms." << std::endl;
}
