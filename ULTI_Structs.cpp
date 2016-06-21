#include "ULTI_Structs.h"
#include <iostream>
namespace ulti{

const coord& coord::operator =(const coord& toset){
 x=toset.x;
 y=toset.y;
 z=toset.z;
 return *this;
}

bool coord::operator ==(const coord& tocheck){
 return (x == tocheck.x && y == tocheck.y && z == tocheck.z);
}
bool coord::operator !=(const coord& tocheck){
 return (x != tocheck.x || y != tocheck.y || z != tocheck.z);
}

coord::coord(){
    x=y=z=0;
}
coord::coord(double tx, double ty, double tz){
    x=tx;
    y=ty;
    z=tz;
}
coord::coord(icoord cor){
    x=cor.x;
    y=cor.y;
    z=cor.z;
}




const icoord& icoord::operator =(const icoord& toset){
 x=toset.x;
 y=toset.y;
 z=toset.z;
 return *this;
}

bool icoord::operator ==(const icoord& tocheck){
 return (x == tocheck.x && y == tocheck.y && z==tocheck.z);
}
bool icoord::operator !=(const icoord& tocheck){
 return (x != tocheck.x || y != tocheck.y || z != tocheck.z);
}

const dcoord& dcoord::operator =(const dcoord& toset){
 if (this!=&toset){
    pos=toset.pos;
    vel=toset.vel;
 }
 return *this;
}

bool dcoord::operator ==(const dcoord& tocheck){
 return (pos==tocheck.pos&&vel==tocheck.vel);
}
bool dcoord::operator !=(const dcoord& tocheck){
 return !(*this==tocheck);
}

dcoord::dcoord(){}
dcoord::dcoord(double ix, double iy, double vx, double vy){pos.x=ix,pos.y=iy;vel.x=vx;vel.y=vy;}
dcoord::dcoord(const dcoord& dc){*this=dc;}

icoord::icoord(){
    x=y=z=0;
}
icoord::icoord(int tx, int ty, int tz){
    x=tx;
    y=ty;
    z=tz;
}
icoord::icoord(coord cor){
    x=cor.x;
    y=cor.y;
    z=cor.z;
}

element::element(){ulval=0;valtype=valuetype::notype;extra=NULL;}
element::~element(){}
    element::element(valuetype typ):element(){valtype=typ;}
    element::element(const std::map<std::string,element>& val):element(){*this=val;}
    element::element(const std::string& val):element(){value=val;valtype=valuetype::stringtype;}
    element::element(int val):element(){*this=val;}
    element::element(short val):element(){*this=val;}
    element::element(long val):element(){*this=val;}
    element::element(long long val):element(){*this=val;}
    element::element(char val):element(){*this=val;}
    element::element(double val):element(){*this=val;}
    element::element(ui64 val):element(){*this=val;}
    element::element(unsigned long val):element(){*this=val;}
    element::element(ui32 val):element(){*this=val;}
    element::element(ui16 val):element(){*this=val;}
    element::element(ui8 val):element(){*this=val;}
    element::element(const char* val):element(){*this=val;}
    element::element(const std::vector<element>& val):element(){*this=val;}
    element::element(std::initializer_list<element> val):element(){*this=val;}

