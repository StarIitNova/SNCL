#ifndef SNCL_YOUTUBE_HPP__
#define SNCL_YOUTUBE_HPP__

#include <string>

namespace SNCL {

struct YoutubeURL {
    std::string videoId;
    int startTime;
};

enum class YoutubeURLType {
    TinyURL, // youtu.be/
    Original // youtube.com/watch?v=
};

YoutubeURL parseYoutubeURL(const std::string &url);
std::string getCanonicalYoutubeURL(const YoutubeURL &url, YoutubeURLType canonical = YoutubeURLType::TinyURL);

} // namespace SNCL

#endif // SNCL_YOUTUBE_HPP__
