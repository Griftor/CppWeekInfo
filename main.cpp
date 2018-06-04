#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <ctime>
using namespace std;

string DAYSOFWEEK [6] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
map<string, string> PRODUCT_CODES = {
        {"21", "Pre-Open"},
        {"17", "Ready to Trade"},
        {"2", "Trading Halt"},
        {"18", "Not available for trading"},
        {"4", "Close"},
        {"26", "Post Close"}
};

// A data type that holds two things - a session and when it happens. Like a session, but less
struct SessionPair {
    string ct_timestamp = "";
    int trading_session_id = 0;
};

// The fundamental container of the Project - Holds information from the file
struct Product {
    vector<SessionPair> session_pairs;
    string product_complex = "";  //1227
    string security_group = "";  // 1151
};

struct WeekdayWithString {
    string weekdayname = "";
    vector<SessionPair> weekdaysessions;
};

// Grabs the next substring up to the next SOH line, after the = sign
string GetNextChunk(string line){
    if(line.empty()){
        return nullptr;
    }
    else{
        int pos;
        pos = line.find(0x1);
        //Once again, the line should have one if it's non-empty, but lord only knows
        if(pos == string::npos){
            cout << "Wasn't able to find the terminating character. Your file is in the wrong format." << endl;
            return nullptr;
        }
        string chunk = line.substr(0, pos);
        pos = line.find("=");
        chunk = chunk.substr(pos + 1, chunk.length());

        return chunk;
    }
}

// Grabs the next label (substring up to SOH, but behind the = sign)
string GetNextLabel(string line){
    if(line == ""){
        return "done";
    }
    else{
        int pos;
        pos = line.find(0x1);
        //Once again, the line should have one if it's non-empty, but lord only knows
        if(pos == string::npos){
            cout << "Wasn't able to find the terminating character. Your file is in the wrong format." << endl;
            return nullptr;
        }
        string chunk = line.substr(0, pos);
        pos = line.find("=");
        chunk = line.substr(0, pos);
        return chunk;
    }
}

// Chomps off from [0] to the index after the next SOH. So like, Del("1=abc2=efgh3=sadkf") => "2=efgh3=sadkf"
string DelUpToNextItem(string line){
    if(line == ""){
        return nullptr;
    }
    else {
        int pos;
        pos = line.find(0x1);
        //Once again, the line should have one if it's non-empty, but lord only knows
        if(pos == string::npos){
            cout << "Wasn't able to find the terminating character. Your file is in the wrong format." << endl;
            return nullptr;
        }
        string rest = line.substr(pos + 1, line.length());
        return rest;
    }
}

//Checks to see if a vector contains a string
bool ContainsString(vector<string> vec, string to_find){
    vector<string>::iterator it;
    for(it = vec.begin(); it != vec.end(); it++){
        if(*it == to_find){
            return true;
        }
    }
    return false;
}

// creates a map from a product security group (Product.security_group) to the Product itself. That way I can loop
// through the list of products fairly easily
map<string, Product> MakeProductMap(string file_name) {
    //Read in the file provided by the first argument
    ifstream my_file(file_name);
    map<string, Product> prod_list;

    if (my_file) {
        // For each line in the file, add it to a map
        // For now I'll sort by the prod description, but that can be tweaked
        string my_line;

        while(getline(my_file, my_line)){
            Product my_product;
            my_line = DelUpToNextItem(my_line); // Chew Off the Chunk 35
            my_line = DelUpToNextItem(my_line); // Chew Off the Market Segment ID
            my_product.product_complex = GetNextChunk(my_line); // Need this one
            my_line = DelUpToNextItem(my_line);
            my_product.security_group = GetNextChunk(my_line); // Need this one too
            my_line = DelUpToNextItem(my_line);
            int no_dates = stoi(GetNextChunk(my_line)); // Need this, but not for long
            my_line = DelUpToNextItem(my_line);
            int i = 0;

            while(i < no_dates){
                my_line = DelUpToNextItem(my_line); // Chew off TradeDateString
                int no_trading_sessions = stoi(GetNextChunk(my_line)); // Only need this for a sec
                my_line = DelUpToNextItem(my_line);
                int j = 0;

                while(j < no_trading_sessions){
                    SessionPair thispair;
                    thispair.trading_session_id = stoi(GetNextChunk(my_line));
                    my_line = DelUpToNextItem(my_line); // Chew off the trading session id
                    string utc_timestamp = GetNextChunk(my_line); // Need to adjust the timestamp 5 hours to get it ct
                    int hour = stoi(utc_timestamp.substr(8, 2));
                    hour -= 5;
                    if(hour < 0) hour += 5;
                    string hourstring = to_string(hour);
                    if(hourstring.length() == 1) hourstring = "0" + hourstring;
                    thispair.ct_timestamp = utc_timestamp.substr(0, 8) + hourstring + utc_timestamp.substr(10, 10);

                    my_line = DelUpToNextItem(my_line); // Chew off the utc timestamp

                    my_product.session_pairs.push_back(thispair);

                    string nextLabel = GetNextLabel(my_line);
                    if(nextLabel == "625"){
                        my_line = DelUpToNextItem(my_line); // Get rid of the interrupt (Cause we don't care what people say)
                    }

                    j ++;
                }
                i++;
            }
            // Now gotta sort the timestamps
            // This works because every date will be bigger than the next one, alphabetically
            // Huh. Well played, CME
            sort( my_product.session_pairs.begin( ), my_product.session_pairs.end( ),
                  []( const SessionPair& a, const SessionPair&b ){
                      return a.ct_timestamp < b.ct_timestamp;
                  } );
            prod_list[my_product.security_group] = my_product;
        }
    }
    else{
        cout << "Failed to open file." << endl;
    }

    return prod_list;
}


