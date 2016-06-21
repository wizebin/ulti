/**

    This is the miscellaneous header file, and contains many many useful functions

**/

#ifndef ULTI_UTILITY_H
#define ULTI_UTILITY_H
#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)||defined(_WIN_32)
    #include <windows.h>
#endif // _WIN_32

namespace ulti{

struct token{
    long stringpos;
    std::string value;
    token();
    token(const std::string& str, long strpos=-1);
    token(const token& tok);
    token& operator=(const token& tok);
};

void permuteRef(std::string& str,int first,std::function<void(const std::string&)> callback);
void permute(const std::string& str, std::function<void(const std::string&)> callback);

//split(...," ") takes ... and splits it into individual strings
//if ... == "hello there my friend" the result would be {"hello","there","my","friend"}
std::vector<std::string> split(const std::string& instring, const std::string& separator);

//Same as split but the separator can be escaped to avoid any potential problems
//meaning the same previous example using "hello\ there my friend" would return
//{"hello there","my","friend"}
std::vector<std::string> splitBackEscaped(const std::string& instring, const std::string& separator);

//Same as split, but returns the [which] data point (meaning between which to which+1 separators) after
//a given string position
token chip(const std::string& instring, const std::string& separator, int which, long after = 0);

//combine a split up vector, unsplit({"hello","there"},"---") becomes "hello---there"
std::string unsplit(const std::vector<std::string>& vec, const std::string& separator);

//similar to split, this will allow you to define a beginning and ending data signature
std::vector<std::string> fragment(const std::string& data, const std::string& head, const std::string& tail);

//the same as fragment, but handles nested depth and only returns topmost complete fragments
std::vector<std::string> fragmentWithNest(const std::string& data, const std::string& head, const std::string& tail);

//same as chip but for fragment, this includes head and tail
std::string findNextFragment(const std::string& data, const std::string& head, const std::string& tail, long startpos = 0);

//same as findNextFragment but does not include head and tail
std::string findContentOfNextFragment(const std::string& data, const std::string& head, const std::string& tail, long startpos = 0);

//same as findContentOfNextFragment but must be after contextword
std::string findContentOfFragmentFollowing(const std::string& data, const std::string& contextword, const std::string& head, const std::string& tail, long startSearch = 0);

//returns a string that includes pattern quantity times
std::string repeat(const std::string& pattern, int quantity);

//fills a block with a single character
void fill(char* data, long datasize, char fillchar);

//fills a block with a pattern
void fill(char* data, long datasize, const std::string& fillstr);

//drops a single instance of indata at &data
void stamp(char* data, const std::string& indata);

//get a quick string representation of an int
template<typename T>
std::string hackString(T in){
    std::stringstream ss;
    ss << in;
    return ss.str();
}

//get a quick(and dirty) <T> from a string
//example int a = revHackString<int>("12");
//For better results try atol or atod which will handle things much better
template<typename T>
T revHackString(const std::string& in){
    std::stringstream ss(in);
    T tmp;
    ss >> tmp;
    return tmp;
}

//Modifies the string that is passed in so each character is lowercase
void strreftolower(std::string& in);

//Makes a copy of in, calls strreftolower on the copy and returns the copy
std::string strtolower(const std::string& in);

//Modifies the string that is passed in so each character is uppercase
void strreftoupper(std::string& in);

//Makes a copy of in, calls strreftoupper on the copy and returns the copy
std::string strtoupper(const std::string& in);

//Uses iterators to make a std::string from a std::wstring, loses precision, be careful
std::string wstrtostr(const std::wstring& str);

//Uses iterators to create a std::wstring from a std::string, gains precision
std::wstring strtowstr(const std::string& str);

//Tokenize, ignoring white space, components of a string
std::vector<token> tokenize(const std::string& str);

//Tokenize, including white space
std::vector<token> fullTokenize(const std::string& str);

//Tokenize, ignoring white space, into a vector of strings instead of tokens
std::vector<std::string> strTokenize(const std::string& str);


//Remove quotes from a string, only if the first and last character are both quotes
std::string stripQuotes(const std::string& str);

//Remove a particular string from a parent string
std::string stripString(const std::string& str, const std::string& tostrip);

//Replace all instances of tofind in str with toreplace
std::string replaceString(const std::string& str, const std::string& tofind, const std::string& toreplace);

//Concatinate strA and strB with glue ("hi","bye","+") = "hi+bye"
std::string joinStrings(const std::string& strA, const std::string& strB, const std::string& glue);

//Modifies str! Replaces escape codes with correct values newlines/tabs/etc
void quickUnescapeJsonRef(std::string& str);

//makes a copy of string, calls quickUnescapeJsonRef and returns the copy
std::string quickUnescapeJson(const std::string& str);

//Escapes special JSON characters, newlines, tabs, commas, and brackets
std::string quickEscapeJson(const std::string& str);

//Escapes xml characters such as < with &lt; and > with &gt;
std::string quickEscapeXml(const std::string& str);

//Unescapes xml characters with the precise format expected as &thing;
std::string quickUnescapeXml(const std::string& str);

//Ummm, not actually sure what I used this for, but anything that can be converted into an output stream
//should work...
template <typename T>
std::string quickEscapeJsonTemplate(const T& t){std::stringstream ss; ss << t; return quickEscapeJson(ss.str());}

//Converts a map into json
std::string quickJson(const std::map<std::string,std::string>& mp);

//Encodes url characters as expected, please ignore the improper _ vs camel case
//This changes any non alphanumeric/-/_/ normal url value into it's %xx equivalent
std::string url_encode(const std::string &value);

//Strip outside spaces from either side of the str
void cfref(std::string& str);

//Copies and calls cfref
std::string cf(const std::string& str);

//Same as chip(str," ",0,startpos)
std::string nextword(const std::string& str, int startpos = 0);

//returns a pointer to the map element if it exists, NULL if not
template<typename T,typename R>
R* mapget(std::map<T,R>& mp, const T & key){
    typename std::map<T,R>::iterator it = mp.find(key); //Typename is required because the std::map isn't defined for this pair yet
    if (it==mp.end())
        return NULL;
    return &it->second;
}

//Returns the value of the characters in a string, meaning the actual bytes, not the representation
template<typename T>
T fromrawstring(const std::string& str){
    return *((T*)&str[0]);
}


//Simple class for easier string inlining
//(stringBuilder() << "hello" << 1 << "bloop").str()
class stringBuilder{
protected:
    std::stringstream mStream;
public:
    template <typename T> inline stringBuilder& operator<<(const T& t)
    {
        mStream << t;
        return * this;
    }
    inline std::string str() const{
        return mStream.str();
    }

