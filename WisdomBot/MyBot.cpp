#include <dpp/dpp.h>
#include <fstream>
#include <string>
#include <random>

using namespace std;

string getToken() {
    ifstream infile("../../../../WisdomBotToken.txt");
    string key;

    if (infile.good()) {
        getline(infile, key);
        return key;
    }
    else {
        cout << "Failed to open file\n";
    }
}

int getLines(string fileName) {
    ifstream infile(fileName);
    string quote;
    int total_lines = 0;

    if (infile.good()) {
        while (getline(infile, quote)) {
            ++total_lines;
        }
        return total_lines;
    }
    else {
        cout << "Something went wrong...";
    }
}

const std::string BOT_TOKEN = getToken();
const dpp::snowflake MY_GUILD_ID  = 957974039362678825;
const int QUOTE_LINES = getLines("../../../wisdomquotes.txt");

string quote() {
    std::random_device random_device;
    std::mt19937 gen(random_device());
    ifstream infile("../../../wisdomquotes.txt");
    string quote;
    int iterated_lines = 0;
        
    if (infile.good()) {
        std::uniform_int_distribution<> distribute(0, QUOTE_LINES);
        int picked_line = distribute(gen);

        while (getline(infile, quote)) {
            if (iterated_lines == picked_line) {
                return quote;
            }
            else {
                iterated_lines++;
            }
        }
        return quote;
    }
    else {
        return "Something went wrong...";
    }
}

int main()
{

    dpp::cluster bot(BOT_TOKEN);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_interaction_create([](const dpp::interaction_create_t& event) {
         if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
         if (event.command.get_command_name() == "quote") {
             event.reply(quote());
         }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.guild_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id), MY_GUILD_ID);
            bot.guild_command_create(dpp::slashcommand("quote", "A random inspiring quote", bot.me.id), MY_GUILD_ID);
        }
    });

    /* Start the bot */
    bot.start(false);

    return 0;
}