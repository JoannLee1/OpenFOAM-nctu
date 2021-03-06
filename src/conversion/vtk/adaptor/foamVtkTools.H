/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2017-2018 OpenCFD Ltd.
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
    Foam::vtk::Tools

Description
    A collection of static methods to assist converting OpenFOAM data
    structures into VTK internal data structures.

    Remapping of the symmTensor order is required in input or output
    directions. OpenFOAM uses (XX, XY, XZ, YY, YZ, ZZ) order,
    VTK uses (XX, YY, ZZ, XY, YZ, XZ) order.

Note
    The class is implemented as headers-only.

SourceFiles
    foamVtkToolsI.H
    foamVtkToolsTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef foamVtkTools_H
#define foamVtkTools_H

#include "labelList.H"
#include "faceList.H"
#include "pointField.H"
#include "symmTensor.H"

// VTK includes
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkIdTypeArray.h>
#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>

#include <utility>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace vtk
{

/*---------------------------------------------------------------------------*\
                        Class vtk::Caching Declaration
\*---------------------------------------------------------------------------*/


//- Bookkeeping for internal caching.
//  Retain an original copy of the geometry as well as a shallow copy
//  with the output fields.
//  The original copy is reused for different timesteps etc.
template<class DataType>
struct Caching
{
    typedef DataType dataType;

    //- The geometry, without any cell/point data
    vtkSmartPointer<dataType> vtkgeom;

    //- The shallow-copy of geometry, plus additional data
    vtkSmartPointer<dataType> dataset;

    //- Number of points associated with the geometry
    inline uint64_t nPoints() const
    {
        return vtkgeom ? vtkgeom->GetNumberOfPoints() : 0;
    }

    //- Clear geometry and dataset
    void clearGeom()
    {
        vtkgeom = nullptr;
        dataset = nullptr;
    }

    //- Return a shallow copy of vtkgeom for manipulation
    vtkSmartPointer<dataType> getCopy() const
    {
        auto copy = vtkSmartPointer<dataType>::New();
        if (vtkgeom)
        {
            copy->ShallowCopy(vtkgeom);
        }
        return copy;
    }

    //- Make a shallow copy of vtkgeom into dataset
    void reuse()
    {
        dataset = vtkSmartPointer<dataType>::New();
        if (vtkgeom)
        {
            dataset->ShallowCopy(vtkgeom);
        }
    }

    //- Set the geometry and make a shallow copy to dataset
    void set(vtkSmartPointer<dataType> geom)
    {
        vtkgeom = geom;
        reuse();
    }

    //- Report basic information to output
    void PrintSelf(std::ostream& os) const
    {
        os << "geom" << nl;
        if (vtkgeom)
        {
            vtkgeom->PrintSelf(std::cout, vtkIndent(2));
        }
        else
        {
            os << "nullptr";
        }
        os << nl;

        os << "copy" << nl;
        if (dataset)
        {
            dataset->PrintSelf(std::cout, vtkIndent(2));
        }
        else
        {
            os << "nullptr";
        }
        os << nl;
    }
};


/*---------------------------------------------------------------------------*\
                         Class vtk::Tools Declaration
\*---------------------------------------------------------------------------*/

class Tools
{
public:

    //- Wrap vtkUnsignedCharArray as a UList
    inline static UList<uint8_t> asUList
    (
        vtkUnsignedCharArray* array,
        const label size
    );

    //- Wrap vtkIdTypeArray as a UList
    inline static UList<vtkIdType> asUList
    (
        vtkIdTypeArray* array,
        const label size
    );

    //- Wrap vtkCellArray as a UList
    inline static UList<vtkIdType> asUList
    (
        vtkCellArray* cells,
        const label nCells,
        const label size
    );


    //- Convert OpenFOAM patch to vtkPolyData
    struct Patch
    {
        //- Convert local patch points to vtkPoints
        template<class PatchType>
        static vtkSmartPointer<vtkPoints> points(const PatchType& p);

        //- Convert patch faces to vtk polygon cells
        template<class PatchType>
        static vtkSmartPointer<vtkCellArray> faces(const PatchType& p);

        //- Convert patch points/faces to vtkPolyData
        template<class PatchType>
        static vtkSmartPointer<vtkPolyData> mesh(const PatchType& p);

        //- Convert patch face normals to vtkFloatArray
        template<class PatchType>
        static vtkSmartPointer<vtkFloatArray> faceNormals(const PatchType& p);
    };


    //- Remapping for some OpenFOAM data types (eg, symmTensor)
    //  \param data[in,out] The data to be remapped in-place
    template<class Type>
    inline static void remapTuple(float data[]) {}

    //- Remapping for some OpenFOAM data types (eg, symmTensor)
    //  \param data[in,out] The data to be remapped in-place
    template<class Type>
    inline static void remapTuple(double data[]) {}

    //- Copy/transcribe OpenFOAM data types to VTK format
    //  This allows a change of data type (float vs double) as well as
    //  addressing any swapping issues (eg, symmTensor)
    //
    //  \param output[out] The output scratch space. Must be long enough
    //     to hold the result.
    //  \param val[in] The input data to copy/transcribe
    template<class Type>
    inline static void foamToVtkTuple(float output[], const Type& val);

    //- Copy/transcribe OpenFOAM data types to VTK format
    //  This allows a change of data type (float vs double) as well as
    //  addressing any swapping issues (eg, symmTensor)
    //
    //  \param output[out] The output scratch space. Must be long enough
    //     to hold the result.
    //  \param val[in] The input data to copy/transcribe
    template<class Type>
    inline static void foamToVtkTuple(double output[], const Type& val);


    // Field Conversion Functions

    //- Copy list to pre-allocated vtk array.
    //  \return number of input items copied
    template<class Type>
    static label transcribeFloatData
    (
        vtkFloatArray* array,
        const UList<Type>& input,
        vtkIdType start = 0         //!< The write offset into output array
    );

    //- Create named field initialized to zero
    template<class Type>
    static vtkSmartPointer<vtkFloatArray> zeroField
    (
        const word& name,
        const label size
    );

    //- Convert field data to a vtkFloatArray
    template<class Type>
    static vtkSmartPointer<vtkFloatArray> convertFieldToVTK
    (
        const word& name,
        const UList<Type>& fld
    );

    //- An identity list of VTK_VERTEX
    static inline vtkSmartPointer<vtkCellArray> identityVertices
    (
        const label size
    );
};


//- Template specialization for symmTensor ordering
template<>
inline void Foam::vtk::Tools::remapTuple<Foam::symmTensor>(float data[])
{
    std::swap(data[1], data[3]);    // swap XY <-> YY
    std::swap(data[2], data[5]);    // swap XZ <-> ZZ
}


//- Template specialization for symmTensor ordering
template<>
inline void Foam::vtk::Tools::remapTuple<Foam::symmTensor>(double data[])
{
    std::swap(data[1], data[3]);    // swap XY <-> YY
    std::swap(data[2], data[5]);    // swap XZ <-> ZZ
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace vtk
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "foamVtkToolsI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "foamVtkToolsTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