    element& element::operator=(const element& in){if(this!=&in){this->ulval=in.ulval;this->valtype=in.valtype;this->value=in.value;this->children=in.children;this->ray=in.ray;extra=in.extra;}return *this;}
	element& element::operator=(const std::string& in) { this->value = in; this->valtype = valuetype::stringtype; return *this; }
    element& element::operator=(const int& in){(*((int*)(&ulval)))=in;valtype=valuetype::inttype; return *this;}
	element& element::operator=(const short& in) {  (*((short*)(&ulval))) = in; valtype = valuetype::shorttype; return *this; }
	element& element::operator=(const long& in) {  (*((long long*)(&ulval))) = in; valtype = valuetype::longlongtype; return *this; }
	element& element::operator=(const long long& in) {  (*((long*)(&ulval))) = in; valtype = valuetype::longtype; return *this; }
	element& element::operator=(const char& in) {  (*((char*)(&ulval))) = in; valtype = valuetype::chartype; return *this; }
	element& element::operator=(const double& in) {  (*((double*)(&ulval))) = in; valtype = valuetype::doubletype; return *this; }
	element& element::operator=(const ui64& in) {  ulval = in; valtype = valuetype::ulonglongtype; return *this; }
	element& element::operator=(const unsigned long& in) {  (*((unsigned long*)(&ulval))) = in; valtype = valuetype::ulongtype; return *this; }
	element& element::operator=(const ui32& in) {  (*((ui32*)(&ulval))) = in; valtype = valuetype::uinttype; return *this; }
	element& element::operator=(const ui16& in) {  (*((ui16*)(&ulval))) = in; valtype = valuetype::ushorttype; return *this; }
	element& element::operator=(const ui8& in) {  (*((ui8*)(&ulval))) = in; valtype = valuetype::bytetype; return *this; }
	element& element::operator=(const std::map<std::string, element>& in) {  children = in; valtype = valuetype::maptype; return *this; }
	element& element::operator=(const char* in) {  value = in; valtype = valuetype::stringtype; return *this; }
	element& element::operator=(const std::vector<element>& in) {  ray = in; valtype = valuetype::arraytype; return*this; }
	element& element::operator=(std::initializer_list<element> in) {  ray = in; valtype = valuetype::arraytype; return*this; }



    bool element::operator==(const element& in)const{if (this==&in)return true;return in.valtype==this->valtype&&in.value==this->value&&this->children==in.children&&this->ray==in.ray;}
    bool element::operator==(const std::string& in)const{return this->valtype==valuetype::stringtype && this->value==in;}
    bool element::operator==(const int& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<int>()==in;}
    bool element::operator==(const short& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<short>()==in;}
    bool element::operator==(const long& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<long>()==in;}
    bool element::operator==(const long long& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<long long>()==in;}
    bool element::operator==(const double& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<double>()==in;}
    bool element::operator==(const char& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<char>()==in;}
    bool element::operator==(const ui64& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<ui64>()==in;}
    bool element::operator==(const unsigned long& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<unsigned long>()==in;}
    bool element::operator==(const ui32& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<ui32>()==in;}
    bool element::operator==(const ui16& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<ui16>()==in;}
    bool element::operator==(const ui8& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<ui8>()==in;}
    bool element::operator==(const bool& in){return this->valtype>=valuetype::inttype && this->valtype<=valuetype::ulonglongtype && getT<bool>()==in;}
    bool element::operator==(const std::map<std::string,element>& in){return this->valtype==valuetype::maptype && this->children==in;}
    bool element::operator==(const char* in){return this->valtype==valuetype::stringtype && this->value == in;}
    bool element::operator==(const std::vector<element>& in){return this->valtype==valuetype::arraytype && this->ray == in;}

    bool element::operator!=(const element& in)const{if (this==&in)return true;return in.valtype==this->valtype&&in.value==this->value&&this->children==in.children&&this->ray==in.ray;}
    bool element::operator!=(const std::string& in)const{return this->valtype==valuetype::stringtype && this->value==in;}
    bool element::operator!=(const int& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<int>()!=in;}
    bool element::operator!=(const short& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<short>()!=in;}
    bool element::operator!=(const long& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<long>()!=in;}
    bool element::operator!=(const long long& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<long long>()!=in;}
    bool element::operator!=(const double& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<double>()!=in;}
    bool element::operator!=(const char& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<char>()!=in;}
    bool element::operator!=(const ui64& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<ui64>()!=in;}
    bool element::operator!=(const unsigned long& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<unsigned long>()!=in;}
    bool element::operator!=(const ui32& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<ui32>()!=in;}
    bool element::operator!=(const ui16& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<ui16>()!=in;}
    bool element::operator!=(const ui8& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<ui8>()!=in;}
    bool element::operator!=(const bool& in){return this->valtype<valuetype::inttype || this->valtype>valuetype::ulonglongtype || getT<bool>()!=in;}
    bool element::operator!=(const std::map<std::string,element>& in){return this->valtype!=valuetype::maptype || this->children!=in;}
    bool element::operator!=(const char* in){return this->valtype!=valuetype::stringtype || this->value != in;}
    bool element::operator!=(const std::vector<element>& in){return this->valtype!=valuetype::arraytype || this->ray != in;}
    //bool element::operator==(std::initializer_list<element> in){return this->valtype==valuetype::arraytype && this->ray == in;}