    // explicit conversion
    //explicit operator int*() const { return nullptr; }
};

//C++ escape
std::string quickEscape(const std::string& str);
//C++ unescape
std::string quickUnescape(const std::string& str);


//format the map to a string for saving or displaying
template<typename T, typename R>
std::string mapToString(const std::map<T,R> settings){
    std::stringstream ret;
    for (auto it=settings.begin(); it!=settings.end(); ++it)
        ret << "\"" <<  quickEscape(ulti::hackString<T>(it->first)) << "\":\"" << quickEscape(ulti::hackString<R>(it->second)) << "\";\n";
    return ret.str();
}

//unformat the map from a previously called mapToString() perhaps from a save file
template<typename T, typename R>
std::map<T,R> stringToMap(const std::string& content){
    std::map<T,R> ret;

    bool inquote=false;
    bool ignorenext = false;
    T key;
    R val;
    std::string curstr;

    for(unsigned int a = 0; a < content.size(); a++){
        if (content[a]=='\\' && !ignorenext){//ignore next char
            ignorenext=true;
        }
        else{
            if (ignorenext){
                curstr += content[a];
                ignorenext=false;
            }
            else{
                if (content[a]=='"'){inquote=!inquote;}
                else if (inquote){curstr += content[a];}
                else if (isspace(content[a])){}
                else if (content[a]==':'){
                    key=revHackString<T>(curstr);
                    curstr="";
                }
                else if (content[a]==';'){
                    val=revHackString<R>(curstr);
                    curstr="";
                    ret[key]=val;
                }
            }
        }
    }
    return ret;
}

//Fix a string's width to a certain character count
std::string fixWidth(const std::string& istr, int width, bool pre = true, const std::string& filler = "0");//precedes with 0s

//Get the corresponding data point from the number of seconds since the 1/1/1970 epoch
int getMonth(time_t timeval);
int getDay(time_t timeval);
int getYear(time_t timeval);
int getHour(time_t timeval);
int getMin(time_t timeval);
int getSec(time_t timeval);
std::string getWeekday(time_t timeval);
std::string getMonthString(time_t timeval);

std::string getMonthStringFromIndex(int index);
std::string getFullMonthStringFromIndex(int index);

std::string quickFormattedDate(time_t timeval);
std::string formatDatetime(time_t timeval);


//must be in simple notation, no scientific or hex, for those try getStringIntType()
bool isStringInteger(const std::string& str);
bool isStringNumeric(const std::string& str);
bool isStringUnsignedInteger(const std::string& str);

//is check the opening sequence of against?
bool stringOpeningSubstringOf(const std::string& check, const std::string& against);
//is check the opening sequence of against, not caring about case?
bool stringCaseOpeningSubstringOf(const std::string& check, const std::string& against);
//1=equal,-1=higher,-2=lower
int stringCaseOpeningSubstringCmp(const std::string& check, const std::string& against);

//time(0)
long currentSecond();
//GetTickCount()
long long currentMilli();

//checks string to verify if it is a US or Canadian Zip or Postal code
bool iszip(const std::string& check);

//Returns a string for a windows error
std::string getWindowsErrorString(long err);

//find the next character not in quotes
long findNextNonQuotedPos(const std::string& data, const std::string& tofind, long start = 0);

//pulls in whole tag <a ....>...</a> or <a .../> or the next data that leads to a tag IE "blah blah<" not including the <
std::string getNextXmlPoint(const std::string& data, long start = 0);

//same as above, just not including opening or closing tag
std::string getContentOfNextXmlPoint(const std::string& data, long start = 0);

//pulls tag name, if any
std::string getNextXmlPointName(const std::string& data, long start = 0);

//pulls any properties of next tag href="blahblah" style="q<bl>"
std::string getNextXmlPointAttributes(const std::string& data, long start = 0);

//internal use mostly, converts character values into string representations of those values in hex format
std::string hexToString(const std::string& hx);
std::string stringToHex(const std::string& str);

//calls getNextXmlPoint() until it returns ""
std::vector<std::string> separateXmlPoints(const std::string& data);

//grabs all attributes aaaa="blah"
std::map<std::string,std::string> splitAttributes(const std::string& data);

//does container start with pattern? same as stringOpeningSubstringOf()
bool startsWith(const std::string container, const std::string pattern);
//does container end with pattern?
bool endsWith(const std::string container, const std::string pattern);


enum stringNumericType{
    invalidNumberString=0,
    unsignedIntegerString,//vanilla
    signedIntegerString,
    doubleString,
    exponentString,//scientific notation
    hexString,
    octalString
};

std::string getNameOfStringType(stringNumericType in);

//parses a string for the resulting integer type, handles scientific,hex,octal,signed,unsigned,and double values
stringNumericType getStringIntType(const std::string& str);
//quick pow function val to the pw
long qpow(long val, long pw);
double qpow(double val, long pw);
//call atod instead
double simpleAtod(const std::string& str, int base = 10);
//internal
std::string cleanhex(const std::string& str);
//internal
unsigned char hexToVal(char ch);
//internal, call atol instead
int simpleAtoi(const std::string& str);
//internal
double complexAtod(const std::string& str);
//internal
int complexAtoi(const std::string& str);
//internal
long hexToLong(const std::string& str);
//internal
long hexToInt(const std::string& str);
//internal
int octToInt(const std::string& str);
//converts a string that contains a number in many formats to a double
double atod(const std::string& str);
//converts a string that contains a number in many formats to a long
long atol(const std::string& str);

//convert a number to a certain base, NEEDS WORK
//example outbase(4,2) outputs "100"
std::string outbase(long num, int rad=10);

//output a number with the digit length a minimum of digits long
std::string fillToDigits(long value, int digits);

//expects xx:yy AM|xx:yypm|xx:yy|xx:AM,
//returns the int  value of the hour
int getMilitaryHour(std::string formattedTime);

//return a negative value always
template <typename T>
T& neg (T& in){
    if (in>0)
        in*=-1;
    return in;
}

//return a positive value always
template <typename T>
T& pos (T& in){
    if (in<0)
        in*=-1;
    return in;
}

};



#endif
