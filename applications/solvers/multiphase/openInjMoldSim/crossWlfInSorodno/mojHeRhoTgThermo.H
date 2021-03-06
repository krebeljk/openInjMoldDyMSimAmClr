/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::mojHeRhoTgThermo

Description
    Energy for a mixture based on density

SourceFiles
    mojHeRhoTgThermo.C

\*---------------------------------------------------------------------------*/

#ifndef mojHeRhoTgThermo_H
#define mojHeRhoTgThermo_H

#include "mojRhoThermo.H"
#include "mojHeThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class mojHeRhoTgThermo Declaration
\*---------------------------------------------------------------------------*/

template<class BasicPsiThermo, class MixtureType>
class mojHeRhoTgThermo
:
    public mojHeThermo<BasicPsiThermo, MixtureType>
{
    // Private Member Functions

        //- Calculate the thermo variables
        void calculate();

        //- Construct as copy (not implemented)
        mojHeRhoTgThermo(const mojHeRhoTgThermo<BasicPsiThermo, MixtureType>&);

    // Private Member Data

        // spec. volume [m3/kg]
        volScalarField vf_;

public:

    //- Runtime type information
    TypeName("mojHeRhoTgThermo");


    // Constructors

        //- Construct from mesh and phase name
        mojHeRhoTgThermo
        (
            const fvMesh&,
            const word& phaseName
        );


    //- Destructor
    virtual ~mojHeRhoTgThermo();


    // Member functions

        //- Update properties
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "mojHeRhoTgThermo.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
