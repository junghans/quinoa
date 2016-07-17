// *****************************************************************************
/*!
  \file      src/IO/ExodusIIMeshReader.h
  \author    J. Bakosi
  \date      Mon 02 May 2016 12:30:40 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     ExodusII mesh reader
  \details   ExodusII mesh reader class declaration.
*/
// *****************************************************************************
#ifndef ExodusIIMeshReader_h
#define ExodusIIMeshReader_h

#include <cstddef>
#include <iosfwd>
#include <vector>
#include <array>
#include <unordered_map>

#include "NoWarning/exodusII.h"

#include "Types.h"
#include "Exception.h"

namespace tk {

class UnsMesh;

//! \brief Supported ExodusII mesh cell types
//! \details This the order in which ExodusIIMeshReader::m_eid stores the
//!   element block IDs.
//! \see ExodusIIMeshReader::readElemBlockIDs()
enum class ExoElemType : int { TET = 0, TRI = 1 };

//! ExodusII mesh-based data reader
//! \details Mesh reader class facilitating reading from mesh-based field data
//!   a file in ExodusII format.
//! \see also http://sourceforge.net/projects/exodusii
class ExodusIIMeshReader {

  public:
    //! Constructor
    explicit ExodusIIMeshReader( const std::string& filename,
                                 int cpuwordsize = sizeof(double),
                                 int iowordsize = sizeof(double) );

    //! Destructor
    ~ExodusIIMeshReader() noexcept;

    //! Read ExodusII mesh from file
    void readMesh( UnsMesh& mesh );

    //! Read only connectivity graph from file
    void readGraph( UnsMesh& mesh );

    //! Read coordinates of a single mesh node from ExodusII file
    //! \param[in] id Node id whose coordinates to read
    //! \param[in,out] x Vector of x coordinates to push to
    //! \param[in,out] y Vector of y coordinates to push to
    //! \param[in,out] z Vector of z coordinates to push to
    void readNode( std::size_t id,
                   std::vector< tk::real >& x,
                   std::vector< tk::real >& y,
                   std::vector< tk::real >& z ) const
    {
      tk::real px, py, pz;
      readNode( id, px, py, pz );
      x.push_back( px );
      y.push_back( py );
      z.push_back( pz );
    }

    //! Read coordinates of a single mesh node from ExodusII file
    //! \param[in] id Node id whose coordinates to read
    //! \param[in,out] coord Array of x, y, and z coordinates
    void readNode( std::size_t id, std::array< tk::real, 3 >& coord ) const
    { readNode( id, coord[0], coord[1], coord[2] ); }

    //! Read coordinates of a number of mesh nodes from ExodusII file
    std::unordered_map< std::size_t, std::array< tk::real, 3 > >
    readNodes( const std::array< std::size_t, 2 >& ext ) const;

    //! Read element block IDs from file
    std::size_t readElemBlockIDs();

    //! Read element connectivity of a single mesh cell from file
    void readElement( std::size_t id,
                      tk::ExoElemType elemtype,
                      std::vector< std::size_t >& conn ) const;

    //! Read element connectivity of a number of mesh cells from file
    void readElements( const std::array< std::size_t, 2 >& extent,
                       tk::ExoElemType elemtype,
                       std::vector< std::size_t >& conn ) const;

    //! Read element connectivity of a number of mesh cells from file
    std::unordered_map< std::size_t, std::vector< std::size_t > >
    readElements( const std::array< std::size_t, 2 >& extent,
                  tk::ExoElemType elemtype ) const;

    //!  Return number of elements in a mesh block in the ExodusII file
    int nel( tk::ExoElemType elemtype ) const;

    //! Read ExodusII header without setting mesh size
    std::size_t readHeader();

  private:
    //! Read ExodusII header with setting mesh size
    void readHeader( UnsMesh& mesh );

    //! Read coordinates of a single mesh node from file
    void readNode( std::size_t id, tk::real& x, tk::real& y, tk::real& z ) const
    {
      ErrChk(
        ex_get_n_coord( m_inFile, static_cast<int64_t>(id)+1, 1, &x, &y, &z )
          == 0,
        "Failed to read coordinates of node " + std::to_string(id) +
        " from ExodusII file: " + m_filename );
    }

    //! Read all node coordinates from ExodusII file
    void readAllNodes( UnsMesh& mesh ) const;

    //! Read all element blocks and mesh connectivity from ExodusII file
    void readAllElements( UnsMesh& mesh );

    const std::string m_filename;          //!< File name

    //! \brief List of number of nodes per element for different element types
    //!   supported in the order of tk::ExoElemType
    const std::array< std::size_t, 2 > m_nnpe {{ 4, 3 }};

    int m_inFile;                       //!< ExodusII file handle
    std::size_t m_nnode;                //!< Number of nodes in file
    std::size_t m_neblk;                //!< Number of element blocks in file
    std::vector< int > m_eid;           //!< Element block IDs
    std::vector< int > m_eidt;          //!< Element block IDs mapped to enum
    std::vector< int > m_nel; //!< Nunmber of elements in a block mapped to enum
};

} // tk::

#endif // ExodusIIMeshReader_h
