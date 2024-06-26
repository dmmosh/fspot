#include "header.h"


// CONSTRUCTORS

// input and type initializer
player::player(std::string* ACCESS_TOKEN, std::string* REFRESH_TOKEN, std::string* USER, std::string* PASS, unsigned long* REFRESH_AT):
input("\0"),
message(""),
type(true),
cover(false),
progress(0), //progress is 0
duration(100), //duration is 100 to avoid division errors
percent(0.0),
is_playing(false),
artist_print(0), //prints no one duh
artists({"no one yet"}), //default json array
name("connecting ? maybe"),
ACCESS_TOKEN(ACCESS_TOKEN),
REFRESH_TOKEN(REFRESH_TOKEN),
USER(USER),
PASS(PASS),
REFRESH_AT(REFRESH_AT),
log_thread(std::jthread(&player::keylog, this)),
song_thread(std::jthread(&player::song_update, this)) //updates every second
{
    log_thread.detach();
    song_thread.detach();
    
    //std::jthread(&players::refresh, this).detach(); // refreshes the token
    //std::jthread log_thread(&main_player::keylog, this); //keylogging enabled
    
    const short row_clear = 4;

    move::down(row_clear);
    move::up(row_clear);

    while(type.load()){ //keeps updating
        unsigned int col_size = col_update();

        // prints minutes / seconds  of progress (in sec)
        std::string title = name + ((artists.size() >1) ? " : [" + std::to_string(artist_print.load()+1) + "] " : " : ") + artists[artist_print.load()];
        
        std::cout << CENTER(title, col_size) <<  NEW;

        if (col_size > 10) {
            int bar_size = col_size-10;

            std::string bar = (((int)(bar_size*percent.load()) > 0) ? std::string((int)(bar_size*percent.load()), '-') : "") + 
                              (((int)(bar_size*(1.0-percent.load())) > 0) ? std::string((int)(bar_size*(1.0-percent.load())), ' ') : "") ;
            std::cout << "  " << BOLD_ON << CENTER("<" + bar + ">", col_size)  << BOLD_OFF << '\r';
        }

        printf("%02i:%02i\n\n", progress.load() / 60, progress.load() % 60);
        //timer(progress);


        short padding = (col_size-message.size())/2;
        if (padding)
            std::cout << std::string(padding, ' ') << INVERT_ON << message << INVERT_OFF << '\r';
        //std::cout<< CENTER(message) << '\r';
        std::cout<< INVERT_ON << " // " << input <<  INVERT_OFF << " "; 
        //move::right(3+input.length());
        SLEEP(0.03);
        move::clear();
        
        padding = (col_size-message.size())/2;
        if (padding)
            std::cout << std::string(padding, ' ') << INVERT_ON << message << INVERT_OFF << '\r';
        //std::cout<< CENTER(message) << '\r';
        std::cout<< INVERT_ON << " // " << input <<  INVERT_OFF << " "; 

        //move::right(3+input.length());
        //move::beginning();

        move::up_clear(row_clear-1);

    }  
    move::down();

};

// CLASS DESTRUCTORS
player::~player(){
    if(cover.load()){
        unsigned short col_size = std::min(col_update()-2, row_update()*2-10);
        move::down(col_size+4);
        move::clear();
        move::up_clear(col_size/2+4);
    //move::clear();
    //move::up_clear(2);

    }
};

// MES   SAGES 
// mini message: meant to be turned off, doesnt use thread
// message: automatically turns off after a given time USE ONLY WHEN NEEDED

// MINI MESSAGE : when it changes a lot
// meant to be turned off, will stay forever if doesnt 
inline void player::MINI_MESSAGE(const std::string& msg){
    message = (std::string("[ ") + msg + " ]"); //assigns new message
}

inline void player::MINI_MESSAGE(const char* msg) {
    message = (std::string("[ ") + msg + " ]"); //assigns new message
};

// standard message 
inline void player::MESSAGE(const std::string msg, const double time){
    MINI_MESSAGE(msg);
    std::jthread( [this, time] {
        std::string temp = message; // temp string
        SLEEP(time); // waits the time
        if (temp == message) MESSAGE_OFF; // turn message off only if theres no new message to replace it
    }).detach();
}

inline void player::MESSAGE(const std::string msg){
    MESSAGE(msg, 5.0);
};

inline void player::MESSAGE(){
    MESSAGE("Loading...", 5.0);
}

