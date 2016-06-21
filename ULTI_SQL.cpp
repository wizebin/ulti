#include "ULTI_SQL.h"

namespace ulti{

 minitype::minitype ( ) {}
 minitype & minitype::operator = ( const minitype & min ) {}
 minitype::minitype ( const minitype & ) {}
 minitype::minitype ( const string & iname , const string & itype    , int isize   , bool idynamic   , int istype   , bool icannull   , const string & idefaultval    , const string & iextra    ) {}
 string minitype::tostring ( ) {}
 minitype & minitype::fromstring ( const string & data ) {}
 string sqltype::tostring ( ) {}
 sqltype & sqltype::fromstring ( const string & data ) {}
 string sqlrow::tostring ( ) {}
 sqlrow & sqlrow::fromstring ( const string & data ) {}
 string generatecpp ( sqltype & itype , sqlTypeStore & istore ) {}
 string generatejava ( sqltype & itype , sqlTypeStore & istore ) {}
 string generateobjc ( sqltype & itype , sqlTypeStore & istore ) {}
 string generatevb6 ( sqltype & itype , sqlTypeStore & istore ) {}
 int retreiveTypeFromTable ( const string & host , const string & db , const string & table , sqlTypeStore & istore ) {}


}
