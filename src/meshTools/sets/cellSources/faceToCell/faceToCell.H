/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2018 OpenCFD Ltd.
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
    Foam::faceToCell

Description
    A topoSetCellSource to select cells based on usage in a face set.

    \heading Dictionary parameters
    \table
        Property    | Description                           | Required | Default
        sets        | The faceSet names to use              | possibly |
        set         | The faceSet name to use               | possibly |
        option      | Selection type (all/any/owner/neighbour)  | yes   |
    \endtable

Note
    Must specify "sets" or "set" (highest to lowest precedence).

SourceFiles
    faceToCell.C

\*---------------------------------------------------------------------------*/

#ifndef faceToCell_H
#define faceToCell_H

#include "topoSetCellSource.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class faceToCell Declaration
\*---------------------------------------------------------------------------*/

class faceToCell
:
    public topoSetCellSource
{
public:
        //- Enumeration defining the valid options
        enum faceAction
        {
            ANY,
            ALL,
            OWNER,
            NEIGHBOUR
        };

private:

        static const Enum<faceAction> faceActionNames_;


        //- Add usage string
        static addToUsageTable usage_;

        //- Names of sets to use
        wordList names_;

        //- Option
        faceAction option_;


    // Private Member Functions

        //- Depending on face to cell option add to or delete from cellSet.
        void combine(topoSet& set, const bool add, const word& setName) const;


public:

    //- Runtime type information
    TypeName("faceToCell");


    // Constructors

        //- Construct from components
        faceToCell
        (
            const polyMesh& mesh,
            const word& setName,
            const faceAction option
        );

        //- Construct from dictionary
        faceToCell(const polyMesh& mesh, const dictionary& dict);

        //- Construct from Istream
        faceToCell(const polyMesh& mesh, Istream& is);


    //- Destructor
    virtual ~faceToCell() = default;


    // Member Functions

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet& set
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
