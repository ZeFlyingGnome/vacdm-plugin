#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include <nlohmann/json.hpp>

#include "types/Pilot.h"

namespace vacdm::com {
class Server {
   public:
    typedef struct ServerConfiguration_t {
        std::string name = "";
        bool allowMasterInSweatbox = false;
        bool allowMasterAsObserver = false;
    } ServerConfiguration;

   private:
    Server();

    std::string m_authToken;
    std::mutex m_clientMutex;
    std::unique_ptr<httplib::Client> m_client;

    bool m_apiIsChecked;
    bool m_apiIsValid;
    std::string m_baseUrl;
    bool m_clientIsMaster;
    std::string m_errorCode;
    ServerConfiguration m_serverConfiguration;

    std::list<std::string> m_supportedAirports;

   public:
    ~Server();
    Server(const Server&) = delete;
    Server(Server&&) = delete;

    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;

    static Server& instance();

    void changeServerAddress(const std::string& url);
    bool checkWebApi();
    ServerConfiguration_t getServerConfig();
    std::list<types::Pilot> getPilots(const std::list<std::string> airports);
    void postPilot(types::Pilot);
    void patchPilot(const nlohmann::json& root);

    /// @brief Sends a post message to the specififed endpoint url with the root as content
    /// @param endpointUrl endpoint url to send the request to
    /// @param root message content
    void sendPostMessage(const std::string& endpointUrl, const nlohmann::json& root);

    /// @brief Sends a patch message to the specified endpoint url with the root as content
    /// @param endpointUrl endpoint url to send the request to
    /// @param root message content
    void sendPatchMessage(const std::string& endpointUrl, const nlohmann::json& root);
    void sendDeleteMessage(const std::string& endpointUrl);

    void updateExot(const std::string& pilot, const std::chrono::utc_clock::time_point& exot);
    void updateTobt(const types::Pilot& pilot, const std::chrono::utc_clock::time_point& tobt, bool manualTobt);
    void updateAsat(const std::string& callsign, const std::chrono::utc_clock::time_point& asat);
    void updateAsrt(const std::string& callsign, const std::chrono::utc_clock::time_point& asrt);
    void updateAobt(const std::string& callsign, const std::chrono::utc_clock::time_point& aobt);
    void updateAort(const std::string& callsign, const std::chrono::utc_clock::time_point& aort);

    void resetTobt(const std::string& callsign, const std::chrono::utc_clock::time_point& tobt,
                   const std::string& tobtState);
    void deletePilot(const std::string& callsign);

    const std::string& errorMessage() const;
    void setMaster(bool master);
    bool getMaster();

    void retrieveSupportedAirports();
    std::list<std::string> getSupportedAirports();

   private:
    // Helper method to initialize/reinitialize the HTTP client
    void initClient();
};
}  // namespace vacdm::com