// refreshes the token 
int player::refresh(){
    // refresh post request response
    MINI_MESSAGE("Refreshing...");
    cpr::Response r = cpr::Post(cpr::Url{TOKEN_URL},
                                cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Authorization", "Basic " + base64::encode(std::string(CLIENT_ID) + ':' + CLIENT_SECRET)}
                                }, 
                                cpr::Parameters{
                                {"grant_type", "refresh_token"},
                                {"refresh_token", *REFRESH_TOKEN}
                                });

    //if successful
    if (r.status_code == 200) { // if token was refreshed successfully
        //std::cout << r.text << NEW << NEW << NEW << NEW;
        MESSAGE("Token refreshed!", 1.0); // message
        json new_token = json::parse(r.text); // new token
        *ACCESS_TOKEN = new_token["access_token"].get<std::string>();
        *REFRESH_AT = new_token["expires_in"].get<unsigned long>();
        *REFRESH_AT += POSIX_TIME;
    } else {
        MESSAGE("Token refresh failed");
    }
    return r.status_code;
}


// CHARACTER INPUT  and keylog
// any subclass
void player::keylog(){
        static unsigned short input_len = 0; //length of input string (without null terminator)

        while(type.load()){
        
        char buf = get_char();
        switch(buf){
            case 0: // if theres nothing in the buffer
                continue;
            break;
            case ENTER:
                commands();
                input_len = 0;
                input[0] = '\0';
            break;  
            case SPACE:


                if (is_playing){
                    MINI_MESSAGE("Pausing...");
                    (void)cpr::Put(INTO("me/player/pause"));
                    MESSAGE("Paused!", 1);
                    is_playing= false;
                } else {
                    MINI_MESSAGE("Playing...");
                    (void)cpr::Put(INTO("me/player/play"));
                    MESSAGE("Playing now!", 1);
                    is_playing= true;
                };
            break;
            case BACKSPACE: 
                if(input_len){ //substract a character from input
                    input_len--;
                    input[input_len] = '\0';
                }
            break;
            case '.': //forward 10 seconds
                if (!strcmp(input, "v")) {
                    volume(true, input_len);
                } else {
                    forward(true);
                }
            break;
            case ',':
                if (!strcmp(input, "v")) {
                    volume(false, input_len);
                } else {
                    forward(false);
                }
            break;
            case '>':
                MESSAGE("Nexting...", 2.0);


                (void)cpr::Post(INTO("me/player/next"));
                SLEEP(0.5); //need a cooldown
            break;
            case '<':
                MESSAGE("Previousing...", 2.0);
                (void)cpr::Post(INTO("me/player/previous"));
                SLEEP(0.5); //need a cooldown
            break;
            default:
                if (input_len < 15) { //make sure the string is within bounds
                    input[input_len] = tolower(buf);
                    input_len++;
                    input[input_len] = '\0';
                }

        }     
        SLEEP(0.0001);
    }
}

// PLAYERS DEFAULTS

void player::forward(const bool forward_back){

    unsigned int x = 1;
    unsigned short sec_ctr, min, sec;
    MINI_MESSAGE("+00:00");
    if(!forward_back) message[2] = '-';

    while(1){ //iterates the ctr

        x = (x+1) % 10000; 
        sec_ctr = forward_fun(x) % 3600;
        if (!sec_ctr) sec_ctr++;
        
         min = sec_ctr /60;
         sec = sec_ctr%60;
    
        if(min<10){
            message[3] = '0';
            message[4] = min+'0';
        } else {
            message[3] = min/10 + '0';
            message[4] = min%10 + '0';
        };
    
        if(sec<10){
            message[6] = '0';
            message[7] = sec+'0';
        } else {
            message[6] = sec/10 + '0';
            message[7] = sec%10 + '0';
        }
        

        //printf("%02i:%02i\n\n", progress / 60, progress % 60);
        //MINI_MESSAGE(((forward_back) ?"+" : "-") + std::string(((sec_ctr/60 <10) ? "0" : "")) + std::to_string(sec_ctr/60) + ":" + std::string(((sec_ctr%60 <10)) ? "0" : "") + std::to_string(sec_ctr%60)); 
        //MINI_MESSAGE( std::string((forward_back) ?"+" : "-") +  timer(sec_ctr));
        if (!get_char()) {
            SLEEP(0.1);
            if (!get_char())
                break;
        }
    }

    if (sec_ctr <2){
        MESSAGE(message.substr(2, 6), 2.0);
        return;
    }  


    if(forward_back) {
        if (progress.load()+sec_ctr > duration.load()){
            MINI_MESSAGE("Nexting...");
            (void)cpr::Post(INTO("me/player/next"));
        } else {
            if (message.size()> 8) message.insert(8, "...");
            //MINI_MESSAGE(std::string("+") + timer(sec_ctr) + "...");

            (void)cpr::Put(INTO("me/player/seek"),
                        cpr::Parameters{{"position_ms", std::to_string((progress.load() + sec_ctr)*1000)}});

        }
    } else {
        if (progress.load()-sec_ctr <= 0){
                MINI_MESSAGE("wind it back!");
                (void)cpr::Put(INTO("me/player/seek"),
                            cpr::Parameters{{"position_ms", "0"}});

        } else {   // if it doesnt, actually go forward
            
            if (message.size()> 8) message.insert(8, "...");
            
            (void)cpr::Put(INTO("me/player/seek"),
                        cpr::Parameters{{"position_ms", std::to_string((progress.load() - sec_ctr)*1000)}});
        }

    }
    MESSAGE_OFF;

}


