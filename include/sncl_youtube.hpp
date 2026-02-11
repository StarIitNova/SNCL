/* SNCL YouTube tools v1.00
   Defines parsers and canonicalizing tools for youtube URLs, stripping tracking data.

   Contributors:
   - StarIitNova (fynotix.dev@gmail.com)
 */

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

// Parses a youtube url as a string, resolving the timestamp and video id.
// Returns a YoutubeURL object, as defined above.
YoutubeURL parseYoutubeURL(const std::string &url);

// Canonicalizes a YoutubeURL object based on the type of output URL you wish for.
// This will strip all tracking data and convert all youtube URLs into a single format.
// For YoutubeURLType::TinyURL you'll get URLs like `https://youtu.be/VIDEO_ID?t=TIMESTAMP`
// For YoutubeURLType::Original you'll get URLs like `https://www.youtube.com/watch?v=VIDEO_ID&t=TIMESTAMP`
std::string getCanonicalYoutubeURL(const YoutubeURL &url, YoutubeURLType canonical = YoutubeURLType::TinyURL);

} // namespace SNCL

#endif // SNCL_YOUTUBE_HPP__
