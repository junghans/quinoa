//******************************************************************************
/*!
  \file      src/IO/NetgenMeshReader.h
  \author    J. Bakosi
  \date      Wed 28 Jan 2015 08:59:09 AM MST
  \copyright 2012-2014, Jozsef Bakosi.
  \brief     Netgen mesh reader class declaration
  \details   Netgen mesh reader class declaration. Only supports tetrahedra.
*/
//******************************************************************************
#ifndef NetgenMeshReader_h
#define NetgenMeshReader_h

#include <Reader.h>
#include <UnsMesh.h>

namespace quinoa {

//! \brief NetgenMeshReader : tk::Reader
//! \details Mesh reader class facilitating reading a mesh from a file saved by
//!   the Netgen mesh generator:
//!   http://sourceforge.net/apps/mediawiki/netgen-mesher.
class NetgenMeshReader : public tk::Reader {

  public:
    //! Constructor
    explicit NetgenMeshReader( const std::string filename, UnsMesh& mesh ) :
      Reader( filename ),
      m_mesh( mesh ) {}

    //! Read Netgen mesh
    void read() override;

  private:
    //! Read nodes
    void readNodes();

    //! Read element connectivity
    void readElements();

    UnsMesh& m_mesh;                   //!< Mesh object
};

} // quinoa::

#endif // NetgenMeshReader_h
