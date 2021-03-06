// Copyright (c) 2013, Sandia Corporation.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
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
// 
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
// 

#include <stk_io/util/Gears.hpp>
#include <math.h>                       // for acos, cos, sin
#include <stk_io/IossBridge.hpp>        // for put_io_part_attribute
#include <stk_mesh/base/BulkData.hpp>   // for BulkData
#include <stk_mesh/base/Comm.hpp>       // for comm_mesh_counts
#include <stk_topology/topology.hpp>    // for topology, etc
#include "stk_mesh/base/FieldBase.hpp"  // for field_data
#include "stk_mesh/base/MetaData.hpp"   // for MetaData, put_field, etc
#include "stk_mesh/base/Types.hpp"      // for EntityId, EntityRank
namespace stk { namespace mesh { class Part; } }





//----------------------------------------------------------------------
//----------------------------------------------------------------------

namespace stk {
namespace io {
namespace util {

//----------------------------------------------------------------------

GearFields::GearFields( stk::mesh::MetaData & S )
  : gear_coord(          S.declare_field<CylindricalField>(stk::topology::NODE_RANK, std::string("gear_coordinates") ) ),
    model_coord(         S.declare_field<CartesianField>(stk::topology::NODE_RANK, std::string("coordinates") ) )
{
  const stk::mesh::Part & universe = S.universal_part();

  stk::mesh::put_field( gear_coord    , universe , SpatialDimension );
  stk::mesh::put_field( model_coord   , universe , SpatialDimension );
}

//----------------------------------------------------------------------

namespace {

stk::mesh::EntityId
identifier( size_t nthick ,  // Number of entities through the thickness
		    size_t nradius , // Number of entities through the radius
		    size_t iz ,      // Thickness index
		    size_t ir ,      // Radial index
		    size_t ia )      // Angle index
{
  return static_cast<stk::mesh::EntityId>(iz + nthick * ( ir + nradius * ia ));
}

}


Gear::Gear( stk::mesh::MetaData & S ,
            const std::string & name ,
            const GearFields & gear_fields ,
            const double center[] ,
            const double rad_min ,
            const double rad_max ,
            const size_t rad_num ,
            const double z_min ,
            const double z_max ,
            const size_t z_num ,
            const size_t angle_num ,
            const int      turn_direction )
  : m_mesh_meta_data( S ),
    m_mesh( NULL ),
    m_gear( S.declare_part(std::string("Gear_").append(name), stk::topology::ELEMENT_RANK) ),
    m_surf( S.declare_part(std::string("Surf_").append(name), m_mesh_meta_data.side_rank()) ),
    m_gear_coord( gear_fields.gear_coord ),
    m_model_coord(gear_fields.model_coord )
{
  enum { SpatialDimension = GearFields::SpatialDimension };

  stk::io::put_io_part_attribute(m_gear);
  stk::io::put_io_part_attribute(m_surf);

  stk::mesh::set_topology( m_gear, stk::topology::HEX_8 );
  stk::mesh::set_topology( m_surf, stk::topology::QUAD_8 );

  // Meshing parameters for this gear:

  const double TWO_PI = 2.0 * acos( static_cast<double>(-1.0) );

  m_center[0] = center[0] ;
  m_center[1] = center[1] ;
  m_center[2] = center[2] ;

  m_z_min     = z_min ;
  m_z_max     = z_max ;
  m_z_inc     = (z_max - z_min) / static_cast<double>(z_num - 1);

  m_rad_min   = rad_min ;
  m_rad_max   = rad_max ;
  m_rad_inc   = (rad_max - rad_min) / static_cast<double>(rad_num - 1);

  m_ang_inc   = TWO_PI / static_cast<double>(angle_num) ;

  m_rad_num   = rad_num ;
  m_z_num     = z_num ;
  m_angle_num = angle_num ;
  m_turn_dir  = turn_direction ;
}


//----------------------------------------------------------------------

stk::mesh::Entity Gear::create_node(const std::vector<stk::mesh::Part*> & parts ,
                                     stk::mesh::EntityId node_id_base ,
                                     size_t iz ,
                                     size_t ir ,
                                     size_t ia ) const
{
  const double angle     = m_ang_inc * ia ;
  const double cos_angle = cos( angle );
  const double sin_angle = sin( angle );

  const double radius = m_rad_min + m_rad_inc * ir ;
  const double x = m_center[0] + radius * cos_angle ;
  const double y = m_center[1] + radius * sin_angle ;
  const double z = m_center[2] + m_z_min + m_z_inc * iz ;

  // Create the node and set the model_coordinates

  stk::mesh::EntityId id_gear = identifier( m_z_num, m_rad_num, iz, ir, ia );
  stk::mesh::EntityId id = node_id_base + id_gear ;

  stk::mesh::Entity node = m_mesh->declare_entity( stk::topology::NODE_RANK, id , parts );

  double * const gear_data    = stk::mesh::field_data( m_gear_coord , node );
  double * const model_data   = stk::mesh::field_data( m_model_coord , node );

  gear_data[0] = radius ;
  gear_data[1] = angle ;
  gear_data[2] = z - m_center[2] ;

  model_data[0] = x ;
  model_data[1] = y ;
  model_data[2] = z ;

  return node;
}

//----------------------------------------------------------------------

void Gear::mesh( stk::mesh::BulkData & M )
{
  stk::mesh::EntityRank element_rank = stk::topology::ELEMENT_RANK;
  stk::mesh::EntityRank side_rank = m_mesh_meta_data.side_rank();

  M.modification_begin();

  m_mesh = & M ;

  const unsigned p_size = M.parallel_size();
  const unsigned p_rank = M.parallel_rank();

  std::vector<size_t> counts ;
  stk::mesh::comm_mesh_counts(M, counts);

  // max_id is no longer available from comm_mesh_stats.
  // If we assume uniform numbering from 1.., then max_id
  // should be equal to counts...
  const stk::mesh::EntityId node_id_base = counts[ stk::topology::NODE_RANK ] + 1 ;
  const stk::mesh::EntityId elem_id_base = counts[ element_rank ] + 1 ;

  const unsigned long elem_id_gear_max =
    m_angle_num * ( m_rad_num - 1 ) * ( m_z_num - 1 );

  std::vector<stk::mesh::Part*> elem_parts ;
  std::vector<stk::mesh::Part*> face_parts ;
  std::vector<stk::mesh::Part*> node_parts ;

  {
    stk::mesh::Part * const p_gear = & m_gear ;
    stk::mesh::Part * const p_surf = & m_surf ;

    elem_parts.push_back( p_gear );
    face_parts.push_back( p_surf );
  }

  for ( unsigned ia = 0 ; ia < m_angle_num ; ++ia ) {
    for ( unsigned ir = 0 ; ir < m_rad_num - 1 ; ++ir ) {
      for ( unsigned iz = 0 ; iz < m_z_num - 1 ; ++iz ) {

        stk::mesh::EntityId elem_id_gear = identifier( m_z_num-1 , m_rad_num-1 , iz , ir , ia );

        if ( ( ( elem_id_gear * p_size ) / elem_id_gear_max ) == p_rank ) {

          stk::mesh::EntityId elem_id = elem_id_base + elem_id_gear ;

          // Create the node and set the model_coordinates

          const size_t ia_1 = ( ia + 1 ) % m_angle_num ;
          const size_t ir_1 = ir + 1 ;
          const size_t iz_1 = iz + 1 ;

          stk::mesh::Entity node[8] ;

          node[0] = create_node( node_parts, node_id_base, iz  , ir  , ia_1 );
          node[1] = create_node( node_parts, node_id_base, iz_1, ir  , ia_1 );
          node[2] = create_node( node_parts, node_id_base, iz_1, ir  , ia   );
          node[3] = create_node( node_parts, node_id_base, iz  , ir  , ia   );
          node[4] = create_node( node_parts, node_id_base, iz  , ir_1, ia_1 );
          node[5] = create_node( node_parts, node_id_base, iz_1, ir_1, ia_1 );
          node[6] = create_node( node_parts, node_id_base, iz_1, ir_1, ia   );
          node[7] = create_node( node_parts, node_id_base, iz  , ir_1, ia   );
#if 0 /* VERIFY_CENTROID */

          // Centroid of the element for verification

          const double TWO_PI = 2.0 * acos( -1.0 );
          const double angle = m_ang_inc * (0.5 + ia);
          const double z = m_center[2] + m_z_min + m_z_inc * (0.5 + iz);

          double c[3] = { 0 , 0 , 0 };

          for ( size_t j = 0 ; j < 8 ; ++j ) {
            double * const coord_data = field_data( m_model_coord , *node[j] );
            c[0] += coord_data[0] ;
            c[1] += coord_data[1] ;
            c[2] += coord_data[2] ;
          }
          c[0] /= 8 ; c[1] /= 8 ; c[2] /= 8 ;
          c[0] -= m_center[0] ;
          c[1] -= m_center[1] ;

          double val_a = atan2( c[1] , c[0] );
          if ( val_a < 0 ) { val_a += TWO_PI ; }
          const double err_a = angle - val_a ;
          const double err_z = z - c[2] ;

          const double eps = 100 * std::numeric_limits<double>::epsilon();

          if ( err_z < - eps || eps < err_z ||
               err_a < - eps || eps < err_a ) {
            std::string msg ;
            msg.append("problem setup element centroid error" );
            throw std::logic_error( msg );
          }
#endif

          stk::mesh::Entity elem =
            M.declare_entity( element_rank, elem_id, elem_parts );

          for ( size_t j = 0 ; j < 8 ; ++j ) {
            M.declare_relation( elem , node[j] ,
                                static_cast<unsigned>(j) );
          }
        }
      }
    }
  }

  // Array of faces on the surface

  {
    const size_t ir = m_rad_num - 1 ;

    for ( size_t ia = 0 ; ia < m_angle_num ; ++ia ) {
      for ( size_t iz = 0 ; iz < m_z_num - 1 ; ++iz ) {

        stk::mesh::EntityId elem_id_gear =
          identifier( m_z_num-1 , m_rad_num-1 , iz , ir-1 , ia );

        if ( ( ( elem_id_gear * p_size ) / elem_id_gear_max ) == p_rank ) {

          stk::mesh::EntityId elem_id = elem_id_base + elem_id_gear ;

          unsigned face_ord = 5 ;
          stk::mesh::EntityId face_id = elem_id * 10 + face_ord + 1;

          stk::mesh::Entity node[4] ;

          const size_t ia_1 = ( ia + 1 ) % m_angle_num ;
          const size_t iz_1 = iz + 1 ;

          node[0] = create_node( node_parts, node_id_base, iz  , ir  , ia_1 );
          node[1] = create_node( node_parts, node_id_base, iz_1, ir  , ia_1 );
          node[2] = create_node( node_parts, node_id_base, iz_1, ir  , ia   );
          node[3] = create_node( node_parts, node_id_base, iz  , ir  , ia   );

          stk::mesh::Entity face =
            M.declare_entity( side_rank, face_id, face_parts );

          for ( size_t j = 0 ; j < 4 ; ++j ) {
            M.declare_relation( face , node[j] ,
                                static_cast<unsigned>(j) );
          }

          stk::mesh::Entity elem = M.get_entity(element_rank, elem_id);

          stk::topology elem_topo = M.bucket(elem).topology();
          stk::mesh::Permutation perm =
                  M.find_permutation(elem_topo, M.begin_nodes(elem),
                                     elem_topo.face_topology(face_ord), node, face_ord);
          ThrowRequireMsg(perm != stk::mesh::INVALID_PERMUTATION,
                          "Gear::mesh():  could not find valid permutation to connect face to element");

          M.declare_relation( elem , face , face_ord, perm );
        }
      }
    }
  }
  // ALAN: modification_begin again? should be modification_end?
  M.modification_begin();
}

//----------------------------------------------------------------------
// Iterate nodes and turn them by the angle

void Gear::turn( double /* turn_angle */ ) const
{
#if 0
  const unsigned Length = 3 ;

  const stk::mesh::BucketVector & ks = m_mesh->buckets( stk::mesh::Node );
  const stk::mesh::BucketVector::const_iterator ek = ks.end();
  stk::mesh::BucketVector::const_iterator ik = ks.begin();
  for ( ; ik != ek ; ++ik ) {
    stk::mesh::Bucket & k = **ik ;
    if ( k.has_superset( m_gear ) ) {
      const size_t n = k.size();
      double * const bucket_gear_data    = stk::mesh::field_data( m_gear_coord,    k.begin() );
      double * const bucket_model_data   = stk::mesh::field_data( m_model_coord,   k.begin() );

      for ( size_t i = 0 ; i < n ; ++i ) {
        double * const gear_data    = bucket_gear_data    + i * Length ;
        double * const model_data   = bucket_model_data   + i * Length ;
        double * const current_data = bucket_current_data + i * Length ;
        double * const disp_data    = bucket_disp_data    + i * Length ;

        const double radius = gear_data[0] ;
        const double angle  = gear_data[1] + turn_angle * m_turn_dir ;

        current_data[0] = m_center[0] + radius * cos( angle );
        current_data[1] = m_center[1] + radius * sin( angle );
        current_data[2] = m_center[2] + gear_data[2] ;

        disp_data[0] = current_data[0] - model_data[0] ;
        disp_data[1] = current_data[1] - model_data[1] ;
        disp_data[2] = current_data[2] - model_data[2] ;
      }
    }
  }
#endif
}

//----------------------------------------------------------------------

}
}
}
