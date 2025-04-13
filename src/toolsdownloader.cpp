#include "toolsdownloader.h"

ToolsDownloader::ToolsDownloader(Network& network)
    : network(network)
{
    // No need to parse versions or builds anymore
}

bool ToolsDownloader::download(const fs::path& to)
{
    try {
        fs::path darwin_path = to / FUSION_ZIP_TOOLS_NAME;
        fs::path darwin_pre15_path = to / FUSION_ZIP_PRE15_TOOLS_NAME;
        
        Logger::info("Downloading darwin.iso from Broadcom");
        network.curlDownload(DARWIN_ISO_URL, darwin_path.string());
        
        if (!fs::exists(darwin_path)) {
            Logger::error("Failed to download darwin.iso");
            return false;
        }
        
        Logger::info("Downloading darwinPre15.iso from Broadcom");
        network.curlDownload(DARWIN_PRE15_ISO_URL, darwin_pre15_path.string());
        
        if (!fs::exists(darwin_pre15_path)) {
            Logger::error("Failed to download darwinPre15.iso");
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        Logger::error("Error downloading tools: " + std::string(e.what()));
        return false;
    }
}
