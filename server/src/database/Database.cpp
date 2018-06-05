#include "Database.h"

#include <cereal/archives/json.hpp>
//#include <cereal/archives/binary.hpp>
//#include <cereal/archives/portable_binary.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

#include "log/Logger.h"

using namespace Log;
using namespace cereal;

Database::Database() {
    //load
    std::ifstream is("ClientDataManager.json", std::ifstream::in);
    if (is.is_open()) {
        try {
            JSONInputArchive archive(is);
            archive(clientDataManager);

            Logger::getInstance().logMessage("Database: Load ClientDataManager.json file");
        } catch (std::exception &e) {
            Logger::getInstance().logError(
                    "Database: Error while loading ClientDataManager.json file: " + std::string(e.what()));
        }
    } else {
        Logger::getInstance().logMessage("Database: ClientDataManager.json file does not exist");
    }
    is.close();
}

Database::~Database() {
    //save
    std::ofstream os("ClientDataManager.json");
    if (os.is_open()) {
        try {
            JSONOutputArchive archive(os);
            archive(clientDataManager);

            Logger::getInstance().logMessage("Database: Save ClientDataManager.json file");
        } catch (std::exception &e) {
            Logger::getInstance().logError(
                    "Database: Error while loading ClientDataManager.json file: " + std::string(e.what()));
        }
    } else {
        Logger::getInstance().logError("Database: Cannot create ClientDataManager.json file for saving");
    }
    os.close();
}

Database &Database::getInstance() {
    static Database database;
    return database;
}

ClientDataManager &Database::getClientDataManager() {
    return clientDataManager;
}
