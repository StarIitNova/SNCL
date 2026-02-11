#include <sncl_test.h>

#include <sncl_youtube.hpp>

using namespace SNCL;

TEST_CASE(YouTube_NormalWatchURL) {
    YoutubeURL yt = parseYoutubeURL("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ");
    return 0;
}

TEST_CASE(YouTube_WatchWithExtraParams) {
    YoutubeURL yt =
        parseYoutubeURL("https://www.youtube.com/watch?v=dQw4w9WgXcQ&feature=share&ab_channel=RickAstley&t=42");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 42, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ?t=42");
    return 0;
}

TEST_CASE(YouTube_ShortURL) {
    YoutubeURL yt = parseYoutubeURL("https://www.youtube.com/shorts/dQw4w9WgXcQ");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 0, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ");
    return 0;
}

TEST_CASE(YouTube_Shortened_youtu_be) {
    YoutubeURL yt = parseYoutubeURL("https://youtu.be/dQw4w9WgXcQ");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 0, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ");
    return 0;
}

TEST_CASE(YouTube_FragmentTimestamp) {
    YoutubeURL yt = parseYoutubeURL("https://www.youtube.com/watch?v=dQw4w9WgXcQ&t=60");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 60, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ?t=60");
    return 0;
}

TEST_CASE(YouTube_FormattedTimestamp) {
    YoutubeURL yt = parseYoutubeURL("https://www.youtube.com/watch?v=dQw4w9WgXcQ&t=2m32s");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 152, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ?t=152");
    return 0;
}

TEST_CASE(YouTube_IgnoreUnnecessaryParams) {
    YoutubeURL yt =
        parseYoutubeURL("https://www.youtube.com/watch?v=dQw4w9WgXcQ&list=PL1234567890&ab_channel=RickAstley");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 0, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt), "https://youtu.be/dQw4w9WgXcQ");
    return 0;
}

TEST_CASE(YouTube_AlternateCanonicalForm) {
    YoutubeURL yt =
        parseYoutubeURL("https://www.youtube.com/watch?v=dQw4w9WgXcQ&list=PL1234567890&ab_channel=RickAstley&t=1s");
    ASSERT_EQUAL(yt.videoId, "dQw4w9WgXcQ");
    ASSERT_EQUALFMT(yt.startTime, 1, "%d != %d");
    ASSERT_EQUAL(getCanonicalYoutubeURL(yt, YoutubeURLType::Original), "https://youtube.com/watch?v=dQw4w9WgXcQ&t=1");
    return 0;
}
