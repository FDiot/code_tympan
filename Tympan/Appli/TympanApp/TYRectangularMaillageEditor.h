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
 * \file TYRectangularMaillageEditor.h
 * \brief gestion de l'edition d'un maillage rectangulaire (fichier header)
 *
 *
 *
 *
 *
 */


#ifndef __TY_RECTANGULAR_MAILLAGE_EDITOR__
#define __TY_RECTANGULAR_MAILLAGE_EDITOR__

#include "TYAbstractSceneEditor.h"

class OGLRectangleElement;
/**
 * \class TYRectangularMaillageEditor
 * \brief gestion de l'edition d'un maillage rectangulaire
 */
class TYRectangularMaillageEditor : public TYAbstractSceneEditor
{
    Q_OBJECT

public:
    TYRectangularMaillageEditor(TYSiteModelerFrame* pModeler);
    ~TYRectangularMaillageEditor();

public slots:
    virtual void init();
    virtual void cancel();
    virtual void close();
    virtual void slotMousePressed(int x, int y, Qt::MouseButton button, Qt::KeyboardModifiers state);
    virtual void slotMouseReleased(int x, int y, Qt::MouseButton button, Qt::KeyboardModifiers state);
    virtual void slotMouseMoved(int x, int y, Qt::MouseButtons button, Qt::KeyboardModifiers state);
    virtual void slotViewTypeChanged(int view);

protected:
    ///Indique si cet editor est actif.
    bool _active;

    /// Pour le dessin de construction
    OGLRectangleElement* _pOGLRectangleElement;
    bool _moving;
};

#endif // __TY_RECTANGULAR_MAILLAGE_EDITOR__
