#include "Data.h"

DataTable* Data::_data = 0;

// Implemented outside because the compiler needs to know
// that DataTable inherits from DataNode.
const DataTable* DataTableIterator::get() const {
    return static_cast<const DataTable*>(iterator->second);
}