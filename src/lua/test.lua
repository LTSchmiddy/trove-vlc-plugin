log.info("Hello from Sol2!")

print(cjson.encode({["Hello"] = "Alex"}))

print(cjson.decode("{\"hello\": null}")["hello"])

function search()
    print ("Function called successfully!")
end