// Makes a vector of Weekdays from a Product list
map<string, vector<WeekdayWithString>> MakeWeekList(map<string, Product> productmap){
    map<string, vector<WeekdayWithString>> weekList;

    map<string, Product>::iterator it;

    // For each product
    for ( it = productmap.begin(); it != productmap.end(); it++ )
    {
        string key = it->first;  // string (key)
        Product myprod = productmap[key];
        vector<SessionPair> myseshpairlist = myprod.session_pairs;

        vector<WeekdayWithString> thisproductweek;

        // For all of the sessions, dump them into a date
        for(int i = 0; i < myseshpairlist.size(); i ++){
            SessionPair myseshpair = myseshpairlist[i];
            string date = myseshpair.ct_timestamp.substr(0, 8);
            int mysessionid = myseshpair.trading_session_id;
            bool dumpedit = false;

            tm timestruct = {};
            timestruct.tm_year = stoi(date.substr(0, 4)) - 1900;
            timestruct.tm_mon = stoi(date.substr(4, 2)) - 1;
            timestruct.tm_mday = stoi(date.substr(6, 2));
            timestruct.tm_hour = 12;    //  To avoid any doubts about summer time, etc.
            mktime( &timestruct );

            int dayofweeknum = timestruct.tm_wday;  //  0...5 for Sunday...Friday. There shouldn't be a Saturday.

            string thisdayofweek = DAYSOFWEEK[dayofweeknum];

            // For all of the days that already exist, see if that session can go in there
            // Minor optimization, really only need to check the last day, I'm pretty sure. I could be wrong
            // and I can't think rn, so I'm going to do every day
            for (int j = 0; j < thisproductweek.size(); j++) {
                if(thisproductweek[j].weekdayname == thisdayofweek){
                    dumpedit = true;
                    thisproductweek[j].weekdaysessions.push_back(myseshpair);
                }
            }

            if(!dumpedit){
                // Then you gotta make a new day!
                WeekdayWithString newday;
                newday.weekdayname = thisdayofweek;
                newday.weekdaysessions.push_back(myseshpair);
                thisproductweek.push_back(newday);
            }
        }

        weekList[key] = thisproductweek;
    }

    return weekList;
}


