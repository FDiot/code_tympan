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

/*!
* \file Step.h
* \author Projet_Tympan
*/

#ifndef __STEP_H
#define __STEP_H

#include "Tympan/models/common/mathlib.h"
#include "Tympan/geometric_methods/AcousticRaytracer/Ray/Ray.h"

/*!
 * \class Step
 * \brief Describe a step in the ray path
 */
class Step
{
public :
	/// Default constructor
    Step(const vec3& Pos = vec3(0., 0., 0.), const vec3& Norme = vec3(0., 0., 0.)) : pos(Pos), norm(Norme) {}
    /// Copy constructor
    Step(const Step& other) { pos = other.pos; norm = other.norm; }
    /// Destructor
    ~Step() {}
    /// Return a step build from ray position and direction
    static Step Ray_adapter(Ray& ray)
    {
        return Step( ray.getPosition(), ray.getDirection() );
    }

public :
    vec3 pos;	//!< Step position
    vec3 norm;	//!< Step normal
};

inline Step operator * (const Step& s, const decimal& a)
{
    return Step(s.pos * a, s.norm * a);
}

inline Step operator + (const Step& s1, const Step& s2)
{
    return Step(s1.pos + s2.pos, s1.norm + s2.norm);
}

#endif // __STEP_H
