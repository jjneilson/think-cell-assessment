#include <map>

template<typename K, typename V>

class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;

public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        if (!(keyBegin < keyEnd))
            return;

	    // Find the insertion points
	    auto itBegin = m_map.upper_bound(keyBegin);
	    auto itEnd = m_map.upper_bound(keyEnd);

	    if (itBegin == itEnd)
	    {
		    m_map.insert(itBegin, std::make_pair(keyBegin,val));
		    m_map.insert(itEnd, std::make_pair(keyEnd, m_valBegin));
	    }

	    // Update the value at the begin key
	    if (itBegin == m_map.end() || itBegin->second != val)
		    m_map.insert_or_assign(itBegin, keyBegin, val);

	    // Remove any entries within the range (excluding the end key)
	    m_map.erase(std::next(itBegin), itEnd);

	    // Insert the end key with the default value if it doesn't already exist
	    if (itEnd == m_map.end() || itEnd->second != m_valBegin)
		    m_map.insert(itEnd, { keyEnd, m_valBegin });
    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};