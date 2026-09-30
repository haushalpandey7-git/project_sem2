#include <windows.h>
#include <winhttp.h>

#include "AIClassifier.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>

std::string AIClassifier::classifyWithAI(const std::string& fileName){

    // Get NVIDIA API key
    char* apiKey = nullptr;
    size_t keySize = 0;

    _dupenv_s(&apiKey, &keySize, "NVIDIA_API_KEY");

    if (apiKey == nullptr){
        std::cout << "Error: NVIDIA API key not found." << std::endl;
        return "Others";
    }

    std::string key(apiKey);
    free(apiKey);

    // NVIDIA server
    LPCWSTR serverName = L"integrate.api.nvidia.com";
    LPCWSTR apiPath = L"/v1/chat/completions";

    // Create HTTP session
    HINTERNET session = WinHttpOpen(
    L"AI Folder Management System",
    WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
    WINHTTP_NO_PROXY_NAME,
    WINHTTP_NO_PROXY_BYPASS,
    0
);

if (!session){
    std::cout << "Error: Could not create HTTP session." << std::endl;
    return "Others";
}

WinHttpSetTimeouts(
    session,
    30000,
    30000,
    60000,
    120000
);

    // Connect to NVIDIA
    HINTERNET connection = WinHttpConnect(
        session,
        serverName,
        INTERNET_DEFAULT_HTTPS_PORT,
        0
    );

    if (!connection){
        std::cout << "Error: Could not connect to NVIDIA API." << std::endl;

        WinHttpCloseHandle(session);
        return "Others";
    }

    // Create HTTPS POST request
    HINTERNET request = WinHttpOpenRequest(
        connection,
        L"POST",
        apiPath,
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        WINHTTP_FLAG_SECURE
    );

    if (!request){
        std::cout << "Error: Could not create API request." << std::endl;

        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        return "Others";
    }

    std::string fileNameOnly =
    std::filesystem::path(fileName).filename().string();

std::string jsonBody =
    "{"
    "\"model\":\"z-ai/glm-5.3-flash\","
    "\"messages\":["
    "{"
    "\"role\":\"system\","
    "\"content\":\"Classify files into exactly one category: Documents, Images, Videos, Audio, Code, or Others. Reply with only the category.\""
    "},"
    "{"
    "\"role\":\"user\","
    "\"content\":\"Classify this file: " + fileNameOnly + "\""
    "}"
    "],"
    "\"temperature\":0,"
    "\"max_tokens\":200"
    "}";

    // Convert API key to wide string
    std::wstring wideKey(key.begin(), key.end());

    // HTTP headers
    std::wstring wideHeaders =
        L"Content-Type: application/json\r\n"
        L"Authorization: Bearer " + wideKey + L"\r\n";

    // Send request
    BOOL result = WinHttpSendRequest(
        request,
        wideHeaders.c_str(),
        static_cast<DWORD>(-1),
        (LPVOID)jsonBody.c_str(),
        static_cast<DWORD>(jsonBody.length()),
        static_cast<DWORD>(jsonBody.length()),
        0
    );

    if (!result){
        std::cout << "Error: Failed to send request." << std::endl;

        WinHttpCloseHandle(request);
        WinHttpCloseHandle(connection);
        WinHttpCloseHandle(session);

        return "Others";
    }

    // Receive response
    result = WinHttpReceiveResponse(
    request,
    nullptr
    );

    if (!result){
    DWORD errorCode = GetLastError();

    std::cout << "Error: Failed to receive response." << std::endl;
    std::cout << "WinHTTP Error Code: "
              << errorCode
              << std::endl;

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connection);
    WinHttpCloseHandle(session);

    return "Others";
    }

    // Read response
    std::string response;

    DWORD bytesAvailable = 0;

    while (WinHttpQueryDataAvailable(
        request,
        &bytesAvailable) &&
        bytesAvailable > 0)
    {
        char* buffer = new char[bytesAvailable + 1];

        DWORD bytesRead = 0;

        if (WinHttpReadData(
            request,
            buffer,
            bytesAvailable,
            &bytesRead))
        {
            buffer[bytesRead] = '\0';
            response += buffer;
        }

        delete[] buffer;
    }

    // Close handles
    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connection);
    WinHttpCloseHandle(session);

    // Display NVIDIA response
std::cout << "\nAI Response:\n";
std::cout << response << std::endl;

// Extract AI classification from JSON response
size_t contentStart = response.find("\"content\":\"");

if (contentStart == std::string::npos){
    std::cout << "Error: Could not find AI classification." << std::endl;
    return "Others";
}

contentStart += 11; // Length of "\"content\":\""

size_t contentEnd = response.find("\"", contentStart);

if (contentEnd == std::string::npos){
    std::cout << "Error: Invalid AI response." << std::endl;
    return "Others";
}

std::string category =
    response.substr(contentStart, contentEnd - contentStart);

// Display extracted category
std::cout << "\nExtracted AI Category: "
          << category
          << std::endl;

// Check whether AI returned a valid category
if (category == "Documents" ||
    category == "Images" ||
    category == "Videos" ||
    category == "Audio" ||
    category == "Code" ||
    category == "Others"){
    return category;
}

std::cout << "Warning: AI returned an unknown category."
          << std::endl;

return "Others";
}