// RETURNS WHETHER OR NOT TO CLEAR THE MESSAGE
void player::volume(const bool add_substr, unsigned short& input_len){
    

    cpr::Response r = cpr::Get(INTO("me/player"));
    if (r.status_code != 200)
        return;

    json resp = json::parse(r.text);
    unsigned short volume = resp["device"]["volume_percent"];
    unsigned short init_volume = volume;

    MINI_MESSAGE("vol   ");

    while(1){ //iterates the ctr

        volume = (add_substr) ? std::min(100, volume+1) : std::max(0, volume-1);

        if (volume == 100){
            message[5] = '1'; message[6] = '0'; message[7] = '0';
        } else {
            message[6] = volume/10 + '0'; message[7] = volume%10 + '0';
        }

        if (!get_char()) {
            SLEEP(0.1);
            if (!get_char())

                break;
        }
    }

    if(abs(volume-init_volume) >1){

        if(volume == 100)
            MINI_MESSAGE("max vol...");
        else if (volume == 0)
            MINI_MESSAGE("muting...");
        else   
            if (message.size()> 8) message.insert(8, "...");

        (void)cpr::Put(INTO("me/player/volume?volume_percent=" + std::to_string(volume)));
        //MESSAGE(message.substr(2, 6), 1.5);
        input[0] = '\0';
        input_len = 0;
        MESSAGE_OFF;
    } else {
        MESSAGE(message.substr(2,6), 1.5);
    }
};


// default commands
void player::commands(){

    // guranteed all commands will contain different first and last chars
    json data; // temp response variable
    cpr::Response r; //temp r variable

    if (!strcmp(input, "play")){ // plays track
        MINI_MESSAGE("Playing...");
        (void)cpr::Put(INTO("me/player/play"));
        MESSAGE("Playing now!", 1);
        is_playing.store(true);


    } else if (!strcmp(input, "pause")){ // pauses track
        MINI_MESSAGE("Pausing...");
        (void)cpr::Put(INTO("me/player/pause"));
        MESSAGE("Paused!", 1);
        is_playing.store(false);


    } else if (!strcmp(input, "pp")) { //plays / pauses track

        if (is_playing.load()){
            MINI_MESSAGE("Pausing...");
            (void)cpr::Put(INTO("me/player/pause"));
            MESSAGE("Paused!", 1);
            is_playing.store(false);


        } else {
            MINI_MESSAGE("Playing...");
            (void)cpr::Put(INTO("me/player/play"));
            MESSAGE("Playing now!", 1);
            is_playing.store(true);


        };

    } else if (!strcmp(input, "quit")){ //quit
        MINI_MESSAGE("Quitting...");
        type.store(false);
        log_thread.request_stop();

    } else if (!strcmp(input, "clear")){
        cover.store(false);
        system("clear");

    } else if (!strcmp(input, "cover")){

        if (exec("command -v icat").size()) {
            if(cover.load()) {
	
                // CLEARS THE COVER ALREADY PRESENT
                unsigned short col_size = std::min(col_update()-2, row_update()*2-10);
                move::down(col_size+4);
                move::clear();
                move::up_clear(col_size/2+4);
                cover.store(false);
                MESSAGE("Covers off!", 1.0);
            } else {
		MINI_MESSAGE("Covering...");
                r = cpr::Get(INTO("me/player"));
                if(r.status_code == 200)
                    cover_fun(json::parse(r.text)["item"]["album"]["images"][0]["url"].get<std::string>());

                cover.store(true); 
                MESSAGE("Covers on!", 1.0);
            }
        } else {
            type.store(false);
            ERROR("Icat not installed. This command required icat.");
        }

    } else if (!strcmp(input, "connect")){
        connect_player();
        
    } else if (!strcmp(input, "refresh")) {
        refresh();

    } else if (!strcmp(input, "hello")) {
        MESSAGE("Hello vro...", 2.0);
        
    }

    return;
};

