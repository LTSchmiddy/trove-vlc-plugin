log.info("Hello from Sol2!")

-- print(cjson.encode({["Hello"] = "Alex"}))
-- print(cjson.decode("{\"hello\": null}")["hello"])

TMDB_API_KEY = "42cb439f9947906214fe04dc7ed1eb16"
TMDB_API_READ_TOKEN = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiI0MmNiNDM5Zjk5NDc5MDYyMTRmZTA0ZGM3ZWQxZWIxNiIsInN1YiI6IjVkNzI5Nzk4MjA5ZjE4NjkxNGZiMDhlZCIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.uyObV2lib7s23d7CoMKqHgzkrVAuzJauP4neA20AsLQ"


function search(query_text)
    local retVal = web_requests.get_args("https://api.themoviedb.org/3/search/movie", {query = query_text}, {"Authorization: Bearer " .. TMDB_API_READ_TOKEN})
    return retVal
end
