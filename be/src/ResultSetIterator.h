//
//  DTResultSet.h
//  soul-sifter
//
//  Created by Robby Neale on 3/15/13.
//
//

#ifndef soul_sifter_DTResultSet_h
#define soul_sifter_DTResultSet_h

#include <vector>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

namespace dogatech {
    
    template <typename T>
    class ResultSetIterator {
    public:
        explicit ResultSetIterator(sql::ResultSet* resultset) : rs(resultset) { }
        virtual ~ResultSetIterator() { delete rs; }
        
        bool next(T* obj) {
            if (rs->next()) {
                T::populateFields(rs, obj);
                return true;
            } else {
                return false;
            }
        }
        
        std::vector<T*>* toVector() {
            std::vector<T*>* v = new std::vector<T*>;
            T* obj = new T();
            while (this->next(obj)) {
                v->push_back(obj);
                obj = new T();
            }
            delete obj;
            delete this;
            return v;
        }
        
    private:
        sql::ResultSet* rs;
        ResultSetIterator();
    };

}

#endif
