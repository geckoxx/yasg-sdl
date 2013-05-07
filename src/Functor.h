/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  johannes <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FUNCTOR_H
#define FUNCTOR_H

namespace yasg {

class Functor
{
public:
    virtual void Call()=0; 
};


template <class TClass> class SpecificFunctor : public Functor
{
private:
    void (TClass::*fpt)();
    TClass* pt2Object;

public:
    SpecificFunctor(TClass* _pt2Object, void(TClass::*_fpt)())
    {
        pt2Object = _pt2Object;
        fpt=_fpt;
    };
    
    virtual void Call()
    {
        (*pt2Object.*fpt)();
    };
};
}

#endif
