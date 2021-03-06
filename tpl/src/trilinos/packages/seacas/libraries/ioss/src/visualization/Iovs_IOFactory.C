/*--------------------------------------------------------------------*/
/*    Copyright 2010 Sandia Corporation.                              */
/*    Under the terms of Contract DE-AC04-94AL85000, there is a       */
/*    non-exclusive license for use of this work by or on behalf      */
/*    of the U.S. Government.  Export of this program may require     */
/*    a license from the United States Government.                    */
/*--------------------------------------------------------------------*/

#include <visualization/Iovs_IOFactory.h>
#include <stddef.h>                     // for NULL
#include <visualization/Iovs_DatabaseIO.h>  // for DatabaseIO
#include <string>                       // for string
#include "Ioss_DBUsage.h"               // for DatabaseUsage
#include "Ioss_IOFactory.h"             // for IOFactory
namespace Ioss { class PropertyManager; }
// #include <visualization/Iovs_Internals.h>

namespace Iovs {

  const IOFactory* IOFactory::factory()
  {
    static IOFactory registerThis;
    return &registerThis;
  }

  IOFactory::IOFactory()
    : Ioss::IOFactory("catalyst")
  {
    // Tell the database to register itself with sierra's product registry.
    // XXX exodus doesn't do this, do we need to?
    // register_library_versions();
  }

  Ioss::DatabaseIO* IOFactory::make_IO(const std::string& filename,
				       Ioss::DatabaseUsage db_usage,
				       MPI_Comm communicator,
				       const Ioss::PropertyManager &properties) const
  {
    return new DatabaseIO(NULL, filename, db_usage, communicator, properties);
  }

  /**
   * Call the sierra product registry and register all dependent third-party libraries
   */
  void IOFactory::register_library_versions() const
  {
    // Internals::register_library_versions();
  }

}
