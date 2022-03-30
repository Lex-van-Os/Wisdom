#include <dpp/dpp.h>
#include <fstream>
#include <string>

using namespace std;

string getToken() {
    ifstream infile("../../../../WisdomBotToken.txt");
    string key;

    if (infile.good()) {
        printf("%s\n", "blabla");
        string key;
        getline(infile, key);
        cout << key << endl;
        return key;
    }
    else {
        cout << "Failed to open file\n";
    }
}

const std::string BOT_TOKEN = getToken();
const dpp::snowflake MY_GUILD_ID  = 957974039362678825;

int main()
{
    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_interaction_create([](const dpp::interaction_create_t& event) {
         if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.guild_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id), MY_GUILD_ID);
        }
    });

    /* Start the bot */
    bot.start(false);

    return 0;
}