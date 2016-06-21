#ifndef ULTI_SQL_H
#define ULTI_SQL_H

//http://dev.mysql.com/doc/refman/5.0/en/mysql-connect.html
#include <string>
#include <vector>
#include <map>
#include <sstream>
using std::stringstream;
using std::map;
using std::vector;
using std::string;
namespace ulti{


struct minitype{
    string name;
    string type;
    string defaultval;
    string extra;

    int siz;
    int keytype;
    int stype;

    bool dynamic;
    bool cannull;

    minitype();
    minitype& operator=(const minitype& min);
    minitype(const minitype&);
    minitype(const string& iname,const string& itype = "",int isize = 0,bool idynamic = 0,int istype = 0, bool icannull = 0, const string& idefaultval = "", const string& iextra = "");
    string tostring();
    minitype& fromstring(const string& data);
};

class sqltype{
protected:

public:
    string db;
    string table;
    string name; //would be table name as well, probably
    vector<minitype> columns;
    map<string,int> colmap;

    string tostring();
    sqltype& fromstring(const string& data);
};

class sqlTypeStore{
protected:

public:
    vector<sqltype> sqltypes;
    map<string,int> typemap;
};

//Actual data elements downloaded from a table, subclass this
class sqlrow{
protected:

public:
    sqltype* type;

    vector<string> cols;

    template<typename T>
    T parseT(int col){
        stringstream ss(cols[col]);
        T tmp;
        ss >> tmp;
        return tmp;
    }
    template<typename T>
    T getT(int col){
        return *(T*)(&cols[col][0]);
    }
    template<typename T>
    int composeT(int col, T thng){
        stringstream ss();
        ss << thng;
        cols[col]=ss.str();
        return ss.good();
    }
    template<typename T>
    void setT(int col, T thng){
        (*(T*)&cols[col][0])=thng;
    }

    string tostring();
    sqlrow& fromstring(const string& data); ///WILL NOT APPLY TYPE
};


string generatecpp(sqltype& itype, sqlTypeStore& istore);
string generatejava(sqltype& itype, sqlTypeStore& istore);
string generateobjc(sqltype& itype, sqlTypeStore& istore);
string generatevb6(sqltype& itype, sqlTypeStore& istore);

int retreiveTypeFromTable(const string& host, const string& db, const string& table, sqlTypeStore& istore);


};

#endif