    void element::clear(){
        ray.clear();
        children.clear();
        ulval=0;
        value="";
        valtype=valuetype::notype;
        extra=NULL;
    }

    element& element::operator << (const ulti::element& prop){
        push_back(prop);
        return *this;
    }
    std::ostream& operator<<(std::ostream& os, const element& dt){
        os << dt.getStr();
        return os;
    }
    std::istream& operator>>(std::istream& is, element& dt){
        std::string setter = "";
        char buf = is.get();
        while(!is.eof()){
            setter.push_back(buf);
            buf = is.get();
        }
        dt.setStr(setter);
        return is;
    }
    element& element::setStr(const std::string& setter){
        children.clear();
        ray.clear();
        value="";
        ulval=0;
        std::string csetter = cf(setter);
        if (csetter.size()>0){
            if (csetter[0]=='['){
                int tostart = 1;
                //array
                int raydepth = 0;
                int mapdepth = 0;
                int stringdepth = 0;
                int closer = csetter.size()-1;
                for(int a = tostart; a < closer; a++){
                    if (csetter[a]=='\\'){a++;continue;}//ignore this and next char
                    if (stringdepth==0 && csetter[a]=='[')raydepth++;
                    if (stringdepth==0 && csetter[a]=='{')mapdepth++;
                    if (stringdepth==0 && csetter[a]=='}')mapdepth--;
                    if (stringdepth==0 && csetter[a]==']')raydepth--;
                    if (csetter[a]=='\"'){
                        stringdepth = !stringdepth;
                    }
                    if (stringdepth==0&&raydepth==0&&mapdepth==0){
                        if (a==closer-1){
                            ray.push_back(element());
                            //ray.resize(ray.size()+1);
                            ray.back().setStr(csetter.substr(tostart,a-tostart+1));
                            tostart=a+1;
                        }
                        else if (csetter[a]==','){
                            ray.push_back(element());
                            //ray.resize(ray.size()+1);
                            ray.back().setStr(csetter.substr(tostart,a-tostart));
                            tostart=a+1;
                        }

                    }

                }
                valtype=valuetype::arraytype;
            }
            else if (csetter[0]=='{'){
                int tostart = 1;
                //map
                int raydepth = 0;
                int mapdepth = 0;
                int stringdepth = 0;
                std::string tname = "";
                int closer = csetter.size()-1;
                for(int a = tostart; a < closer; a++){
                    char buf = csetter[a];
                    if (csetter[a]=='\\'){a++;continue;}//ignore this and next char
                    if (stringdepth==0 && csetter[a]=='[')raydepth++;
                    if (stringdepth==0 && csetter[a]=='{')mapdepth++;
                    if (stringdepth==0 && csetter[a]=='}')mapdepth--;
                    if (stringdepth==0 && csetter[a]==']')raydepth--;

                    if (csetter[a]=='\"'){
                        stringdepth = !stringdepth;
                    }
                    if (stringdepth==0&&raydepth==0&&mapdepth==0){
                        if (a==closer-1){
                            children[tname].setStr(csetter.substr(tostart,a-tostart+1));
                        }
                        else if (csetter[a]==','){
                            //ray.push_back(element());
                            //ray.back().setStr(csetter.substr(tostart,a));
                            children[tname].setStr(csetter.substr(tostart,a-tostart));
                            tostart=a+1;
                        }
                        else if (csetter[a]==':'){
                            tname = stripQuotes(csetter.substr(tostart,a-tostart));
                            tostart = a+1;
                        }

                    }

                }
                valtype=valuetype::maptype;
            }
            else if (csetter[0]=='\"'){
                value=ulti::stripQuotes(ulti::quickUnescapeJson(csetter));
                valtype=valuetype::stringtype;
                //string
            }
            else if (csetter[0]=='<'){
                setXml(csetter);
            }
            else{
                stringNumericType dtype = ulti::getStringIntType(csetter);
                if (dtype!=ulti::stringNumericType::invalidNumberString){

                    if (dtype==ulti::stringNumericType::doubleString||dtype==ulti::stringNumericType::exponentString){
                        *this = (double)ulti::atod(csetter);
                        valtype=valuetype::doubletype;
                    }
                    else{
                        *this = ulti::atol(csetter);
                        valtype=valuetype::longtype;
                    }
                }
                else{
                    if (strtolower(csetter)=="true"){
                        *this = true;
                    }
                    else if (strtolower(csetter)=="false"){
                        *this = false;
                    }
                }

                //i64
            }
        }
        return *this;
    }
    //WE MAY CONSIDER SWAPPING THESE TWO FUNCTIONS. THE FRIEND OSTREAM AND THE GETSTR FUNCTION, FOR EFFECIENCY'S SAKE
    std::string element::getStr() const{
        static std::vector<std::function<void (const element&,std::ostream&)> > funcs = {
            [](const element& dt,std::ostream& ss){ss << "";},
            [](const element& dt,std::ostream& ss){ss << dt.value;},
            [](const element& dt,std::ostream& ss){ss << (*((int*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((double*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((long*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((long long*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((short*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((char*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((bool*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui8*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui16*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui32*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((unsigned long*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui64*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << "{";auto et = (dt.children.end());if(et!=dt.children.begin()){--et;} for(auto it = dt.children.begin(); it!=dt.children.end(); it++){std::string elquote=(it->second.valtype==valuetype::stringtype||it->second.valtype==valuetype::chartype?"\"":"");ss << "\"" << ulti::quickEscapeJson(it->first) << "\":" << elquote << (it->second.valtype==valuetype::stringtype?quickEscapeJsonTemplate(it->second):it->second) << elquote; if ((it)!=et){ss << ",";}} ss << "}";},
            [](const element& dt,std::ostream& ss){ss << "[";auto et = (dt.ray.end());if(et!=dt.ray.begin()){--et;}  for(auto it = dt.ray.begin(), et = --(dt.ray.end()); it!=dt.ray.end(); it++){std::string elquote=(it->valtype==valuetype::stringtype||it->valtype==valuetype::chartype?"\"":"");ss << elquote<<  (it->valtype==valuetype::stringtype?quickEscapeJsonTemplate(*it):*it)  << elquote; if (it!=et){ss << ",";}} ss << "]";},
            [](const element& dt,std::ostream& ss){ss << dt.getXml();}
            };
        std::stringstream ss;
        funcs[this->valtype](*this,ss);
        return ss.str();
    }

    std::string element::getJson() const{
        static std::vector<std::function<void (const element&,std::ostream&)> > funcs = {
            [](const element& dt,std::ostream& ss){ss << "";},
            [](const element& dt,std::ostream& ss){ss << dt.value;},
            [](const element& dt,std::ostream& ss){ss << (*((int*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((double*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((long*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((long long*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((short*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((char*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((bool*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui8*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui16*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui32*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((unsigned long*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << (*((ui64*)(&dt.ulval)));},
            [](const element& dt,std::ostream& ss){ss << "{";auto et = (dt.children.end());if(et!=dt.children.begin()){--et;} for(auto it = dt.children.begin(); it!=dt.children.end(); it++){std::string elquote=(it->second.valtype==valuetype::stringtype||it->second.valtype==valuetype::chartype?"\"":"");ss << "\"" << ulti::quickEscapeJson(it->first) << "\":" << elquote << (it->second.valtype==valuetype::stringtype?quickEscapeJsonTemplate(it->second):it->second) << elquote; if ((it)!=et){ss << ",";}} ss << "}";},
            [](const element& dt,std::ostream& ss){ss << "[";auto et = (dt.ray.end());if(et!=dt.ray.begin()){--et;}  for(auto it = dt.ray.begin(), et = --(dt.ray.end()); it!=dt.ray.end(); it++){std::string elquote=(it->valtype==valuetype::stringtype||it->valtype==valuetype::chartype?"\"":"");ss << elquote<<  (it->valtype==valuetype::stringtype?quickEscapeJsonTemplate(*it):*it)  << elquote; if (it!=et){ss << ",";}} ss << "]";},
            [](const element& dt,std::ostream& ss){ss << dt.getXml();}
            };
        std::stringstream ss;
        funcs[this->valtype](*this,ss);
        return ss.str();
    }

    std::string element::getXml() const{
        static std::vector<std::function<std::string (const element&)> > funcs = {
            [](const element& dt)->std::string{return "";},
            [](const element& dt)->std::string{return dt.value;},
            [](const element& dt)->std::string{return ulti::hackString(*((int*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((double*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((long*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((long long*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((short*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((char*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((bool*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((ui8*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((ui16*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((ui32*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((unsigned long*)(&dt.ulval)));},
            [](const element& dt)->std::string{return ulti::hackString(*((ui64*)(&dt.ulval)));},
            [](const element& dt)->std::string{return dt.getXml();},
            [](const element& dt)->std::string{return dt.getXml();},
            [](const element& dt)->std::string{return dt.getXml();}
            };

        if (valtype==valuetype::xmltype||valtype==valuetype::maptype||valtype==valuetype::arraytype){
            std::vector<std::string> props;
            std::vector<std::string> kids;
            for(auto it = ray.begin(); it != ray.end(); it++){
                kids.push_back(funcs[it->valtype](*it));
            }
            for(auto it = children.begin(); it != children.end(); it++){
                props.push_back((ulti::stringBuilder() << it->first << "=\"" << ulti::quickEscapeXml(funcs[it->second.valtype](it->second)) << "\"").str());
            }

            return (ulti::stringBuilder() << "<" << value << (props.size()>0?" ":"") << ulti::unsplit(props," ") << ">" << ulti::unsplit(kids,"") << "</" << value << ">").str();
        }
        else{
            return ulti::quickEscapeXml(funcs[valtype](*this));
        }
    }
    element& element::setXml(const std::string& setter){
        std::vector<std::string> vec = ulti::separateXmlPoints(setter);
        if (vec.size()==1){
            std::string tmp = ulti::getContentOfNextXmlPoint(vec.back());
            auto vec2 = ulti::separateXmlPoints(tmp);
            if (vec2.size()>0){
                value=ulti::getNextXmlPointName(vec.back());
                valtype=ulti::valuetype::xmltype;
                std::map<std::string,std::string> att = ulti::splitAttributes(ulti::getNextXmlPointAttributes(vec.back()));

                for(auto it = att.begin(); it != att.end(); it++){
                    if (it->first.size()>0){
                        children[it->first]=it->second;
                    }
                }

                for(int a = 0; a < vec2.size(); a++){
                    ray.push_back(element());
                    ray.back().setXml(vec2[a]);
                }
            }
            else{
                value=ulti::getNextXmlPointName(vec.back());
                valtype=ulti::valuetype::xmltype;
                std::map<std::string,std::string> att = ulti::splitAttributes(ulti::getNextXmlPointAttributes(vec.back()));
                for(auto it = att.begin(); it != att.end(); it++){
                    if (it->first.size()>0){
                        children[it->first]=it->second;
                    }
                }
                if (value.size()==0){
                    value=setter;
                    valtype=valuetype::stringtype;
                }
            }
        }
        else{
            if (vec.size()>0){
                value=ulti::getNextXmlPointName(vec.back());
                valtype=ulti::valuetype::xmltype;
                std::map<std::string,std::string> att = ulti::splitAttributes(ulti::getNextXmlPointAttributes(vec.back()));

                for(auto it = att.begin(); it != att.end(); it++){
                    if (it->first.size()>0){
                        children[it->first]=it->second;
                    }
                }

                for(int a = 0; a < vec.size(); a++){
                    ray.push_back(element());
                    ray.back().setXml(vec[a]);
                }
            }
            else{
                *this = setter;

            }
        }
        return *this;
    }

    element& element::operator[] (const int dex){
        valtype=valuetype::arraytype;
        if (dex>=ray.size()){
                ray.resize(dex+1);
        }
        return ray[dex];
    }
    element& element::operator[] (const std::string& dex){
        valtype=valuetype::maptype;
        return children[dex];
    }

    long element::size()const{
        if (valtype==valuetype::arraytype||valtype==valuetype::xmltype){
            return ray.size();
        }
        else if (valtype==valuetype::maptype){
            return children.size();
        }
        else return 0;
    }
    element& element::back(){
        if (valtype==valuetype::arraytype||valtype==valuetype::xmltype){
            return ray.back();
        }
        else if (valtype==valuetype::maptype){
            return (--children.end())->second;
        }
        else return *this;
    }
    element& element::front(){
        if (valtype==valuetype::arraytype||valtype==valuetype::xmltype){
            return ray.front();
        }
        else if (valtype==valuetype::maptype){
            return (children.begin())->second;
        }
        else return *this;
    }
    void element::push_back(const element& in){
        valtype==ulti::valuetype::arraytype;
        ray.push_back(in);
    }
    void element::pop_back(){
        ray.pop_back();
    }
    bool element::erase(const int dex){
        if (valtype==valuetype::arraytype||valtype==valuetype::xmltype){
            ray.erase(ray.begin()+dex);
            return true;
        }
        return false;
    }
    bool element::erase(const std::string& dex){
        if (valtype==valuetype::maptype||valtype==valuetype::xmltype){
            children.erase(dex);
            return true;
        }
        return false;
    }
    bool element::checkKey(const std::string& key)const{
        if (valtype==valuetype::maptype||valtype==valuetype::xmltype){
            return (children.find(key)!=children.end());
        }
        return false;
    }
    bool element::ismap()const{
        return (valtype==valuetype::maptype||valtype==valuetype::xmltype);
    }
    bool element::isray()const{
        return (valtype==valuetype::arraytype||valtype==valuetype::xmltype);
    }
    valuetype element::getType()const{
        return (valuetype)valtype;
    }
    void element::setType(valuetype invaltype){
        valtype=invaltype;
    }

    void* element::getExtra(){
        return extra;
    }
    void element::setExtra(void* ex){
        extra=ex;
    }
    ulti::element element::keys(){
        element ret(valuetype::arraytype);
        for(auto it = children.begin(); it!= children.end(); it++){
            ret.push_back(it->first);
        }
        return ret;
    }

    std::string stringlayout(ulti::element& el, int depth){
        std::stringstream ss;
        if (el.getType()==ulti::arraytype){
            for(int a = 0; a < el.size(); a++){
                ss << ulti::repeat(" ",depth) << a << "\r\n";
                ss << stringlayout(el[a],depth+1) << "\r\n";
            }
        }
        else if (el.getType()==ulti::maptype){
            ulti::element mp = el.keys();
            for(int b = 0; b < mp.size(); b++){
                ss << ulti::repeat(" ",depth) << mp[b] << "\r\n";
                //if (el[mp[b].getStr()].getType()==ulti::valuetype::arraytype||el[mp[b].getStr()].getType()==ulti::valuetype::maptype){
                    ss << stringlayout(el[mp[b].getStr()],depth+1) << "\r\n";
                //}
            }
        }
        else{
            ss << ulti::repeat(" ",depth) << ": " << el.getStr();
        }
        return ss.str();
    }

}