// MAIN PLAYER CLASS


void player::song_update() {
    static int tmp_dur = 0;
    static bool error = false;
    static std::string tmp_name = "NO CONNECTION";
    while(type.load()){    

        cpr::Response r = cpr::Get(INTO("me/player"));

        if(r.status_code == 200){
            if(error){
                MESSAGE_OFF;
                error = false;
            }

            json data = json::parse(r.text);
            is_playing.store((bool)data["is_playing"]);
            progress.store((unsigned int)data["progress_ms"]/1000); //progress in seconds

            duration.store((unsigned int)data["item"]["duration_ms"]/1000);

            percent.store((double)progress/duration);

            name = data["item"]["name"].get<std::string>();

            // IF THERES BEEN A SONG SWITCH
            if(tmp_dur != duration.load() && tmp_name != name){
                MESSAGE("New track!", 1.0);

                if (tmp_dur >= 3600) {
                    type.store(false);  
                    ERROR("Song's too long. FSpot can't play songs longer than 1 hour.");
                };



                
                if (cover.load()){ // if cover is shown
                    cover_fun(data["item"]["album"]["images"][0]["url"].get<std::string>());

                }

                artists = {};
                for (const auto& artist: data["item"]["artists"]){
                    artists.push_back(artist["name"].get<std::string>());
                }
            }


            tmp_dur = duration.load()/1000;

            tmp_name = name;

        } else if (r.status_code == 204){
            artists = {"none"};
            name= "NO PLAYBACK";
        } else {
            MINI_MESSAGE("ERR " + std::to_string(r.status_code));
            error = true;
            progress.store(0);
            percent.store(0);
            duration.store(100);
            artists = {"mr. nuh uh"};
            name = "NO CONNECTION";
            //refresh();
        }   

        if (POSIX_TIME + (tmp_dur-progress.load()) >= *REFRESH_AT) refresh(); //if next song is over the token expire, refresh it
        if (artists.size()) artist_print.store((artist_print.load()+1) % artists.size());

        SLEEP(1);
        

    }
};


// cover function where data is a pointer to json of "me/player"
void player::cover_fun(const std::string& url){
       //std::cout << url << NEW << NEW << NEW << NEW;
       //std::string url = "https://i.scdn.co/image/ab67616d00001e02ff9ca10b55ce82ae553c8228";

       // Perform the HTTP GET request
       auto response = cpr::Get(cpr::Url{url});
       // Check if the request was successful
       if (response.status_code == 200) {
           // Open a file stream to save the downloaded image
           std::ofstream imageFile(FOLDER+ ".cover.jpg", std::ofstream::binary);
           
           // Write the image data to the file
           imageFile.write(response.text.c_str(), response.text.length());
           imageFile.close();
            unsigned short col_size = std::min(col_update()-2, row_update()*2-10);
           std::string spacing = "\n" + std::string((col_update()-col_size) /2, ' '); //center with spacing
           std::string cover_str = exec("icat --width " + std::to_string(col_size) + " " +  FOLDER + ".cover.jpg");
            cover_str.resize(cover_str.size()-1);

           std::string::size_type n = 0;
           int new_line = 1;
           while ( ( n = cover_str.find( "\n", n ) ) != std::string::npos )
           {   
               new_line++;
               cover_str.replace( n, 1, spacing);
               n += 7;
           }
           std::cout<<  NEW << spacing << cover_str;
           move::up(new_line+1);
           // Close the file stream
       }
}

void player::connect_player(){
    MINI_MESSAGE("Playering...");
    unsigned short timer = 40;
    while(timer){
        SLEEP(0.05);
        cpr::Response r = cpr::Get(INTO("me/player/devices"));

        if (r.status_code != 200){
            MESSAGE_OFF;
            type.store(false);  
            ERROR("Something else happened. Failed to connect player.");
            return;
        }
        json devices = json::parse(r.text);

        for(const auto& device: devices["devices"]){
            if (device["name"].get<std::string>() == "fspot player"){
                cpr::Response change = cpr::Put(INTO("me/player"), cpr::Body{{ "device_ids", {device["id"].get<std::string>()} }});
                std::cout << change.text << NEW ;
            }
        }
        std::cout << NEW << NEW << NEW << NEW;

        timer--;    
    }

    MESSAGE_OFF;
};



