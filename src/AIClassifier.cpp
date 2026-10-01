#include "AIClassifier.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

using json = nlohmann::json;

// Callback function to receive API response
static size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp){
    size_t totalSize = size * nmemb;

    std::string* response =
        static_cast<std::string*>(userp);

    response->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

std::string AIClassifier::classifyWithAI(
    const std::string& fileName){
    // Get NVIDIA API key
    char* apiKey = nullptr;
    size_t keySize = 0;

    _dupenv_s(
        &apiKey,
        &keySize,
        "NVIDIA_API_KEY"
    );

    if (apiKey == nullptr){
        std::cout
            << "Error: NVIDIA API key not found."
            << std::endl;

        return "Others";
    }

    std::string key(apiKey);
    free(apiKey);

    // Get only the file name
    std::string fileNameOnly =
        std::filesystem::path(fileName)
            .filename()
            .string();

    // Create JSON request
    json requestBody = {
        {"model", "z-ai/glm-5.3-flash"},{
            "messages",
            json::array({
                {
                    {"role", "system"},
                    {
                        "content",
                        "Classify files into exactly one category: "
                        "Documents, Images, Videos, Audio, Code, or Others. "
                        "Reply with only the category."
                    }
                },
                {
                    {"role", "user"},
                    {
                        "content",
                        "Classify this file: " + fileNameOnly
                    }
                }
            })
        },
        {"temperature", 0},
        {"max_tokens", 200}
    };

    // Convert JSON object to string
    std::string jsonBody =
        requestBody.dump();

    // Response from NVIDIA
    std::string response;

    // Initialize libcurl
    CURL* curl = curl_easy_init();

    if (curl == nullptr){
        std::cout
            << "Error: Could not initialize libcurl."
            << std::endl;

        return "Others";
    }

    // HTTP headers
    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    std::string authorization =
        "Authorization: Bearer " + key;

    headers = curl_slist_append(
        headers,
        authorization.c_str()
    );

    // NVIDIA API URL
    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://integrate.api.nvidia.com/v1/chat/completions"
    );

    curl_easy_setopt(
    curl,
    CURLOPT_CAINFO,
    "D:/msys64/ucrt64/etc/ssl/certs/ca-bundle.crt"
    );

    // POST request
    curl_easy_setopt(
        curl,
        CURLOPT_POST,
        1L
    );

    // Request body
    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        jsonBody.c_str()
    );

    // Headers
    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    // Response callback
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    // Timeout
    curl_easy_setopt(
        curl,
        CURLOPT_TIMEOUT,
        120L
    );

    // Perform request
    CURLcode result =
        curl_easy_perform(curl);

    if (result != CURLE_OK){
        std::cout
            << "Error: NVIDIA API request failed."
            << std::endl;

        std::cout
            << "libcurl Error: "
            << curl_easy_strerror(result)
            << std::endl;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        return "Others";
    }

    // Get HTTP status code
    long httpCode = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &httpCode
    );

    // Clean up libcurl
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Display HTTP status
    std::cout
        << "\nHTTP Status: "
        << httpCode
        << std::endl;

   

    // Check HTTP response
    if (httpCode != 200){
        std::cout
            << "Error: NVIDIA API returned HTTP "
            << httpCode
            << std::endl;

        return "Others";
    }

    // Parse JSON response
    try{
        json responseJson =
            json::parse(response);

        std::string category =
            responseJson
                ["choices"]
                [0]
                ["message"]
                ["content"]
                .get<std::string>();

        // Remove leading/trailing spaces
        size_t start =
            category.find_first_not_of(" \t\n\r");

        size_t end =
            category.find_last_not_of(" \t\n\r");

        if (start != std::string::npos &&
            end != std::string::npos){
            category =
                category.substr(
                    start,
                    end - start + 1
                );
        }

        std::cout
         << "AI Classification: "
         << category
         << std::endl;

        // Check valid category
        if (category == "Documents" ||
            category == "Images" ||
            category == "Videos" ||
            category == "Audio" ||
            category == "Code" ||
            category == "Others"){
            return category;
        }

        std::cout
            << "Warning: AI returned an unknown category."
            << std::endl;
    }
    catch (const json::exception& e){
        std::cout
            << "Error: Could not parse NVIDIA JSON response."
            << std::endl;

        std::cout
            << "JSON Error: "
            << e.what()
            << std::endl;
    }

    return "Others";
}

