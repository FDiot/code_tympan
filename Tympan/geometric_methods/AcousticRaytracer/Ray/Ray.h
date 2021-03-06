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

#ifndef RAY_H
#define RAY_H

#include <memory>
#include <boost/shared_ptr.hpp>

#include "Base.h"
#include "Geometry/mathlib.h"
#include "Acoustic/Source.h"
#include "Acoustic/Event.h"
#include "Acoustic/Diffraction.h"

using namespace std;


typedef std::pair<bitSet, bitSet> signature;
/**
 * \brief : Describes a ray by a pair of unsigned int. The first one gives the source number (in the range 0-4095)
 *         and the receptor number (in the range 0-1048576) as a bit field.
 *         The second one describes the sequences of events by their types (user could decide what 1 represent, may be REFLEXION
 *         or DIFFRACTION)
 */
class Ray : public Base
{

public:
	/// Constructors
    Ray() : Base(), position(), direction(), mint(0), maxt(100000), source(NULL), recepteur(NULL), nbReflexion(0), nbDiffraction(0), cumulDistance(0.), cumulDelta(0.)
	{ 
		name = "unknow ray"; 
	}

    Ray(const vec3& _position, const vec3& _direction) : Base(), position(_position), direction(_direction), mint(0), maxt(100000), source(NULL), recepteur(NULL), nbReflexion(0), nbDiffraction(0), cumulDistance(0.), cumulDelta(0.)
    { 
		name = "unknow ray";
	}
    /// Copy constructors
    Ray(const Ray& other) : Base(other)
    {
        position = vec3(other.position);
        direction = vec3(other.direction);
        mint = other.mint;
        maxt = other.maxt;
        source = other.source;
        recepteur = other.recepteur;
        constructId = other.constructId;
        for (unsigned int i = 0; i < other.events.size(); i++)
        {
            events.push_back(other.events.at(i));
        }

        nbDiffraction = other.nbDiffraction;
        nbReflexion = other.nbReflexion;
		cumulDistance = other.cumulDistance;
		cumulDelta = other.cumulDelta;
    }

    Ray(Ray* other)
    {
        position = vec3(other->position);
        direction = vec3(other->direction);
        mint = other->mint;
        maxt = other->maxt;
        source = other->source;
        recepteur = other->recepteur;
        constructId = other->constructId;
        for (unsigned int i = 0; i < other->events.size(); i++)
        {
            events.push_back(other->events.at(i));
        }

        nbDiffraction = other->nbDiffraction;
        nbReflexion = other->nbReflexion;
		cumulDistance = other->cumulDistance;
		cumulDelta = other->cumulDelta;

    }
    /// Destructor
    virtual ~Ray() { }

	/*!
	 * \fn decimal computeEventsSequenceLength()
	 * \brief Compute the length of the sequence of events
	 */
	decimal computeEventsSequenceLength();

	/*!
    * \fn void computeLongueur()
    * \brief Compute the distance traveled (length) by the ray and the result is set into the longueur attribute
    */
    void computeLongueur();

    /*!
     * \fn decimal computeTrueLength(const vec3& ref, const vec3& lastPos, vec3& closestPoint);
     * \brief	Compute ray length from source to closestPoint
	 *			closestPoint is the projection of ref on the line passing by lastPos and the position of the last event (or source if the ray has no events)
     * \param ref
     * \param lastPos
     * \param closestPoint
     */
	decimal computeTrueLength(const vec3& ref, const vec3& lastPos, vec3& closestPoint);
    /*!
     * \fn decimal computePertinentLength(const vec3& ref, const vec3& lastPos, vec3& closestPoint);
     * \brief Compute ray length from last pertinent event (i.e. source or last diffraction)
	 *        to the nearest point of the "event" located at ref position
     */
	decimal computePertinentLength(const vec3& ref, const vec3& lastPos, vec3& closestPoint);

    /*!
	 * \fn void* getLastPertinentEventOrSource(typeevent evType = DIFFRACTION) const;
	 * \brief Return a pointer to the last event of type evType or source if none
     */
	Base* getLastPertinentEventOrSource(typeevent evType = DIFFRACTION);

	/*!
	 * \fn vec3 computeLocalOrigin( Base *ev);
	 * \brief Return position of ev which might be a source or an event
	 */
	inline vec3 computeLocalOrigin( Base *ev)
	{
		if ( dynamic_cast<Source*>(ev) )
		{
            // if ev is the source
			return dynamic_cast<Source*>(ev)->getPosition();
		}
		else 
		{
            // if ev is a standard event
			return dynamic_cast<Event*>(ev)->getPosition();
		}
	}

