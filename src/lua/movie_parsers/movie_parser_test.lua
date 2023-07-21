path = "House.S01E01.720p.BluRay.x264.300MB-Pahe.in.mkv"
expr = "(S\\d+E\\d+)"


regex_out = {}
regex.search_iter(path, expr, regex_out, {}, {})

print(cjson.encode(regex_out))