#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
using namespace std;

// A subset of TradeDate, containing info from each Session (there are several per day)
struct Session {
    string utc_timestamp = ""; //341
    int trading_session_id = 0;  //336
    // Enumeration of 336 - 21: Pre-Open; 17: Ready to trade; 2: Trading halt(Pause);
    // 18: Not available for trading; 4: Close; 26: Post Close;
    int trading_session_interrupt = 0;  // [625] (only available when trading_session_id = 21)
};

// A subset of Product, containing information from each day from the week schedule
struct TradeDate {
    vector<Session> sessions;
    string trade_date_string = ""; // YYYYMMDD 75
    int no_trading_sessions = 0; // 386
};

// A data type that holds two things - a session and when it happens. Like a session, but less
struct SessionPair {
    string utc_timestamp = "";
    int trading_session_id = 0;
};

// The fundamental container of the Project - Holds information from the file
struct Product {
    vector<TradeDate> trade_dates;
    vector<SessionPair> session_pairs;
    string product_complex = "";  //1227
    string security_group = "";  // 1151
    int market_segment_id = 0;  //1300
    int no_dates = 0;  // 580
};

struct WeekdayWithString {
    string datestring = "";
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

// Chomps off from [0] to the index after the next SOH. So like, Del("1=abc[]2=efgh[]3=sadkf[]") = "2=efgh[]3=sadkf[]"
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
            // Delete the first chunk 35
            Product my_product;
            my_line = DelUpToNextItem(my_line);
            my_product.market_segment_id = stoi(GetNextChunk(my_line));
            my_line = DelUpToNextItem(my_line);
            my_product.product_complex = GetNextChunk(my_line);
            my_line = DelUpToNextItem(my_line);
            my_product.security_group = GetNextChunk(my_line);
            my_line = DelUpToNextItem(my_line);
            my_product.no_dates = stoi(GetNextChunk(my_line));
            my_line = DelUpToNextItem(my_line);
            vector<TradeDate> dates;
            int i = 0;

            while(i < my_product.no_dates){
                TradeDate this_date;
                this_date.trade_date_string = GetNextChunk(my_line);
                my_line = DelUpToNextItem(my_line);
                this_date.no_trading_sessions = stoi(GetNextChunk(my_line));
                my_line = DelUpToNextItem(my_line);
                vector<Session> sessions;
                int j = 0;

                while(j < this_date.no_trading_sessions){
                    Session this_session;
                    this_session.trading_session_id = stoi(GetNextChunk(my_line));
                    my_line = DelUpToNextItem(my_line);
                    this_session.utc_timestamp = GetNextChunk(my_line);
                    my_line = DelUpToNextItem(my_line);

                    SessionPair thispair;
                    thispair.trading_session_id = this_session.trading_session_id;
                    thispair.utc_timestamp = this_session.utc_timestamp;
                    my_product.session_pairs.push_back(thispair);

                    string nextLabel = GetNextLabel(my_line);
                    if(nextLabel == "625"){
                        this_session.trading_session_interrupt = stoi(GetNextChunk(my_line));
                        my_line = DelUpToNextItem(my_line);
                    }
                    else{
                        this_session.trading_session_interrupt = 0;
                    }

                    sessions.push_back(this_session);
                    j ++;
                }
                this_date.sessions = sessions;
                dates.push_back(this_date);
                i++;
            }

            my_product.trade_dates = dates;
            prod_list[my_product.security_group] = my_product;
        }
    }
    else{
        cout << "Failed to open file." << endl;
    }

    return prod_list;
}


int main() {
    // Set the filename
    string filename = "C:\\Users\\Grift\\Desktop\\workStuffs\\GetWeekInfo\\dataFiles\\TradingSessionListMemorialDay.dat";
    map<string, Product> productmap = MakeProductMap(filename);

    // So now what we're going to want to do is create a week and populate each day
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
            string date = myseshpair.utc_timestamp.substr(0, 8);
            int mysessionid = myseshpair.trading_session_id;
            bool dumpedit = false;

            // For all of the days that already exist, see if that session can go in there
            // Minor optimization, really only need to check the last day, I'm pretty sure. I could be wrong
            // and I can't think rn, so I'm going to do every day
            for (int j = 0; j < thisproductweek.size(); j++) {
                if(thisproductweek[j].datestring == date){
                    dumpedit = true;
                    thisproductweek[j].weekdaysessions.push_back(myseshpair);
                }
            }
            if(!dumpedit){
                // Then you gotta make a new day!
                WeekdayWithString newday;
                newday.datestring = date;
                newday.weekdaysessions.push_back(myseshpair);
                thisproductweek.push_back(newday);
            }
        }

        weekList[key] = thisproductweek;
    }

    /*  Just to run through all that again for my own sanity:
     *      I created a map from Product Descriptions to Lists of Weekdays (A 'week')
     *      To populate each 'week', I read in every session pair, and compared it to the rest of the list
     *      If the pair's date already existed, I would add it to that date
     *      Otherwise, I would make a new date, and append it to the week
     *      Doing this repeatedly should ensure that every day in a week is covered *BY DATE* rather than by
     *          the stamp given in the Trade Date Element
     *
     *      Some possible optimizations - could read the file faster, now that I'm not using most of the data
     *      Could probably save on memory by not reading in that data, two birds, one processor (and a GPU to boot)
     * */
}