    /*!
    * \fn double getLongueur()
    * \brief Return the traveled distance by the ray
    * \return Traveled distance by the ray.
    */
    double getLongueur() const {return longueur;}

    /*!
    * \fn unsigned int getDiff()
    * \brief Return the diffractions number encountered by the ray.
    * \return Reflections diffractions encountered by the ray.
    */
    unsigned int getDiff() const { return nbDiffraction; }

    /*!
    * \fn unsigned int getReflex()
    * \brief Return the reflections number encountered by the ray.
    * \return Reflections number encountered by the ray.
    */
    unsigned int getReflex() const { return nbReflexion; }

    /*!
     * \fn unsigned int getNbEvents();
     * \brief Return the total number of events
     */
    unsigned int getNbEvents() const { return nbDiffraction + nbReflexion; }

    /*!
    * \fn std::vector<boost::shared_ptr<Event> >* getEvents()
    * \brief Return the events array encountered by the ray
    * \return Events array encountered by the ray.
    */
    std::vector<boost::shared_ptr<Event> >* getEvents() { return &events; }

    /*!
    * \fn const std::vector<boost::shared_ptr<Event> >* getEvents() const
    * \brief Return the events array encountered by the ray
    * \return Events array encountered by the ray.
    */
    const std::vector<boost::shared_ptr<Event> >* getEvents() const { return &events; }

    /**
     * \fn getFaceHistory()
     * \brief Return the array of faces id encountered by the ray
     */
    vector<unsigned int>getFaceHistory();

    /**
     * \fn getPrimitiveHistory()
     * \brief Return the array of primitives id encountered by the ray
     */
    vector<unsigned int> getPrimitiveHistory();

    /*!
    * \fn Source* getSource()
    * \brief Return the ray source
    * \return Pointer to the source
    */
    Source* getSource() { return source; }

    /*!
    * \fn Recepteur* getRecepteur()
    * \brief Return the ray receptor.
    * \return Pointer to the ray receptor. NULL if the ray has no associated receptor
    */
    void* getRecepteur() { return recepteur; }

    /*!
     * \fn signature getSignature(const typeevent& ev = SPECULARREFLEXION);
     * \brief Compute the signature (i.e. std::pair<unsigned int, unsigned int>) of the ray)
     */
    signature getSignature(const typeevent& typeEv = SPECULARREFLEXION);

    /*!
     * \fn decimal getThickness( const decimal& distance, bool diffraction);
     * \brief Compute the thickness of the ray after having traveled a certain distance 
     * depending on the type of source which generated the ray (spherical or diffraction source)
     */
	decimal getThickness( const decimal& distance, bool diffraction);

    /*!
     * \fn decimal getSolidAngle( bool &diffraction)
     * \brief Compute the solid angle associated with the ray (depends on the type of source which generated the ray and the number of rays it generated)
     * \param diffraction Set diffraction true if last pertinent event is a diffraction
     */
	decimal getSolidAngle( bool &diffraction );

    /*!
     * \fn bitSet getSRBitSet(const unsigned& int source_id, const unsigned int & receptor_id);
     * \brief Compute the bitSet associated with a source and a receptor
     */
    inline bitSet getSRBitSet(const unsigned int& source_id, const unsigned int& receptor_id)
    {
        // The source id is stored in 12 bits, receptor is stored in 20 bits
        assert((source_id < 4096) && (receptor_id < 1048576));
        bitSet SR = source_id;
        SR = SR << 20;
        return SR += receptor_id;
    }

    /*!
     * \fn bitSet getEventsBitSet(const typeevent& typeEv);
     * \brief Compute the bitSet associated with a list of events of type evType
     */
    bitSet getEventsBitSet(const typeevent& typeEv);

    /*!
     * \fn decimal getcumulDelta();
     * \brief Return the cumulative difference between the rays length and its length when ignoring diffractions and taking the direct path between reflections instead
     */

     decimal getCumulDelta() const { return cumulDelta; }

    /*!
     * \fn decimal getCumulDistance()
     * \brief Return the cumulative length since the last reflection event
     */

     decimal getCumulDistance() const { return cumulDistance; }

    /*!
     * \fn unsigned long long int getConstructId()
     * \brief Return Ray id
     */

     unsigned long long int getConstructId() const { return constructId; }

    /*!
     * \fn decimal getMint()
     * \brief Return mint
     */

     decimal getMint() const { return mint; }

