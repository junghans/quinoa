# command that copies files during build
TRIBITS_COPY_FILES_TO_BINARY_DIR(copy_input_files_for_driver
    SOURCE_FILES
        input_template.xml
        rcbTest.xml
        multiJaggedTest.xml
        GeomGenWeight.txt
        chacoSimple3dTest.xml
        chacoSimpleTest.xml
        chacoVwgtTest.xml    
        chacoVwgt2Test.xml    
        chacoEdgeWeightTest.xml
        geometricTest.xml
        geometricVwgtTest.xml
        geometricVwgt2Test.xml
        Rectilinear.pmgen
        Rectilinear2D.pmgen
        Spherical.pmgen
        Poisson.pmgen
        pamgenRectilinearTest2D.xml 
        pamgenRectilinearTest3D.xml 
        pamgenSphericalTest.xml 
        pamgenPoissonTest.xml 
        galeriTest.xml 
        simple.pmgen
        simplePamgen.xml
        simple3d.pmgen
        simple3dPamgen.xml
        pulpTest.xml
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}
)

