//
//  IntervalMap.cpp
//  SculpCore
//
//  Created by Andrey on 25.01.2022.
//

#include <map>

template<typename K, typename V>
class IntervalMap
{
public:
	
    IntervalMap(V const& val)
	: m_valBegin(val)
	{}

	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        
        if (!( keyBegin < keyEnd ))
            return;
        
        auto right_val = m_valBegin;
        auto it = m_map.upper_bound(keyEnd);
        if (it != m_map.begin()) {
            right_val = std::next(it, -1)->second;
        }
        
        auto left_pair = m_map.emplace(keyBegin, val);
        auto left = left_pair.first;
        if (!left_pair.second)
            left->second = val;
        
        auto right = m_map.emplace_hint(it, keyEnd, right_val);
        
        assert((std::next(right)) == it);
        
        bool same_left = false;
        if (m_map.begin() != left)
            same_left = std::next(left, -1)->second == val;
        else
            same_left = (val == m_valBegin);
        
        bool same_right = val == right_val;
        
        if (m_map.end() != left && !same_left)
            std::advance(left, 1);
        
        if (m_map.end() != right && same_right)
            std::advance(right, 1);
        
        m_map.erase(left, right);
        
	}

	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
            (--it);
			return it->second;
		}
	}
    
    void print() const
    {
        for (auto v : m_map)
        {
            printf("{%i : %c}, ", v.first, v.second);
        }
        printf("\n");
    }
    
    bool checkCanonical() const
    {
        auto v = m_valBegin;
        
        for (auto entry : m_map)
        {
            if (v == entry.second)
                return false;
            
            v = entry.second;
        }
        
        if (!m_map.empty() && m_map.rbegin()->second != 'q')
            return false;
        
        return true;
    }
    
private:
    
    V m_valBegin;
    std::map<K,V> m_map;
};
