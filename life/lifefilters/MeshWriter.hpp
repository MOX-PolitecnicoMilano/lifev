//@HEADER
/*
*******************************************************************************

    Copyright (C) 2004, 2005, 2007 EPFL, Politecnico di Milano, INRIA
    Copyright (C) 2010 EPFL, Politecnico di Milano, Emory University

    This file is part of LifeV.

    LifeV is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LifeV is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LifeV.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/
//@HEADER

/*!
    @file
    @brief Writes the mesh in medit format

    This file contains the namespace MeshWriters. There a method to write meshes in format MEDIT (ASCII) is implemented.

    @author Paolo Crosetto <crosetto@iacspc70.epfl.ch>
    @date 31 May 2011


 */

#ifndef MESHWRITER_H
#define MESHWRITER_H 1

#include <life/lifecore/LifeV.hpp>
#include <life/lifemesh/ElementShapes.hpp>

namespace LifeV {

//! MeshWriter - Short description of the class
/*!
    @author Paolo Crosetto

    This file contains the namespace MeshWriters. There a method to write meshes in format MEDIT (ASCII) is implemented.
    The files in MEDIT format have the extension .mesh and can be read by several softwares (like MEDIT, from INRIA, or GMSH)
 */
namespace MeshWriter
{

template <typename Mesh>
void writeMeshMedit  (std::string fname, Mesh& mesh )
{
    std::ofstream ofile( fname.c_str() );

    ASSERT( ofile, "Error: Output file cannot be open" );

    ofile << "MeshVersionFormatted 1\n";
    ofile << "Dimension\n3\n";
    ofile << "\n";
    ofile << "Vertices\n";

    UInt nVertices = mesh.numVertices();
    ofile << nVertices << "\n";

    int ig;

    std::map<int, int> localToGlobalNode = mesh.localToGlobalNode();
    std::map<int, int>::iterator im;


    for ( UInt i = 0; i < nVertices; ++i )
    {
        ofile << mesh.pointList( i ).x() << " "
              << mesh.pointList( i ).y() << " "
              << mesh.pointList( i ).z() << " "
              << mesh.pointList( i ).marker() << "\n";
    }
    ofile << "\n";

    typedef typename Mesh::FaceShape FaceShape;

    switch ( FaceShape::S_shape )
    {
    case QUAD:
        ofile << "Quadrilaterals\n";
        break;
    case TRIANGLE:
        ofile << "Triangles\n";
        break;
    default:
        ERROR_MSG( "BdShape not implement in MEDIT writer" );
    }

    UInt nBdF = mesh. numBFaces();
    ofile << nBdF << "\n";

    UInt nVerticesPerFace = FaceShape::S_numVertices;


    for ( ID k = 0; k < nBdF; ++k )
    {
        for ( ID i = 0; i < nVerticesPerFace; ++i )
        {
            ofile << mesh.boundaryFace( k ).point( i ).id()+1
                  << " ";
        }
        ofile << mesh.boundaryFace( k ).marker() << "\n";
    }
    ofile << "\n";

    typedef typename Mesh::VolumeShape ElementShape;

    switch ( ElementShape::S_shape )
    {
    case HEXA:
        ofile << "Hexaedra\n";
        break;
    case TETRA:
        ofile << "Tetrahedra\n";
        break;
    default:
        ERROR_MSG( "Shape not implement in MEDIT writer" );
    }

    UInt nElements = mesh.numVolumes();
    ofile << nElements << "\n";

    UInt nVerticesPerElement = ElementShape::S_numVertices;

    UInt ielem;

    for ( ID k = 0; k < nElements; ++k )
    {
        for ( ID i = 0; i < nVerticesPerElement; ++i )
        {
            ielem =  mesh.volume( k ).point( i ).localId();

            ofile << ielem+1
                  << " ";
        }
        ofile << mesh.volume( k ).marker() << "\n";
    }
    ofile.close();
}

} // Namespace MeshWriter
} // Namespace LifeV

#endif /* MESHWRITER_H */