// HELPER FUNCTIONS

unsigned short col_update(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

unsigned short row_update(){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
};


inline constexpr int forward_fun(const int x_val){
    return (int)((double)x_val*x_val/70);
};

inline static char* timer(const unsigned int seconds){
    static char timer[] = "00:00";

    //strcpy(timer, "00:00");

    unsigned short min = seconds /60;
    unsigned short sec = seconds%60;

    if(min<10){
        timer[0] = '0';
        timer[1] = min+'0';
    } else {
        timer[0] = min/10 + '0';
        timer[1] = min%10 + '0';
    };

    if(sec<10){
        timer[3] = '0';
        timer[4] = sec+'0';
    } else {
        timer[3] = sec/10 + '0';
        timer[4] = sec%10 + '0';
    }

    return timer;

};

// default center
inline std::string CENTER( std::string input){
    return CENTER(input, col_update());
}

// center function, has unsigned int to reduce col_update calls
// usually called in loops, which usually have their own col_size variable
// NOTE: changes the input variable if needs concat
inline std::string CENTER( std::string input, const unsigned short col_size){
    if (input.empty()) {
        input = std::string("");
    
    } else if (input.size()+4 >= col_size) {
        input = CENTER(input.substr(0,col_size-8) + "...");
    } else {
        unsigned short padding = (col_size-input.size())/2;
        input.insert(0, std::string((padding > 0) ? padding : 1, ' '));
    };

    return input;
};



void print_logo(){
    unsigned short col_size = col_update();
    unsigned short title_num;

    if (col_size < 42)
        title_num = 1;
    else if (col_size < 52)
        title_num = 2;
    else if (col_size < 123)
        title_num = 3;
    else 
        title_num = 4;
    
    std::ifstream title_text(FOLDER + "titles/title" + std::to_string(title_num) + ".txt");


    move::up();
    if (title_text.is_open()) {
        std::string line;
        while (std::getline(title_text, line)) {
            std::cout << BOLD_ON << line << BOLD_OFF << NEW; // Print each line
        }
        title_text.close(); // Close the file
    } else {
        std::cout << BOLD_ON << "FSPOT" << BOLD_OFF << NEW;
    }
};

std::string exec(const std::string& cmd) {
    std::array<char, 200> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) { //exception handle
        throw std::runtime_error("popen() failed!");
    } 
    std::string out = "";

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {

        out.append(buffer.data()); // pushes the output to the back of the array
        
    }
    return out;
}

char get_char(){
        
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 0;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    
    return buf;

};


namespace move{
    inline void clear()                           { std::cout << "\x1b[2K\r"; };
    inline void clear(const std::string& newline) { std::cout << "\x1b[2K\r" <<  newline;  };
    
    inline void up()              { printf("\x1b[1A"); };
    inline void up(const int amt) { printf("\x1b[%iA", amt); };

    inline void down()              { printf("\x1b[1B"); };
    inline void down(const int amt) { printf("\x1b[%iB", amt); };

    inline void beginning() { printf("\r"); };

    inline void up_clear()        { std::cout << "\x1b[1A\x1b[2K\r"; };
    void up_clear(int amt) { while(amt) { std::cout << "\x1b[1A\x1b[2K\r"; amt--; } };
    void up_clear(const std::string& word_above, const int col_size) {
        if(word_above.size() < col_size) {
            up_clear();
        } else {
            up_clear((int)ceil((double)word_above.size()/(double)col_size));
        }
    };

    inline void left()              { printf("\x1b[1D"); };
    inline void left(const int amt) { printf("\x1b[%iD", amt); };

    inline void right()              { printf("\x1b[1C"); };
    inline void right(const int amt) { printf("\x1b[%iC", amt); };


}
namespace base64{


    std::string encode(const std::string &in) {

        std::string out;

        int val = 0, valb = -6;
        for (unsigned char c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
                valb -= 6;
            }
        }
        if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
        while (out.size()%4) out.push_back('=');
        return out;
    }

    std::string encode(const char* in){
        return encode(std::string(in));
    };

    std::string decode(const std::string &in) {

        std::string out;

        std::vector<int> T(256,-1);
        for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

        int val=0, valb=-8;
        for (unsigned char c : in) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val>>valb)&0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    std::string decode(const char* in) {
        return decode(std::string(in));
    };

}