    /*!
     * \fn decimal getMaxt()
     * \brief Return maxt
     */

     decimal getMaxt() const { return maxt; }

    /*!
     * \fn vec3 getFinalPosition()
     * \brief Return ending point of the ray (this ending point is set when a the ray hits a receptor in engine.searchForReceptor)
     */

     vec3 getFinalPosition() const { return finalPosition; }

    /*!
     * \fn vect3 getDirection()
     * \brief Return direction of the ray 
     */

     vec3 getDirection() const { return direction; }

    /*!
     * \fn vect3 getPosition()
     * \brief Return starting point ray
     */

     vec3 getPosition() const { return position; }
    
    /*!
     * \fn void setPosition (vec3 _position)
     * \brief set the starting point ray
     */

     void setPosition (vec3 _position){
        position = _position;
     }

    /*!
     * \fn void setdirection (vec3 _direction)
     * \brief set the direction if the ray
     */

     void setDirection (vec3 _direction){
        direction=_direction;
     }

    /*!
     * \fn void setFinalPosition (vec3 _finalPosition)
     * \brief set the ending point of the ray
     */

     void setFinalPosition (vec3 _finalPosition){
        finalPosition=_finalPosition;
     }

    /*!
     * \fn void setMint (decimal _mint)
     * \brief set the Mint
     */

     void setMint (decimal _mint){
        mint=_mint;
     }

    /*!
     * \fn void setMaxt (decimal _maxt)
     * \brief set the maxt
     */

     void setMaxt (decimal _maxt){
        maxt=_maxt;
     }

    /*!
     * \fn void setSource (Source* _source)
     * \brief set the pointer to the source of the ray 
     */

     void setSource (Source* _source){
        source=_source;
     }

    /*!
     * \fn void setRecepteur (void* _recepteur)
     * \brief set the pointer to the receptor of the ray
     */

     void setRecepteur (void* _recepteur){
        recepteur=_recepteur;
     }

    /*!
     * \fn void setLongueur (decimal _longueur) 
     * \brief set the distance traveled by the ray
     */

     void setLongueur (decimal _longueur){
        longueur=_longueur;
     }

    /*!
     * \fn void setConstructId (unsigned long long int _constructId)
     * \brief set the ray id
     */

      void setConstructId (unsigned long long int _constructId){
        constructId=_constructId;
     }


    /*!
     * \fn void setNbReflexion (unsigned int _nbReflexion) 
     * \brief set the reflections number for the ray
     */

     void setNbReflexion (unsigned int _nbReflexion){
        nbReflexion=_nbReflexion;
     }

    /*!
     * \fn void setNbDiffraction (unsigned int _nbDiffraction) 
     * \brief set the diffractions number for the ray
     */

     void setNbDiffraction (unsigned int _nbDiffraction){
        nbDiffraction=_nbDiffraction;
     }

    /*!
     * \fn void setCumulDistance (decimal _cumulDistance)
     * \brief set the cumulative distance by the ray computed at each step
     */

     void setCumulDistance (decimal _cumulDistance){
       cumulDistance = _cumulDistance;
     }

    /*!
     * \fn void setCumulDelta (decimal _cumulDelta)
     * \brief set the cumulative walking step difference by the ray computed at each step
     */

     void setCumulDelta (decimal _cumulDelta){
        cumulDelta=_cumulDelta;
     }

    /*!
     * \fn void setCumulDelta (decimal _cumulDelta)
     * \brief set the cumulative walking step difference by the ray computed at each step
     */

     void addEvent (boost::shared_ptr<Event> _event){
        events.push_back(_event);
     }

	 vector<unsigned int> getEventSignature();
 
protected:
    vec3 position;                              //!< Starting point ray
    vec3 direction;                             //!< Direction vector for the ray at the source
    vec3 finalPosition;                         //!< Ending point of the ray
    decimal mint;
    decimal maxt;
    Source* source;                             //!< Pointer to the source of the ray
    void* recepteur;                            //!< Pointer to the receptor of the ray
    decimal longueur;                           //!< Distance traveled by the ray
    unsigned long long int constructId;         //!< Ray id
    unsigned int nbReflexion;                   //!< Reflections number for the ray
    unsigned int nbDiffraction;                 //!< Diffractions number for the ray
    decimal cumulDistance;                      //!< Cumulative length since last valid reflexion
    decimal cumulDelta;                         //!< Cumulative difference by the ray computed at each step
    std::vector<boost::shared_ptr<Event>> events; //!< Events list for the ray

};


#endif
