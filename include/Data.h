#ifndef __luadata_data_h__
#define __luadata_data_h__

#include <string>
#include <map>
#include <stdexcept>

#include "DataLoader.h"

// Typedefs
typedef std::string data_key;

typedef bool bool_d;
typedef double number_d;
typedef int int_d;
typedef std::string string_d;

// Forward declaration of all classes.
class DataNode;
template<class T> class DataValue;
class DataTable;
class DataTableIterator;

/**
 * Base element for named data storage.
 */
class DataNode
{
protected:
    const std::string m_name;

    DataNode(const std::string name) :
        m_name(name) {}

public:
    /**
     * Get the name of this data node.
     */
    const std::string& name() const {
        return m_name;
    }

private:
	// Disable copy.
	DataNode(const DataNode&);
	const DataNode& operator=(const DataNode&);

    virtual void foo() {}
};

/**
 * Storage for a key-value pair of typed data. 
 */
template<class T>
class DataValue : public DataNode
{
    friend class DataLoader;
	friend class DataTable;

    const T m_value;

    DataValue(const std::string name, const T value) :
        DataNode(name), m_value(value) {}

	inline const T& value() const {
		return m_value;
	}
};

/**
 * Storage for data number that can be retreived as either
 * integer or floating point values.
 */
class DataNumber
{
	friend class DataLoader;
	friend class DataTable;

	number_d m_dValue;
	int_d m_iValue;

	DataNumber(number_d value) : m_dValue(value) {
		double intpart;
		modf(m_dValue, &intpart);
		m_iValue = int_d(intpart);
	}

	inline const int_d& asInteger() const {
		return m_iValue;
	}

	inline const number_d& asDouble() const {
		return m_dValue;
	}
};

/**
 * Iterator over a data table, with quick data access methods.
 */
class DataTableIterator
{
    friend class DataTable;

    std::map<const std::string, const DataNode*>::const_iterator iterator;

    DataTableIterator(std::map<const std::string, const DataNode*>::const_iterator i) :
      iterator(i) {}

public:
    /**
     * Makes the iterator advance forward.
     */
    DataTableIterator& operator++() {
        ++iterator;
        return *this;
    }

    /**
     * Inequality comparator.
     */
    bool operator !=(const DataTableIterator& right) const {
        return iterator != right.iterator;
    }

    /**
     * Get the name of the node pointed by this iterator.
     */
    const std::string& name() const {
        return iterator->first;
    }

    /**
     * Get the value of the node as a data table.
     */
    const DataTable* get() const;

    /**
     * Get the value of the node as a typed value.
     */
    template<class T>
    const T& get() const {
        return static_cast<const DataValue<T>*>(iterator->second)->value();
    }
};

/**
 * Storage for a collection of key-value.
 */
class DataTable : public DataNode
{
    friend class DataLoader;

    std::map<const std::string, const DataNode*> m_content;

    DataTable(const std::string name) :
        DataNode(name), m_content(std::map<const std::string, const DataNode*>()) {}

public:
    /**
     * Get the value of the node as a data table.
     */
    const DataTable* get(const std::string& name) const
	{
        return static_cast<const DataTable*>(m_content.at(name));
    }
    
    /**
     * Get the value of the node as a typed value.
     */
    template<class T>
    const T& get(const std::string& name) const
	{
        return static_cast<const DataValue<T>*>(m_content.at(name))->value();
    }

	/**
	 * Get the value of the node as an integer value.
	 */
	template<>
	const int_d& get(const std::string& name) const
	{
        return get<DataNumber>(name).asInteger();
	}

	/**
	 * Get the value of the node as a floating point value.
	 */
	template<>
	const number_d& get(const std::string& name) const
	{
        return get<DataNumber>(name).asDouble();
	}

    /**
     * Get the number of elements inside this table.
     */
    std::map<const std::string, const DataNode*>::size_type size() const {
        return m_content.size();
    }

    /**
     * Get an iterator on this table pointing at its beginning.
     */
    DataTableIterator begin() const {
        return DataTableIterator(m_content.begin());
    }

    /**
     * Get an iterator on this table pointing at its end.
     */
    DataTableIterator end() const {
        return DataTableIterator(m_content.end());
    }

private:
    /**
     * Add something inside this table, will be called by friend class.
     */
    void add(const DataNode* value) {
        m_content[value->name()] = value;
    }
};

/**
 * Static class to store and access data.
 */
class Data
{
	friend class DataLoader;

	static DataTable* _data;

public:
	/**
	 * Gets a root data table.
	 */
	static const DataTable* get(const std::string& name) {
		return _data->get(name);
	}
};

#endif /* __luadata_data_h__ */