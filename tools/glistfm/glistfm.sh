#!/bin/bash

USERNAME=$1
songs=`curl -s http://ws.audioscrobbler.com/1.0/user/${USERNAME}/recenttracks.rss |\
grep -E "^\ +<title>.*</title>$" |\
sed -r -e 's/<(\/)?title>//g' -e 's/^(\ )+(.*)*$/\2/g'`

last_song=`echo "$songs" | head -n 1`

if [[ "$#" -eq 1 ]]; then
    echo "$last_song"
    exit
fi 

case "$2" in
    -t) #last song or now playing
        echo "$last_song"
        exit
    ;;
    -a) #last 10 songs
        echo "$songs"
        exit
    ;;
esac

