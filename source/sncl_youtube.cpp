#include <sncl_youtube.hpp>

#include <sstream>

namespace SNCL {

namespace dd__ {

int ConvertTimeSignatureToSeconds(const std::string &sig) {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    size_t pos = 0;
    while (pos < sig.size()) {
        if (!std::isdigit(sig.at(pos)))
            break;

        int start = pos;
        while (pos < sig.size() && std::isdigit(sig.at(pos)))
            pos++;

        if (pos >= sig.size()) {
            seconds = std::stoi(sig.substr(start));
            break;
        }

        int val = std::stoi(sig.substr(start, pos - start));
        char c = sig.at(pos);
        switch (c) {
        case 'h':
        case 'H':
            hours = val;
            break;
        case 'm':
        case 'M':
            minutes = val;
            break;
        case 's':
        case 'S':
            seconds = val;
            break;
        default:
            seconds = val;
            break;
        }

        pos++;
    }

    return hours * 60 * 60 + minutes * 60 + seconds;
}

} // namespace dd__

YoutubeURL parseYoutubeURL(const std::string &url) {
    YoutubeURL result;
    result.startTime = 0;

    std::string host, path, query;

    auto pos = url.find("://");
    std::string remainder = (pos != std::string::npos) ? url.substr(pos + 3) : url;

    pos = remainder.find('/');
    host = remainder.substr(0, pos);
    path = (pos != std::string::npos) ? remainder.substr(pos) : "/";

    if (host == "youtu.be") {
        result.videoId = path.substr(1);
    } else if (host.find("youtube.com") != std::string::npos) {
        if (path.find("/watch") == 0) {
            auto qpos = url.find("?");
            if (qpos != std::string::npos) {
                std::string query = url.substr(qpos + 1);
                std::stringstream ss(query);
                std::string kv;
                while (std::getline(ss, kv, '&')) {
                    auto eq = kv.find('=');
                    if (eq != std::string::npos) {
                        std::string key = kv.substr(0, eq);
                        std::string val = kv.substr(eq + 1);
                        if (key == "v") {
                            result.videoId = val;
                        } else if (key == "t") {
                            result.startTime = dd__::ConvertTimeSignatureToSeconds(val);
                        }
                    }
                }
            }
        } else if (path.find("/shorts/") == 0) {
            result.videoId = path.substr(8);
        }
    }

    return result;
}

std::string getCanonicalYoutubeURL(const YoutubeURL &url, YoutubeURLType canonical) {
    switch (canonical) {
    case YoutubeURLType::TinyURL: {
        std::string urlstr = "https://youtu.be/" + url.videoId;
        if (url.startTime > 0)
            urlstr += "?t=" + std::to_string(url.startTime);
        return urlstr;
    }
    case YoutubeURLType::Original: {
        std::string urlstr = "https://youtube.com/watch?v=" + url.videoId;
        if (url.startTime > 0)
            urlstr += "&t=" + std::to_string(url.startTime);
        return urlstr;
    }
    }
}

} // namespace SNCL
