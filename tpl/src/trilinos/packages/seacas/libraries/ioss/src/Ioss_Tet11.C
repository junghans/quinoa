// Copyright(C) 1999-2010
// Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
// certain rights in this software.
//         
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Sandia Corporation nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <Ioss_Tet11.h>
#include <Ioss_ElementVariableType.h>   // for ElementVariableType
#include <assert.h>                     // for assert
#include "Ioss_CodeTypes.h"             // for IntVector
#include "Ioss_ElementTopology.h"       // for ElementTopology


//------------------------------------------------------------------------
// Define a variable type for storage of this elements connectivity
namespace Ioss {
  class St_Tet11 : public ElementVariableType
  {
  public:
    static void factory() {static St_Tet11 registerThis;}

  protected:
    St_Tet11()
      : ElementVariableType("tetra11", 11) {}
  };
}

// ========================================================================
namespace {
  struct Constants {
    static const int nnode     = 11;
    static const int nedge     =  6;
    static const int nedgenode =  3;
    static const int nface     =  4;
    static const int nfacenode =  6;
    static const int nfaceedge =  3;
    static int edge_node_order[nedge][nedgenode];
    static int face_node_order[nface][nfacenode];
    static int face_edge_order[nface][nfaceedge];
    static int nodes_per_face[nface+1];
    static int edges_per_face[nface+1];
  };
  
  // Edge numbers are zero-based [0..number_edges)
  int Constants::edge_node_order[nedge][nedgenode] = // [edge][edge_node]
    { {0,1,4}, {1,2,5}, {2,0,6}, {0,3,7}, {1,3,8}, {2,3,9} };
  
  // Face numbers are zero-based [0..number_faces)
  int Constants::face_node_order[nface][nfacenode] = // [face][face_node]
    { {0,1,3,4,8,7}, {1,2,3,5,9,8},
      {0,3,2,7,9,6}, {0,2,1,6,5,4} };
  
  int Constants::face_edge_order[nface][nfaceedge] = // [face][face_edge]
    { {0,4,3}, {1,5,4}, {3,5,2}, {2,1,0} };

  // face 0 returns number of nodes for all faces if homogenous
  //        returns -1 if faces have differing topology
  int Constants::nodes_per_face[nface+1] =
    {6,6,6,6,6};
  
  // face 0 returns number of edges for all faces if homogenous
  //        returns -1 if faces have differing topology
  int Constants::edges_per_face[nface+1] =
    {3,3,3,3,3};
}

void Ioss::Tet11::factory()
{
  static Ioss::Tet11 registerThis;
  Ioss::St_Tet11::factory();
}

// A tet10 with a center node.
Ioss::Tet11::Tet11()
  : Ioss::ElementTopology("tetra11", "Tetrahedron_11")
{
  Ioss::ElementTopology::alias("tetra11", "tet11");
  Ioss::ElementTopology::alias("tetra11", "Solid_Tet_11_3D");
}

Ioss::Tet11::~Tet11() {}

int Ioss::Tet11::parametric_dimension()           const {return  3;}
int Ioss::Tet11::spatial_dimension()           const {return  3;}
int Ioss::Tet11::order()               const {return  2;}

int Ioss::Tet11::number_corner_nodes() const {return 4;}
int Ioss::Tet11::number_nodes()        const {return Constants::nnode;}
int Ioss::Tet11::number_edges()        const {return Constants::nedge;}
int Ioss::Tet11::number_faces()        const {return Constants::nface;}

int Ioss::Tet11::number_nodes_edge(int /* edge */) const {return  Constants::nedgenode;}

int Ioss::Tet11::number_nodes_face(int face) const
{
  // face is 1-based.  0 passed in for all faces.
  assert(face >= 0 && face <= number_faces());
  return  Constants::nodes_per_face[face];
}

int Ioss::Tet11::number_edges_face(int face) const
{
  // face is 1-based.  0 passed in for all faces.
  assert(face >= 0 && face <= number_faces());
  return Constants::edges_per_face[face];
}

Ioss::IntVector Ioss::Tet11::edge_connectivity(int edge_number) const
{
  assert(edge_number > 0 && edge_number <= Constants::nedge);
  Ioss::IntVector connectivity(Constants::nedgenode);

  for (int i=0; i < Constants::nedgenode; i++)
    connectivity[i] = Constants::edge_node_order[edge_number-1][i];

  return connectivity;
}

Ioss::IntVector Ioss::Tet11::face_connectivity(int face_number) const
{
  assert(face_number > 0 && face_number <= number_faces());
  Ioss::IntVector connectivity(Constants::nodes_per_face[face_number]);

  for (int i=0; i < Constants::nodes_per_face[face_number]; i++)
    connectivity[i] = Constants::face_node_order[face_number-1][i];

  return connectivity;
}

Ioss::IntVector Ioss::Tet11::element_connectivity() const
{
  Ioss::IntVector connectivity(number_nodes());
  for (int i=0; i < number_nodes(); i++)
    connectivity[i] = i;
  return connectivity;
}

Ioss::ElementTopology* Ioss::Tet11::face_type(int face_number) const
{
  // face_number == 0 returns topology for all faces if
  // all faces are the same topology; otherwise, returns NULL
  // face_number is 1-based.

  assert(face_number >= 0 && face_number <= number_faces());
//  return Ioss::ElementTopology::factory("triface6");
  return Ioss::ElementTopology::factory("tri6");
}

Ioss::ElementTopology* Ioss::Tet11::edge_type(int edge_number) const
{
  // edge_number == 0 returns topology for all edges if
  // all edges are the same topology; otherwise, returns NULL
  // edge_number is 1-based.

  assert(edge_number >= 0 && edge_number <= number_edges());
  return Ioss::ElementTopology::factory("edge3");
}

Ioss::IntVector Ioss::Tet11::face_edge_connectivity(int face_number) const
{
  assert(face_number > 0 && face_number <= Constants::nface);

  int nface_edge = number_edges_face(face_number);
  Ioss::IntVector fcon(nface_edge);

  for (int i=0; i < nface_edge; i++)
    fcon[i] = Constants::face_edge_order[face_number-1][i];

  return fcon;
}
