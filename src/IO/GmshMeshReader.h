//******************************************************************************
/*!
  \file      src/IO/GmshMeshReader.h
  \author    J. Bakosi
  \date      Sun 06 Apr 2014 11:02:41 AM MDT
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Gmsh reader class declaration
  \details   Gmsh reader class declaration
*/
//******************************************************************************
#ifndef GmshMeshReader_h
#define GmshMeshReader_h

#include <vector>
#include <map>

#include <Reader.h>
#include <GmshMesh.h>

namespace quinoa {

//! GmshMeshReader : Reader
class GmshMeshReader : public tk::Reader {

  public:
    //! Constructor
    explicit GmshMeshReader( const std::string filename, GmshMesh& mesh ) :
      Reader(filename),
      m_version(0.0),       // 0.0: uninitialized
      m_datasize(0),        //   0: uninitialized
      m_type(-1),           //  -1: uninitialized, 0: ASCII, 1: binary
      m_mesh(mesh) {}

    //! Destructor, default compiler generated
    ~GmshMeshReader() noexcept override = default;

    //! Read Gmsh mesh
    void read() override;

  private:
    //! Don't permit copy constructor
    GmshMeshReader(const GmshMeshReader&) = delete;
    //! Don't permit copy assigment
    GmshMeshReader& operator=(const GmshMeshReader&) = delete;
    //! Don't permit move constructor
    GmshMeshReader(GmshMeshReader&&) = delete;
    //! Don't permit move assigment
    GmshMeshReader& operator=(GmshMeshReader&&) = delete;

    //! Read mandatory "$MeshFormat--$EndMeshFormat" section
    void readMeshFormat();

    //! Read "$Nodes--$EndNodes" section
    void readNodes();

    //! Read "$Elements--$EndElements" section
    void readElements();

    //! Read "$PhysicalNames--$EndPhysicalNames" section
    void readPhysicalNames();

    //! Add new element
    void addElem( int type, const std::vector< int >& nodes );

    //! Add new element tags
    void addElemTags( int type, const std::vector< int >& tags );

    //! Get mesh type
    int type() const {
      Assert( m_type != -1, tk::ExceptType::FATAL, "Mesh type is not yet set");
      return m_type;
    }

    // Get mesh type queries
    bool isASCII() const {
      Assert( m_type != -1, tk::ExceptType::FATAL, "Mesh type is not yet set");
      return m_type == 0 ? true : false;
    }
    bool isBinary() const {
      Assert( m_type != -1, tk::ExceptType::FATAL, "Mesh type is not yet set");
      return m_type == 1 ? true : false;
    }

    tk::real m_version;                 //!< Mesh version in mesh file
    int m_datasize;                     //!< Data size in mesh file
    int m_type;                         //!< Mesh file type: 0:ASCII, 1:binary
    GmshMesh& m_mesh;                   //!< Mesh object

    //! Gmsh element types and their number of nodes,
    //! all Gmsh-supported listed, Quinoa-supported uncommented,
    //! See Gmsh documentation for element ids as keys
    const std::map< int, int > m_elemNodes {
      {  1,   2 },      // 2-node line
      {  2,   3 },      // 3-node triangle
    //{  3,   4 },      // 4-node quadrangle
      {  4,   4 },      // 4-node tetrahedron
    //{  5,   8 },      // 8-node hexahedron
    //{  6,   6 },      // 6-node prism
    //{  7,   5 },      // 5-node pyramid
    //{  8,   3 },      // 3-node second order line
    //{  9,   6 },      // 6-node second order triangle
    //{ 10,   9 },      // 9-node second order quadrangle
    //{ 11,  10 },      // 10-node second order tetrahedron
    //{ 12,  27 },      // 27-node second order hexahedron
    //{ 13,  18 },      // 18-node second order prism
    //{ 14,  14 },      // 14-node second order pyramid
      { 15,   1 }       // 1-node point
    //{ 16,   8 },      // 8-node second order quadrangle
    //{ 17,  20 },      // 20-node second order hexahedron
    //{ 18,  15 },      // 15-node second order prism
    //{ 19,  13 },      // 13-node second order pyramid
    //{ 20,   9 },      // 9-node third order incomplete triangle
    //{ 21,  10 },      // 10-node third order triangle
    //{ 22,  12 },      // 12-node fourth order incomplete triangle
    //{ 23,  15 },      // 15-node fourth order triangle
    //{ 24,  15 },      // 15-node fifth order incomplete triangle
    //{ 25,  21 },      // 21-node fifth order complete triangle
    //{ 26,   4 },      // 4-node third order edge
    //{ 27,   5 },      // 5-node fourth order edge
    //{ 28,   6 },      // 6-node fifth order edge
    //{ 29,  20 },      // 20-node third order tetrahedron
    //{ 30,  35 },      // 35-node fourth order tetrahedron
    //{ 31,  56 },      // 56-node fifth order tetrahedron
    //{ 92,  64 },      // 64-node third order hexahedron
    //{ 93, 125 }       // 125-node fourth order hexahedron
    };
};

} // quinoa::

#endif // GmshMeshReader_h