// Returns a 4-digit integer in the form HHMM. If it's 3-digit, it's HMM. If it's 0, the exchange was closed
int CalculateOpenHours(vector<SessionPair> sessionvector){
    int timehours = 0;
    int timeminutes = 0;
    int openhour = 0;
    int openminute = 0;
    int closehour = 0;
    int closeminute = 0;
    bool tradingopen = false;

    vector<SessionPair>::iterator it;
    for(it = sessionvector.begin(); it != sessionvector.end(); it++){
        SessionPair mypair = *it;

        if(mypair.trading_session_id == 17){
            // If you find an open
            openhour = stoi(mypair.ct_timestamp.substr(8, 2));
            openminute = stoi(mypair.ct_timestamp.substr(10, 2));
            tradingopen = true;
        }
        else if(mypair.trading_session_id != 17 && tradingopen){ // Literally any other label stops trading
            // However, some products close while it's in the pre-open phase
            closehour = stoi(mypair.ct_timestamp.substr(8, 2));
            closeminute = stoi(mypair.ct_timestamp.substr(10, 2));
            // If you find a close
            if(openhour + openminute == 0){
                timehours += closehour;
                timeminutes += closeminute;
            }
            else{ // The only other option is that we opened earlier today, so do that
                timehours += closehour - openhour;
                timeminutes += closeminute - openminute;
                // We do have to scrub the time to make sure it works in an HHMM format
                if(closeminute - openminute < 0){
                    timehours -= 1;
                    timeminutes += 60;
                }
                if(timeminutes >= 60){
                    if(timeminutes >= 120){
                        timehours += 2;
                        timeminutes -= 120;
                    }
                    else {
                        timehours += 1;
                        timeminutes -= 60;
                    }
                }
            }

            // And in either case, trading has closed, so
            tradingopen = false;
        }
    }

    // Only other case then - market was open and it is now midnight
    if(tradingopen){
        timehours += 24 - openhour - 1; // The minus one is to account for the minutes. If it's > 60, it'll give it back
        timeminutes += 60 - openminute;
        // Unfortunately, I have to change two things *cough* pointers? *cough* but fortunately, no negative this time
        //And it can't be 120, so that's nice
        if(timeminutes >= 60){
            timehours += 1;
            timeminutes -= 60;
        }
    }

    int time = timehours * 100 + timeminutes;
    return time;
}


// Gets the day of the week. Weekday Name enumeration: Monday, Tuesday, etc.
vector<SessionPair> GetDayFromList(string WeekdayName, vector<WeekdayWithString> week){
    WeekdayWithString theday;

    vector<WeekdayWithString>::iterator it;

    for(it = week.begin(); it != week.end(); it ++){
        WeekdayWithString today = *it;
        if(today.weekdayname == WeekdayName){
            theday = today;
        }
    }

    return theday.weekdaysessions;
}


int main() {
    // Set the filenames
    string baselinefilename =
            "C:\\Users\\Grift\\Desktop\\workStuffs\\GetWeekInfo\\dataFiles\\TradingSessionList.dat";
    string specialfilename =
            "C:\\Users\\Grift\\Desktop\\workStuffs\\GetWeekInfo\\dataFiles\\TradingSessionListMemorialDay.dat";

    map<string, Product> baselineproductmap = MakeProductMap(baselinefilename);
    map<string, vector<WeekdayWithString>> baselineweeklist = MakeWeekList(baselineproductmap);

    cout << "Read in the baseline!" << endl;

    map<string, Product> specialproductmap = MakeProductMap(specialfilename);
    map<string, vector<WeekdayWithString>> specialweeklist = MakeWeekList(specialproductmap);

    cout << "Read in the special schedule!" << endl;


    // Okay, so now let's do some comparison
    map<string, vector<WeekdayWithString>>::iterator it;

    // For each product
    for(it = baselineweeklist.begin(); it != baselineweeklist.end(); it++){
        string key = it->first;
        // If both the symbols exist
        if(baselineweeklist.find(key) != baselineweeklist.end() && specialweeklist.find(key) != specialweeklist.end()){
            vector<WeekdayWithString> baselineweekprod = baselineweeklist[key];
            vector<WeekdayWithString> specialweekprod = specialweeklist[key];

            for (const string &day : DAYSOFWEEK) {
                vector<SessionPair> baselineweekday = GetDayFromList(day, baselineweekprod);
                vector<SessionPair> holidayweekday = GetDayFromList(day, specialweekprod);

                int baselinehours = CalculateOpenHours(baselineweekday);
                int holidayhours = CalculateOpenHours(holidayweekday);

                if(holidayhours > 0 && baselinehours == 0){
                    cout << "The product " << key << " had " << day << " off." << endl;
                }
                else{
                    if(baselinehours != holidayhours){
                        if(holidayhours == 0){
                            cout << "The product " << key << " did not trade on " << day << endl;
                        }
                        else{
                            cout << "The product " << key << " traded for " << to_string(holidayhours)
                                 << " when it normally trades for " << to_string(baselinehours) << " hours on "
                                 << day << endl;
                        }
                    }
                }
            }
        }
    }

}