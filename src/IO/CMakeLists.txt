cmake_minimum_required(VERSION 2.8.5)

project(IO CXX)

add_library(IO
            MeshFactory.C
            GmshMeshReader.C            # readers
            STLTxtMeshReader.C
            NetgenMeshReader.C
            ExodusIIMeshReader.C
            HyperMeshReader.C
            PDFWriter.C                 # writers
            TxtStatWriter.C
            DiagWriter.C
            GmshMeshWriter.C
            NetgenMeshWriter.C
            ExodusIIMeshWriter.C
            H5PartWriter.C
#            SiloWriter.C
)

INSTALL(TARGETS IO
  	RUNTIME DESTINATION bin COMPONENT Runtime
  	LIBRARY DESTINATION lib COMPONENT Runtime
 	ARCHIVE DESTINATION lib COMPONENT Development
)

# Include function for adding Charm++ modules
include(charm)

add_library(AsyncIO
            ParticleWriter.C
)

addCharmModule( "particlewriter" "AsyncIO" )

# Add extra dependency of AsyncIO on Transporter charm module. This is required
# so that transporter.decl.h and transporter.def.h are generated before AsyncIO
# including those.
add_dependencies( "AsyncIO" "transporterCharmModule" )

INSTALL(TARGETS AsyncIO
  	RUNTIME DESTINATION bin COMPONENT Runtime
  	LIBRARY DESTINATION lib COMPONENT Runtime
 	ARCHIVE DESTINATION lib COMPONENT Development
)
