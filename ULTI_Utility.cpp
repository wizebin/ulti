#include "ULTI_Utility.h"

namespace ulti{

token::token():token(""){}
token::token(const std::string& str, long strpos){
    value=str;
    stringpos=strpos;
}
token::token(const token& tok){
    *this=tok;
}
token& token::operator=(const token& tok){
    if (this!=&tok){
        value=tok.value;
        stringpos=tok.stringpos;
    }
    return *this;
}


//Quick permutation function with callbacks
void permuteRef(std::string& str,int first,std::function<void(const std::string&)> callback){
    //if we have exhausted all possible swaps, this is a permutation
    if (first==str.size()){
        if (callback!=NULL){
            callback(str);
        }
    }
    else{
        //we have more characters to swap on this path
        for(int current = first; current < str.size(); current++){
            std::swap(str[first],str[current]);
            //branch our permutation for this swap
            permuteRef(str,first+1,callback);
            std::swap(str[first],str[current]);
            //restore original value
        }
    }
}
//helper function, makes it simple to permute static strings or standard strings
void permute(const std::string& str, std::function<void(const std::string&)> callback){
    std::string subject = str;
    permuteRef(subject,0,callback);
}
std::vector<std::string> split(const std::string& instring, const std::string& separator){
    std::vector<std::string> ret;
    long pos = instring.find(separator),prevstart = 0;
    while(pos!=std::string::npos){
        ret.push_back(instring.substr(prevstart,pos-prevstart));
        prevstart=pos+separator.size();
        pos = instring.find(separator, pos+separator.size());
    }
    if (prevstart <= instring.size()){
        ret.push_back(instring.substr(prevstart,instring.size()-prevstart));
    }
    return ret;
}
std::vector<std::string> splitBackEscaped(const std::string& instring, const std::string& separator){
    std::vector<std::string> ret;
    long pos = instring.find(separator),prevstart = 0;
    while(pos!=std::string::npos){
        if (pos>1&&pos-1=='\\'){
            //do nothing, it's escaped
        }
        else{
            ret.push_back(instring.substr(prevstart,pos-prevstart));
            prevstart=pos+separator.size();
        }
        pos = instring.find(separator, pos+separator.size());
    }
    if (prevstart <= instring.size()){
        ret.push_back(instring.substr(prevstart,instring.size()-prevstart));
    }
    return ret;
}

token chip(const std::string& instring, const std::string& separator, int which, long after){
    token ret;ret.stringpos=-1;
    long pos = instring.find(separator,after),prevstart = after,cur=after;
    while(pos!=std::string::npos){
        if (which==cur++){ret.value = instring.substr(prevstart,pos-prevstart);ret.stringpos=prevstart;return ret;}
        prevstart=pos+separator.size();
        pos = instring.find(separator, pos+separator.size());
    }
    if (prevstart <= instring.size()){
        if (which==cur){ ret.value=instring.substr(prevstart,instring.size()-prevstart);ret.stringpos=prevstart;return ret;}
    }
    return ret;
}

//Inclusive with both head and tail
std::vector<std::string> fragment(const std::string& data, const std::string& head, const std::string& tail){
    std::vector<std::string> ret;
    long hpos = data.find(head);
    long tpos = data.find(tail,hpos+head.size());
    while(hpos!=std::string::npos&&tpos!=std::string::npos){
        ret.push_back(data.substr(hpos,tpos+tail.size()-hpos));
        hpos = data.find(head,tpos+tail.size());
        tpos = data.find(tail,hpos+head.size());
    }
    return ret;
}
std::vector<std::string> fragmentWithNest(const std::string& data, const std::string& head, const std::string& tail){
    std::vector<std::string> ret;
    //head = + 1, tail = -1; Find until 0;
    long cursor = 0;
    while(cursor < data.size()){
        int depth = 0;
        long hpos = data.find(head,cursor);
        cursor = hpos+head.size();
        if (hpos != std::string::npos)
            depth++;
        else
            break;
        while(depth>0){
            long hcur = data.find(head,cursor);
            long tcur = data.find(tail,cursor);
            if (tcur==std::string::npos){
                cursor=-1;
            }
            if (hcur < tcur && hcur != std::string::npos){
                cursor = hcur+head.size();
                depth++;
            }
            else{
                cursor = tcur+tail.size();
                depth--;
            }
        }
        if (cursor>=0 && cursor > hpos){
            ret.push_back(data.substr(hpos,cursor-hpos));
        }
        else{
            break;
        }
    }
    return ret;
}
std::string nextword(const std::string& str, int startpos){
    long x=startpos;
    for(int a = startpos; a < str.size(); a++){
        if (!isspace(str[a])){
            x=a;
            break;
        }
    }
    for(int a = x+1; a < str.size(); a++){
        if (isspace(str[a])){
            return str.substr(x,a-x);
        }
    }
    return str.substr(x,str.size()-x);
}

std::string findNextFragment(const std::string& data, const std::string& head, const std::string& tail, long startpos){
    std::vector<std::string> ret;
    long hpos = data.find(head,startpos);
    long tpos = data.find(tail,hpos+head.size());
    if(hpos!=std::string::npos&&tpos!=std::string::npos){
        return data.substr(hpos,tpos+tail.size()-hpos);
    }
    return "";
}
std::string findContentOfNextFragment(const std::string& data, const std::string& head, const std::string& tail, long startpos){
    std::string ret = findNextFragment(data,head,tail,startpos);
    if (ret.size()>=head.size()+tail.size()){
        return ret.substr(head.size(),ret.size()-(head.size()+tail.size()));
    }
    return ret;
}
std::string findContentOfFragmentFollowing(const std::string& data, const std::string& contextword, const std::string& head, const std::string& tail, long startpos){
    long cpos = data.find(contextword,startpos);
    if (cpos!=std::string::npos){
        cpos+=contextword.size();
        std::string ret = findNextFragment(data,head,tail,cpos);
        if (ret.size()>=head.size()+tail.size()){
            ret=ret.substr(head.size(),ret.size()-(head.size()+tail.size()));
        }
        return ret;
    }
    return "";
}

std::string unsplit(const std::vector<std::string>& vec, const std::string& separator){
    std::stringstream ss;
    if (vec.size()>0){
        ss << vec[0];
        for(int a = 1; a < vec.size(); a++){
            ss << separator;
            ss << vec[a];
        }
    }
    else{
        return "";
    }
    return ss.str();
}



bool isdelim(char spc){
    return (spc==';' || spc=='\r' || spc=='\n');
}
bool isna(char spc){
    return (spc==' '||spc=='\t'||spc<32||spc>128);
}
std::string cf(const std::string& str){
    std::string tmp = str;
    cfref(tmp);
    return tmp;
}
void cfref(std::string& str){
    for(int start = 0; start < str.size(); start++){
        if (!isspace(str[start])){
            for(int finish = str.size()-1; finish >= start; finish--){
                if (!isspace(str[finish])){
                    str = str.substr(start,finish-start+1);
                    return;
                }
            }
            str="";
            return;
        }
    }
    str="";
}

bool isspecial(char spc){
    return (!isalnum(spc)) && (!isdelim(spc)) && (!isna(spc));
}
//Defines things that should seperate tokens
bool tokendel(char ch){
    return (ch==' '||ch=='\r'||ch=='\n'||ch=='\t');
}

//Defines things that should be handled as seperate tokens. This will
//Allow us to ignore the possibility of white space
bool tokencmd(char ch){
    return (ch=='(' || ch==')' || ch=='*' || ch=='\'' || ch=='\"' || ch=='=' || ch =='+' || ch=='-' || ch=='/' ||ch==',');
}

bool tokencmd2(char ch){
    return ((!(isalnum(ch)||ch=='_')) && (!isspace(ch)));
}

///A token is a programmatically important symbol. Such as a typename, variable name, static value, math symbol, etc
std::vector<token> tokenize(const std::string& str){
    std::string cur; std::vector<token> ret;
    for(int a = 0; a < str.size(); a++){
        if(tokendel(str[a])){
            if(cur.size()>0){
                ret.push_back(token(cur,a-cur.size()));
                cur="";
            }
        }
        else if (tokencmd2(str[a])){
            if(cur.size()>0){
                ret.push_back(token(cur,a-cur.size()));
                cur="";
            }
            ret.push_back(token(std::string()+str[a],a));
        }
        else{
            cur+=str[a];
        }
    }
    if (cur.size()>0)
        ret.push_back(token(cur,str.size()-cur.size()));
    return ret;
}
std::vector<token> fullTokenize(const std::string& str){
    std::string cur; std::vector<token> ret;
    for(int a = 0; a < str.size(); a++){
        if(tokendel(str[a])||tokencmd2(str[a])){
            if(cur.size()>0){
                ret.push_back(token(cur,a-cur.size()));
                cur="";
            }
            ret.push_back(token(std::string()+str[a],a));
        }
        else{
            cur+=str[a];
        }
    }
    if (cur.size()>0)
        ret.push_back(token(cur,str.size()-cur.size()));
    return ret;
}

std::vector<std::string> strTokenize(const std::string& str){

    std::string cur; std::vector<std::string> ret;
    for(int a = 0; a < str.size(); a++){
        if(tokendel(str[a])){
            if(cur.size()>0){
                ret.push_back(cur);
                cur="";
            }
        }
        else if (tokencmd2(str[a])){
            if(cur.size()>0){
                ret.push_back(cur);
                cur="";
            }
            ret.push_back(std::string()+str[a]);
        }
        else{
            cur+=str[a];
        }
    }
    if (cur.size()>0)
        ret.push_back(cur);
    return ret;

}
std::string stripQuotes(const std::string& str){
    if (str.size()>1&&str[0]==str[str.size()-1]&&(str[0]=='"'||str[0]=='\'')){
        return str.substr(1,str.size()-2);
    }
    return str;
}

std::string replaceString(const std::string& str, const std::string& tofind, const std::string& toreplace){
    std::string ret = str;
    long lastfound = ret.find(tofind);
    while(lastfound!=std::string::npos){
        ret.replace(lastfound,tofind.size(),toreplace);
        lastfound = ret.find(tofind, lastfound+toreplace.size());//
    }
    return ret;
}

std::string stripString(const std::string& str, const std::string& tostrip){
    return replaceString(str,tostrip,"");
}

std::string joinStrings(const std::string& strA, const std::string& strB, const std::string& glue){
    std::string ret;
    ret.append(strA);
    if (strA.size()>0&&strB.size()>0)
        ret.append(glue);
    ret.append(strB);
    return ret;
}

void strreftolower(std::string& in){
    for(unsigned int a= 0; a < in.size(); a++){
        in[a]=tolower(in[a]);
    }
}
std::string strtolower(const std::string& in){
    std::string ret = in;
    strreftolower(ret);
    return ret;
}

void strreftoupper(std::string& in){
    for(unsigned int a= 0; a < in.size(); a++){
        in[a]=toupper(in[a]);
    }
}
std::string strtoupper(const std::string& in){
    std::string ret = in;
    strreftoupper(ret);
    return ret;
}

std::string wstrtostr(const std::wstring& str){
    return std::string(str.begin(),str.end());
}
std::wstring strtowstr(const std::string& str){
    return std::wstring(str.begin(),str.end());
}
void quickUnescapeJsonRef(std::string& str){

    for(int a = 0; a < str.size()-1; a++){
        if (str[a]=='\\'){
            if (str[a+1]=='n'){
                str.erase(str.begin()+a);
                str[a]='\n';
            }
            else if (str[a+1]=='r'){
                str.erase(str.begin()+a);
                str[a]='\r';
            }
            else if (str[a+1]=='t'){
                str.erase(str.begin()+a);
                str[a]='\t';
            }
            else if (str[a+1]=='f'){
                str.erase(str.begin()+a);
                str[a]='\f';
            }
            else{
                str.erase(str.begin()+a);
            }
            continue;

        }
    }
}
void quickEscapeJsonRef(std::string& str){
    for(int a = 0; a < str.size(); a++){
        if (str[a]=='"'||str[a]=='\\'||str[a]=='/'||str[a]=='\b'||str[a]=='\f'||str[a]=='\n'||str[a]=='\r'||str[a]=='\t'){
            str.insert(a,"\\");
            a+=1;
            continue;
        }
    }
}
std::string quickEscapeJson(const std::string& str){
    std::string ret = str;
    quickEscapeJsonRef(ret);
    return ret;
}
std::string quickUnescapeJson(const std::string& str){
    std::string ret = str;
    quickUnescapeJsonRef(ret);
    return ret;
}
std::string quickJson(const std::map<std::string,std::string>& mp){
    std::stringstream ret;
    ret << "{";
    auto it = mp.begin();
    if (it != mp.end())
        ret << "\"" << quickEscapeJson(it->first) << "\":\"" << quickEscapeJson(it->second) << "\"";
    it++;
    for(; it != mp.end(); ++it){
        ret << "," << "\"" << quickEscapeJson(it->first) << "\":\"" << quickEscapeJson(it->second) << "\"";
    }
    ret << "}";
    return ret.str();
}
std::string quickEscapeXml(const std::string& str){
    std::string ret = ulti::replaceString(str,"&","&amp;");
    ret = ulti::replaceString(ret,"<","&lt;");
    ret = ulti::replaceString(ret,">","&gt;");
    ret = ulti::replaceString(ret,"\'","&apos;");
    ret = ulti::replaceString(ret,"\"","&quot;");
    return ret;
}
std::string quickUnescapeXml(const std::string& str){
    std::string ret = ulti::replaceString(str,"&amp;","&");
    ret = ulti::replaceString(ret,"&lt;","<");
    ret = ulti::replaceString(ret,"&gt;",">");
    ret = ulti::replaceString(ret,"&apos;","\'");
    ret = ulti::replaceString(ret,"&quot;","\"");
    return ret;
}

std::string quickEscape(const std::string& str){
    std::string ret;
    ret.reserve(str.size());
    for(unsigned int a = 0; a < str.size(); a++){
        if (str[a]=='\\'||str[a]=='"'||str[a]==':'||str[a]==';'){
            ret += '\\';
        }
        ret += str[a];
    }
    return ret;
}
std::string quickUnescape(const std::string& str){
    std::string ret;
    ret.reserve(str.size());
    for(unsigned int a = 0; a<str.size(); a++){
        if (str[a]=='\\'){
            a++;
            if (a < str.size()){
                ret += str[a];
            }
            else{

            }
        }
        else{
            ret += str[a];
        }
    }
    return ret;
}
std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}
std::string repeat(const std::string& pattern, int quantity){
    std::string ret = "";
    if (quantity>0&&pattern.size()>0){
        ret.reserve(pattern.size()*quantity);
        for(int a = 0; a < quantity; a++){
            ret.append(pattern);
        }
    }
    return ret;
}
void fill(char* data, long datasize, char fillchar){
    memset((void*)data,fillchar,datasize);
}
void fill(char* data, long datasize, const std::string& fillstr){
    long cur = 0;
    while(cur+fillstr.size()<datasize){
        memcpy(&data[cur],&fillstr[0],fillstr.size());
        cur+=fillstr.size();
    }
    if (cur<datasize){
        memcpy(&data[cur],&fillstr[0],datasize-cur);
        cur+=(datasize-cur);
    }
}
void stamp(char* data, const std::string& indata){
    memcpy(&data,&indata[0],indata.size());
}
std::string fixWidth(const std::string& istr, int width, bool pre, const std::string& filler){
    if (pre){
        if (istr.size()<width){
            return ulti::repeat(filler,width-istr.size())+istr;
        }
    }
    else{
        if (istr.size()<width){
            return istr+ulti::repeat(filler,width-istr.size());
        }
    }
    return istr;
}
int getMonth(time_t timeval){
    struct tm * timeinfo;
    timeinfo = localtime(&timeval);
    if (timeinfo!=NULL)
        return timeinfo->tm_mon + 1;
    return -1;
}
int getDay(time_t timeval){
    struct tm * timeinfo;
    timeinfo = localtime(&timeval);
    if (timeinfo!=NULL)
        return timeinfo->tm_mday;
    return -1;
}
int getYear(time_t timeval){
    struct tm * timeinfo;
    timeinfo = localtime(&timeval);
    if (timeinfo!=NULL)
        return timeinfo->tm_year+1900;
    return -1;
}
int getHour(time_t timeval){
    struct tm * timeinfo;
    timeinfo = localtime(&timeval);
    if (timeinfo!=NULL)
        return timeinfo->tm_hour;
    return -1;
}
int getMin(time_t timeval){
    struct tm * timeinfo;
    timeinfo = localtime(&timeval);
    if (timeinfo!=NULL)
        return timeinfo->tm_min;
    return -1;
}
int getSec(time_t timeval){
    struct tm * timeinfo;
    timeinfo = localtime(&timeval);
    if (timeinfo!=NULL)
        return timeinfo->tm_sec;
    return -1;
}
std::string getWeekday(time_t timeval){
    struct tm * timeinfo;
    static std::string days[7] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

    timeinfo = localtime(&timeval);

    if (timeinfo!=NULL)
        return days[timeinfo->tm_wday];
    return "err";
}
std::string getMonthStringFromIndex(int index){
    static std::string months[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    return months[index];
}
std::string getFullMonthStringFromIndex(int index){
    static std::string months[] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    if (index>=0&&index<=11)
        return months[index];
    return "";
}
std::string getMonthString(time_t timeval){
    long mon = getMonth(timeval);
    if (mon>0)
        return getMonthStringFromIndex(mon-1);
    return "err";
}


std::string quickFormattedDate(time_t timeval){
    return (stringBuilder() << getMonthString(timeval) << " " << ulti::fixWidth(ulti::hackString(getDay(timeval)),2) << ", " << ulti::fixWidth(ulti::hackString(getYear(timeval)),4) << " " << ulti::fixWidth(ulti::hackString(getHour(timeval)),2) << ":" << ulti::fixWidth(ulti::hackString(getMin(timeval)),2)).str();
}
std::string formatDatetime(time_t timeval){
    return (stringBuilder() << ulti::fixWidth(ulti::hackString(getYear(timeval)),4) << "-" << ulti::fixWidth(ulti::hackString(getMonth(timeval)),2) << "-" << ulti::fixWidth(ulti::hackString(getDay(timeval)),2)  << " " << ulti::fixWidth(ulti::hackString(getHour(timeval)),2) << ":" << ulti::fixWidth(ulti::hackString(getMin(timeval)),2)).str();
}

bool isStringInteger(const std::string& str){
    if (str.size()==0)
        return false;
    if (str[0]=='-'||isdigit(str[0])){
        for(int a = 1; a < str.size(); a++){
            if (!isdigit(str[a])){
                return false;
            }
        }
    }
    return true;
}
bool isStringNumeric(const std::string& str){
    if (str.size()==0)
        return false;
    if (str[0]=='-'||isdigit(str[0])){
        int pointcount = 0;
        for(int a = 1; a < str.size(); a++){
            if (!isdigit(str[a])){
                if (str[a]=='.'&&pointcount==0){
                    pointcount++;
                }
                else{
                    return false;
                }
            }
        }
    }
    else{
        return false;
    }
    return true;
}
bool isStringUnsignedInteger(const std::string& str){
    if (str.size()==0)
        return false;

    for(int a = 0; a < str.size(); a++){
        if (!isdigit(str[a])){
            return false;
        }
    }
    return true;
}

bool stringOpeningSubstringOf(const std::string& check, const std::string& against){
    if (check.size()>against.size())
        return false;
    for(int a = 0;a < check.size(); a++){
        if (check[a]!=against[a])
            return false;
    }
    return true;
}
bool stringCaseOpeningSubstringOf(const std::string& check, const std::string& against){
    if (check.size()>against.size())
        return false;
    for(int a = 0;a < check.size(); a++){
        if (toupper(check[a])!=toupper(against[a]))
            return false;
    }
    return true;
}
int stringCaseOpeningSubstringCmp(const std::string& check, const std::string& against){
    if (check.size()>against.size())
        return 0;
    for(int a = 0;a < check.size(); a++){
        if (toupper(check[a])!=toupper(against[a])){
            return (toupper(check[a])>toupper(against[a]))-2;//trick to return -1 or -2 correctly
        }
    }
    return 1;
}

bool iszip(const std::string& check){
    if (check.size() == 5 && isStringUnsignedInteger(check)){
        return true;
    }
    if (check.size()>6 && check[5]=='-' && isStringUnsignedInteger(check.substr(0,5)) && isStringUnsignedInteger(check.substr(6,check.size()-6))){
        return true;
    }
    if (check.size() > 2 && check.size() <= 6){
        //Check postal code. Digit Alpha Digit Alpha etc
        for(int a = 0;a < check.size(); a++){
            if (a%2==0){
                if (!isdigit(check[a])){
                    return false;
                }
            }
            else{
                if (!isalpha(check[a])){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

long currentSecond(){
    return time(0);
}
long long currentMilli(){
    #if defined(_WIN32)||defined(_WIN_32)
        return GetTickCount();
    #else
        return -1;
    #endif // _WIN_32
}

std::string getWindowsErrorString(long err){
    //Get the error message, if any.
    DWORD errorMessageID = err;
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    ulti::cfref(message);

    return message;
}

long findNextNonQuotedPos(const std::string& data, const std::string& tofind, long start){
    bool inquote = false;bool inapos = false;
    int foundpos=0;
    for(int a = start; a < data.size(); a++){
        if (data[a]=='\"'){
            inquote=!inquote;
            foundpos=0;
        }
        else if (data[a]=='\''){
            foundpos=0;
            inapos=!inapos;
        }
        else if (data[a]==tofind[foundpos] && !inquote){
            foundpos++;
            if (foundpos==tofind.size()){
                return a-tofind.size()+1;
            }
        }
        else{
            foundpos=0;
        }
    }

    return -1;
}

std::string getNextXmlPoint(const std::string& data, long start){
    if (start>data.size())
        return "";
    long tp = findNextNonQuotedPos(data,"<",start);
    long tc = findNextNonQuotedPos(data,"<!",start);
    bool special = false;
    if (tc==tp&&tc>=0){
        special=true;
    }
    if (tp>start||(tp==-1 && data.size()-start>0)){
        std::string tret = data.substr(start,tp-start);
        if (cf(tret)!=""){
            return tret;
        }
        return "";
    }

    long tp2 = findNextNonQuotedPos(data," ",tp+1);
    long tp0 = -1;
    long tp3 = findNextNonQuotedPos(data,">",tp+1);
    for(int a = tp3-1; a >= 0; a--){
        if (!isspace(data[a])){
            char tbuf = data[a];
            if (data[a]=='/'){
                tp0 = a;
            }
            break;
        }
    }

    if ((tp0 < tp3 && tp0>=0)||special){
        if (tp<0){
            return "";
        }
        return data.substr(tp,tp3-tp+1);
    }

    if (tp2>tp3||tp2==-1)
        tp2=tp3;


    std::string tagname = cf(data.substr(tp+1,tp2-tp-1));

    int depth = 1;

    long tp4 = findNextNonQuotedPos(data,tagname,tp3+1);
    bool isclose = 0;
    long tp5 = tp3;
    bool searching = true;
    while(searching){
        int sdepth = depth;
        for(int a = tp4; a>=tp3; a--){
            if (data[a]=='>'){
                depth=sdepth;
                break;
            }
            if (data[a]=='/'){
                depth--;
                isclose=true;
            }
            else if (data[a]=='<'){
                if (!isclose){
                    depth++;
                    break;
                }
                tp5=a;
                break;
            }
        }
        if (depth==0){
            searching=false;
            long tp6 = findNextNonQuotedPos(data,">",tp5+1);
            return data.substr(start,(tp6-start)+1);
        }
        tp4 = findNextNonQuotedPos(data,tagname,tp4+1);
        if (tp4==-1)
            return "";
    }
    return "";
}

std::vector<std::string> separateXmlPoints(const std::string& data){
    std::vector<std::string> ret;
    int tr = 0;
    while(1){
        std::string tmp = getNextXmlPoint(data,tr);
        if (tmp.size()>0){
            ret.push_back(tmp);
        }
        else
            break;
        tr+=tmp.size();
    }
    return ret;

}
std::string getContentOfNextXmlPoint(const std::string& data, long start){
    long tp = findNextNonQuotedPos(data,"<",start);
    if (tp>start){
        std::string tret = data.substr(start,tp-start);
        if (cf(tret)!=""){
            return "";
        }
    }

    long tp2 = findNextNonQuotedPos(data," ",tp+1);
    long tp0 = -1;
    long tp3 = findNextNonQuotedPos(data,">",tp+1);

    for(int a = tp3; a >= 0; a--){
        if (!isspace(data[a])){
            if (data[a]=='/'){
                tp0 = a;
            }
            break;
        }
    }

    if (tp0<tp3&&tp0>=0)
        return "";

    if (tp2>tp3||tp2==-1)
        tp2=tp3;


    std::string tagname = cf(data.substr(tp+1,tp2-tp-1));

    int depth = 1;

    long tp4 = findNextNonQuotedPos(data,tagname,tp3+1);
    bool isclose = 0;
    long tp5 = tp3;
    bool searching = true;
    while(searching){
        int sdepth = depth;
        for(int a = tp4; a>=tp3; a--){
            if (data[a]=='>'){
                depth=sdepth;
                break;
            }
            else if (data[a]=='/'){
                depth--;
                isclose=true;
            }
            else if (data[a]=='<'){
                if (!isclose){
                    depth++;
                    break;
                }
                tp5=a;
                break;
            }
        }
        if (depth==0){
            searching=false;
            return data.substr(tp3+1,(tp5)-(tp3)-1);
        }
        tp4 = findNextNonQuotedPos(data,tagname,tp4+1);
        if (tp4==-1)
            return "";
    }
    return "";
}
std::string getNextXmlPointName(const std::string& data, long start){
    long tp = findNextNonQuotedPos(data,"<",start);
    if (tp>start){
        std::string tret = data.substr(start,tp-start);
        if (cf(tret)!=""){
            return "";
        }
    }
    long tp2 = findNextNonQuotedPos(data," ",tp+1);
    long tp3 = findNextNonQuotedPos(data,">",tp+1);
    long tp7 = findNextNonQuotedPos(data,"/",tp+1);
    if (tp3==-1)
        return "";

    if (tp2==-1||tp3<tp2){
        tp2 = tp3;
        if (tp7 < tp2 && tp7>=0){
            tp2 = tp7;
        }
    }


    std::string tret = data.substr(tp+1,tp2-tp-1);
    return tret;
}
std::string getNextXmlPointAttributes(const std::string& data, long start){
    long tp = findNextNonQuotedPos(data,"<",start);
    if (tp>start){
        std::string tret = data.substr(start,tp-start);
        if (cf(tret)!=""){
            return "";
        }
    }
    long tp2 = findNextNonQuotedPos(data," ",tp+1);
    if (tp2==-1){
        return "";
    }
    long tp3 = findNextNonQuotedPos(data,">",tp+1);
    if (tp3-tp2-1<0)
        return "";
    std::string tret = data.substr(tp2+1,tp3-tp2-1);
    if (tret.size()>0&&tret.back()=='/')
        tret.pop_back();
    return tret;
}
std::map<std::string,std::string> splitAttributes(const std::string& data){
    std::map<std::string,std::string> ret;
    bool inquote = 0;
    bool inapos = 0;
    std::string curtag;
    long spos = 0;
    for(int a = 0; a < data.size(); a++){
        if (data[a]=='='&&!inquote&&!inapos){
            curtag=data.substr(spos,a-spos);
            spos=a+1;
        }
        else if ((data[a]==' '||a==data.size()-1)&&!inquote&&!inapos){
            ret[curtag]=data.substr(spos+1,a-spos-2);
            spos=a+1;
        }
        else if (data[a]=='\"' && !inapos){
            inquote=!inquote;
        }
        else if (data[a]=='\'' && !inquote){
            inapos=!inapos;
        }
    }
    if (spos < data.size() && curtag.size()>0 && !inquote && !inapos){
        ret[curtag]=data.substr(spos+1,data.size()-spos-2);
    }
    return ret;
}

char hexToChar(const std::string& str){
    if (str.size()>=2){
        unsigned char low = toupper(str[0]) - '0';
        unsigned char high = toupper(str[1]) - '0';
        if (low > 9){
            low -= ('A'-'9')-1;
        }
        if (high > 9){
            high -= ('A'-'9')-1;
        }
        return (char)(low << 4) + high;
    }
    else if (str.size()==1){
        unsigned char tmp = toupper(str[0])-'0';
        if (tmp>9){
            tmp -= ('A'-'9')-1;
        }
        return (char)tmp;
    }
}
std::string charToHex(char a){
    std::string ret;
    unsigned char low = a & 0x0f;
    unsigned char hi = (a & 0xf0) >> 4;
    if (hi>=0x0a)ret.push_back('A'+(hi-10));
    else ret.push_back((char)'0'+hi);
    if (low>=0x0a)ret.push_back('A'+(low-10));
    else ret.push_back((char)'0'+low);
    return ret;
}

std::string hexToString(const std::string& hx){
    std::stringstream ret;
    for(int a = 0; a < hx.size(); a+=2){
        ret << hexToChar(hx.substr(a,2));
    }
    return ret.str();
}
std::string stringToHex(const std::string& str){
    std::stringstream ret;
    for(int a = 0; a < str.size(); a++){
        ret << charToHex(str[a]);
    }
    return ret.str();
}

bool startsWith(const std::string container, const std::string pattern){
    if (container.size()>pattern.size()){
        for(int a = 0; a < pattern.size(); a++){
            if (pattern[a]!=container[a])
                return false;
        }
        return true;
    }
    return false;
}
bool endsWith(const std::string container, const std::string pattern){
    if (container.size()>pattern.size()){
        for(int a = container.size()-1,b=pattern.size()-1; b>=0; b--,a--){
            if (pattern[b]!=container[a])
                return false;
        }
        return true;
    }
    return false;
}

std::string getNameOfStringType(stringNumericType in){
    if (in==invalidNumberString)
        return "INVALID";
    if (in==unsignedIntegerString)
        return "unsigned int";
    if (in==signedIntegerString)
        return "int";
    if (in==doubleString)
        return "double";
    if (in==exponentString)
        return "exponent";
    if (in==hexString)
        return "hex";
    if (in==octalString)
        return "oct";

    return "UNKNOWN";
}
stringNumericType getStringIntType(const std::string& str){
    if(str.size()==0)return invalidNumberString;
    int negatives=0,positives=0,exponents=0,periods=0,alpha=0,digits=0,hexcount=0,octcount=0,bincount=0,highalpha=0,spaces=0,questionable=0;
    int qtys[10] = {0,0,0,0,0,0,0,0,0,0};
    for(int a = 0; a < str.size(); a++){
        if (!isdigit(str[a])){
            if (isalpha(str[a])){
                char buf = toupper(str[a]);
                if (buf=='E'){
                    exponents++;
                }
                else if (buf=='X'){
                    hexcount++;
                    //probably hex
                }
                else if (buf=='O'){
                    octcount++;
                    //probably octal
                }
                else if (buf=='B'){
                    bincount++;
                }
                if (buf>'F'){
                    highalpha++;
                    //almost certainly invalid if not hex or octal
                    //handling base64 (really anything from 16-64) would change this significantly
                }
                alpha++;
            }
            else if (str[a]=='-'){
                negatives++;
                if (a==str.size()-1||(a>0 && a!=spaces && (!isalpha(toupper(str[a-1]))) && (!isalpha(toupper(str[a+1])))))
                    return invalidNumberString;
            }else if (str[a]=='+'){
                positives++;
                if (a==str.size()-1||(a>0 && a!=spaces && (!isalpha(toupper(str[a-1]))) && (!isalpha(toupper(str[a+1])))))
                    return invalidNumberString;
            }else if (str[a]=='.'){
                periods++;
            }else if (str[a]==' '){
                spaces++; //could be invalidated by spaces, but we'll swallow them
            }else{
                //everything else is questionable
                questionable++;
                return invalidNumberString;
            }
        }
        else{
            digits++;
            int setting = str[a]-'0';
            if (setting>9)
                int butt = 6;
            qtys[setting]++;
        }
    }
    if (highalpha>1||(exponents==0&&(negatives>1||positives>1||periods>1))){
        return invalidNumberString;
    }
    if (alpha>0){
        if (highalpha==1){
            if (negatives+positives<2&&periods<2){

                if (hexcount==1&&(alpha>1||digits>0))
                    return hexString;

                else if (octcount==1&&digits>0){
                    for(int a = 8; a < 10; a++)
                        if (qtys[a]>0)
                            return invalidNumberString;
                    return octalString;
                }
            }
        }
        else{
            if (exponents>0){
                if (alpha==1){ //HEX + SCIENTIFIC = INVALID
                    if (negatives+positives<3&&periods<3&&digits>0){
                        return exponentString;
                    }
                }
            }
            if (octcount>0){
                //obviously not compatible with multiple alphas
                return invalidNumberString;
            }
            if (hexcount>0){
                //multiple high alphas are invalid (anything above 'f')
                return invalidNumberString;
            }
        }
    }

    if (negatives+positives<2&&periods<2){
        if (alpha>0&&highalpha==0)
            return hexString;
        if (periods==1&&digits>0)
            return doubleString;
        if (negatives==1||positives==1&&digits>0)
            return signedIntegerString;
        if (negatives==0&&positives==0&&periods==0&&alpha==0&&digits>0)
            return unsignedIntegerString;
    }
    return invalidNumberString;
}

long qpow(long val, long pw){
    if (pw==0)
        return 1;
    long ret = val;
    for(int a = 1; a < pw; a++){
        ret *= val;
    }
    return ret;
}
double qpow(double val, long pw){
    if (pw==0)
        return 1;
    if (pw<0)
        return 1/qpow(val,pw*-1);
    double ret = val;
    for(int a = 1; a < pw; a++){
        ret *= val;
    }
    return ret;
}

double simpleAtod(const std::string& str, int base){
    double dbase = base, ret = 0;
    int digit = 0;
    for(int a = str.size()-1; a>=0; a--){
        if (isdigit(str[a])){
            ret += ((double)(str[a]-'0'))*qpow(dbase,digit);
            digit++;
        }
        else{
            if (str[a]=='.'){
                ret/=qpow(dbase,digit);
                digit=0;
            }
            else if (str[a]=='-'){
                ret*=-1;
            }
        }
    }
    return ret;
}

std::string cleanhex(const std::string& str){
    int start=-1,end=0;bool neg = false;
    for(int a = 0; a < str.size(); a++){
        if (isspace(str[a])||str[a]=='0'||str[a]=='+'){

        }
        else if (str[a]=='x'||str[a]=='X'){
            start=a+1;
            break;
        }
        else if (str[a]=='-'){
            neg=true;
        }
        else{
            start=a;
            break;
        }
    }
    if (start==-1)
        return "";
    for(int a = str.size()-1; a >= start; a--){
        if (isspace(str[a])){

        }
        else{
            end=a+1;
            break;
        }
    }
    if (end==0)
        return "";
    if (neg){
        std::string ret;
        ret.push_back('-');
        ret += str.substr(start,end-start);
        return ret;
    }

    return str.substr(start,end-start);
}
unsigned char hexToVal(char ch){
    unsigned char tmp = toupper(ch)-'0';
    if (tmp>9){
        tmp -= ('A'-'9')-1;
    }
    return tmp;
}

int simpleAtoi(const std::string& str){
    return (int)simpleAtod(str);
}
long atol(const std::string& str){
    return (long)atod(str);
}
double complexAtod(const std::string& str){
    bool neg = false;
    int splitter = -1;
    for(int a = 0; a < str.size(); a++){
        if (str[a]==' '||str[a]=='0'){}//ignore
        else if (str[a]=='-'){neg=true;}
        else if (str[a]=='e'||str[a]=='E'){
            //split here
            splitter= a;
        }
    }
    std::string sbase = str.substr(0,splitter);
    std::string sexp = str.substr(splitter+1);
    double base = simpleAtod(sbase);
    long exp = simpleAtoi(sexp);

    return base * qpow(10.0,exp);
}
int complexAtoi(const std::string& str){
    return (int)complexAtod(str);
}
long hexToLong(const std::string& str){
    std::string cleaned = cleanhex(str);bool neg=false;
    long buf=0;
    for(int a = cleaned.size()-1,cur=0; a >=0; a--){
        if (isalnum(cleaned[a])){
            buf += hexToVal(cleaned[a]) << (4*cur++);
        }
        else if (cleaned[a]=='-'){
            if (a==0)
                neg=true;
            else{}//Invalid. Assert? Throw? Return Struct?
        }
    }
    if (neg)buf*=-1;
    return buf;
}
long hexToInt(const std::string& str){
    return (int)hexToLong(str);
}
int octToInt(const std::string& str){
    return (int)simpleAtod(str,8);
}
double atod(const std::string& str){
    stringNumericType stype = getStringIntType(str);
    if (stype==invalidNumberString){
        return 0;
    }
    if (stype==unsignedIntegerString||stype==signedIntegerString){
        return simpleAtoi(str);
    }
    else if (stype==doubleString){
        return simpleAtod(str);
    }
    else if (stype==hexString){
        return hexToInt(str);
    }
    else if (stype==octalString){
        return octToInt(str);
    }
    else{//scientific notation mostly
        return complexAtod(str);
    }
    return 0;
}

std::string outbase(long num, int rad){
    static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static int i;
    std::string buf; buf.resize(65);

    /* bounds check for radix */
    if (rad < 2 || rad > 62)
        return "";
    /* if num is zero */
    if (!num)
        return "0";

    /* null terminate buf, and set i at end */
    i = 65;

    if (num > 0) {  /* if positive... */
        while (num) { /* until num is 0... */
            /* go left 1 digit, divide by radix, and set digit to remainder */
            buf[--i] = digits[num % rad];
            num /= rad;
        }
    } else {    /* same for negative, but negate the modulus and prefix a '-' */
        while (num) {
            buf[--i] = digits[-(num % rad)];
            num /= rad;
        }
        buf[--i] = '-';
    }
    /* return a duplicate of the used portion of buf */
    return buf.substr(i);
}

std::string fillToDigits(long value, int digits){
    std::string sval = ulti::hackString(value);
    while (sval.size()<digits){
        sval.insert(0,"0");
    }
    return sval;
}

int getMilitaryHour(std::string formattedTime){
    cfref(formattedTime);
    strreftoupper(formattedTime);
    std::string hour = chip(formattedTime,":",0).value;
    if (hour.size()>2){
        hour=hour.substr(hour.size()-2,2);
    }
    int ihour = ulti::atol(hour);
    if (ihour>24)
        return 0;//could return -1
    int pmpos = formattedTime.find("PM");
    int ampos = formattedTime.find("AM");
    if (ampos==std::string::npos&&pmpos==std::string::npos)
        return ihour;
    if (pmpos!=std::string::npos){
        if (ihour<12)ihour+=12;
        return ihour;
    }
    if (ihour==12)ihour-=12;
        return ihour;
}

};

